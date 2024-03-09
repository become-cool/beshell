#include "DB.hpp"
#include <cassert>

using namespace std ;

namespace be::sqlite{
    DEFINE_NCLASS_META(DB, NativeClass)
    std::vector<JSCFunctionListEntry> DB::methods = {
        JS_CFUNC_DEF("execSync", 0, DB::execSync),
        JS_CFUNC_DEF("close", 0, DB::close),
    } ;

    DB::DB(JSContext * ctx, sqlite3 * db)
        : NativeClass(ctx,build(ctx))
        , db(db)
    {
        assert(db) ;
    }
    
    DB::~DB() {
        if(db) {
            sqlite3_close(db) ;
        }
    }

    typedef struct {
        JSContext * ctx ;
        JSValue rows ;
        int rowCnt ;
    } sql_exec_opaque_t ;

    static int sqlite3_callback(void * sql_opa, int argc, char **argv, char **azColName) {
        #define OPA ((sql_exec_opaque_t*)sql_opa)
        int i;
        printf("Callback function called:\n");

        JSValue row = JS_NewArray(OPA->ctx) ;
        for (i = 0; i<argc; i++){
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
            JS_SetPropertyUint32(OPA->ctx, row, i, JS_NewString(OPA->ctx, argv[i])) ;
        }
        printf("\n");

        JS_SetPropertyUint32(OPA->ctx, OPA->rows, OPA->rowCnt++, row) ;
        return 0;
    }

    JSValue DB::execSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(DB, thisdb)
        if( !thisdb->db ) {
            JSTHROW("db not open")
        }
        CHECK_ARGC(1)
        ARGV_TO_STRING(0, sql)
        
        sql_exec_opaque_t opa ;
        opa.rows = JS_NewArray(ctx) ;
        opa.ctx = ctx ;
        opa.rowCnt = 0 ;
        
        char *zErrMsg = nullptr;
        int rc = sqlite3_exec(thisdb->db, sql.c_str(), sqlite3_callback, (void *)&opa, &zErrMsg);

        if(rc != SQLITE_OK) {
            ds(zErrMsg)
            JS_ThrowReferenceError(ctx, "SQL exec error: %s", zErrMsg? zErrMsg: "unknown") ;
            if(zErrMsg) {
                sqlite3_free(zErrMsg);
            }
            JS_FreeValue(ctx,opa.rows) ;
            return JS_EXCEPTION ;
        }

        return opa.rows ;
    }

    JSValue DB::close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(DB, thisdb)
        if(thisdb->db) {
            sqlite3_close(thisdb->db) ;
            thisdb->db = nullptr ;
        }
        return JS_UNDEFINED ;
    }
}