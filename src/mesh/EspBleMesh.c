#include <esp_system.h>
#include "EspBleMesh.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_log.h"
#include "esp_ble_mesh_defs.h"
#include "esp_bt_device.h"
#include <string.h>
#include <esp_ble_mesh_networking_api.h>
#include <esp_ble_mesh_common_api.h>
#include <esp_ble_mesh_provisioning_api.h>
#include <esp_ble_mesh_local_data_operation_api.h>
#include <freertos/event_groups.h>
#include "esp_ble_mesh_config_model_api.h"
#include "mesh_buf.h"

#define MESH_MSG_REPLIED_BIT      BIT0
#define MESH_MSG_FAIL_BIT         BIT1
# define SYNC_DO_LOCK(s)          xSemaphoreTakeRecursive(s, portMAX_DELAY)
# define SYNC_DO_UNLOCK(s)        xSemaphoreGiveRecursive(s)

#define MESH_TAG "BE_MESH"

SemaphoreHandle_t sync_send_lock = NULL;
EventGroupHandle_t mesh_send_event_group;

mesh_msg_handler vnd_handler = NULL;

static uint8_t dev_uuid[16] = {};

/** |--------------------------->sig config server <---------------------------| **/
static esp_ble_mesh_cfg_srv_t config_server = {
        .relay = ESP_BLE_MESH_RELAY_ENABLED,
        .beacon = ESP_BLE_MESH_BEACON_ENABLED,
#if defined(CONFIG_BLE_MESH_FRIEND)
        .friend_state = ESP_BLE_MESH_FRIEND_ENABLED,
#else
        .friend_state = ESP_BLE_MESH_FRIEND_NOT_SUPPORTED,
#endif
#if defined(CONFIG_BLE_MESH_GATT_PROXY_SERVER)
        .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_ENABLED,
#else
        .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_NOT_SUPPORTED,
#endif
        .default_ttl = 7,
        /* 3 transmissions with 20ms interval */
        .net_transmit = ESP_BLE_MESH_TRANSMIT(2, 20),
        .relay_retransmit = ESP_BLE_MESH_TRANSMIT(2, 20),
};

static esp_ble_mesh_model_t root_models[] = {
        ESP_BLE_MESH_MODEL_CFG_SRV(&config_server),
};

/** |--------------------------->vnd model <---------------------------| **/
static const esp_ble_mesh_client_op_pair_t vnd_op_pair[] = {
        {VND_MODEL_OP_SET, VND_MODEL_OP_SET_ACK},
        {VND_MODEL_OP_GET, VND_MODEL_OP_GET_ACK},
};

static esp_ble_mesh_client_t vendor_client = {
        .op_pair_size = ARRAY_SIZE(vnd_op_pair),
        .op_pair = vnd_op_pair,
};
//client opt code
static esp_ble_mesh_model_op_t client_vnd_op[] = {
        ESP_BLE_MESH_MODEL_OP(VND_MODEL_OP_SET_ACK, 1),
        ESP_BLE_MESH_MODEL_OP(VND_MODEL_OP_GET_ACK, 1),
        ESP_BLE_MESH_MODEL_OP(VND_MODEL_OP_PROPS, 4),
        ESP_BLE_MESH_MODEL_OP_END,
};
//server opt code
static esp_ble_mesh_model_op_t server_vnd_op[] = {
        ESP_BLE_MESH_MODEL_OP(VND_MODEL_OP_SET, 1),
        ESP_BLE_MESH_MODEL_OP(VND_MODEL_OP_SET_UN_ACK, 1),
        ESP_BLE_MESH_MODEL_OP(VND_MODEL_OP_GET, 1),
        ESP_BLE_MESH_MODEL_OP_END,
};
//vnd model
static esp_ble_mesh_model_t vnd_models[] = {
        ESP_BLE_MESH_VENDOR_MODEL(VND_CID, VND_MODEL_ID_CLIENT,
                                  client_vnd_op, NULL, &vendor_client),
        ESP_BLE_MESH_VENDOR_MODEL(VND_CID, VND_MODEL_ID_SERVER,
                                  server_vnd_op, NULL, NULL),
};

esp_ble_mesh_elem_t elements[] = {
        ESP_BLE_MESH_ELEMENT(0, root_models, vnd_models),
};

static esp_ble_mesh_comp_t composition = {
        .cid = VND_CID,
        .elements = elements,
        .element_count = ARRAY_SIZE(elements)
};

static esp_ble_mesh_prov_t provision = {
        .uuid = dev_uuid,
        .uri = "123456",
#if 0
        .output_size = 4,
        .output_actions = ESP_BLE_MESH_DISPLAY_NUMBER,
        .input_actions = ESP_BLE_MESH_PUSH,
        .input_size = 4,
#else
        .output_size = 0,
        .output_actions = 0,
#endif
};

static void ble_mesh_config_server_cb(esp_ble_mesh_cfg_server_cb_event_t event,
                                      esp_ble_mesh_cfg_server_cb_param_t *param) {
    if (event == ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT) {
        switch (param->ctx.recv_op) {
            case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD:
                ESP_LOGI(MESH_TAG, "net_idx 0x%04x, app_idx 0x%04x",
                         param->value.state_change.appkey_add.net_idx,
                         param->value.state_change.appkey_add.app_idx);
                ESP_LOG_BUFFER_HEX("AppKey", param->value.state_change.appkey_add.app_key, 16);
                break;
            default:
                break;
        }
    }
}

static bool is_status_resp(uint32_t opcode) {
    for (size_t i = 0; i < ARRAY_SIZE(vnd_op_pair); i++) {
        if (vnd_op_pair[i].status_op == opcode) {
            return true;
        }
    }
    return false;
}

static void ble_mesh_custom_model_cb(esp_ble_mesh_model_cb_event_t event,
                                     esp_ble_mesh_model_cb_param_t *param) {
    switch (event) {
        case ESP_BLE_MESH_MODEL_OPERATION_EVT:
            ESP_LOGI(MESH_TAG, "Recv 0x06%" PRIx32,
                     param->model_operation.opcode);
            if (is_status_resp(param->model_operation.opcode)) {
                ESP_LOGI(MESH_TAG, "Recv 0x06%" PRIx32 " status response",
                         param->model_operation.opcode);
                xEventGroupSetBits(mesh_send_event_group, MESH_MSG_REPLIED_BIT);
            } else {
                //handle message
                if (vnd_handler != NULL) {
                    uint8_t *data = malloc(sizeof(uint8_t) * param->model_operation.length);
                    vnd_handler(param->model_operation.opcode, data, param->model_operation.length);
                    free(data);
                }
            }
            break;
        case ESP_BLE_MESH_MODEL_SEND_COMP_EVT:
            if (param->model_send_comp.err_code) {
                ESP_LOGE(MESH_TAG, "Failed to send message 0x%06" PRIx32, param->model_send_comp.opcode);
                xEventGroupSetBits(mesh_send_event_group, MESH_MSG_FAIL_BIT);
                break;
            }
            ESP_LOGI(MESH_TAG, "Send ESP_BLE_MESH_MODEL_SEND_COMP_EVT 0x%06" PRIx32, param->model_send_comp.opcode);
            break;
        case ESP_BLE_MESH_CLIENT_MODEL_SEND_TIMEOUT_EVT:
            xEventGroupSetBits(mesh_send_event_group, MESH_MSG_FAIL_BIT);
            break;
        case ESP_BLE_MESH_CLIENT_MODEL_RECV_PUBLISH_MSG_EVT:

        default:
            break;
    }
}

//配网消息回调
static void ble_mesh_provisioning_cb(esp_ble_mesh_prov_cb_event_t event,
                                     esp_ble_mesh_prov_cb_param_t *param) {
    switch (event) {
        case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:
            ESP_LOGI(MESH_TAG, "ESP_BLE_MESH_PROV_REGISTER_COMP_EVT, err_code %d", param->prov_register_comp.err_code);
            break;
        case ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT:
            ESP_LOGI(MESH_TAG, "ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT, err_code %d",
                     param->node_prov_enable_comp.err_code);
            break;
        case ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT:
            ESP_LOGI(MESH_TAG, "ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT, bearer %s",
                     param->node_prov_link_open.bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT");
            break;
        case ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT:
            ESP_LOGI(MESH_TAG, "ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT, bearer %s",
                     param->node_prov_link_close.bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT");
            break;
        case ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT:
            ESP_LOGI(MESH_TAG, "ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT");
            break;
        case ESP_BLE_MESH_NODE_PROV_RESET_EVT:
            ESP_LOGI(MESH_TAG, "ESP_BLE_MESH_NODE_PROV_RESET_EVT");
            break;
        case ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT:
            ESP_LOGI(MESH_TAG, "ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT, err_code %d",
                     param->node_set_unprov_dev_name_comp.err_code);
            break;
        default:
            break;
    }
}

/** |--------------------------->发送消息<---------------------------| **/


static void init_mesh_msg_ctx(esp_ble_mesh_msg_ctx_t *ctx, uint16_t addr) {
    ctx->net_idx = 0;
    ctx->app_idx = 0;
    ctx->addr = addr;
    ctx->send_ttl = MSG_SEND_TTL;
    ctx->send_rel = MSG_SEND_REL;
}

static esp_err_t do_send_mesh_msg(uint16_t addr, uint32_t opcode, uint8_t *data, uint8_t data_size, bool need_rsp) {
    SYNC_DO_LOCK(sync_send_lock);
    esp_ble_mesh_msg_ctx_t ctx = {0};
    init_mesh_msg_ctx(&ctx, addr);
    esp_err_t err = ESP_OK;
    ESP_LOGI(MESH_TAG, "send mesh msg addr=%d", addr);
    if (need_rsp) {
        err = esp_ble_mesh_client_model_send_msg(&vnd_models[0], &ctx, opcode,
                                                 sizeof(data), data, MSG_SEND_TIMEOUT, true, ROLE_NODE);
        if (err) {
            ESP_LOGE(MESH_TAG, "Failed to send message %s", esp_err_to_name(err));
            SYNC_DO_UNLOCK(sync_send_lock);
            return err;
        }
        //等待消息
        EventBits_t bits = xEventGroupWaitBits(mesh_send_event_group,
                                               MESH_MSG_REPLIED_BIT | MESH_MSG_FAIL_BIT,
                                               pdTRUE,
                                               pdFALSE,
                                               MSG_SEND_TIMEOUT + 1000 / portTICK_PERIOD_MS);
        SYNC_DO_UNLOCK(sync_send_lock);
        err = (bits & MESH_MSG_REPLIED_BIT) ? ESP_OK : ESP_FAIL;
        if (err) {
            ESP_LOGE(MESH_TAG, "waiting for reply timeout.....");
        }
    } else {
        err = esp_ble_mesh_server_model_send_msg(&vnd_models[1], &ctx, opcode, data_size, data);
    }
    SYNC_DO_UNLOCK(sync_send_lock);
    return err;
}


static esp_err_t send_client_mesh_msg(uint16_t target, uint32_t opcode, uint8_t *buffer, size_t length, uint8_t cycle) {

    esp_err_t err = do_send_mesh_msg(target, opcode, buffer, length, true);
    if (!err) {
        return err;
    }
    if (cycle > 0) {
        return send_client_mesh_msg(target, opcode, buffer, length, --cycle);
    }
    return ESP_FAIL;
}

esp_err_t send_vnd_client_msg(uint16_t dst, uint32_t opcode, uint8_t *data, size_t size) {
    return send_client_mesh_msg(dst, opcode, data, size, MSG_RETRY_COUNT);
}

esp_err_t send_vnd_server_msg(uint16_t dst, uint32_t opcode, uint8_t *data, size_t size) {
    return do_send_mesh_msg(dst, opcode, data, size, false);
}

int bluetooth_init() {
    esp_err_t ret;
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(MESH_TAG, "%s initialize controller failed", __func__);
        return ret;
    }
    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(MESH_TAG, "%s enable controller failed", __func__);
        return ret;
    }
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(MESH_TAG, "%s init bluetooth failed", __func__);
        return ret;
    }
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(MESH_TAG, "%s enable bluetooth failed", __func__);
        return ret;
    }
    ESP_LOGI(MESH_TAG, "bluetooth_init success");
    return 0;
}

int init_mesh(char *device_name, uint32_t uuid_prefix) {
    esp_err_t err = bluetooth_init();
    dev_uuid[0] = (uuid_prefix >> 24) & 0xFF;
    dev_uuid[1] = (uuid_prefix >> 16) & 0xFF;
    dev_uuid[2] = (uuid_prefix >> 8) & 0xFF;
    dev_uuid[3] = uuid_prefix & 0xFF;
    memcpy(dev_uuid + 4, esp_bt_dev_get_address(), BD_ADDR_LEN);

    mesh_send_event_group = xEventGroupCreate();
    sync_send_lock = xSemaphoreCreateRecursiveMutex();

    //config server回调配置
    esp_ble_mesh_register_config_server_callback(ble_mesh_config_server_cb);
    esp_ble_mesh_register_custom_model_callback(ble_mesh_custom_model_cb);
    //初始化mesh
    err = esp_ble_mesh_init(&provision, &composition);
    if (err != ESP_OK) {
        ESP_LOGE(MESH_TAG, "Failed to initialize mesh stack (err %d)", err);
        return err;
    }
    if (!esp_ble_mesh_node_is_provisioned()) {
        ESP_LOGI(MESH_TAG, "start ble mesh provisioning...");
        esp_ble_mesh_register_prov_callback(ble_mesh_provisioning_cb);
        esp_ble_mesh_set_unprovisioned_device_name(device_name);
        err = esp_ble_mesh_node_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
        if (err != ESP_OK) {
            ESP_LOGE(MESH_TAG, "Failed to enable mesh node (err %d)", err);
            return err;
        }
    }
    err = esp_ble_mesh_client_model_init(&vnd_models[0]);
    if (err) {
        ESP_LOGE(MESH_TAG, "Failed to initialize vendor client");
        return err;
    }
    return 0;
}

void set_vnd_msg_handler(mesh_msg_handler handler) {
    if (handler != NULL) {
        vnd_handler = handler;
    }
}


uint16_t get_primary_addr() {
    return esp_ble_mesh_get_primary_element_address();
}