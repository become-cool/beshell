#include "CS1237.hpp"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "qjs_utils.h"
#include "soc/gpio_num.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace std ;

namespace be::driver::adc {

    #define cs1237_dout_set_input()         \
            gpio_reset_pin(this->dout) ;    \
            gpio_set_direction(this->dout, GPIO_MODE_INPUT) ;

    #define cs1237_dout_set_output(val)     \
            gpio_reset_pin(this->dout) ;    \
            gpio_set_direction(this->dout, GPIO_MODE_OUTPUT) ; \
            gpio_set_level(this->dout, val) ;

    #define DOUT_SET()    gpio_set_level(this->dout, 1)
    #define DOUT_CLR()    gpio_set_level(this->dout, 0)
    #define SCLK_SET()    gpio_set_level(this->sclk, 1)
    #define SCLK_CLR()    gpio_set_level(this->sclk, 0)
    #define READ_DOUT_STATUS() gpio_get_level(this->dout)
    #define TX_BIT_1        \
            SCLK_SET();     \
            delay_2us(1);   \
            DOUT_SET();     \
            SCLK_CLR();     \
            delay_2us(1);
    #define TX_BIT_0        \
            SCLK_SET();     \
            delay_2us(1);   \
            DOUT_CLR();     \
            SCLK_CLR();     \
            delay_2us(1);
    

    static void delay_2us(int cnt) {
        esp_rom_delay_us(2*cnt) ;
    }

    static void bsp_delay_ms(int ms) {
        vTaskDelay(ms/portTICK_PERIOD_MS) ;
    }


    DEFINE_NCLASS_META(CS1237, NativeClass)
    std::vector<JSCFunctionListEntry> CS1237::methods = {
        JS_CFUNC_DEF("setup", 0, CS1237::setup),
        JS_CFUNC_DEF("readConfig", 0, CS1237::readConfig),
        JS_CFUNC_DEF("writeConfig", 0, CS1237::writeConfig),
        JS_CFUNC_DEF("ready", 0, CS1237::ready),
        JS_CFUNC_DEF("read", 0, CS1237::read),
        JS_CFUNC_DEF("powerDown", 0, CS1237::powerDown),
    } ;

    CS1237::CS1237(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {
    }

    JSValue CS1237::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new CS1237(ctx) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    
    void CS1237::setPin(gpio_num_t dout, gpio_num_t sclk) {
        this->dout = dout ;
        this->sclk = sclk ;
        
        gpio_reset_pin(this->sclk) ;
        gpio_set_direction(this->sclk, GPIO_MODE_OUTPUT) ;
        gpio_set_level(this->sclk,0) ;

        cs1237_dout_set_input()

    }
    uint8_t CS1237::readConfig() {
        
        unsigned char  i;
        unsigned char  reg_data=0;
        unsigned int count_i=0;


        cs1237_dout_set_output(1);
        DOUT_SET(); //OUT????????
        cs1237_dout_set_input();
        SCLK_CLR();//???????
        while(1==READ_DOUT_STATUS())//???��??????????
        {
            bsp_delay_ms(1);
            count_i++;
            if(count_i > 300)
            {
                cs1237_dout_set_output(1);
                SCLK_SET();	// CLK=1;
                DOUT_SET();	// OUT=1;
                return 1;//?????????????????
            }
        }

        for(i=0; i<29; i++) // ??????1??29?????
        {
            SCLK_SET();	// CLK=1;
            delay_2us(1);
            SCLK_CLR();	// CLK=0;
            delay_2us(1);
        }

        cs1237_dout_set_output(1);
        
        TX_BIT_1    // 30
        TX_BIT_0    // 31
        TX_BIT_1    // 32
        TX_BIT_0    // 33
        TX_BIT_1    // 34
        TX_BIT_1    // 35
        TX_BIT_0    // 36
        TX_BIT_0    // 37

        reg_data=0;
        cs1237_dout_set_input();
        for(i=0; i<8; i++) // ??38 - 45????????????????
        {
            SCLK_SET();	// CLK=1;
            delay_2us(1);
            SCLK_CLR();	// CLK=0;
            delay_2us(1);
            reg_data <<= 1;
            if(1==READ_DOUT_STATUS())
                reg_data++;
        }

        //??46??????
        SCLK_SET();	// CLK=1;
        delay_2us(1);
        SCLK_CLR();	// CLK=0;
        delay_2us(1);

        cs1237_dout_set_output(1);
        DOUT_SET(); //OUT????????

        return reg_data;
    }
    
    void CS1237::writeConfig(uint8_t reg_data) {
        unsigned char i;
    //	unsigned char dat;
        unsigned int count_i=0;//????????

    //	dat = 0X28;//��??????? ???REF ???640HZ PGA=64 ???A 0X28

        cs1237_dout_set_output(1);
        DOUT_SET();//OUT????????
        cs1237_dout_set_input();
        SCLK_CLR();//???????
        delay_2us(1);
        while(1==READ_DOUT_STATUS())//???CS237?????
        {
            bsp_delay_ms(2);
            count_i++;
            if(count_i > 300)
            {
                cs1237_dout_set_output(1);
                DOUT_SET();//OUT????????
                SCLK_SET();//CLK????????
                return;//?????????????????
            }
        }
        for(i=0; i<29; i++) // 1 - 29
        {
            SCLK_SET();//CLK=1;
            delay_2us(1);
            SCLK_CLR();//CLK=0;
            delay_2us(1);
        }
        cs1237_dout_set_output(1);

        TX_BIT_1  // 30
        TX_BIT_1  // 31
        TX_BIT_0  // 32
        TX_BIT_0  // 33
        TX_BIT_1  // 34
        TX_BIT_0  // 35
        TX_BIT_1  // 36

        //37     ��????0x65
        SCLK_SET();//CLK=1;
        delay_2us(1);
        SCLK_CLR();//CLK=0;
        delay_2us(1);

        for(i=0; i<8; i++) // 38 - 45?????????��8��????
        {
            SCLK_SET();//CLK=1;
            delay_2us(1);
            if(reg_data&0x80)
                DOUT_SET();//OUT = 1
            else
                DOUT_CLR();
            reg_data <<= 1;
            SCLK_CLR();//CLK=0;
            delay_2us(1);
        }
        DOUT_SET();//OUT = 1
        SCLK_SET();//CLK=1;
        delay_2us(1);
        SCLK_CLR();//CLK=0;
        delay_2us(1);
    }
    bool CS1237::read(int * adc_data) {
        
        unsigned char i;
        unsigned long dat=0;//��ȡ��������
        unsigned int count_i=0;//�����ʱ��
        unsigned int count_y=0;//�����ʱ��
        signed long temp;

        //dout����Ϊ����
        cs1237_dout_set_input();
        if(1==READ_DOUT_STATUS()) {
            return false ;
        }
        // count_i = 0;
        // while(1==READ_DOUT_STATUS())//�ȴ�оƬ׼��������
        // {
        //     delay_2us(1);
        //     count_i++;
        //     if(count_i > 300)
        //     {
        //         cs1237_dout_set_input();
        //         return false;//��ʱ����ֱ���˳�����
        //     }
        // }
        count_i = 0;
        count_y = 0;
        while(count_i<=5)
        {
            if(0==READ_DOUT_STATUS()) {
                count_i+=1;
            }
            else {
                count_y+=1;
            }

            if(count_y>3) {
                cs1237_dout_set_input();
                return false;//��ʱ����ֱ���˳�����
            }
        }

        dat=0;
        for(i=0; i<24; i++) //��ȡ24λ��Чת��
        {
            SCLK_SET();	// CLK=1;
            delay_2us(1);
            dat <<= 1;
            if(1==READ_DOUT_STATUS())
                dat ++;
            SCLK_CLR();	// CLK=0;
            delay_2us(1);
        }

        for(i=0; i<3; i++) //����ǰ���ʱ�� ����3��ʱ��
        {
            SCLK_SET();	// CLK=1;
            delay_2us(1);
            SCLK_CLR();	// CLK=0;
            delay_2us(1);
        }

        cs1237_dout_set_input();

        if(dat&0x00800000)// �ж��Ǹ��� ���λ24λ�Ƿ���λ
        {
            temp=-(((~dat)&0x007FFFFF) + 1);// �����Դ��
        }
        else
            temp=dat; // �����Ĳ������Դ��

        *adc_data = temp;

        return true ;
    }
    bool CS1237::ready() {
        return READ_DOUT_STATUS()==0;
    }
    void CS1237::powerDown(bool down) {
        return ;
    }

    #define CHECK_PIN \
        if(that->dout==GPIO_NUM_NC || that->sclk==GPIO_NUM_NC) { \
            JSTHROW("Pin not set") ; \
        }

    JSValue CS1237::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        THIS_NCLASS(CS1237, that)
        gpio_num_t GET_INT_PROP(argv[0], "dout", dout, gpio_num_t, )
        gpio_num_t GET_INT_PROP(argv[0], "sclk", sclk, gpio_num_t, )
        that->setPin(dout,sclk) ;
        return JS_UNDEFINED ;
    }

    JSValue CS1237::readConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(CS1237, that)
        CHECK_PIN
        return JS_NewUint32(ctx, that->readConfig()) ;
    }
    JSValue CS1237::writeConfig(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(CS1237, that)
        CHECK_PIN
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, value)
        that->writeConfig(value) ;
        return JS_UNDEFINED ;
    }
    JSValue CS1237::ready(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(CS1237, that)
        CHECK_PIN
        return that->ready()? JS_TRUE: JS_FALSE ;
    }
    JSValue CS1237::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(CS1237, that)
        CHECK_PIN
        int value = 0 ;
        if(!that->read(&value)) {
            JSTHROW("Read failed") ;
        }
        return JS_NewInt32(ctx, value) ;
    }
    JSValue CS1237::powerDown(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(CS1237, that)
        CHECK_PIN
        return JS_UNDEFINED ;
    }
}