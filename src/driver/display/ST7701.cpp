#include "ST7701.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3


using namespace std ;

namespace be::driver::display {

    DEFINE_NCLASS_META(ST7701, RGB565)

        
    // #define GPIO_LCD_RST    (GPIO_NUM_NC)
    // #define GPIO_LCD_CS     (GPIO_NUM_0)
    // #define GPIO_LCD_SDA    (GPIO_NUM_12)
    // #define GPIO_LCD_SCK    (GPIO_NUM_14)

    #define LCD_CS_Clr()    gpio_set_level(pin_cs, 0)
    #define LCD_CS_Set()    gpio_set_level(pin_cs, 1)
    #define LCD_SCK_Clr()   gpio_set_level(pin_sck, 0)
    #define LCD_SCK_Set()   gpio_set_level(pin_sck, 1)
    #define LCD_SDA_Clr()   gpio_set_level(pin_sda, 0)
    #define LCD_SDA_Set()   gpio_set_level(pin_sda, 1)

    ST7701::ST7701(JSContext * ctx, JSValue _jsobj, uint16_t width, uint16_t height)
        : RGB565(ctx, build(ctx, _jsobj), width, height)
    {}

    JSValue ST7701::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new ST7701(ctx) ;
        obj->self = std::shared_ptr<ST7701> (obj) ;
        return obj->jsobj ;
    }

    std::vector<JSCFunctionListEntry> ST7701::methods = {
        JS_CFUNC_DEF("setup", 0, ST7701::setup),
    } ;

    JSValue ST7701::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(ST7701,that)
        ASSERT_ARGC(1)

        if( JS_IsException(RGB565::setup(ctx, this_val, argc, argv)) ) {
            return JS_EXCEPTION ;
        }

        JSValue pin = JS_GetPropertyStr(ctx, argv[0], "pin");
        if(!JS_IsObject(pin)){
            JSTHROW("missing pin property")
        }

        GET_INT32_PROP(pin, "sda", that->pin_sda, )
        GET_INT32_PROP(pin, "sck", that->pin_sck, )
        GET_INT32_PROP(pin, "cs", that->pin_cs, )
        GET_INT32_PROP_OPT(pin, "rst", that->pin_rst, GPIO_NUM_NC)

        // dn4(that->pin_sda,that->pin_sck,that->pin_cs,that->pin_rst)
        
        gpio_config_t io_conf = {
            .pin_bit_mask = 1ULL << that->pin_sda, 
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE
        };
        gpio_config(&io_conf);
        gpio_set_level(that->pin_sda, 1);
        gpio_set_pull_mode(that->pin_sda, GPIO_PULLUP_ONLY);

        io_conf.pin_bit_mask = 1ULL << that->pin_sck;
        gpio_config(&io_conf);
        gpio_set_level(that->pin_sck, 1);
        gpio_set_pull_mode(that->pin_sck, GPIO_PULLUP_ONLY);

        io_conf.pin_bit_mask = 1ULL << that->pin_cs;
        gpio_config(&io_conf);
        gpio_set_level(that->pin_cs, 1);
        gpio_set_pull_mode(that->pin_sck, GPIO_PULLUP_ONLY);

        that->initReg() ;

        return JS_UNDEFINED ;
    }


    void ST7701::write9b(uint16_t data)
    {
        uint8_t i;
        LCD_CS_Clr();
        for(i = 0; i < 9; i++)
        {
            LCD_SCK_Clr();
            if(data & 0x100)   LCD_SDA_Set();
            else               LCD_SDA_Clr();
            LCD_SCK_Set();
            data <<= 1;
        }
        LCD_CS_Set();;
    }

    void ST7701::writeCmd(uint8_t cmd)
    {
        uint16_t temp = 0;
        temp = temp | cmd;
        write9b(temp);
    }

    void ST7701::writeData(uint8_t data)
    {
        uint16_t temp = 0x100;
        temp = temp | data;
        write9b(temp);
    }

    void ST7701::initReg(void)
    {
        //PAGE1
        writeCmd(0xFF);    
        writeData(0x77);
        writeData(0x01);
        writeData(0x00);
        writeData(0x00);
        writeData(0x10);

        writeCmd(0xC0);    
        writeData(0x3B);
        writeData(0x00);

        writeCmd(0xC1);    
        writeData(0x0D);
        writeData(0x02);

        writeCmd(0xC2);    
        writeData(0x31);
        writeData(0x05);

        writeCmd(0xCd);
        writeData(0x08);

        writeCmd(0xB0);    
        writeData(0x00); //Positive Voltage Gamma Control
        writeData(0x11);
        writeData(0x18);
        writeData(0x0E);
        writeData(0x11);
        writeData(0x06);
        writeData(0x07);
        writeData(0x08);
        writeData(0x07);
        writeData(0x22);
        writeData(0x04);
        writeData(0x12);
        writeData(0x0F);
        writeData(0xAA);
        writeData(0x31);
        writeData(0x18);

        writeCmd(0xB1);    
        writeData(0x00); //Negative Voltage Gamma Control
        writeData(0x11);
        writeData(0x19);
        writeData(0x0E);
        writeData(0x12);
        writeData(0x07);
        writeData(0x08);
        writeData(0x08);
        writeData(0x08);
        writeData(0x22);
        writeData(0x04);
        writeData(0x11);
        writeData(0x11);
        writeData(0xA9);
        writeData(0x32);
        writeData(0x18);

        //PAGE1
        writeCmd(0xFF);    
        writeData(0x77);
        writeData(0x01);
        writeData(0x00);
        writeData(0x00);
        writeData(0x11);

        writeCmd(0xB0);    writeData(0x60); //Vop=4.7375v
        writeCmd(0xB1);    writeData(0x32); //VCOM=32
        writeCmd(0xB2);    writeData(0x07); //VGH=15v
        writeCmd(0xB3);    writeData(0x80);
        writeCmd(0xB5);    writeData(0x49); //VGL=-10.17v
        writeCmd(0xB7);    writeData(0x85);
        writeCmd(0xB8);    writeData(0x21); //AVDD=6.6 & AVCL=-4.6
        writeCmd(0xC1);    writeData(0x78);
        writeCmd(0xC2);    writeData(0x78);

        writeCmd(0xE0);    
        writeData(0x00);
        writeData(0x1B);
        writeData(0x02);

        writeCmd(0xE1);   
        writeData(0x08);
        writeData(0xA0);
        writeData(0x00);
        writeData(0x00);
        writeData(0x07);
        writeData(0xA0);
        writeData(0x00);
        writeData(0x00);
        writeData(0x00);
        writeData(0x44);
        writeData(0x44);

        writeCmd(0xE2);    
        writeData(0x11);
        writeData(0x11);
        writeData(0x44);
        writeData(0x44);
        writeData(0xED);
        writeData(0xA0);
        writeData(0x00);
        writeData(0x00);
        writeData(0xEC);
        writeData(0xA0);
        writeData(0x00);
        writeData(0x00);

        writeCmd(0xE3);    
        writeData(0x00);
        writeData(0x00);
        writeData(0x11);
        writeData(0x11);

        writeCmd(0xE4);    
        writeData(0x44);
        writeData(0x44);

        writeCmd(0xE5);    
        writeData(0x0A);
        writeData(0xE9);
        writeData(0xD8);
        writeData(0xA0);
        writeData(0x0C);
        writeData(0xEB);
        writeData(0xD8);
        writeData(0xA0);
        writeData(0x0E);
        writeData(0xED);
        writeData(0xD8);
        writeData(0xA0);
        writeData(0x10);
        writeData(0xEF);
        writeData(0xD8);
        writeData(0xA0);

        writeCmd(0xE6);   
        writeData(0x00);
        writeData(0x00);
        writeData(0x11);
        writeData(0x11);

        writeCmd(0xE7);    
        writeData(0x44);
        writeData(0x44);

        writeCmd(0xE8);    
        writeData(0x09);
        writeData(0xE8);
        writeData(0xD8);
        writeData(0xA0);
        writeData(0x0B);
        writeData(0xEA);
        writeData(0xD8);
        writeData(0xA0);
        writeData(0x0D);
        writeData(0xEC);
        writeData(0xD8);
        writeData(0xA0);
        writeData(0x0F);
        writeData(0xEE);
        writeData(0xD8);
        writeData(0xA0);

        writeCmd(0xEB);    
        writeData(0x02);
        writeData(0x00);
        writeData(0xE4);
        writeData(0xE4);
        writeData(0x88);
        writeData(0x00);
        writeData(0x40);

        writeCmd(0xEC);    
        writeData(0x3C);
        writeData(0x00);

        writeCmd(0xED);    
        writeData(0xAB);
        writeData(0x89);
        writeData(0x76);
        writeData(0x54);
        writeData(0x02);
        writeData(0xFF);
        writeData(0xFF);
        writeData(0xFF);
        writeData(0xFF);
        writeData(0xFF);
        writeData(0xFF);
        writeData(0x20);
        writeData(0x45);
        writeData(0x67);
        writeData(0x98);
        writeData(0xBA);

        writeCmd(0x36);    
        writeData(0x00);

        //-----------VAP & VAN---------------
        writeCmd(0xFF);    
        writeData(0x77);
        writeData(0x01);
        writeData(0x00);
        writeData(0x00);
        writeData(0x13);

        writeCmd(0xE5);    
        writeData(0xE4);

        writeCmd(0xFF);
        writeData(0x77);
        writeData(0x01);
        writeData(0x00);
        writeData(0x00);
        writeData(0x00);

        writeCmd(0x3A);   //0x70 RGB888, 0x60 RGB666, 0x50 RGB565
        writeData(0x60);

        writeCmd(0x21);   //Display Inversion On

        writeCmd(0x11);   //Sleep Out
        vTaskDelay(pdMS_TO_TICKS(100));

        writeCmd(0x29);   //Display On
        vTaskDelay(pdMS_TO_TICKS(50));
    }

}

#endif