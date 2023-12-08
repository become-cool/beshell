#pragma once
#include "telnet/Protocal.hpp"

namespace be {
    class BeShell ;
    class REPL {
    private:
        BeShell * beshell ;
    public:
        REPL(BeShell * beshell) ;
        void input(Package & pkg) ;
    } ;
}