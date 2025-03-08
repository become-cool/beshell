#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#ifndef IMU_ALGO_AXIS9_H_
#define IMU_ALGO_AXIS9_H_


typedef struct
{
	float a1;
	float a2;
	float b0;
	float b1;
	float b2;
}QST_Filter;

typedef struct
{
	float _delay_element_1;
	float _delay_element_2;
}QST_Filter_Buffer;


#ifdef __cplusplus
extern "C" {
#endif

/*!
* \功能：实现加速度计和陀螺仪二阶滤波器.
* \输入： float sample_freq:加速度计或陀螺仪滤波采样频率.
* \输入： float cutoff_freq:加速度计或陀螺仪滤波截至频率.
* \输出： QST_Filter *filter:滤波器参数.
* \返回值：无.
* \return void
 */
void set_cutoff_frequency(float sample_freq, float cutoff_freq, QST_Filter *filter);

/*!
* \功能：实现加速度计和陀螺仪二阶滤波器.
* \输入： float sample:加速度计或陀螺仪原始值.
* \输入： QST_Filter_Buffer *buffer:加速度计或陀螺仪滤波缓存.
* \输入： QST_Filter *filter:加速度计或陀螺仪滤波器参数.
* \输出： 无.
* \返回值：加速度计或陀螺仪后值.
* \return void
 */
float Filter_Apply(float sample, QST_Filter_Buffer *buffer, QST_Filter *filter);

/*!
 * \功能：实现姿态四元素更新（调度周期建议为50hz~200hz）.
* \输入： float fusion_accel[3]:加速度X/Y/Z三轴值（单位：m/s2）.
* \输入： float fusion_gyro[3]:陀螺仪X/Y/Z三轴值（单位：rad/s）.
* \输入： float *fusion_dt:函数调度周期（单位：s）.
* \输出： float *rpy:三维姿态角（单位：degree）.
* \输出： float *quaternion:输出姿态四元数.
* \输出： float *line_accel:输出三轴线性加速度值.
* \返回值：无.
 */
void qst_fusion_update(float fusion_accel[3], float fusion_gyro[3], float *fusion_dt, float *rpy, float *quaternion, float *line_accel);



//extern int qmi8658_read_reg(unsigned char reg, unsigned char *buf, unsigned short len);
void init_state_recognition(unsigned char (*read)(unsigned char, unsigned char *, unsigned short));

#ifdef __cplusplus
}
#endif

#endif /* GYRO_ALGO_H_ */
