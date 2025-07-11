#include "SQLite3.hpp"
#include "DB.hpp"
#include "deps/sqlite3/sqlite3.h"
#include <fs/FS.hpp>

using namespace std ;

namespace be::sqlite{

    const char * SQLite3::name = "sqlite3" ;

    SQLite3::SQLite3(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        exportClass<DB>() ;
        exportFunction("setup",setup,0) ;
        exportFunction("open",open,0) ;
    }

    JSValue SQLite3::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        sqlite3_initialize() ;
        return JS_UNDEFINED ;
    }
    JSValue SQLite3::open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ASSERT_ARGC(1)
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