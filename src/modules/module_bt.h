#ifndef __H_MODULE_BT__
#define __H_MODULE_BT__

#include "quickjs-libc.h"


// typedef enum {
//     ESP_BT_MODE_IDLE       = 0x00,   /*!< Bluetooth is not running */
//     ESP_BT_MODE_BLE        = 0x01,   /*!< Run BLE mode */
//     ESP_BT_MODE_CLASSIC_BT = 0x02,   /*!< Run Classic BT mode */
//     ESP_BT_MODE_BTDM       = 0x03,   /*!< Run dual mode */
// } esp_bt_mode_t;


/**
 * @brief Controller config options, depend on config mask.
 *        Config mask indicate which functions enabled, this means
 *        some options or parameters of some functions enabled by config mask.
 */
// typedef struct {
//     /*
//      * Following parameters can not be configured runtime when call esp_bt_controller_init()
//      * They will be overwritten by constant values from menuconfig options or from macros.
//      * So, do not modify the value when esp_bt_controller_init()
//      */
//     uint32_t magic;                         /*!< Magic number */
//     uint32_t version;                       /*!< version number of the defined structure */
//     /*
//      * Following parameters can be configured runtime, when call esp_bt_controller_init()
//      */
//     uint16_t controller_task_stack_size;    /*!< Bluetooth controller task stack size */
//     uint8_t controller_task_prio;           /*!< Bluetooth controller task priority */
//     uint8_t controller_task_run_cpu;        /*!< CPU num that Bluetooth controller task runs on */
//     uint8_t bluetooth_mode;                 /*!< Controller mode: BR/EDR, BLE or Dual Mode */
//     uint8_t ble_max_act;                    /*!< BLE maximum number of air activities */
//     uint8_t sleep_mode;                     /*!< controller sleep mode */
//     uint8_t sleep_clock;                    /*!< controller sleep clock */
//     uint8_t ble_st_acl_tx_buf_nb;           /*!< controller static ACL TX BUFFER number */
//     uint8_t ble_hw_cca_check;               /*!< controller hardware triggered CCA check */
//     uint16_t ble_adv_dup_filt_max;          /*!< maxinum number of duplicate scan filter */
//     bool coex_param_en;                     /*!< deprecated */
//     uint8_t ce_len_type;                    /*!< connection event length computation method */
//     bool coex_use_hooks;                    /*!< deprecated */
//     uint8_t hci_tl_type;                    /*!< HCI transport layer, UART, VHCI, etc */
//     esp_bt_hci_tl_t *hci_tl_funcs;          /*!< hci transport functions used, must be set when hci_tl_type is UART */
//     uint8_t txant_dft;                      /*!< default Tx antenna */
//     uint8_t rxant_dft;                      /*!< default Rx antenna */
//     uint8_t txpwr_dft;                      /*!< default Tx power */
//     uint32_t cfg_mask;
//     uint8_t scan_duplicate_mode;            /*!< scan duplicate mode */
//     uint8_t scan_duplicate_type;            /*!< scan duplicate type */
//     uint16_t normal_adv_size;               /*!< Normal adv size for scan duplicate */
//     uint16_t mesh_adv_size;                 /*!< Mesh adv size for scan duplicate */
//     uint8_t coex_phy_coded_tx_rx_time_limit;  /*!< limit on max tx/rx time in case of connection using CODED-PHY with Wi-Fi coexistence */
//     uint32_t hw_target_code;                /*!< hardware target */
//     uint8_t slave_ce_len_min;
//     uint8_t hw_recorrect_en;
//     uint8_t cca_thresh;                     /*!< cca threshold*/
// } esp_bt_controller_config_t;



void be_module_bt_init() ;
void be_module_bt_require(JSContext *ctx) ;
void be_module_bt_loop(JSContext *ctx) ;
void be_module_bt_reset(JSContext *ctx) ;

#endif