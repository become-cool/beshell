#include "module/serial/I2C.hpp"
using namespace std;

namespace be {
    I2C::I2C(JSContext * ctx, uint8_t _busnum)
        : NativeObject(ctx, classID, "I2C")
        , busnum(_busnum)
    {
    }
}