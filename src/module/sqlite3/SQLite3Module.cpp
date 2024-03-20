#include "SQLite3Module.hpp"
#include "DB.hpp"
#include "sqlite3/sqlite3.h"
#include <fs/FS.hpp>

namespace be::sqlite{
    SQLite3Module::SQLite3Module(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportClass<DB>() ;
        exportFunction("setup",setup,0) ;
        exportFunction("open",open,0) ;
    }

    JSValue SQLite3Module::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        sqlite3_initialize() ;
        return JS_UNDEFINED ;
    }
    JSValue SQLite3Module::open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        string ARGV_TO_PATH(0, path)
        
        sqlite3 * cdb = nullptr ;
        int rc = sqlite3_open(path.c_str(), &cdb) ;
        if( rc!=SQLITE_OK ) {
            JSTHROW("sqlite3_open() failed: %d\n",rc) ;
        }
    
        DB * db = new DB(ctx, cdb) ;
        db->shared() ;

        return db->jsobj ;
    }

}