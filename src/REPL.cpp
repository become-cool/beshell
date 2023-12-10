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
        size_t content_len = pkg.body_len ;
        // 字符串末尾的 0
        if(pkg.body[pkg.body_len-1] == 0){
            content_len -- ;
        } ;
        JSValue ret = beshell->engine->eval((char *)pkg.body, content_len,"eval") ;
        if(JS_IsException(ret)) {
            beshell->engine->dumpError(pkg.head.fields.cmd!=LINE) ;
        }
        else {
            if(pkg.head.fields.cmd==LINE) {
                if(ch->echoInput) {
                    if(beshell->telnet){
                        beshell->telnet->output((char *)pkg.body, pkg.body_len) ;
                    }
                }
                beshell->engine->print(ret, -1, ch) ;
                if(beshell->telnet) {
                    beshell->telnet->output("\n", 1) ;
                }
            } else {
                beshell->engine->print(ret, pkg.head.fields.pkgid, ch) ;
            }
        }
        JS_FreeValue(beshell->engine->ctx, ret) ;
    }
}