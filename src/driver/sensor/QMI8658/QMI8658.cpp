#include "QMI8658.hpp"
#include "qmi8658-driver.hpp"
#include "imualgo_axis9.h"
#include "quickjs/quickjs.h"
#include <cmath>

using namespace std ;

static be::I2C * i2cbus = nullptr ;
    
int mx_i2c1_write(uint8_t addr, uint8_t reg, uint8_t value) {

    int ret = i2cbus->write<uint8_t,uint8_t>(addr>>1, reg, value) ;

    // printf("mx_i2c1_write(addr:%d, reg: %d, value: %d)\n", ret, addr, reg, value) ;

    return ret? 1: 0  ;
}
int mx_i2c1_read(uint8_t addr, uint8_t reg, uint8_t * buffer, int len) {

    int ret = i2cbus->read<uint8_t>(addr>>1, reg, buffer, len) ;

    // printf("mx_i2c1_read(addr:%d, reg: %d, len: %d, value[0]=%d)\n", addr, reg, len, buffer[0]) ;

    return ret? 1: 0  ;
}



extern QST_Filter gyro_filter;
extern QST_Filter accel_filter;

namespace be::driver::sensor {


    DEFINE_NCLASS_META(QMI8658, I2CDevice)
    std::vector<JSCFunctionListEntry> QMI8658::methods = {
        JS_CFUNC_DEF("setup", 0, QMI8658::setup),
        JS_CFUNC_DEF("readXYZ", 0, QMI8658::readXYZ),
        JS_CFUNC_DEF("startReceive", 0, QMI8658::startReceive),
        JS_CFUNC_DEF("stopReceive", 0, QMI8658::stopReceive),
    } ;

    QMI8658::QMI8658(JSContext * ctx, JSValue _jsobj)
        : I2CDevice(ctx,build(ctx,_jsobj))
    {}

    JSValue QMI8658::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new QMI8658(ctx) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    JSValue QMI8658::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if( I2CDevice::setup(ctx, this_val, argc, argv)==JS_EXCEPTION ){
            return JS_EXCEPTION ;
        }
        THIS_NCLASS(QMI8658, that)

        i2cbus = that->i2c ;

        if( !qmi8658_init() ) {
            JSTHROW("QMI8658 init failed") ;
        }
        
        set_cutoff_frequency(100, 1, &gyro_filter);      //第一个参数为频率100hz，与算法库调用周期有关，如例程中算法库调用周期为10ms 
        set_cutoff_frequency(100, 2, &accel_filter);

        printf("accel_filter: %f, %f, %f, %f, %f\n", accel_filter.a1, accel_filter.a2, accel_filter.b0, accel_filter.b1, accel_filter.b2) ;
        printf("gyro_filter: %f, %f, %f, %f, %f\n", gyro_filter.a1, gyro_filter.a2, gyro_filter.b0, gyro_filter.b1, gyro_filter.b2) ;

        init_state_recognition(qmi8658_read_reg);

        return JS_UNDEFINED ;
    }
    JSValue QMI8658::readXYZ(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        float acc[3], gyro[3] ;
        qmi8658_read_xyz(acc,gyro) ;

        JSValue arr = JS_NewArray(ctx) ;
        JS_SetPropertyUint32(ctx,arr,0,JS_NewFloat64(ctx,acc[0])) ;
        JS_SetPropertyUint32(ctx,arr,1,JS_NewFloat64(ctx,acc[1])) ;
        JS_SetPropertyUint32(ctx,arr,2,JS_NewFloat64(ctx,acc[2])) ;
        JS_SetPropertyUint32(ctx,arr,3,JS_NewFloat64(ctx,gyro[0])) ;
        JS_SetPropertyUint32(ctx,arr,4,JS_NewFloat64(ctx,gyro[1])) ;
        JS_SetPropertyUint32(ctx,arr,5,JS_NewFloat64(ctx,gyro[2])) ;

        return arr ;
    }
    
    void QMI8658::task_listen2(QMI8658 * that) {
        printf("task_listen()\n");

        QST_Filter_Buffer gyro_buf[3];
        QST_Filter_Buffer accel_buf[3];

        float accl[3],gyro[3];
        float euler_angle[3] = {0, 0, 0};
    
        float accel_correct[3] = {0, 0, 0};
        float gyro_correct[3] = {0, 0, 0};
        float dt = 0.01f;
        float quater[4] = {0, 0, 0, 0};
        float line_acc[3] = {0, 0, 0};

        while(1) {


            qmi8658_read_xyz(accl, gyro);
            // printf("%f, %f, %f, %f, %f, %f\n", accl[0], accl[1], accl[2], gyro[0], gyro[1], gyro[2]) ;
            
            accel_correct[0] = Filter_Apply(accl[0],&accel_buf[0],&accel_filter);
            accel_correct[1] = Filter_Apply(accl[1],&accel_buf[1],&accel_filter);
            accel_correct[2] = Filter_Apply(accl[2],&accel_buf[2],&accel_filter);
            gyro_correct[0] = Filter_Apply(gyro[0],&gyro_buf[0],&gyro_filter);
            gyro_correct[1] = Filter_Apply(gyro[1],&gyro_buf[1],&gyro_filter);
            gyro_correct[2] = Filter_Apply(gyro[2],&gyro_buf[2],&gyro_filter);

            qst_fusion_update(accel_correct, gyro_correct, &dt, euler_angle, quater, line_acc);

            printf("pitch:%.2f, roll:%.2f, yaw:%.2f \n", euler_angle[0], euler_angle[1], euler_angle[2]); ;
                  
            vTaskDelay((1000/125)/portTICK_PERIOD_MS);
        }
    
    }


    #define SAMPLE_RATE   125    // 100Hz采样频率
    #define CALIBRATE_CNT 100    // 校准采样次数
    #define ALPHA         0.98f  // 互补滤波系数
    
    // 全局状态变量
    typedef struct {
        float accel[3];     // 加速度计数据(x,y,z)
        float gyro[3];      // 陀螺仪数据(roll,pitch,yaw)
        float angle[3];     // 融合后姿态角(roll,pitch,yaw)
        float gyro_bias[3]; // 陀螺仪零偏
        uint32_t timestamp; // 时间戳(单位ms)
        int calibration_done; // 校准完成标志
        int rotation_count; // 旋转圈数累计
    } IMU_State;

    void QMI8658::task_listen(QMI8658 * that) {
        printf("task_listen()\n");
        // float accl[3],gyro[3];
        // float euler_angle[3] = {0, 0, 0};
    
        IMU_State imu = {0};
        float last_yaw = 0;
        

        while(1) {    
            

            // 读取原始传感器数据
            qmi8658_read_xyz(imu.accel, imu.gyro);
            // printf("%0.3f, %0.3f, %0.3f, %0.3f, %0.3f, %0.3f\n", imu.accel[0], imu.accel[1], imu.accel[2], imu.gyro[0], imu.gyro[1], imu.gyro[2]) ;
            // printf("%+02.3f, %+02.3f, %+02.3f\n", imu.gyro[0], imu.gyro[1], imu.gyro[2]) ;
            
            // vTaskDelay(8/portTICK_PERIOD_MS);
            // continue;
            
            // 2. 校准阶段（设备需静止放置）
            if(!imu.calibration_done) {
                static int calib_cnt = 0;
                for(int i=0; i<3; i++) 
                    imu.gyro_bias[i] += imu.gyro[i];
                
                if(++calib_cnt >= CALIBRATE_CNT) {
                    for(int i=0; i<3; i++)
                        imu.gyro_bias[i] /= CALIBRATE_CNT;
                    imu.calibration_done = 1;
                            printf("陀螺仪零偏: %.2f, %.2f, %.2f\n", imu.gyro_bias[0], imu.gyro_bias[1], imu.gyro_bias[2]);
                }
                vTaskDelay(10/portTICK_PERIOD_MS); // 等待10ms
                continue;
            }

            // 3. 计算时间间隔（假设精确计时）
            uint32_t current_ticks = xTaskGetTickCount();
            uint32_t delta_ticks = current_ticks - imu.timestamp;
            float dt = (float)delta_ticks / configTICK_RATE_HZ; // 直接使用系统时钟频率
            
            // 调试输出
            printf("时间统计: 当前=%lu 上次=%lu 间隔=%lu dt=%.3f秒\n", current_ticks, imu.timestamp, delta_ticks, dt);

            imu.timestamp = current_ticks;

            // continue;
        

            // 4. 互补滤波处理
            // 加速度计计算瞬时角度
            float acc_roll = atan2f(imu.accel[1], imu.accel[2]) * 180/M_PI;
            float acc_pitch = atan2f(-imu.accel[0], 
                sqrtf(imu.accel[1]*imu.accel[1] + imu.accel[2]*imu.accel[2])) * 180/M_PI;
            
            // 陀螺仪积分（扣除零偏）
            imu.angle[0] += (imu.gyro[0] ) * dt; // roll
            imu.angle[1] += (imu.gyro[1]) * dt; // pitch
            imu.angle[2] += (imu.gyro[2]) * dt; // yaw
            
            // printf("gyro:%.2f, %.2f, %.2f, angle: %0.03f, %0.03f, %0.03f, dt=%.3f\n", imu.gyro[0], imu.gyro[1], imu.gyro[2], imu.angle[0], imu.angle[1], imu.angle[2], dt);

            // 融合计算
            imu.angle[0] = ALPHA*imu.angle[0] + (1-ALPHA)*acc_roll;
            imu.angle[1] = ALPHA*imu.angle[1] + (1-ALPHA)*acc_pitch;
    
            
            // printf("pitch:%+03.2f, roll:%+03.2f, yaw:%+03.2f \n",  imu.angle[1], imu.angle[0], imu.angle[2]);
                  
            vTaskDelay(1/portTICK_PERIOD_MS);
        }
    
    }
    
    JSValue QMI8658::startReceive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(QMI8658, that)
        if(that->taskListenerHandle == nullptr) {
            xTaskCreatePinnedToCore((TaskFunction_t)task_listen, "task-listen", 5*1024, that, 5, &that->taskListenerHandle, 1);
        }
        return JS_UNDEFINED ;
    }
    JSValue QMI8658::stopReceive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(QMI8658, that)
        if(that->taskListenerHandle == nullptr) {
            vTaskDelete(that->taskListenerHandle) ;
            that->taskListenerHandle = nullptr ;
        }
        return JS_UNDEFINED ;
    }
}