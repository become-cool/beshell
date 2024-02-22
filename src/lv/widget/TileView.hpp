#pragma once

#include "Obj.hpp"
#include "lvgl.h"

namespace be::lv {

    class TileView: public Obj {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;

    public:
        TileView(JSContext * ctx, lv_obj_t * parent=nullptr) ;
        TileView(JSContext * ctx, JSValue jsobj, lv_obj_t * lvobj) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


// AUTO GENERATE CODE START [GETSETS] --------
        static JSValue getTileActive(JSContext *ctx, JSValueConst this_val) ;
// AUTO GENERATE CODE END [GETSETS] --------

// AUTO GENERATE CODE START [METHODS] --------
        static JSValue addTile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setTile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setTileByIndex(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
// AUTO GENERATE CODE END [METHODS] --------
    } ;

}
