#include "module/serial/I2S.hpp"
using namespace std;

namespace be {
  I2S::I2S(JSContext * ctx, uint8_t _busnum)
        : NativeObject(ctx, classID, "I2S")
        , busnum(_busnum)
    {
    }
}