#include "DisplayModule.hpp"
#include "RGB565.hpp"
#include "ST7701.hpp"

namespace be {
namespace driver {
namespace display {

DisplayModule::DisplayModule(JSContext * ctx, const char * name)
    : NativeModule(ctx, name, 0)
{
    exportClass<RGB565>() ;
    exportClass<ST7701>() ;
}

}}}