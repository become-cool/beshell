#include "Display.hpp"
using namespace std;


namespace be {
namespace driver {
namespace display {

    
    Display::Display(
        JSContext * _ctx
        , JSClassID _classID
        , const char * _name
        , NativeClassDefineFunc _funcDefineClass
        , NativeObject * _parent )
        : NativeObject(_ctx, _classID, _name, nullptr, nullptr, nullptr, _funcDefineClass)
    {}
    
}}}