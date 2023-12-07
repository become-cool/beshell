#include "REPL.hpp"
#include "BeShell.hpp"



namespace beshell {
    REPL::REPL(BeShell * _beshell)
        : beshell(_beshell)
    {
    }

    void REPL::input(Package & pkg) {
        
        assert(beshell) ;
        JSValue ret = beshell->engine.eval((char *)pkg.body, pkg.body_len,"eval") ;
        if(JS_IsException(ret)) {
            beshell->engine.dumpError(pkg.head.fields.cmd!=LINE) ;
        }
        else {
            if(pkg.head.fields.cmd==LINE) {
                beshell->telnet.output((char *)pkg.body, pkg.body_len) ;
                beshell->engine.print(ret, false) ;
                beshell->telnet.output("\n", 1) ;
            } else {
                beshell->engine.print(ret, true, pkg.head.fields.pkgid) ;
            }
        }
        JS_FreeValue(beshell->engine.ctx, ret) ;
    }
}