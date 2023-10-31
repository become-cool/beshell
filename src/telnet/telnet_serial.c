
#include "telnet_serial.h"
#include "module_telnet.h"
#include "telnet_protocal.h"
#include "module_fs.h"
#include "driver/uart.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include "esp_vfs.h"
#include "utils.h"
#include "js_main_loop.h"
#include "esp_vfs_dev.h"
#include "logging.h"
#include <sys/errno.h>
#include <sys/types.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

LOG_TAG("telnet")


void be_telnet_serial_reset(JSContext *ctx) {
	telnet_prot_reset() ;
}


uart_config_t uart_config = {
	.baud_rate = 115200,			// 921600
	.data_bits = UART_DATA_8_BITS,
	.parity    = UART_PARITY_DISABLE,
	.stop_bits = UART_STOP_BITS_1,
	.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	.source_clk = UART_SCLK_APB,
};
int uart_fd;


static void send_pkg_func(uint8_t * data, size_t datalen) {
	uart_write_bytes(0, data, datalen);
}


static struct timeval telnet_tv = {
	.tv_sec = 0,
	.tv_usec = 0,
};
static fd_set telnet_rfds;
static uint8_t telnet_buffer[127] ;
// static size_t telnet_received = 0 ;

SemaphoreHandle_t sema ;


static void task_uart_receive(void * data) {

	while(1) {

        vTaskDelay(1) ;

		while(1) {
			FD_ZERO(&telnet_rfds);

			int fds = -1 ;
			FD_SET(uart_fd, &telnet_rfds);
			fds = uart_fd ;

			telnet_tv.tv_sec = 0 ;
			telnet_tv.tv_usec = 1000 ;  // 1ms
			
			int s = select(fds + 1, &telnet_rfds, NULL, NULL, &telnet_tv);
			if (s < 0) {
				LOGE("Select failed: errno %d", errno);
				break;;
			} else if (s == 0) {
				// ESP_LOGI(TAG, "Timeout has been reached and nothing has been received");
				break ;
			} 

			// UART 接收
			if (FD_ISSET(uart_fd, &telnet_rfds)) {

				size_t chunklen = uart_read_bytes(0, telnet_buffer, sizeof(telnet_buffer), 0) ;

				xSemaphoreTake( sema, portMAX_DELAY ) ;

				be_telnet_proto_receive(telnet_buffer, chunklen) ;

				xSemaphoreGive( sema ) ;
			}
		}
	}
}

void be_telnet_serial_init() {

	be_telnet_proto_init(send_pkg_func) ;

	telnet_prot_func_reset = task_reset ;

	// uart telnet ----------
    uart_driver_install(UART_NUM_0, 2*1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
	if ((uart_fd = open("/dev/uart/0", O_RDWR)) != -1) {
		esp_vfs_dev_uart_use_driver(0);
	}

	sema = xSemaphoreCreateMutex() ;
	xTaskCreatePinnedToCore(task_uart_receive, "telnet-uart", 1024*3, NULL,10, NULL, 1) ;
}


inline void be_telnet_serial_loop(JSContext *ctx) {
	xSemaphoreTake( sema, 0 ) ;
	be_telnet_proto_loop(ctx) ;
	xSemaphoreGive( sema ) ;
}
