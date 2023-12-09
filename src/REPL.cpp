#include "REPL.hpp"
#include "BeShell.hpp"
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
        JSValue ret = beshell->engine->eval((char *)pkg.body, pkg.body_len,"eval") ;
        if(JS_IsException(ret)) {
            beshell->engine->dumpError(pkg.head.fields.cmd!=LINE) ;
        }
        else {
            if(pkg.head.fields.cmd==LINE) {
                if(ch->echoInput) {
                    beshell->telnet->output((char *)pkg.body, pkg.body_len) ;
                }
                beshell->engine->print(ret, -1, ch) ;
                if(ch->echoInput) {
                    beshell->telnet->output("\n", 1) ;
                }
            } else {
                beshell->engine->print(ret, pkg.head.fields.pkgid, ch) ;
            }
        }
        JS_FreeValue(beshell->engine->ctx, ret) ;
    }
}