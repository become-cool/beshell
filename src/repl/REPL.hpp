#pragma once
#include <vector>
#include <set>
#include <memory>
#include "deps/quickjs/quickjs-libc.h"
#include "telnet/Protocal.hpp"
#include "deps/cmdline/cmdline.hpp"
// #include "deps/argparse.hpp"
#include "deps/Options.hpp"


namespace be {
    class BeShell ;
    class TelnetChannel ;


	typedef std::function<void(BeShell * , TelnetChannel *, Options & args)> REPLCommandHandler;
    typedef struct {
        REPLCommandHandler handler = nullptr ;
        std::unique_ptr<Options> args ;
        std::set<std::string> alias ;
    } REPLCommand ;

    class REPL {
    private:
        BeShell * beshell ;
        std::map<std::string, std::shared_ptr<REPLCommand> > commands ;
        std::map<std::string, std::shared_ptr<REPLCommand> > commandAlias ;
    public:
        REPL(BeShell * beshell) ;
        void input(Package & pkg, TelnetChannel * ch) ;

        bool execCommand(TelnetChannel *, const char * input, int iptLen=-1) ;
        void registerCommand(const char * name, const char * usage, REPLCommandHandler handler) ;
        void alias(const char * alias, const char * origin) ;
    } ;
}