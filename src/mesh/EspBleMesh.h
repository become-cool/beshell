
#define  MSG_SEND_TTL                           7
#define  MSG_SEND_REL                           false
#define  MSG_SEND_TIMEOUT                       5000
#define  MSG_RETRY_COUNT                        3                                                /*!< 消息重发次数*/
#define VND_CID                                 0x02E5                                           /*!< 自定义CID*/
#define VND_MODEL_ID_CLIENT                     0x0000                                           /*!< 自定义CID客户端*/
#define VND_MODEL_ID_SERVER                     0x0001                                           /*!< 自定义CID服务端*/


#define VND_MODEL_OP_SET                     ESP_BLE_MESH_MODEL_OP_3(0x00, VND_CID)            /*!< 设置属性*/
#define VND_MODEL_OP_SET_UN_ACK              ESP_BLE_MESH_MODEL_OP_3(0x01, VND_CID)            /*!< 设置属性不需要ACK*/
#define VND_MODEL_OP_SET_ACK                 ESP_BLE_MESH_MODEL_OP_3(0x02, VND_CID)            /*!< 响应设置属性*/
#define VND_MODEL_OP_GET                     ESP_BLE_MESH_MODEL_OP_3(0x03, VND_CID)            /*!< 获取属性*/
#define VND_MODEL_OP_GET_ACK                 ESP_BLE_MESH_MODEL_OP_3(0x04, VND_CID)            /*!< 响应获取属性*/
#define VND_MODEL_OP_PROPS                   ESP_BLE_MESH_MODEL_OP_3(0x05, VND_CID)            /*!< 属性上报*/

typedef void (*mesh_msg_handler)(uint32_t opcode, uint8_t *data, size_t size);

/**
 * 初始化ble mesh协议
 * @param device_name 设备名称
 * @param uuid_prefix uuid前缀
 * @return
 */
int init_mesh(char *device_name, uint32_t uuid_prefix);

/**
 * 设置消息处理器
 * @param handler  处理器
 */
void set_vnd_msg_handler(mesh_msg_handler handler);

/**
 * 发送vnd client消息
 * @param dst           目标地址
 * @param opcode        操作码
 * @param data          数据
 * @param size          数据大小
 * @return
 */
esp_err_t send_vnd_client_msg(uint16_t dst, uint32_t opcode, uint8_t *data, size_t size);

/**
 * 发送vnd server消息
 * @param dst           目标地址
 * @param opcode        操作码
 * @param data          数据
 * @param size          数据大小
 * @return
 */
esp_err_t send_vnd_server_msg(uint16_t dst, uint32_t opcode, uint8_t *data, size_t size);

/**
 * 获取节点地址
 * @return
 */
uint16_t get_primary_addr();