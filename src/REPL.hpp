#pragma once
#include "telnet/Protocal.hpp"
#include "deps/cmdline/cmdline.hpp"

namespace be {
    class BeShell ;
    class TelnetChannel ;
    class REPL {
    private:
        BeShell * beshell ;
    public:
        REPL(BeShell * beshell) ;
        void input(Package & pkg, TelnetChannel * ch) ;
        
        static std::vector<std::string> resolveCommand(const std::string &arg) ;
    } ;
}