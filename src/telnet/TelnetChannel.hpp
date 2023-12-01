#pragma once
#include "Protocal.hpp"

namespace beshell {
    class TelnetChannel {
    private:
    protected:
        PackageProcFunc packageHandler = nullptr;
    public:
        TelnetChannel(PackageProcFunc packageHandler=nullptr) ;
        virtual void setup () = 0 ;
        virtual void loop () = 0 ;
        virtual void send (Package & pkg) = 0 ;
    } ;
}