#include "AHT20.hpp"
#include <iostream>

#ifndef BIT
#define BIT(nr)                 (1UL << (nr))
#endif
#define AT581X_STATUS_Calibration_Enable    (3)             /* 1 --Calibration enable; 0 --Calibration disable */
#define AT581X_STATUS_CRC_FLAG              (4)
#define AT581X_STATUS_BUSY_INDICATION       (7)
using namespace std;
using namespace be;

namespace be::driver {
    DEFINE_NCLASS_META(AHT20, NativeClass)

    std::vector<JSCFunctionListEntry> AHT20::methods = {
            JS_CFUNC_DEF("method", 0, AHT20::jsMethod),
    };

    AHT20::AHT20(JSContext *ctx, JSValue _jsobj)
            : NativeClass(ctx, build(ctx, _jsobj)) {
    }

    JSValue AHT20::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new AHT20(ctx);
        return obj->jsobj;
    }

    JSValue AHT20::jsMethod(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        cout << "not implement" << endl;
        return JS_UNDEFINED;
    }


    void AHT20::provider(DriverModule *dm) {
        dm->exportClass<AHT20>();
    }

    void AHT20::use() {
        DriverModule::providers.push_back(provider);
    }

    int AHT20::readTemperatureHumidity(uint32_t *temperature_raw, float *temperature,
                                       uint32_t *humidity_raw, float *humidity) {
        uint8_t buf[] = {0x33, 0x00};
        i2c->write<u_int8_t, u_int8_t *>(addr, reg, buf);
#ifdef ESP_PLATFORM
        vTaskDelay(pdMS_TO_TICKS(100));
#endif
        uint8_t status;
        if (!i2c->read<uint8_t, uint8_t>(addr, addr, status)) {
            return -1;
        }
        if ((status & BIT(AT581X_STATUS_Calibration_Enable)) &&
            (status & BIT(AT581X_STATUS_CRC_FLAG)) &&
            ((status & BIT(AT581X_STATUS_BUSY_INDICATION)) == 0)) {
            uint8_t data[7];
            if (!i2c->read(addr, addr, data, 7)) {
                return -2;
            }
            if (aht20_calc_crc(buf, 6) != buf[6]) {
                uint32_t raw_data;
                raw_data = buf[1];
                raw_data = raw_data << 8;
                raw_data += buf[2];
                raw_data = raw_data << 8;
                raw_data += buf[3];
                raw_data = raw_data >> 4;
                *humidity_raw = raw_data;
                *humidity = (float) raw_data * 100 / 1048576;

                raw_data = buf[3] & 0x0F;
                raw_data = raw_data << 8;
                raw_data += buf[4];
                raw_data = raw_data << 8;
                raw_data += buf[5];
                *temperature_raw = raw_data;
                *temperature = (float) raw_data * 200 / 1048576 - 50;
                return 0;
            } else {
                return -1;
            }
        } else {
            return -3;
        }
    }

    uint8_t AHT20::aht20_calc_crc(uint8_t *data, uint8_t len) {
        uint8_t i;
        uint8_t byte;
        uint8_t crc = 0xFF;

        for (byte = 0; byte < len; byte++) {
            crc ^= data[byte];
            for (i = 8; i > 0; --i) {
                if ((crc & 0x80) != 0) {
                    crc = (crc << 1) ^ 0x31;
                } else {
                    crc = crc << 1;
                }
            }
        }

        return crc;
    }

    JSValue AHT20:: readTemperatureHumidity(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        return JS_UNDEFINED ;
    }
}