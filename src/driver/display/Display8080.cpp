#include "Display8080.hpp"
#include "DisplayModule.hpp"
using namespace std;

namespace be {
namespace driver {
namespace display {

    JSClassID Display8080::classID ;

    Display8080::Display8080(JSContext * ctx)
        : Display(ctx)
    {
    }

    void Display8080::use(const be::BeShell & beshell) {
        DisplayModule::use(beshell) ;
    }

}}}