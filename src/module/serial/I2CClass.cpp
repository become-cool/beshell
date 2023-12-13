#include "module/I2CClass.hpp"
using namespace std;



namespace be {
    
    I2CClass::classID = 0 ;
    I2CClass::classDef = {
        "I2C",
        .finalizer = I2CClass::jsFinalizer,
    } ;

    I2CClass::I2CClass() {

    }
}