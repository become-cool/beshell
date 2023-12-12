#include "REPL.hpp"
#include "BeShell.hpp"
#include "telnet/Telnet.hpp"
#include "debug.h"
#include <cassert>
#include <iomanip>

using namespace std ;

namespace be {
    REPL::REPL(BeShell * _beshell)
        : beshell(_beshell)
    {}

    void REPL::input(Package & pkg, TelnetChannel * ch) {
        assert(beshell) ;
        if(pkg.body_len<1) {
            return ;
        }

        int rspnId = pkg.head.fields.cmd==LINE? -1: pkg.head.fields.pkgid ;
        bool echo = pkg.head.fields.cmd==LINE && beshell->telnet ;

        size_t content_len = pkg.body_len ;
        // 不含字符串末尾的 0
        if(pkg.body[pkg.body_len-1] == 0){
            content_len -- ;
        } ;
        
        JSValue ret = beshell->engine->eval((char *)pkg.body, content_len,"eval") ;

        if(JS_IsException(ret)) {
            beshell->engine->dumpError(rspnId) ;
        }
        else {
            if(echo && !ch->disableEcho) {
                ch->send((char *)pkg.body, pkg.body_len) ;
            }
            beshell->engine->print(ret, rspnId, RSPN, ch) ;
            if(echo) {
                ch->send("\n", 1) ;
            }
        }
        JS_FreeValue(beshell->engine->ctx, ret) ;
    }
}