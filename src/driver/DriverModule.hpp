#pragma once

#include "NativeModule.hpp"
#include <vector>

namespace be::driver {

    class DriverModule ;
    
	typedef void (*NClassProvider)(DriverModule * dm) ;
	typedef void (*ExtProvider)(JSContext *ctx, DriverModule * dm) ;

    class DriverModule: public be::NativeModule {
    private:
        static std::vector<NClassProvider> providers ;
        static std::map<std::string, ExtProvider> extProviders ;
        
        template <typename D>
        static void nclassProvider(DriverModule *dm) {
            dm->exportClass<D>();
        }
        
        template <typename D>
        static void extProvider(JSContext *ctx, DriverModule *dm) {
            D::exportDriver(ctx) ;
        }
        
    protected:
        void exports(JSContext *ctx) ;

    public:
        DriverModule(JSContext * ctx, const char * name) ;

        // NativeClass 类
        template <typename D, typename std::enable_if<std::is_base_of<NativeClass, D>::value, int>::type = 0>
        static void useDriver() {
            providers.push_back(nclassProvider<D>);
        }

        template <typename D, typename std::enable_if<!std::is_base_of<NativeClass, D>::value, int>::type = 0>
        static void useDriver() {
            extProviders[D::name] = extProvider<D>;
        }


        static JSValue loadeDeviceFromJson(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    } ;

}



   