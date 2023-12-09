#pragma once
#include "telnet/Protocal.hpp"

namespace be {
    class BeShell ;
    class TelnetChannel ;
    class REPL {
    private:
        BeShell * beshell ;
    public:
        REPL(BeShell * beshell) ;
        void input(Package & pkg, TelnetChannel * ch) ;
    } ;
}