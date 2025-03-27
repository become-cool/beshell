#include "Flash.hpp"


namespace be{
    
    DEFINE_NCLASS_META(Partition, NativeClass)

    char const * const Flash::name = "flash" ;

    std::vector<JSCFunctionListEntry> Partition::methods = {
        JS_CFUNC_DEF("erase", 0, Partition::erase),
        JS_CFUNC_DEF("read", 0, Partition::read),
        JS_CFUNC_DEF("write", 0, Partition::write),
        JS_CGETSET_DEF("label", Partition::label,Partition::invalidSetter),
        JS_CGETSET_DEF("name", Partition::label,Partition::invalidSetter),
        JS_CGETSET_DEF("type", Partition::type,Partition::invalidSetter),
        JS_CGETSET_DEF("subtype", Partition::subtype,Partition::invalidSetter),
        JS_CGETSET_DEF("address", Partition::address,Partition::invalidSetter),
        JS_CGETSET_DEF("size", Partition::size,Partition::invalidSetter),
        JS_CGETSET_DEF("readonly", Partition::readonly,Partition::invalidSetter),
        JS_CGETSET_DEF("encrypted",Partition::encrypted,Partition::invalidSetter),
    } ;


    Partition::Partition(const esp_partition_t * partition, JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
        , partition(partition)
    {
        shared() ;
    }

    JSValue Partition::erase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Partition, self)
        CHECK_ARGC(2)
        ARGV_TO_UINT32(0, offset)
        ARGV_TO_UINT32(1, length)
        esp_err_t err = esp_partition_erase_range(self->partition, offset, length) ;
        if(err!=ESP_OK) {
            JSTHROW("erase failed: %d", err)
        }
        return JS_UNDEFINED ;
    }
    JSValue Partition::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Partition, self)
        CHECK_ARGC(2)
        ARGV_TO_UINT32(0, offset)
        ARGV_TO_UINT32(1, length)

        if(length>1024*1024) {
            JSTHROW("read length too long, max 1M")
        }
        if(offset+length>self->partition->size) {
            JSTHROW("out of partition range")
        }

        uint8_t * buffer = (uint8_t*) malloc(length) ;
        esp_err_t err = esp_partition_read(self->partition, offset, buffer, length) ;
        if(err!=ESP_OK) {
            free(buffer) ;
            JSTHROW("read failed: %d", err)
        }

        return JS_NewArrayBuffer(ctx, buffer, length, freeArrayBuffer, NULL, false) ;  
    }
    JSValue Partition::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(Partition, self)
        CHECK_ARGC(2)
        ARGV_TO_UINT32(0, offset)
        ARGV_TO_ARRAYBUFFER(1, data, datalen)

        esp_err_t err = esp_partition_write(self->partition, offset, data, datalen) ;
        if(err!=ESP_OK) {
            JSTHROW("write failed: %d", err)
        }
        return JS_UNDEFINED ;
    }

    JSValue Partition::label(JSContext *ctx, JSValueConst this_val) {
        THIS_NCLASS(Partition, self)
        return JS_NewString(ctx, self->partition->label) ;
    }
    JSValue Partition::type(JSContext *ctx, JSValueConst this_val) {
        THIS_NCLASS(Partition, self)
        switch(self->partition->type) {
            case ESP_PARTITION_TYPE_APP: return JS_NewString(ctx, "app") ;
            case ESP_PARTITION_TYPE_DATA: return JS_NewString(ctx, "data") ;
            case ESP_PARTITION_TYPE_BOOTLOADER: return JS_NewString(ctx, "bootloader") ;
            case ESP_PARTITION_TYPE_PARTITION_TABLE: return JS_NewString(ctx, "partition_table") ;
            default: return JS_NewString(ctx, "UNKNOWN") ;
        }
    }
    JSValue Partition::subtype(JSContext *ctx, JSValueConst this_val) {
        THIS_NCLASS(Partition, self)
        return JS_NewInt32(ctx, self->partition->subtype) ;
    }
    JSValue Partition::address(JSContext *ctx, JSValueConst this_val) {
        THIS_NCLASS(Partition, self)
        return JS_NewInt32(ctx, self->partition->address) ;
    }
    JSValue Partition::size(JSContext *ctx, JSValueConst this_val) {
        THIS_NCLASS(Partition, self)
        return JS_NewInt32(ctx, self->partition->size) ;
    }
    JSValue Partition::readonly(JSContext *ctx, JSValueConst this_val) {
        THIS_NCLASS(Partition, self)
        return JS_NewBool(ctx, self->partition->readonly) ;
    }
    JSValue Partition::encrypted(JSContext *ctx, JSValueConst this_val) {
        THIS_NCLASS(Partition, self)
        return JS_NewBool(ctx, self->partition->encrypted) ;
    }
    JSValue Partition::invalidSetter(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
        JSTHROW("property is readonly") ;
    }



    Flash::Flash(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportFunction("allPartitions",allPartitions,0) ;
        exportFunction("partition",partition,0) ;
    }

    JSValue Flash::allPartitions(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
        // 创建分区迭代器（匹配所有类型和子类型）
        esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
        if (it == NULL) {
            JSTHROW("can't find partition")
        }

        JSValue list = JS_NewObject(ctx) ;

        const esp_partition_t *partition;
        while (it && (partition = esp_partition_get(it)) != NULL) {

            auto jspartition = new Partition(partition, ctx) ;
            JS_SetPropertyStr(ctx, list, partition->label, jspartition->jsobj) ;
            
            it = esp_partition_next(it);
        }

        esp_partition_iterator_release(it);

        return list ;
    }

    JSValue Flash::partition(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        std::string ARGV_TO_STRING(0, name)

        const esp_partition_t * partition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, name.c_str()) ;
        if(!partition) {
            JSTHROW("partition not found")
        }
        auto jspartition = new Partition(partition, ctx) ;
        return jspartition->jsobj ;
    }
}