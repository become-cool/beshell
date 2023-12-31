#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"

#include "disp_st77XX.h"
#include "module_serial.h"
#include "debug.h"

#define TAG "ST7789"
#define	_DEBUG_ 0

#define SPI_Command_Mode 0
#define SPI_Data_Mode 1



bool st77xx_spi_init(st77xx_dev_t * dev, uint8_t spi, uint8_t cs, uint8_t dc, uint32_t frequency) {
	
	printf("st77xx_spi_init() spi=%d, cs=%d, dc=%d, freq=%d\n",spi,cs,dc,frequency) ;

	esp_err_t ret;

	ESP_LOGI(TAG, "GPIO_CS=%d",cs);
	gpio_pad_select_gpio( cs );
	gpio_set_direction( cs, GPIO_MODE_OUTPUT );
	gpio_set_level( cs, 0 );

	ESP_LOGI(TAG, "GPIO_DC=%d",dc);
	gpio_pad_select_gpio( dc );
	gpio_set_direction( dc, GPIO_MODE_OUTPUT );
	gpio_set_level( dc, 0 );

	spi_device_interface_config_t devcfg={
		.clock_speed_hz = frequency,
		.queue_size = 8,
		.mode = SCREEN_SPI_MODE,
		.flags = SPI_DEVICE_NO_DUMMY,
	};

	if ( cs >= 0 ) {
		devcfg.spics_io_num = cs;
	} else {
		devcfg.spics_io_num = -1;
	}
	
	spi_device_handle_t handle;
	ret = spi_bus_add_device( spi, &devcfg, &handle);
	ESP_LOGD(TAG, "spi_bus_add_device() %d",ret);
	dev->_dc = dc;
	dev->spi_dev = handle;

	return ret==ESP_OK ;
}


void spi_write_bytes(spi_device_handle_t dev, const uint8_t* data, size_t len) {

	spi_transaction_t trans;
    memset(&trans, 0, sizeof(spi_transaction_t));
	esp_err_t ret;

	if ( len > 0 ) {
		// memset( &trans, 0, sizeof( spi_transaction_t ) );
		trans.length = len * 8;
		trans.tx_buffer = data;
		trans.flags = trans.flags & (~SPI_TRANS_USE_RXDATA) ;

		// // 加入到发送任务队列
		// if(spi_device_queue_trans( dev, &trans, portMAX_DELAY )!=ESP_OK) {
		// 	return ;
		// }
		// // 检查任务完成
		// spi_transaction_t * rtrans = NULL ;
		// while(1) {
		// 	if(spi_device_get_trans_result(dev, &rtrans, portMAX_DELAY)!=ESP_OK) {
		// 		break ;
		// 	}
		// 	if(rtrans==&trans) {
		// 		break ;
		// 	}
		// }

		// ret = spi_device_transmit( dev, &trans );
		ret = spi_device_polling_transmit( dev, &trans );
	}
}

void st77xx_command(st77xx_dev_t * dev, uint8_t cmd) {
	static uint8_t __Byte = 0;
	__Byte = cmd;
	gpio_set_level( dev->_dc, SPI_Command_Mode );
	spi_write_bytes( dev->spi_dev, &__Byte, 1 );
}

void st77xx_data(st77xx_dev_t * dev, uint8_t * data, int len) {
	gpio_set_level( dev->_dc, SPI_Data_Mode );
	spi_write_bytes( dev->spi_dev, data, len );
}

void st77xx_data_byte(st77xx_dev_t * dev, uint8_t data) {
	static uint8_t __Byte = 0;
	__Byte = data;
	gpio_set_level( dev->_dc, SPI_Data_Mode );
	spi_write_bytes( dev->spi_dev, &__Byte, 1 );
}

void st77xx_data_word(st77xx_dev_t * dev, uint16_t data)
{
	static uint8_t __Word[2];
	__Word[0] = (data >> 8) & 0xFF;
	__Word[1] = data & 0xFF;
	gpio_set_level( dev->_dc, SPI_Data_Mode );
	spi_write_bytes( dev->spi_dev, __Word, 2);
}


void st77xx_addr(st77xx_dev_t * dev, uint16_t addr1, uint16_t addr2) {
	static uint8_t __Addr[4];
	__Addr[0] = (addr1 >> 8) & 0xFF;
	__Addr[1] = addr1 & 0xFF;
	__Addr[2] = (addr2 >> 8) & 0xFF;
	__Addr[3] = addr2 & 0xFF;
	gpio_set_level( dev->_dc, SPI_Data_Mode );
	spi_write_bytes( dev->spi_dev, __Addr, 4);
}

static void delay(int ms) {
	int _ms = ms + (portTICK_PERIOD_MS - 1);
	TickType_t xTicksToDelay = _ms / portTICK_PERIOD_MS;
	// ESP_LOGD(TAG, "ms=%d _ms=%d portTICK_PERIOD_MS=%d xTicksToDelay=%d",ms,_ms,portTICK_PERIOD_MS,xTicksToDelay);
	vTaskDelay(xTicksToDelay);
}

typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

void st7789_init(st77xx_dev_t * dev,  st77xx_conf_t conf) {

	dev->_width = conf.width;
	dev->_height = conf.height;
	dev->_offsetx = conf.offsetx;
	dev->_offsety = conf.offsety;

	st77xx_command(dev, 0x01);	//Power Control 1
	delay(150);

	st77xx_command(dev, 0x11);	// 唤醒
	delay(120);

	// 关于 0x36, 0x37 旋转屏幕，参考 st7789 文档，以及：
	// https://github.com/notro/fbtft/issues/523

	// 方向: 0x20, 0x40, 0x80 组合确定
	// if(rotation==90) {
	// 	st77xx_command(dev, 0x36);
	// 	st77xx_data_byte(dev, 0x20|0x80);	
	// 	// 屏幕开始位置偏移80像素
	// 	st77xx_command(dev, 0x37);
	// 	st77xx_data_byte(dev, 0);	
	// 	st77xx_data_byte(dev, 0x50);	
	// }
	// else {
		st77xx_command(dev, 0x36);	
		st77xx_data_byte(dev, conf.MADCTL);	
	// }
	
	st77xx_command(dev, 0x3A);	// 颜色模式： RGB565 (16Bit)
	st77xx_data_byte(dev, 0x05);

	st77xx_command(dev, 0xC6);	// 帧率: 60hz
	st77xx_data_byte(dev, 0x0F);
	
	st77xx_command(dev, 0xD0);	// power control
	st77xx_data_byte(dev, 0xA4);
	st77xx_data_byte(dev, 0xA1);

	st77xx_command(dev, 0x21);	// 反色
	st77xx_command(dev, 0x29);	// 打开屏幕

	// 屏幕尺寸
	st77xx_command(dev, 0x2A);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 240);
	
	st77xx_command(dev, 0x2B);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 240);	

	return ;

	dev->_width = conf.width;
	dev->_height = conf.height;
	dev->_offsetx = conf.offsetx;
	dev->_offsety = conf.offsety;

	st77xx_command(dev, 0x01);	//Power Control 1
	delay(150);

	st77xx_command(dev, 0x11);	// 唤醒
	delay(120);

	// 关于 0x36, 0x37 旋转屏幕，参考 st7789 文档，以及：
	// https://github.com/notro/fbtft/issues/523

	// 方向: 0x20, 0x40, 0x80 组合确定
	// if(rotation==90) {
	// 	st77xx_command(dev, 0x36);
	// 	st77xx_data_byte(dev, 0x20|0x80);	
	// }
	// else {
		// st77xx_command(dev, 0x36);	
		// st77xx_data_byte(dev, conf.MADCTL);	
	// }
	
	// 屏幕开始位置偏移80像素
	// st77xx_command(dev, 0x37);
	// st77xx_data_byte(dev, 0);	
	// st77xx_data_byte(dev, 0x50);	
	
	st77xx_command(dev, 0x3A);	// 颜色模式： RGB565 (16Bit)
	st77xx_data_byte(dev, 0x05);

	st77xx_command(dev, 0xC6);	// 帧率: 60hz
	st77xx_data_byte(dev, 0x0F);
	
	st77xx_command(dev, 0xD0);	// power control
	st77xx_data_byte(dev, 0xA4);
	st77xx_data_byte(dev, 0xA1);

	st77xx_command(dev, 0x36);
	st77xx_data_byte(dev, conf.MADCTL);	
	printf("ST7798 REG [0x36] -> %d\n", conf.MADCTL) ;

	if(conf.invColor) {
		st77xx_command(dev, 0x21);	// 反色
	}

	st77xx_command(dev, 0x29);	// 打开屏幕

	// 屏幕尺寸
	st77xx_command(dev, 0x2A);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 240);
	
	st77xx_command(dev, 0x2B);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 0);
	st77xx_data_byte(dev, 240);	
}

// int width, int height, int offsetx, int offsety, uint8_t MADCTL
void st7789v_init(st77xx_dev_t * dev,  st77xx_conf_t conf) {

	dev->_width = conf.width;
	dev->_height = conf.height;
	dev->_offsetx = conf.offsetx;
	dev->_offsety = conf.offsety;

	st77xx_command(dev, 0x01);	//Power Control 1
	delay(150);

	st77xx_command(dev, 0x11);	// 唤醒
	delay(120);

	if(conf.invColor) {
		st77xx_command(dev, 0x21);	// 反色
	}

	//--------------------------------------Display Setting------------------------------------------//
	// 关于 0x36, 0x37 旋转屏幕，参考 st7789 文档，以及：
	// https://github.com/notro/fbtft/issues/523
	// 方向: 0x20, 0x40, 0x80 组合确定
	// if(rotation==90) {
		st77xx_command(dev, 0x36);
		st77xx_data_byte(dev, conf.MADCTL);	
	// }
	// else {
	// 	st77xx_command(dev, 0x36);	
	// 	st77xx_data_byte(dev, 0);	
	// }

	// st77xx_command(dev, 0x36);
	// if(USE_HORIZONTAL==0)
	// 	st77xx_data_byte(dev, 0x00);
	// else if(USE_HORIZONTAL==1)
	// 	st77xx_data_byte(dev, 0xC0);
	// else if(USE_HORIZONTAL==2)
	// 	st77xx_data_byte(dev, 0x70);
	// else
	// 	st77xx_data_byte(dev, 0xA0);	

	st77xx_command(dev, 0x3A);	// 颜色模式： RGB565 (16Bit)
	st77xx_data_byte(dev, 0x05);
//--------------------------------ST7789V Frame rate setting----------------------------------//
	st77xx_command(dev, 0xb2);
	st77xx_data_byte(dev, 0x0c);
	st77xx_data_byte(dev, 0x0c);
	st77xx_data_byte(dev, 0x00);
	st77xx_data_byte(dev, 0x33);
	st77xx_data_byte(dev, 0x33);
	st77xx_command(dev, 0xb7);
	st77xx_data_byte(dev, 0x74);   //VGH=14.97V,VGL=-7.67V
//---------------------------------ST7789V Power setting--------------------------------------//
	st77xx_command(dev, 0xbb);
	st77xx_data_byte(dev, 0x1F);
	st77xx_command(dev, 0xc0);
	st77xx_data_byte(dev, 0x2c);
	st77xx_command(dev, 0xc2);
	st77xx_data_byte(dev, 0x01);
	st77xx_command(dev, 0xc3);
	st77xx_data_byte(dev, 0x10);
	st77xx_command(dev, 0xc4);
	st77xx_data_byte(dev, 0x20);
	
	// 帧率
	st77xx_command(dev, 0xc6);
	st77xx_data_byte(dev, 0x0f);
	// st77xx_data_byte(dev, 0x0B);

	// 电流
	// st77xx_command(dev , 0xE9);     
	// st77xx_data_byte(dev , 0x11);   
	// st77xx_data_byte(dev , 0x11);   
	// st77xx_data_byte(dev , 0x03);

	st77xx_command(dev, 0xd0);
	st77xx_data_byte(dev, 0xa4);
	st77xx_data_byte(dev, 0xa1);
//--------------------------------ST7789V gamma setting---------------------------------------//
	st77xx_command(dev, 0xE0);
	st77xx_data_byte(dev, 0xD0);
	st77xx_data_byte(dev, 0x07);
	st77xx_data_byte(dev, 0x0E);
	st77xx_data_byte(dev, 0x0B);
	st77xx_data_byte(dev, 0x0A);
	st77xx_data_byte(dev, 0x14);
	st77xx_data_byte(dev, 0x38);
	st77xx_data_byte(dev, 0x33);
	st77xx_data_byte(dev, 0x4F);
	st77xx_data_byte(dev, 0x37);
	st77xx_data_byte(dev, 0x16);
	st77xx_data_byte(dev, 0x16);
	st77xx_data_byte(dev, 0x2A);
	st77xx_data_byte(dev, 0x2E);

	st77xx_command(dev, 0xE1);
	st77xx_data_byte(dev, 0xD0);
	st77xx_data_byte(dev, 0x0B);
	st77xx_data_byte(dev, 0x10);
	st77xx_data_byte(dev, 0x08);
	st77xx_data_byte(dev, 0x08);
	st77xx_data_byte(dev, 0x06);
	st77xx_data_byte(dev, 0x35);
	st77xx_data_byte(dev, 0x54);
	st77xx_data_byte(dev, 0x4D);
	st77xx_data_byte(dev, 0x0A);
	st77xx_data_byte(dev, 0x14);
	st77xx_data_byte(dev, 0x14);
	st77xx_data_byte(dev, 0x2C);
	st77xx_data_byte(dev, 0x2F);



	// 开机
	st77xx_command(dev, 0x29); 
	delay(20);
}



// Draw multi pixel
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// colors:colors
void st77xx_draw_rect(st77xx_dev_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t * colors) {
	
	// printf("st77xx_draw_rect() dev->_width=%d, dev->_width=%d, dc=%d\n",dev->_width,dev->_height,dev->_dc) ;

	if (x1 >= dev->_width) {
		printf("x1(%d) out of dev->_width (%d) \n", x1, dev->_width) ;
		return;
	}
	if (x2 >= dev->_width) x2=dev->_width-1;
	if (y1 >= dev->_height) {
		printf("y1(%d) out of dev->_height (%d) \n", y1, dev->_height) ;
		return;
	}
	if (y2 >= dev->_height) y2=dev->_height-1;

	// ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x1 + dev->_offsetx;
	uint16_t _x2 = x2 + dev->_offsetx;
	uint16_t _y1 = y1 + dev->_offsety;
	uint16_t _y2 = y2 + dev->_offsety;

	// printf("draw rect (%d,%d) -> (%d,%d); dc:%d\n", _x1,_y1, _x2, _y2, dev->_dc) ;

	if( spi_device_acquire_bus(dev->spi_dev, portMAX_DELAY) != ESP_OK ){
		return ;
	}
	// g_spi_bus1_busy ++ ;
	// xSemaphoreTake(g_spi_bus1_mutex, portMAX_DELAY) ;
    // printf("draw start(P15=%d)\n", gpio_get_level(15));
	// vTaskDelay(5/portTICK_PERIOD_MS) ;

	st77xx_command(dev, 0x2A);	// set column(x) address
	st77xx_addr(dev, _x1, _x2);
	st77xx_command(dev, 0x2B);	// set Page(y) address
	st77xx_addr(dev, _y1, _y2);
	st77xx_command(dev, 0x2C);	//	Memory Write
	
	size_t size = (x2-x1+1) * (y2-y1+1) ;
	
	gpio_set_level( dev->_dc, SPI_Data_Mode );
	spi_write_bytes( dev->spi_dev, (uint8_t *)colors, size*2);

	spi_device_release_bus(dev->spi_dev) ;

	// g_spi_bus1_busy -- ;
	// printf("draw end\n") ;
	// xSemaphoreGive( g_spi_bus1_mutex );

	return ;

}

// static bool spi_master_write_color(st77xx_dev_t * dev, uint16_t color, uint16_t size) {
// 	// static uint8_t buff[1024];

// }


// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void st77xx_fill_rect(st77xx_dev_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {

	if (x1 >= dev->_width) return;
	if (x2 >= dev->_width) x2=dev->_width-1;
	if (y1 >= dev->_height) return;
	if (y2 >= dev->_height) y2=dev->_height-1;

	// ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	x1 = x1 + dev->_offsetx;
	x2 = x2 + dev->_offsetx;
	y1 = y1 + dev->_offsety;
	y2 = y2 + dev->_offsety;

	
	uint16_t line_size = x2-x1+1;
	uint8_t * buff = heap_caps_malloc(line_size*2, MALLOC_CAP_DMA) ;
	int index = 0;
	for(int i=0;i<line_size;i++) {
		buff[index++] = (color >> 8) & 0xFF;
		buff[index++] = color & 0xFF;
	}

	st77xx_command(dev, 0x2A);	// set column(x) address
	st77xx_addr(dev, x1, x2);
	st77xx_command(dev, 0x2B);	// set Page(y) address
	st77xx_addr(dev, y1, y2);
	st77xx_command(dev, 0x2C);	//	Memory Write

	gpio_set_level( dev->_dc, SPI_Data_Mode );
	for(int i=y1;i<=y2;i++){
		spi_write_bytes( dev->spi_dev, (uint8_t *)buff, line_size*2);
	}

	free(buff) ;
}
