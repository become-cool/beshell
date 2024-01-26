#include "module/serial/SPI.hpp"
using namespace std;

namespace be {
  SPI::SPI(JSContext * ctx, uint8_t _busnum)
        : NativeObject(ctx, classID, "SPI")
        , busnum(_busnum)
    {
    }
}