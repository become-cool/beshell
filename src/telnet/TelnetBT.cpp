#include "TelnetBT.hpp"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_bt.h"

// @todo move to BLE module
static esp_err_t bluetooth_init() {
    esp_err_t ret = ESP_OK;
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        printf("%s initialize controller failed\n", __func__);
        return ret;
    }
    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        printf("%s enable controller failed\n", __func__);
        return ret;
    }
    ret = esp_bluedroid_init();
    if (ret) {
        printf("%s init bluetooth failed\n", __func__);
        return ret;
    }
    ret = esp_bluedroid_enable();
    if (ret) {
        printf("%s enable bluetooth failed\n", __func__);
        return ret;
    }
    return ret;
}

namespace be {    
    // void TelnetBT::setup(const char * name, uint16_t characteristicID, uint16_t serviceID) {

    //     // bluetooth_init();

    // }
}