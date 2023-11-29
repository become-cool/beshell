#pragma once

namespace beshell {
    class TelnetChannel {
    private:
    public:
        TelnetChannel() ;
        virtual void setup () = 0 ;
        virtual void loop () = 0 ;
    } ;
}