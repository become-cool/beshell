#include "module_driver.h"
// #include "driver_camera.h"
// #include "driver_stepper_mcpwm.h"
#include "driver_stepper_timer.h"
#include "driver_rmt.h"
#include "driver_rainbow.h"
#include "driver_common.h"

#include "module_fs.h"
#include "utils.h"

#include "esp_system.h"
#include "driver/sdspi_host.h"
#include "driver/sdmmc_host.h"
#include "vfs_fat_internal.h"
#include "sdmmc_cmd.h"
#include "diskio_sdmmc.h"
#include "diskio_impl.h"
#include "driver/i2s.h"
#include "module_serial.h"

    

#define TAG "driver"

/**
 * 
 * @param spi num 
 * @param cs 
 * @param path 
 * @param khz = 20000 
 */
static JSValue js_driver_mount_sd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    CHECK_ARGC(3)

    ARGV_TO_UINT8(0, spi)
    ARGV_TO_UINT8(1, cs)

    JS2VSFPath(mntPath, argv[2]) ;

    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = cs;
    slot_config.host_id = spi;

    sdmmc_card_t* card;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = spi ;

    if(argc>3 && !JS_IsUndefined(argv[3])){
        ARGV_TO_UINT32(3, khz)
        host.max_freq_khz = khz ;
    }

    dn2(spi, cs)

    ret = esp_vfs_fat_sdspi_mount(mntPath, &host, &slot_config, &mount_config, &card);

    free(mntPath) ;

    if (ret == ESP_OK) {
        dn(card->max_freq_khz)
        sdmmc_card_print_info(stdout, card);
    }
    else {
        dn(ret)
        if (ret == ESP_FAIL) {
            THROW_EXCEPTION("Failed to mount the card .\n")
        } else {
            THROW_EXCEPTION("Failed to initialize the card (%s).\n",esp_err_to_name(ret))
        }
    }


    return JS_TRUE ;
}




/***************参数定义***************/
#define STATEconfirm		0x0C			//状态机确认 回读STATEconfirm的寄存值确认IC正常工作状态
#define NORMAL_I2S			0x00
#define NORMAL_LJ			0x01
#define NORMAL_DSPA			0x03
#define NORMAL_DSPB			0x07
#define Format_Len24		0x00
#define Format_Len20		0x01
#define Format_Len18		0x02
#define Format_Len16		0x03
#define Format_Len32		0x04

#define VDDA_3V3			0x00
#define VDDA_1V8			0x01
#define MCLK_PIN			0x00
#define SCLK_PIN			0x01
/***************参数定义***************/

/**************************************************/
#define MSMode_MasterSelOn	0				//产品主从模式选择:默认选择0为SlaveMode,打开为1选择MasterMode
#define Ratio 				64				//实际Ratio=MCLK/LRCK比率,需要和实际时钟比例匹配
#define Format 				NORMAL_I2S		//数据格式选择,需要和实际时序匹配
#define Format_Len			Format_Len16	//数据长度选择,需要和实际时序匹配
#define SCLK_DIV			4				//SCLK分频选择:(选择范围1~128),SCLK=MCLK/SCLK_DIV
#define SCLK_INV			0				//默认对齐方式为下降沿,1为上升沿对齐,需要和实际时序匹配
#define MCLK_SOURCE			MCLK_PIN		//是否硬件没接MCLK需要用SCLK当作MCLK
#define EQ7bandOn			0				//开启七段EQ下配置1，三段或者不使用配置0
#define VDDA_VOLTAGE		VDDA_3V3		//模拟电压选择为3V3还是1V8,需要和实际硬件匹配
#define DAC_Volume			191				//DAC数字增益:(选择范围0~255),191:0DB,±0.5dB/Step
#define DACHPModeOn			0				//输出负载开启HP驱动:默认选择关闭0,打开为1

/**************************************************/

#define I2CWRNBYTE_CODEC(r,b) if(i2c_write_byte(i2c,addr,r,b)!=ESP_OK) { return JS_FALSE ;}


/**
 * 
 * @param i2c num 
 * @param addr 
 */
static JSValue js_driver_ES8156_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)

    ARGV_TO_UINT8(0, i2c)
    ARGV_TO_UINT8(0, addr)
    if(i2c!=0 &&i2c!=1) {
        THROW_EXCEPTION("arg %s must be 0 or 1", "i2c")
    }
    
	I2CWRNBYTE_CODEC(0x02,(MCLK_SOURCE<<7) + (SCLK_INV<<4) +  (EQ7bandOn<<3) + 0x04 + MSMode_MasterSelOn);	
	
    I2CWRNBYTE_CODEC(0x20,0x2A);	
    I2CWRNBYTE_CODEC(0x21,0x3C);	
    I2CWRNBYTE_CODEC(0x22,0x02);	
    I2CWRNBYTE_CODEC(0x24,0x07);	
    I2CWRNBYTE_CODEC(0x23,0x40 + (0x30*VDDA_VOLTAGE));	

	I2CWRNBYTE_CODEC(0x0A,0x01);	
	I2CWRNBYTE_CODEC(0x0B,0x01);	
	I2CWRNBYTE_CODEC(0x11,NORMAL_I2S + (Format_Len<<4));
	I2CWRNBYTE_CODEC(0x14,DAC_Volume);
    
    // if SCLK/LRCLK == 64 :
    I2CWRNBYTE_CODEC(0x01,0xA1);//64 Ratio(MCLK/LRCK)	
    I2CWRNBYTE_CODEC(0x09,0x02);//64 Ratio(MCLK/LRCK)	
    I2CWRNBYTE_CODEC(0x03,0x00);//LRCK H
    I2CWRNBYTE_CODEC(0x04,0x40);//LRCK=MCLK/64
    I2CWRNBYTE_CODEC(0x05,SCLK_DIV);//BCLK=MCLK/2

	I2CWRNBYTE_CODEC(0x0D,0x14);
	I2CWRNBYTE_CODEC(0x18,0x00);
	I2CWRNBYTE_CODEC(0x08,0x3F);
	I2CWRNBYTE_CODEC(0x00,0x02);
	I2CWRNBYTE_CODEC(0x00,0x03);
	I2CWRNBYTE_CODEC(0x25,0x20);

    return JS_TRUE ;
}

// void js_driver_ES5168_set_mode() {

// 	if(DACHPModeOn == 0)//接PA或者LOUT,内部不开启负载驱动,省功耗
//     {
//         I2CWRNBYTE_CODEC(0x20,0x2A);	
//         I2CWRNBYTE_CODEC(0x21,0x3C);	
//         I2CWRNBYTE_CODEC(0x22,0x02);	
//         I2CWRNBYTE_CODEC(0x24,0x07);	
//         I2CWRNBYTE_CODEC(0x23,0x40 + (0x30*VDDA_VOLTAGE));	
//     }
// 	if(DACHPModeOn == 1)//接耳机,内部开启负载驱动,支持耳机输出
//     {
//         I2CWRNBYTE_CODEC(0x20,0x16);	
//         I2CWRNBYTE_CODEC(0x21,0x3F);	
//         I2CWRNBYTE_CODEC(0x22,0x0A);	
//         I2CWRNBYTE_CODEC(0x24,0x01);	
//         I2CWRNBYTE_CODEC(0x23,0xCA + (0x30*VDDA_VOLTAGE));	
//     }
// }

void * audio_dma = NULL;

static JSValue js_driver_hold_dma(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_STRING(0, type)
    ARGV_TO_UINT32(1, size)

    void ** p ;
    if( strcmp(type,"audio")==0 ) {
        p = & audio_dma ;
    }
    else {
        THROW_EXCEPTION("unknow type: %s", type)
    }

    JS_FreeCString(ctx,type) ;

    if((*p)==NULL) {
#ifdef PLATFORM_ESP32
        (*p) = heap_caps_malloc(size, MALLOC_CAP_DMA) ;
#else
        (*p) = 0xFF ;
#endif
    }

    return (*p)!=0? JS_TRUE : JS_FALSE ;
}
static JSValue js_driver_release_dma(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING(0, type)

    void ** p ;
    if( strcmp(type,"audio")==0 ) {
        p = & audio_dma ;
    }
    else {
        THROW_EXCEPTION("unknow type: %s", type)
    }
    JS_FreeCString(ctx,type) ;

    if(*p) {
        free(*p) ;
        *p = NULL ;
    }

    return JS_UNDEFINED ;
}


void be_module_driver_init() {
    be_module_driver_camera_init() ;
    // be_module_driver_stepper_mcpwm_init() ;
    be_module_driver_stepper_timer_init() ;
    be_module_driver_rainbow_init() ;
}

void be_module_driver_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue driver = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "driver", driver);

    JS_SetPropertyStr(ctx, driver, "mountSD", JS_NewCFunction(ctx, js_driver_mount_sd, "mountSD", 1));
    JS_SetPropertyStr(ctx, driver, "ES8156Setup", JS_NewCFunction(ctx, js_driver_ES8156_setup, "ES8156Setup", 1));
    JS_SetPropertyStr(ctx, driver, "holdDMA", JS_NewCFunction(ctx, js_driver_hold_dma, "holdDMA", 1));
    JS_SetPropertyStr(ctx, driver, "releaseDMA", JS_NewCFunction(ctx, js_driver_release_dma, "releaseDMA", 1));

    be_module_driver_camera_require(ctx, driver) ;    

    // be_module_driver_stepper_mcpwm_require(ctx, driver) ;
    be_module_driver_stepper_timer_require(ctx, driver) ;
    be_module_driver_rmt_require(ctx, driver) ;
    be_module_driver_rainbow_require(ctx, driver) ;

    be_module_driver_common_require(ctx, driver) ;

    JS_FreeValue(ctx, beapi);
}

void be_module_driver_loop(JSContext *ctx) {}

void be_module_driver_reset(JSContext *ctx) {
    be_module_driver_camera_reset(ctx) ;   
    // be_module_driver_stepper_mcpwm_reset(ctx) ;
    be_module_driver_stepper_timer_reset(ctx) ;
}

