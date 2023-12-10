#include "FSModule.hpp"


namespace be {
    FSModule::FSModule(): NativeModule("fs") {
        replGlobal = true ;

        // exportFunction("isFile", isFile) ;
    }
}