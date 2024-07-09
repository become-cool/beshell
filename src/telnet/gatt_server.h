
#ifndef BESHELL_86PANEL_GATT_SERVICE_H
#define BESHELL_86PANEL_GATT_SERVICE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef void (*be_telnet_gatt_msg_handler_t)(uint8_t *data, size_t size);

void be_telnet_gatt_server_start(const char * name, uint16_t characteristicID, uint16_t serviceID, uint16_t appID);

void be_telnet_gatt_server_stop();

void be_telnet_gatt_server_set_msg_handler(be_telnet_gatt_msg_handler_t handler);

void be_telnet_gatt_server_send(uint8_t *data, size_t size, bool need_confirm);

#endif //BESHELL_86PANEL_GATT_SERVICE_H
#ifdef __cplusplus
}
#endif