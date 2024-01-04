#include "module/serial/UART.hpp"
using namespace std;

namespace be {
  UART::UART(JSContext * ctx, uint8_t _busnum)
        : NativeObject(ctx, classID, "UART", defineClass)
        , busnum(_busnum)
    {
    }
    void UART::defineClass(NativeClass *) {
    }
}