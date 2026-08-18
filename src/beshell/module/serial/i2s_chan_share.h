#pragma once

#include "hal/i2s_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 返回 serial 模块 I2S（新版 ng 驱动）指定端口的 TX 通道句柄（i2s_chan_handle_t）。
 *
 * 仅在未启用 CONFIG_BESHELL_SERIAL_I2S_USE_LEGACY 时由 beshell 组件提供实体；
 * 使用 weak 声明，legacy 配置下符号不存在（地址为 NULL），调用前必须判空。
 *
 * 通道由 serial.i2sX.setup() 创建，未初始化时返回 NULL。
 */
void * beshell_i2s_std_tx_handle(i2s_port_t busnum) __attribute__((weak)) ;

#ifdef __cplusplus
}
#endif
