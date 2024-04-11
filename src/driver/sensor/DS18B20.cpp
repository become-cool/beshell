#include "DS18B20.hpp"
#include <esp_system.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp32/rom/ets_sys.h"

using namespace std ;


#define ESP_HIGH gpio_set_level(pin, 1)    //设置高电平
#define ESP_LOW gpio_set_level(pin, 0)    //设置低电平
#define ESP_GET gpio_get_level(pin)    //读取电平
#define ESP_OUT_MODE gpio_set_direction(pin, GPIO_MODE_OUTPUT)    //设置输出模式
#define ESP_INPUT_MODE gpio_set_direction(pin, GPIO_MODE_INPUT)    //设置输入模式
 
#define SKIP_ROM 0xCC    //跳过ROM操作
#define TRANSFORMATION_TEMP 0x44    //转换温度操作
#define READ_TEMP 0xBE    //读数据操作


namespace be::driver::sensor {
    DEFINE_NCLASS_META(DS18B20, NativeClass)
    std::vector<JSCFunctionListEntry> DS18B20::methods = {
        JS_CFUNC_DEF("setup", 0, DS18B20::setup),
        JS_CFUNC_DEF("measure", 0, DS18B20::measure),
        JS_CFUNC_DEF("read", 0, DS18B20::read),
    } ;

    DS18B20::DS18B20(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue DS18B20::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new DS18B20(ctx) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    JSValue DS18B20::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(DS18B20, that)
        CHECK_ARGC(1)

        GET_INT32_PROP(argv[0], "pin", that->pin, )

        esp_rom_gpio_pad_select_gpio(that->pin);
        that->init() ;

        return JS_UNDEFINED ;
    }

    JSValue DS18B20::measure(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(DS18B20, that)
        if( that->init()!=1 ) {
            JSTHROW("DS18B20 init falid") ;
        }
        that->writeByte(SKIP_ROM);
        that->writeByte(TRANSFORMATION_TEMP);
        return JS_UNDEFINED ;
    }

    JSValue DS18B20::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(DS18B20, that)
        if( that->init()!=1 ) {
            JSTHROW("DS18B20 init falid") ;
        }

        unsigned char low=0, high=0;
        that->writeByte(SKIP_ROM);
        that->writeByte(READ_TEMP);

        low = that->readByte();
        high = that->readByte();

        float value = (float)(low + (high * 256)) / 16;

        return JS_NewFloat64(ctx, value) ;
    }

    
    unsigned char DS18B20::init(void) {

        unsigned int init_state = 0;
        ESP_OUT_MODE;
        ESP_LOW;
        ets_delay_us(480);
        ESP_HIGH;
    
        ESP_INPUT_MODE;//设置输入模式，就是高阻态，此时上拉电阻提供高电平
        ets_delay_us(70);
        init_state = (gpio_get_level(pin) == 0);
    
        ets_delay_us(410);
        return init_state;
    }

    void DS18B20::writeBit(char bit) {
        if (bit & 1) {
            ESP_OUT_MODE;
            ESP_LOW;
            ets_delay_us(6);
            ESP_INPUT_MODE;//设置输入模式，就是高阻态，此时上拉电阻提供高电平
            ets_delay_us(64);
        }
        else {
            ESP_OUT_MODE;
            ESP_LOW;
            ets_delay_us(60);
            ESP_INPUT_MODE;//设置输入模式，就是高阻态，此时上拉电阻提供高电平
            ets_delay_us(10);
        }
    }

    unsigned char DS18B20::readBit(void) {
        unsigned char bit = 0;
        ESP_OUT_MODE;
        
        ESP_LOW;
        ets_delay_us(6);
        ESP_INPUT_MODE;//设置输入数据
        ets_delay_us(9);
        bit = ESP_GET;
        ets_delay_us(55);
    
        return bit;
    }

    void DS18B20::writeByte(unsigned char data) {
        unsigned char i;
        unsigned char a_data;
        for (i = 0; i < 8; i++) {
            a_data = data >> i;//每写一位数据，就向右移一位，保证8位数据都能写入
            a_data &= 0x01;//与操作，只留刚刚右移的一位数据
            writeBit(a_data);//写入一位数据
        }
        ets_delay_us(100);
    }

    unsigned char DS18B20::readByte(void) {
        unsigned char i;
        unsigned char data = 0;
        
        for (i = 0; i < 8; i++) {
            if (readBit())
                data |= 0x01 << i;//读取到1就会向左移相应的位数
            ets_delay_us(15);
        }
        return data;
    }
}