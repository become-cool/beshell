#include "driver/display/Bus8080.hpp"
using namespace std;

namespace be {
namespace driver {
namespace display {

    JSClassID Bus8080::classID = 0 ;
    Bus8080::Bus8080(JSContext * ctx, uint8_t _busnum)
        : NativeObject(ctx, classID, "Bus8080", nullptr, nullptr, nullptr, defineClass)
        // , busnum(_busnum)
    {
    }
    void Bus8080::defineClass(NativeClass *) {
    }
}}}