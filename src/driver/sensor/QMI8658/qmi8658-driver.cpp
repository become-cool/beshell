#include "qmi8658-driver.hpp"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "debug.h"


int mx_i2c1_write(uint8_t addr, uint8_t reg, uint8_t value) ;
int mx_i2c1_read(uint8_t addr, uint8_t reg, uint8_t * buffer, int len) ;

//#define QMI8658_UINT_MG_DPS
#define M_PI			(3.14159265358979323846f)
#define ONE_G			(9.807f)
#define QFABS(x)		(((x)<0.0f)?(-1.0f*(x)):(x))

#if defined(STATIC_CALIBRATION)
#define MAX_STATIC_CALI_COUNTER      100
uint16_t static_cali_count = 0;
uint8_t static_cali_flag = 0;
int32_t accel_static_calibration_sum[3] = {0, 0, 0};
int32_t gyro_static_calibration_sum[3] = {0, 0, 0};
int16_t static_offset_gyro[3] = {0, 0, 0};
int16_t static_offset_acc[3] = {0, 0, 0};
#endif

#if defined(GYRO_DYNAMIC_CALIBRATION)
#define BUFFSER_SIZE_ 10
#define MAX_STATIC_COUNT_ 20

float gyro_buffer_speed_[BUFFSER_SIZE_] = {0, 0};
float accel_buffer_speed_[BUFFSER_SIZE_] = {0, 0};

int16_t gyro_static_offset_[3] = {0, 0, 0};
uint32_t static_delay_ = 0;
uint32_t static_flag_ = 0;
uint8_t imu_offset_static_ = 0;
float gyro_raw_static_deviation = 0;
float accel_raw_static_deviation = 0;
uint16_t cali_count_static_ = 0;
int32_t gyro_staticn_sum_[3] = {0, 0, 0};

#endif

#if defined(QMI8658_USE_FIFO)
uint8_t fifo_discard = 0;
#endif


#define I2C_FAIL				0
#define I2C_SUCCESS				1

static qmi8658_state g_imu;

int qmi8658_write_reg(unsigned char reg, unsigned char value)
{
	int ret=0;
	unsigned int retry = 0;

	while((!ret) && (retry++ < 5))
	{
// #if defined(QMI8658_USE_SPI)
// 		ret = qst_imu_spi_write(reg,value);
// #elif defined(QST_USE_SW_I2C)
// 		ret = qst_sw_writereg(g_imu.slave<<1, reg, value);
// #else
		ret = mx_i2c1_write(g_imu.slave<<1, reg, value);
// #endif
		if(ret == I2C_SUCCESS) ret = 1;
	}
	return ret;
}

int qmi8658_read_reg(unsigned char reg, unsigned char* buf, unsigned short len)
{
	int ret=0;
	unsigned int retry = 0;

	while((!ret) && (retry++ < 5))
	{
// #if defined(QMI8658_USE_SPI)
// 		ret = qst_8658_spi_read(reg, buf, len);
// #elif defined(QST_USE_SW_I2C)
// 		ret = qst_sw_readreg(g_imu.slave<<1, reg, buf, len);
// #else
		ret = mx_i2c1_read(g_imu.slave<<1, reg, buf, len);
// #endif
		if(ret == I2C_SUCCESS) ret = 1;
	}
	return ret;
}

void qmi8658_delay(unsigned int ms)
{
	vTaskDelay(ms/portTICK_PERIOD_MS) ;
}

void qmi8658_delay_us(unsigned int us)
{
	esp_rom_delay_us(us) ;
}


void qmi8658_axis_convert(short data_a[3], short data_g[3], int layout)
{
	short raw[3],raw_g[3];

	raw[0] = data_a[0];
	raw[1] = data_a[1];
	raw_g[0] = data_g[0];
	raw_g[1] = data_g[1];

	if(layout >=4 && layout <= 7)
	{
		data_a[2] = -data_a[2];
		data_g[2] = -data_g[2];
	}

	if(layout%2)
	{
		data_a[0] = raw[1];
		data_a[1] = raw[0];
		
		data_g[0] = raw_g[1];
		data_g[1] = raw_g[0];
	}
	else
	{
		data_a[0] = raw[0];
		data_a[1] = raw[1];

		data_g[0] = raw_g[0];
		data_g[1] = raw_g[1];
	}

	if((layout==1)||(layout==2)||(layout==4)||(layout==7))
	{
		data_a[0] = -data_a[0];
		data_g[0] = -data_g[0];
	}
	if((layout==2)||(layout==3)||(layout==6)||(layout==7))
	{
		data_a[1] = -data_a[1];
		data_g[1] = -data_g[1];
	}
}

void qmi8658_config_acc(enum qmi8658_AccRange range, enum qmi8658_AccOdr odr, enum qmi8658_LpfConfig lpfEnable, enum qmi8658_StConfig stEnable)
{
	unsigned char ctl_dada;

	switch(range)
	{
		case Qmi8658AccRange_2g:
			g_imu.ssvt_a = (1<<14);
			break;
		case Qmi8658AccRange_4g:
			g_imu.ssvt_a = (1<<13);
			break;
		case Qmi8658AccRange_8g:
			g_imu.ssvt_a = (1<<12);
			break;
		case Qmi8658AccRange_16g:
			g_imu.ssvt_a = (1<<11);
			break;
		default: 
			range = Qmi8658AccRange_8g;
			g_imu.ssvt_a = (1<<12);
	}
	if(stEnable == Qmi8658St_Enable)
		ctl_dada = (unsigned char)range|(unsigned char)odr|0x80;
	else
		ctl_dada = (unsigned char)range|(unsigned char)odr;
		
	qmi8658_write_reg(Qmi8658Register_Ctrl2, ctl_dada);
// set LPF & HPF
	qmi8658_read_reg(Qmi8658Register_Ctrl5, &ctl_dada, 1);
	ctl_dada &= 0xf0;
	if(lpfEnable == Qmi8658Lpf_Enable)
	{
		ctl_dada |= A_LSP_MODE_3;
		ctl_dada |= 0x01;
	}
	else
	{
		ctl_dada &= ~0x01;
	}
	//ctl_dada = 0x00;
	qmi8658_write_reg(Qmi8658Register_Ctrl5,ctl_dada);
// set LPF & HPF
}

void qmi8658_config_gyro(enum qmi8658_GyrRange range, enum qmi8658_GyrOdr odr, enum qmi8658_LpfConfig lpfEnable, enum qmi8658_StConfig stEnable)
{
	// Set the CTRL3 register to configure dynamic range and ODR
	unsigned char ctl_dada; 

	// Store the scale factor for use when processing raw data
	switch (range)
	{
		case Qmi8658GyrRange_16dps:
			g_imu.ssvt_g = 2048;
			break;			
		case Qmi8658GyrRange_32dps:
			g_imu.ssvt_g = 1024;
			break;
		case Qmi8658GyrRange_64dps:
			g_imu.ssvt_g = 512;
			break;
		case Qmi8658GyrRange_128dps:
			g_imu.ssvt_g = 256;
			break;
		case Qmi8658GyrRange_256dps:
			g_imu.ssvt_g = 128;
			break;
		case Qmi8658GyrRange_512dps:
			g_imu.ssvt_g = 64;
			break;
		case Qmi8658GyrRange_1024dps:
			g_imu.ssvt_g = 32;
			break;
		case Qmi8658GyrRange_2048dps:
			g_imu.ssvt_g = 16;
			break;
//		case Qmi8658GyrRange_4096dps:
//			g_imu.ssvt_g = 8;
//			break;
		default: 
			range = Qmi8658GyrRange_512dps;
			g_imu.ssvt_g = 64;
			break;
	}

	if(stEnable == Qmi8658St_Enable)
		ctl_dada = (unsigned char)range|(unsigned char)odr|0x80;
	else
		ctl_dada = (unsigned char)range | (unsigned char)odr;
	qmi8658_write_reg(Qmi8658Register_Ctrl3, ctl_dada);

// Conversion from degrees/s to rad/s if necessary
// set LPF & HPF
	qmi8658_read_reg(Qmi8658Register_Ctrl5, &ctl_dada,1);
	ctl_dada &= 0x0f;
	if(lpfEnable == Qmi8658Lpf_Enable)
	{
		ctl_dada |= G_LSP_MODE_3;
		ctl_dada |= 0x10;
	}
	else
	{
		ctl_dada &= ~0x10;
	}
	//ctl_dada = 0x00;
	qmi8658_write_reg(Qmi8658Register_Ctrl5,ctl_dada);
// set LPF & HPF
}


int qmi8658_send_ctl9cmd(enum qmi8658_Ctrl9Command cmd)
{
	unsigned char status1 = 0x00;
	unsigned short count=0;
	unsigned char status_reg = Qmi8658Register_StatusInt;	
	unsigned char cmd_done = 0x80;
	unsigned char retry = 0;
	int ret1 = 0;
	int ret2 = 0;

#if defined(QMI8658_SYNC_SAMPLE_MODE)
	if(g_imu.cfg.syncSample == 1)
	{
		status_reg = Qmi8658Register_Status1;
		cmd_done = 0x01;
	}
#endif
	while(retry++ < 3)
	{
		qmi8658_write_reg(Qmi8658Register_Ctrl9, (unsigned char)cmd);	// write commond to ctrl9

		qmi8658_read_reg(status_reg, &status1, 1);
		while(((status1&cmd_done)!=cmd_done)&&(count++<100))		// read statusINT until bit7 is 1
		{
			qmi8658_delay(1);
			qmi8658_read_reg(status_reg, &status1, 1);
		}
		//qmi8658_log("ctrl9 cmd (%d) done1 count=%d\n", cmd, count);
		if(count < 100)
		{
			ret1 = 1;
		}
		else
		{
			ret1 = 0;
		}

		qmi8658_write_reg(Qmi8658Register_Ctrl9, qmi8658_Ctrl9_Cmd_Ack);	// write commond  0x00 to ctrl9
		count = 0;
		qmi8658_read_reg(status_reg, &status1, 1);
		while(((status1&cmd_done)==cmd_done)&&(count++<100))		// read statusINT until bit7 is 0
		{
			qmi8658_delay(1);	// 1 ms
			qmi8658_read_reg(status_reg, &status1, 1);
		}
		//qmi8658_log("ctrl9 cmd (%d) done2 count=%d\n", qmi8658_Ctrl9_Cmd_Ack, count);
		if(count < 100)
		{
			ret2 = 1;
		}
		else
		{
			ret2 = 0;
		}

		if((ret1==0) || (ret2==0))
		{
			continue;
		}
		else
		{
			break;
		}
	}

	if(ret1 && ret2)
	{
		return 1;
	}
	else
	{
		qmi8658_log("qmi8658_send_ctl9cmd fail cmd=%d\n", cmd);
		return 0;
	}
}

unsigned char qmi8658_readStatusInt(void)
{
	unsigned char status_int;

	qmi8658_read_reg(Qmi8658Register_StatusInt, &status_int, 1);

	return status_int;
}

unsigned char qmi8658_readStatus0(void)
{
	unsigned char status0;

	qmi8658_read_reg(Qmi8658Register_Status0, &status0, 1);

	return status0;
}

unsigned char qmi8658_readStatus1(void)
{
	unsigned char status1;
	
	qmi8658_read_reg(Qmi8658Register_Status1, &status1, 1);

	return status1;
}

float qmi8658_readTemp(void)
{
	unsigned char buf[2];
	short temp = 0;
	float temp_f = 0;
	qmi8658_read_reg(Qmi8658Register_Tempearture_L, buf, 2);
	temp = ((short)buf[1]<<8)|buf[0];
	temp_f = (float)temp/256.0f;

	return temp_f;
}

void qmi8658_read_timestamp(unsigned int *tim_count)
{
	unsigned char	buf[3];
	unsigned int timestamp;

	if(tim_count)
	{
		qmi8658_read_reg(Qmi8658Register_Timestamp_L, buf, 3);
		timestamp = (unsigned int)(((unsigned int)buf[2]<<16)|((unsigned int)buf[1]<<8)|buf[0]);
		if(timestamp > g_imu.timestamp)
			g_imu.timestamp = timestamp;
		else
			g_imu.timestamp = (timestamp+0x1000000-g_imu.timestamp);

		*tim_count = g_imu.timestamp;		
	}
}


#if defined(STATIC_CALIBRATION)
uint8_t imu_static_calibration(short acc_raw[3], short gyro_raw[3])
{
		uint8_t axis;
		if(static_cali_flag == 1) return 1;
	
    if(static_cali_count == 0)
    {
        memset((void *)accel_static_calibration_sum, 0, sizeof(accel_static_calibration_sum));
        memset((void *)gyro_static_calibration_sum, 0, sizeof(gyro_static_calibration_sum));
        static_cali_count++;
    }
    else if(static_cali_count < MAX_STATIC_CALI_COUNTER)
    {
        for(axis = 0; axis < 3; axis++)
        {
            gyro_static_calibration_sum[axis] += gyro_raw[axis];
            if(axis == 2)
            {
				accel_static_calibration_sum[axis] += (acc_raw[axis] - g_imu.ssvt_a);			
            }
            else
            {
              accel_static_calibration_sum[axis] += acc_raw[axis];
            }
        }
        static_cali_count++;
    }
    else if(static_cali_count == MAX_STATIC_CALI_COUNTER)
    {
			for(axis = 0; axis < 3; axis++)
			{
					static_offset_acc[axis] = (accel_static_calibration_sum[axis] / (MAX_STATIC_CALI_COUNTER - 1));
					static_offset_gyro[axis] = (gyro_static_calibration_sum[axis] / (MAX_STATIC_CALI_COUNTER - 1));
			}
			static_cali_count = 0;
			printf("%d,%d,%d,%d,%d,%d\r\n",static_offset_acc[0],static_offset_acc[1],static_offset_acc[2],static_offset_gyro[0],static_offset_gyro[1],static_offset_gyro[2]);
			static_cali_flag = 1;		
			return 1;
    }
	return 0;

}

#endif

#if defined(GYRO_DYNAMIC_CALIBRATION)

void qst_fusion_raw_offset_update(int16_t fusion_accel[3],int16_t fusion_gyro[3])
{
  unsigned char axis = 0;
	float gyro_recip_norm,accel_recip_norm,gyro_sum,accel_sum;
	float gyro_average,accel_average,gyro_standard_deviation,accel_standard_deviation;

#if defined(STATIC_CALIBRATION)
	if(static_cali_flag == 0) return;
#endif
	gyro_recip_norm = sqrtf(fusion_gyro[0] * fusion_gyro[0] + fusion_gyro[1] * fusion_gyro[1] + fusion_gyro[2] * fusion_gyro[2]);
	accel_recip_norm = sqrtf(fusion_accel[0] * fusion_accel[0] + fusion_accel[1] * fusion_accel[1] + fusion_accel[2] * fusion_accel[2]);
	gyro_sum = 0;
	accel_sum = 0;
	for(axis = 0; axis < BUFFSER_SIZE_ - 1; axis++)
	{
	accel_buffer_speed_[axis] = accel_buffer_speed_[axis + 1];
	accel_sum = accel_sum + accel_buffer_speed_[axis];

			gyro_buffer_speed_[axis] = gyro_buffer_speed_[axis + 1];
			gyro_sum = gyro_sum + gyro_buffer_speed_[axis];
	}

	accel_buffer_speed_[axis] = accel_recip_norm;
	accel_sum = accel_sum + accel_buffer_speed_[axis];
	accel_average = accel_sum / BUFFSER_SIZE_;

	gyro_buffer_speed_[axis] = gyro_recip_norm;
	gyro_sum = gyro_sum + gyro_buffer_speed_[axis];
	gyro_average = gyro_sum / BUFFSER_SIZE_;

	gyro_sum = 0;
	accel_sum = 0;

	for(axis = 0; axis < BUFFSER_SIZE_; axis++)
	{
		accel_sum = accel_sum + (accel_buffer_speed_[axis] - accel_average) * (accel_buffer_speed_[axis] - accel_average);
		gyro_sum = gyro_sum + (gyro_buffer_speed_[axis] - gyro_average) * (gyro_buffer_speed_[axis] - gyro_average);
	}
	accel_standard_deviation = sqrtf(accel_sum / BUFFSER_SIZE_);
	gyro_standard_deviation = sqrtf(gyro_sum / BUFFSER_SIZE_);
    
	//printf("GYRO_DYNAMIC:%d	%f	%f	%f	%f\n", static_flag_,accel_standard_deviation, accel_raw_static_deviation,gyro_standard_deviation, gyro_raw_static_deviation);	
	if((accel_standard_deviation < accel_raw_static_deviation) && (gyro_standard_deviation < gyro_raw_static_deviation))
	{
		if(static_flag_ == 0)
		{
			if(static_delay_ < 40)
			{
				static_delay_++;
			}
			else
			{
				static_delay_ = 0;
				static_flag_ = 1;
			}
		}
		else
		{
			if(imu_offset_static_ != 1)
			{
				if(cali_count_static_ == 0)
				{
					for(axis = 0; axis < 3; axis++)
					{
						gyro_staticn_sum_[axis] = 0;
					}
					cali_count_static_++;
				}
				else if(cali_count_static_ < MAX_STATIC_COUNT_ + 1)
				{
					for(axis = 0; axis < 3; axis++)
					{
						gyro_staticn_sum_[axis] += fusion_gyro[axis];
					}
					cali_count_static_++;
				}
				else if(cali_count_static_ == MAX_STATIC_COUNT_ + 1)
				{
					for(axis = 0; axis < 3; axis++)
					{
						gyro_static_offset_[axis] = (gyro_staticn_sum_[axis] / MAX_STATIC_COUNT_);
					}

					imu_offset_static_ = 1;
					cali_count_static_ = 0;
				}
			}
		}

	}
	else
	{
		static_delay_ = 0;
		static_flag_ = 0;
		imu_offset_static_ = 0;
		cali_count_static_ = 0;
	}
}

#endif

void qmi8658_read_sensor_data(float acc[3], float gyro[3])
{
	unsigned char	buf_reg[12];
	short 			raw_acc_xyz[3];
	short 			raw_gyro_xyz[3];

	qmi8658_read_reg(Qmi8658Register_Ax_L, buf_reg, 12);
	raw_acc_xyz[0] = (short)((unsigned short)(buf_reg[1]<<8) |( buf_reg[0]));
	raw_acc_xyz[1] = (short)((unsigned short)(buf_reg[3]<<8) |( buf_reg[2]));
	raw_acc_xyz[2] = (short)((unsigned short)(buf_reg[5]<<8) |( buf_reg[4]));

	raw_gyro_xyz[0] = (short)((unsigned short)(buf_reg[7]<<8) |( buf_reg[6]));
	raw_gyro_xyz[1] = (short)((unsigned short)(buf_reg[9]<<8) |( buf_reg[8]));
	raw_gyro_xyz[2] = (short)((unsigned short)(buf_reg[11]<<8) |( buf_reg[10]));

	qmi8658_axis_convert(raw_acc_xyz, raw_gyro_xyz, 0);
#if defined(STATIC_CALIBRATION)
	if(imu_static_calibration(raw_acc_xyz,raw_gyro_xyz) == 1)
	{
		raw_acc_xyz[0] = raw_acc_xyz[0] - static_offset_acc[0];
		raw_acc_xyz[1] = raw_acc_xyz[1] - static_offset_acc[1];
		raw_acc_xyz[2] = raw_acc_xyz[2] - static_offset_acc[2];
		raw_gyro_xyz[0] = raw_gyro_xyz[0] - static_offset_gyro[0];
		raw_gyro_xyz[1] = raw_gyro_xyz[1] - static_offset_gyro[1];
		raw_gyro_xyz[2] = raw_gyro_xyz[2] - static_offset_gyro[2];
	}
#endif
#if defined(GYRO_DYNAMIC_CALIBRATION)	
	qst_fusion_raw_offset_update(raw_acc_xyz,raw_gyro_xyz);
	raw_gyro_xyz[0] = raw_gyro_xyz[0] - gyro_static_offset_[0];
	raw_gyro_xyz[1] = raw_gyro_xyz[1] - gyro_static_offset_[1];
	raw_gyro_xyz[2] = raw_gyro_xyz[2] - gyro_static_offset_[2];
#endif	

	
	//printf("%d	%d	%d\n",raw_gyro[0],raw_gyro[1],raw_gyro[2]);
#if defined(QMI8658_UINT_MG_DPS)
	// mg
	acc[0] = (float)(raw_acc_xyz[0]*1000.0f)/g_imu.ssvt_a;
	acc[1] = (float)(raw_acc_xyz[1]*1000.0f)/g_imu.ssvt_a;
	acc[2] = (float)(raw_acc_xyz[2]*1000.0f)/g_imu.ssvt_a;
#else
	// m/s2
	acc[0] = (float)(raw_acc_xyz[0]*ONE_G)/g_imu.ssvt_a;
	acc[1] = (float)(raw_acc_xyz[1]*ONE_G)/g_imu.ssvt_a;
	acc[2] = (float)(raw_acc_xyz[2]*ONE_G)/g_imu.ssvt_a;
#endif

#if defined(QMI8658_UINT_MG_DPS)
	// dps
	gyro[0] = (float)(raw_gyro_xyz[0]*1.0f)/g_imu.ssvt_g;
	gyro[1] = (float)(raw_gyro_xyz[1]*1.0f)/g_imu.ssvt_g;
	gyro[2] = (float)(raw_gyro_xyz[2]*1.0f)/g_imu.ssvt_g;
#else
	// rad/s
	gyro[0] = (float)(raw_gyro_xyz[0]*M_PI)/(g_imu.ssvt_g*180);		// *pi/180
	gyro[1] = (float)(raw_gyro_xyz[1]*M_PI)/(g_imu.ssvt_g*180);
	gyro[2] = (float)(raw_gyro_xyz[2]*M_PI)/(g_imu.ssvt_g*180);
#endif
}



#if defined(QMI8658_EN_CGAIN)
static void readIndirectData(unsigned int startAddr, unsigned char *buf, unsigned char bytesNum)
{
	unsigned char addrList[4];
	unsigned char ctrlData = 0;

	qmi8658_delay(1);
	qmi8658_write_reg(BANK0_INDIRECT_CTRL_ADDR, 0x7F);
	ctrlData = (1 << 7)+ (bytesNum << 3) + (0 << 2)+ (0 << 1)+ 1;
	qmi8658_write_reg(BANK0_INDIRECT_CTRL_ADDR, ctrlData);	// apply new settings
	qmi8658_delay(1);
	// Start reading the data
	qmi8658_read_reg(BANK0_INDIRECT_SYS_DATA_ADDR, buf, bytesNum);
}

unsigned char qmi8658_read_cgain(void)
{
	unsigned char data, start_en, cgain;
		
	readIndirectData(QMI8658_EN_CGAIN, &data, 1);
	start_en = (data & 0x80) >> 7;
	cgain = data & 0x3F;
//	printf("qmi8658_read_cgain: %d %d\r\n", cgain, start_en);
	if(cgain >= 63)
	{
		qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);
		qmi8658_delay(10);
		qmi8658_enableSensors(QMI8658_ACCGYR_ENABLE);
		qmi8658_delay(10);
	}

	return cgain;
}
#endif


void qmi8658_read_xyz(float acc[3], float gyro[3])
{
	unsigned char	status = 0;
	unsigned char data_ready = 0;
	int	retry = 0;

	while(retry++ < 3)
	{
#if defined(QMI8658_EN_CGAIN)
		qmi8658_read_cgain();
#endif		
// #if defined(QMI8658_SYNC_SAMPLE_MODE)
		qmi8658_read_reg(Qmi8658Register_StatusInt, &status, 1);
		if((status&0x01)&&(status&0x02))
		{
			data_ready = 1;
			qmi8658_delay_us(12);	// delay 12us <=500Hz�� 12us 1000Hz, 4us 2000Hz 2us > 2000Hz
			break;
		}
// #else
// 		qmi8658_read_reg(Qmi8658Register_Status0, &status, 1);
// 		//qmi8658_log("status0 0x%x\n", status);
// 		if(status&0x03)
// 		{
// 			data_ready = 1;
// 			break;
// 		}
// #endif
	}
	if(data_ready)
	{
		qmi8658_read_sensor_data(acc, gyro);
		g_imu.imu[0] = acc[0];
		g_imu.imu[1] = acc[1];
		g_imu.imu[2] = acc[2];
		g_imu.imu[3] = gyro[0];
		g_imu.imu[4] = gyro[1];
		g_imu.imu[5] = gyro[2];
	}
	else
	{
		acc[0] = g_imu.imu[0];
		acc[1] = g_imu.imu[1];
		acc[2] = g_imu.imu[2];
		gyro[0] = g_imu.imu[3];
		gyro[1] = g_imu.imu[4];
		gyro[2] = g_imu.imu[5];
		//qmi8658_log("data ready fail!\n");
	}
}

#if defined(QMI8658_SYNC_SAMPLE_MODE)
void qmi8658_enable_AHB_clock(int enable)
{
	if(enable)
	{
		qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);
		qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x00);
		qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_AHB_Clock_Gating);
	}
	else
	{
		qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);
		qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x01);
		qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_AHB_Clock_Gating);
	}
}
#endif

void qmi8658_enableSensors(unsigned char enableFlags)
{
#if defined(QMI8658_SYNC_SAMPLE_MODE)
	qmi8658_write_reg(Qmi8658Register_Ctrl7, enableFlags | 0x80);
#else
	qmi8658_write_reg(Qmi8658Register_Ctrl7, enableFlags);		// QMI8658_DRDY_DISABLE
#endif
	g_imu.cfg.enSensors = enableFlags&0x03;

#if defined(QMI8658_USE_FIFO)
	fifo_discard = 0;
#endif
	qmi8658_delay(2);
}

void qmi8658_dump_reg(void)
{
#if 1
    unsigned char read_data[8];
 
    qmi8658_read_reg(Qmi8658Register_Ctrl1, read_data, 8);
    qmi8658_log("Ctrl1[0x%x]\nCtrl2[0x%x]\nCtrl3[0x%x]\nCtrl4[0x%x]\nCtrl5[0x%x]\nCtrl6[0x%x]\nCtrl7[0x%x]\nCtrl8[0x%x]\n",
                    read_data[0],read_data[1],read_data[2],read_data[3],read_data[4],read_data[5],read_data[6],read_data[7]);
    //qmi8658_read_reg(Qmi8658Register_FifoWmkTh, read_data, 4);
    //qmi8658_log("FIFO reg[0x%x 0x%x 0x%x 0x%x]\n", read_data[0],read_data[1],read_data[2],read_data[3]);
    qmi8658_log("\n");
#else

#endif
}

void qmi8658_get_chip_info(void)
{
	unsigned char revision_id = 0x00;
	unsigned char firmware_id[3];
	unsigned char uuid[6];
	unsigned int uuid_low, uuid_high;

	qmi8658_read_reg(Qmi8658Register_Revision, &revision_id, 1);
	qmi8658_read_reg(Qmi8658Register_firmware_id, firmware_id, 3);
	qmi8658_read_reg(Qmi8658Register_uuid, uuid, 6);
	uuid_low = (unsigned int)((unsigned int)(uuid[2]<<16)|(unsigned int)(uuid[1]<<8)|(uuid[0]));
	uuid_high = (unsigned int)((unsigned int)(uuid[5]<<16)|(unsigned int)(uuid[4]<<8)|(uuid[3]));
	//qmi8658_log("VS ID[0x%x]\n", revision_id);
	qmi8658_log("**FW ID[%d %d %d] Revision;0x%x\n", firmware_id[2], firmware_id[1],firmware_id[0],revision_id);
	qmi8658_log("**UUID HEX[0x%x %x]\n", uuid_high ,uuid_low);
	qmi8658_log("**UUID Decimal H[%d] L[%d]\n", uuid_high ,uuid_low);
}

void qmi8658_soft_reset(void)
{
	unsigned char reset_done = 0x00;
	int retry = 0;

	qmi8658_log("qmi8658_soft_reset \n");
	qmi8658_write_reg(Qmi8658Register_Reset, 0xb0);
	qmi8658_delay(10);	// delay	
	while(reset_done != 0x80)
	{
		qmi8658_delay(1);
		qmi8658_read_reg(Qmi8658Register_Reset_done, &reset_done, 1);
		if(retry++ > 500)
		{
			break;
		}
	}
	qmi8658_log("qmi8658_soft_reset done retry=%d\n", retry);
}

void qmi8658_get_gyro_gain(unsigned char cod_data[6])
{
	qmi8658_read_reg(Qmi8658Register_Dvx_L, &cod_data[0], 6);
	qmi8658_log("cod data[0x%x 0x%x 0x%x 0x%x 0x%x 0x%x]\n", cod_data[0],cod_data[1],cod_data[2],
															cod_data[3],cod_data[4],cod_data[5]);
}

void qmi8658_apply_gyr_gain(unsigned char cod_data[6])
{
	qmi8658_enableSensors(QMI8658_DISABLE_ALL);
	qmi8658_write_reg(Qmi8658Register_Cal1_L, cod_data[0]);
	qmi8658_write_reg(Qmi8658Register_Cal1_H, cod_data[1]);
	qmi8658_write_reg(Qmi8658Register_Cal2_L, cod_data[2]);
	qmi8658_write_reg(Qmi8658Register_Cal2_H, cod_data[3]);
	qmi8658_write_reg(Qmi8658Register_Cal3_L, cod_data[4]);
	qmi8658_write_reg(Qmi8658Register_Cal3_H, cod_data[5]);

	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_Apply_Gyro_Gain);
}

void qmi8658_on_demand_cali(void)
{
	unsigned char cod_status = 0x00;
	qmi8658_log("qmi8658_on_demand_cali start\n");
	qmi8658_write_reg(Qmi8658Register_Ctrl9, (unsigned char)qmi8658_Ctrl9_Cmd_On_Demand_Cali);
	qmi8658_delay(2200);	// delay 2000ms above
	qmi8658_write_reg(Qmi8658Register_Ctrl9, (unsigned char)qmi8658_Ctrl9_Cmd_Ack);
	qmi8658_delay(10);		// delay
	qmi8658_read_reg(Qmi8658Register_Cod_Status, &cod_status, 1);
	if(cod_status)
	{
		qmi8658_log("qmi8658_on_demand_cali fail! status=0x%x\n", cod_status);
	}
	else
	{
		qmi8658_get_gyro_gain(g_imu.cod_data);
		qmi8658_log("qmi8658_on_demand_cali done! cod[%d %d %d]\n", 
			(unsigned short)(g_imu.cod_data[1]<<8|g_imu.cod_data[0]),
			(unsigned short)(g_imu.cod_data[3]<<8|g_imu.cod_data[2]),
			(unsigned short)(g_imu.cod_data[5]<<8|g_imu.cod_data[4]));
	}
}

void qmi8658_config_reg(unsigned char low_power)
{
	qmi8658_enableSensors(QMI8658_DISABLE_ALL);
	if(low_power)
	{
		g_imu.cfg.enSensors = QMI8658_ACC_ENABLE;
		g_imu.cfg.accRange = Qmi8658AccRange_8g;
		g_imu.cfg.accOdr = Qmi8658AccOdr_LowPower_128Hz;
		g_imu.cfg.gyrRange = Qmi8658GyrRange_1024dps;
		g_imu.cfg.gyrOdr = Qmi8658GyrOdr_125Hz;
	}
	else
	{
		g_imu.cfg.enSensors = QMI8658_ACCGYR_ENABLE;
		g_imu.cfg.accRange = Qmi8658AccRange_8g;
		g_imu.cfg.accOdr = Qmi8658AccOdr_125Hz;
		g_imu.cfg.gyrRange = Qmi8658GyrRange_1024dps;
		g_imu.cfg.gyrOdr = Qmi8658GyrOdr_125Hz;
	}

	dn(g_imu.cfg.accOdr)
	
	if(g_imu.cfg.enSensors & QMI8658_ACC_ENABLE)
	{
		qmi8658_config_acc(g_imu.cfg.accRange, g_imu.cfg.accOdr, Qmi8658Lpf_Disable, Qmi8658St_Disable);
	}
	if(g_imu.cfg.enSensors & QMI8658_GYR_ENABLE)
	{
		qmi8658_config_gyro(g_imu.cfg.gyrRange, g_imu.cfg.gyrOdr, Qmi8658Lpf_Disable, Qmi8658St_Disable);
	}
}


unsigned char qmi8658_get_id(void)
{
	unsigned char qmi8658_chip_id = 0x00;
	unsigned char qmi8658_slave[2] = {QMI8658_SLAVE_ADDR_L, QMI8658_SLAVE_ADDR_H};
	int retry = 0;
	unsigned char iCount = 0;

	while(iCount<2)
	{
		g_imu.slave = qmi8658_slave[iCount];
		retry = 0;
		while((qmi8658_chip_id != 0x05)&&(retry++ < 5))
		{
			qmi8658_read_reg(Qmi8658Register_WhoAmI, &qmi8658_chip_id, 1);
		}
		qmi8658_log("qmi8658 slave = 0x%x WhoAmI = 0x%x\n", g_imu.slave, qmi8658_chip_id);
		if(qmi8658_chip_id == 0x05)
		{
			g_imu.cfg.syncSample = 0;
			g_imu.cfg.ctrl8_value = 0xc0;
			qmi8658_soft_reset();
			qmi8658_write_reg(Qmi8658Register_Ctrl1, 0x60|QMI8658_INT2_ENABLE|QMI8658_INT1_ENABLE);
			qmi8658_get_chip_info();
#if defined(QMI8658_USE_GYRO_STARTUP_TEST)	// check 0x45
			unsigned char opt_status = 0x00;
			qmi8658_write_reg(Qmi8658Register_Ctrl2, 0x25);
			qmi8658_write_reg(Qmi8658Register_Ctrl3, 0x66);
			qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x03);
			qmi8658_delay(300);
			qmi8658_read_reg(0x45, &opt_status, 1);
			//qmi8658_log("opt_status = 0x%x\n", opt_status);
			if(opt_status != 0x80)
			{
				qmi8658_log("**ERROR[0x45=0x%x]\n", opt_status);
				return 0;
			}
			else
			{
				qmi8658_log("**SUCCESS[0x45=0x%x]\n", opt_status);
			}
			qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);
#endif
#if defined(QMI8658_USE_HW_SELFTEST)
			GY_PWR_EN();
			qmi8658_do_hw_selftest(QMI8658_ACCGYR_ENABLE);
			GY_PWR_OFF();
#endif
			qmi8658_on_demand_cali();
			qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);
			qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
#if defined(QMI8658_EN_CGAIN)
			qmi8658_write_reg(BANK0_INDIRECT_SYS_ADDR_7_0_ADDR, 0x2a);
			qmi8658_write_reg(BANK0_INDIRECT_SYS_ADDR_15_8_ADDR, 0x00);
			qmi8658_write_reg(BANK0_INDIRECT_SYS_ADDR_23_16_ADDR, 0x06);
			qmi8658_write_reg(BANK0_INDIRECT_SYS_ADDR_31_24_ADDR, 0x00);
#endif
			break;
		}
		iCount++;
	}
	if(qmi8658_chip_id != 0x05)
	{		
		qmi8658_log("**ERROR1[id=0x%x]\n", qmi8658_chip_id);
	}

	return qmi8658_chip_id;
}

#if defined(QMI8658_USE_AMD)||defined(QMI8658_USE_NO_MOTION)||defined(QMI8658_USE_SIG_MOTION)
void qmi8658_config_motion(void)
{
	g_imu.cfg.ctrl8_value = 0xc0;	// &= (~QMI8658_CTRL8_ANYMOTION_EN);
	qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
	qmi8658_delay(2);
	qmi8658_enableSensors(QMI8658_DISABLE_ALL);

	qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x03);		// any motion X threshold(uint 1/32 g)
	qmi8658_write_reg(Qmi8658Register_Cal1_H, 0x03);		// any motion Y threshold(uint 1/32 g)
	qmi8658_write_reg(Qmi8658Register_Cal2_L, 0x03);		// any motion Z threshold(uint 1/32 g)
	qmi8658_write_reg(Qmi8658Register_Cal2_H, 0x02);		// no motion X threshold(uint 1/32 g)
	qmi8658_write_reg(Qmi8658Register_Cal3_L, 0x02);		// no motion X threshold(uint 1/32 g)
	qmi8658_write_reg(Qmi8658Register_Cal3_H, 0x02);		// no motion X threshold(uint 1/32 g)

	qmi8658_write_reg(Qmi8658Register_Cal4_L, 0xf7);		// MOTION_MODE_CTRL
	qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x01);		// value 0x01

	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_Motion);

	qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x03);		// AnyMotionWindow. 
	qmi8658_write_reg(Qmi8658Register_Cal1_H, 0xff);		// NoMotionWindow 
	qmi8658_write_reg(Qmi8658Register_Cal2_L, 0x2c);		// SigMotionWaitWindow[7:0]
	qmi8658_write_reg(Qmi8658Register_Cal2_H, 0x01);		// SigMotionWaitWindow [15:8]
	qmi8658_write_reg(Qmi8658Register_Cal3_L, 0x64);		// SigMotionConfirmWindow[7:0]
	qmi8658_write_reg(Qmi8658Register_Cal3_H, 0x00);		// SigMotionConfirmWindow[15:8]
	//qmi8658_write_reg(Qmi8658Register_Cal4_L, 0xf7);
	qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x02);		// value 0x02

	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_Motion);
}

#if defined(QMI8658_USE_AMD)
void qmi8658_enable_amd(unsigned char enable, enum qmi8658_Interrupt int_map, unsigned char low_power)
{
	if(enable)
	{
		unsigned char ctrl1;

		qmi8658_write_reg(Qmi8658Register_Ctrl8, 0xc0);
		qmi8658_enableSensors(QMI8658_DISABLE_ALL);
		qmi8658_config_reg(low_power);

		qmi8658_read_reg(Qmi8658Register_Ctrl1, &ctrl1, 1);
		if(int_map == qmi8658_Int1)
		{
			ctrl1 |= QMI8658_INT1_ENABLE;
			g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_INT_SEL;
		}
		else if(int_map == qmi8658_Int2)
		{
			ctrl1 |= QMI8658_INT2_ENABLE;
			g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_INT_SEL);
		}
		qmi8658_write_reg(Qmi8658Register_Ctrl1, ctrl1);// enable int for dev-E

		g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_ANYMOTION_EN;
		qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
		qmi8658_delay(1);
		qmi8658_enableSensors(g_imu.cfg.enSensors);
	}
	else
	{
		g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_ANYMOTION_EN);
		qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
	}
}
#endif

#if defined(QMI8658_USE_NO_MOTION)
void qmi8658_enable_no_motion(unsigned char enable, enum qmi8658_Interrupt int_map)
{
	if(enable)
	{
		unsigned char ctrl1;

		qmi8658_enableSensors(QMI8658_DISABLE_ALL);

		qmi8658_read_reg(Qmi8658Register_Ctrl1, &ctrl1, 1);
		if(int_map == qmi8658_Int1)
		{
			ctrl1 |= QMI8658_INT1_ENABLE;
			g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_INT_SEL;
		}
		else if(int_map == qmi8658_Int2)
		{
			ctrl1 |= QMI8658_INT2_ENABLE;
			g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_INT_SEL);
		}
		qmi8658_write_reg(Qmi8658Register_Ctrl1, ctrl1);// enable int for dev-E

		// recommend odr		
		qmi8658_config_acc(Qmi8658AccRange_8g, Qmi8658AccOdr_62_5Hz, Qmi8658Lpf_Disable, Qmi8658St_Disable);
		qmi8658_config_gyro(Qmi8658GyrRange_1024dps, Qmi8658GyrOdr_62_5Hz, Qmi8658Lpf_Disable, Qmi8658St_Disable);

		g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_NOMOTION_EN;
		qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
		qmi8658_delay(1);

		qmi8658_enableSensors(QMI8658_ACCGYR_ENABLE);
	}
	else
	{
		g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_NOMOTION_EN);
		qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
	}
}
#endif

#if defined(QMI8658_USE_SIG_MOTION)
void qmi8658_enable_sig_motion(unsigned char enable, enum qmi8658_Interrupt int_map)
{
	if(enable)
	{
		unsigned char ctrl1;

		qmi8658_enableSensors(QMI8658_DISABLE_ALL);

		qmi8658_read_reg(Qmi8658Register_Ctrl1, &ctrl1, 1);
		if(int_map == qmi8658_Int1)
		{
			ctrl1 |= QMI8658_INT1_ENABLE;
			g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_INT_SEL;
		}
		else if(int_map == qmi8658_Int2)
		{
			ctrl1 |= QMI8658_INT2_ENABLE;
			g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_INT_SEL);
		}		
		qmi8658_write_reg(Qmi8658Register_Ctrl1, ctrl1);// enable int for dev-E

		g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_SIGMOTION_EN|QMI8658_CTRL8_ANYMOTION_EN|QMI8658_CTRL8_NOMOTION_EN;
		qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
		qmi8658_delay(1);

		qmi8658_enableSensors(QMI8658_ACCGYR_ENABLE);
	}
	else
	{
		g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_SIGMOTION_EN);
		qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
	}
}
#endif

#endif

#if defined(QMI8658_USE_TAP)
unsigned char qmi8658_readTapStatus(void)
{
	unsigned char status;
	
	qmi8658_read_reg(Qmi8658Register_Tap_Status, &status, 1);

	return status;
}

void qmi8658_config_tap(void)
{
	unsigned char peakWindow = 0x1e;	//0x1e;
	unsigned char priority = 0x05;
	unsigned short TapWindow = 100;
	unsigned short DTapWindow = 500;

	unsigned char alpha = 0x08;
	unsigned char gamma = 0x20;
	unsigned short peakMagThr = 0x0599;
	unsigned short UDMThr = 0x0199;

	qmi8658_write_reg(Qmi8658Register_Cal1_L, peakWindow & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal1_H, priority & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal2_L, TapWindow & 0x00FF);
	qmi8658_write_reg(Qmi8658Register_Cal2_H, (TapWindow >> 8) & 0x00FF);
	qmi8658_write_reg(Qmi8658Register_Cal3_L, DTapWindow & 0x00FF);
	qmi8658_write_reg(Qmi8658Register_Cal3_H, (DTapWindow >> 8) & 0x00FF);
	qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x01);
	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_EnableTap);

	qmi8658_write_reg(Qmi8658Register_Cal1_L, alpha & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal1_H, gamma & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal2_L, peakMagThr & 0x00FF);
	qmi8658_write_reg(Qmi8658Register_Cal2_H, (peakMagThr>>8) & 0x00FF);
	qmi8658_write_reg(Qmi8658Register_Cal3_L, UDMThr & 0x00FF);
	qmi8658_write_reg(Qmi8658Register_Cal3_H, (UDMThr>>8) & 0x00FF);
	qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x02);
	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_EnableTap);
}

void qmi8658_enable_tap(unsigned char enable, enum qmi8658_Interrupt int_map)
{
	if(enable)
	{
		unsigned char ctrl1;

		qmi8658_enableSensors(QMI8658_DISABLE_ALL);

		qmi8658_read_reg(Qmi8658Register_Ctrl1, &ctrl1, 1);
		if(int_map == qmi8658_Int1)
		{
			ctrl1 |= QMI8658_INT1_ENABLE;
			g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_INT_SEL;
		}
		else if(int_map == qmi8658_Int2)
		{
			ctrl1 |= QMI8658_INT2_ENABLE;
			g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_INT_SEL);
		}
		qmi8658_write_reg(Qmi8658Register_Ctrl1, ctrl1);// enable int for dev-E

		g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_TAP_EN;
		qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
		qmi8658_delay(1);

		qmi8658_enableSensors(QMI8658_ACCGYR_ENABLE);
	}
	else
	{
		g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_TAP_EN);
		qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
	}

}
#endif

#if defined(QMI8658_USE_PEDOMETER)
void qmi8658_config_pedometer(unsigned short odr)
{
	float Rate = (float)(1000.0f/odr);
	unsigned short ped_sample_cnt = (unsigned short)(0x0032);//6;//(unsigned short)(0x0032 / finalRate) ;
	unsigned short ped_fix_peak2peak = 100;	//0x00AC;//0x0006;//0x00CC;
	unsigned short ped_fix_peak = 116;	//0x00AC;//0x0006;//0x00CC;
	unsigned short ped_time_up = (unsigned short)(2000 / Rate);
	unsigned char ped_time_low = (unsigned char) (300 / Rate);
	unsigned char ped_time_cnt_entry = 8;
	unsigned char ped_fix_precision = 0;
	unsigned char ped_sig_count = 1;//�Ʋ�����1

	qmi8658_write_reg(Qmi8658Register_Cal1_L, ped_sample_cnt & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal1_H, (ped_sample_cnt >> 8) & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal2_L, ped_fix_peak2peak & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal2_H, (ped_fix_peak2peak >> 8) & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal3_L, ped_fix_peak & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal3_H, (ped_fix_peak >> 8) & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x01);
	qmi8658_write_reg(Qmi8658Register_Cal4_L, 0x02);
	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_EnablePedometer);

	qmi8658_write_reg(Qmi8658Register_Cal1_L, ped_time_up & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal1_H, (ped_time_up >> 8) & 0xFF);
	qmi8658_write_reg(Qmi8658Register_Cal2_L, ped_time_low);
	qmi8658_write_reg(Qmi8658Register_Cal2_H, ped_time_cnt_entry);
	qmi8658_write_reg(Qmi8658Register_Cal3_L, ped_fix_precision);
	qmi8658_write_reg(Qmi8658Register_Cal3_H, ped_sig_count);
	qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x02);
	qmi8658_write_reg(Qmi8658Register_Cal4_L, 0x02);
	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_EnablePedometer);
}

void qmi8658_enable_pedometer(unsigned char enable)
{
	if(enable)
	{
		g_imu.cfg.ctrl8_value |= QMI8658_CTRL8_PEDOMETER_EN;
	}
	else
	{
		g_imu.cfg.ctrl8_value &= (~QMI8658_CTRL8_PEDOMETER_EN);
	}
	qmi8658_write_reg(Qmi8658Register_Ctrl8, g_imu.cfg.ctrl8_value);
}

unsigned int qmi8658_read_pedometer(void)
{
	unsigned char buf[3];

    qmi8658_read_reg(Qmi8658Register_Pedo_L, buf, 3);	// 0x5a
	g_imu.step = (unsigned int)((buf[2]<<16)|(buf[1]<<8)|(buf[0]));

	return g_imu.step;
}

void qmi8658_reset_pedometer(void)
{
	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_ResetPedometer);
}
#endif

#if defined(QMI8658_USE_FIFO)

void qmi8658_config_fifo(unsigned char watermark, enum qmi8658_FifoSize size, enum qmi8658_FifoMode mode, enum qmi8658_Interrupt int_map, unsigned char sensor)
{
    unsigned char ctrl1;

    qmi8658_write_reg(Qmi8658Register_FifoCtrl, 0x00);
    qmi8658_enableSensors(QMI8658_DISABLE_ALL);
    qmi8658_delay(2);
    qmi8658_read_reg(Qmi8658Register_Ctrl1, &ctrl1, 1);
    if (int_map == qmi8658_Int1)
    {
        ctrl1 |= QMI8658_FIFO_MAP_INT1;
    }
    else if (int_map == qmi8658_Int2)
    {
        ctrl1 &= QMI8658_FIFO_MAP_INT2;
    }
    qmi8658_write_reg(Qmi8658Register_Ctrl1, ctrl1);

    g_imu.cfg.fifo_ctrl = (unsigned char)(size | mode);
	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_Rst_Fifo);
    qmi8658_write_reg(Qmi8658Register_FifoCtrl, g_imu.cfg.fifo_ctrl);
    qmi8658_write_reg(Qmi8658Register_FifoWmkTh, watermark);

	qmi8658_enableSensors(sensor);
}

void fifo_data_processing(unsigned char* fifo_data,unsigned short count)
{
	unsigned short i=0;
	unsigned short fifo_count = 0;
	short raw_acc_xyz[3],raw_gyro_xyz[3];
	float acc[3],gyro[3];
	for(i=0;i<count;i++)
	{
		if(fifo_discard<8) fifo_discard++; //Discard the first 8 sets of data
		else
		{
			raw_acc_xyz[0] = (short)((unsigned short)(fifo_data[fifo_count+1]<<8) |( fifo_data[fifo_count+0]));
			raw_acc_xyz[1] = (short)((unsigned short)(fifo_data[fifo_count+3]<<8) |( fifo_data[fifo_count+2]));
			raw_acc_xyz[2] = (short)((unsigned short)(fifo_data[fifo_count+5]<<8) |( fifo_data[fifo_count+4]));
			if(g_imu.cfg.enSensors == QMI8658_ACCGYR_ENABLE)
			{
				raw_gyro_xyz[0] = (short)((unsigned short)(fifo_data[fifo_count+7]<<8) |( fifo_data[fifo_count+6]));
				raw_gyro_xyz[1] = (short)((unsigned short)(fifo_data[fifo_count+9]<<8) |( fifo_data[fifo_count+8]));
				raw_gyro_xyz[2] = (short)((unsigned short)(fifo_data[fifo_count+11]<<8) |( fifo_data[fifo_count+10]));
			}

#if defined(STATIC_CALIBRATION)
			if(imu_static_calibration(raw_acc_xyz,raw_gyro_xyz) == 1)
			{
				raw_acc_xyz[0] = raw_acc_xyz[0] - static_offset_acc[0];
				raw_acc_xyz[1] = raw_acc_xyz[1] - static_offset_acc[1];
				raw_acc_xyz[2] = raw_acc_xyz[2] - static_offset_acc[2];

				if(g_imu.cfg.enSensors == QMI8658_ACCGYR_ENABLE)
				{
					raw_gyro_xyz[0] = raw_gyro_xyz[0] - static_offset_gyro[0];
					raw_gyro_xyz[1] = raw_gyro_xyz[1] - static_offset_gyro[1];
					raw_gyro_xyz[2] = raw_gyro_xyz[2] - static_offset_gyro[2];
				}

			}
#endif


#if defined(GYRO_DYNAMIC_CALIBRATION)	
			if(g_imu.cfg.enSensors == QMI8658_ACCGYR_ENABLE)
			{
				qst_fusion_raw_offset_update(raw_acc_xyz,raw_gyro_xyz);
				raw_gyro_xyz[0] = raw_gyro_xyz[0] - gyro_static_offset_[0];
				raw_gyro_xyz[1] = raw_gyro_xyz[1] - gyro_static_offset_[1];
				raw_gyro_xyz[2] = raw_gyro_xyz[2] - gyro_static_offset_[2];
			}
#endif	
			//printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",fifo_data[fifo_count+0],fifo_data[fifo_count+1],fifo_data[fifo_count+2],fifo_data[fifo_count+3],fifo_data[fifo_count+4],fifo_data[fifo_count+5],fifo_data[fifo_count+6],fifo_data[fifo_count+7],fifo_data[fifo_count+8],fifo_data[fifo_count+9],fifo_data[fifo_count+10],fifo_data[fifo_count+11]);
			acc[0] = (float)(raw_acc_xyz[0]*ONE_G)/g_imu.ssvt_a;
			acc[1] = (float)(raw_acc_xyz[1]*ONE_G)/g_imu.ssvt_a;
			acc[2] = (float)(raw_acc_xyz[2]*ONE_G)/g_imu.ssvt_a;
			if(g_imu.cfg.enSensors == QMI8658_ACCGYR_ENABLE)
			{
				// rad/s
				gyro[0] = (float)(raw_gyro_xyz[0]*M_PI)/(g_imu.ssvt_g*180);		// *pi/180
				gyro[1] = (float)(raw_gyro_xyz[1]*M_PI)/(g_imu.ssvt_g*180);
				gyro[2] = (float)(raw_gyro_xyz[2]*M_PI)/(g_imu.ssvt_g*180);
			}

			
			printf("%f	%f	%f	%f	%f	%f\n",acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2]);
		}

		if(g_imu.cfg.enSensors == QMI8658_ACCGYR_ENABLE)
		{
			fifo_count+=12;
		}
		else
		{
			fifo_count+=6;
		}
		
	}
}

unsigned short qmi8658_read_fifo(unsigned char *data)
{
	unsigned char fifo_status[2] = {0, 0};
	unsigned char fifo_sensors = 1;
	unsigned short fifo_bytes = 0;
	unsigned short fifo_level = 0;

	if((g_imu.cfg.fifo_ctrl & 0x03) != qmi8658_Fifo_Bypass)
	{	
		qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_Req_Fifo);
		qmi8658_read_reg(Qmi8658Register_FifoCount, fifo_status, 2);
		fifo_bytes = (unsigned short)(((fifo_status[1] & 0x03) << 8) | fifo_status[0]);
		if((g_imu.cfg.enSensors == QMI8658_ACC_ENABLE) || (g_imu.cfg.enSensors == QMI8658_GYR_ENABLE))
		{
			fifo_sensors = 1;
		}
		else if(g_imu.cfg.enSensors == QMI8658_ACCGYR_ENABLE)
		{
			fifo_sensors = 2;
		}
		fifo_level = fifo_bytes / (3 * fifo_sensors);
		fifo_bytes = fifo_level * (6 * fifo_sensors);
	
		if (fifo_level > 0)
		{
			qmi8658_read_reg(Qmi8658Register_FifoData, data, fifo_bytes);
			qmi8658_read_reg(Qmi8658Register_FifoCount, fifo_status, 2);
			fifo_bytes = (unsigned short)(((fifo_status[1]&0x03)<<8)|fifo_status[0]);
			if(fifo_bytes > 0)
			{
				qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_Rst_Fifo);
			}
		}
	
		qmi8658_write_reg(Qmi8658Register_FifoCtrl, g_imu.cfg.fifo_ctrl);
	}

	return fifo_level;
}
#endif

#if defined(QMI8658_USE_HW_SELFTEST)
void qmi8658_do_hw_selftest(int enSensor) //��ʼ������� ��оƬ˿ӡ���ϣ�ˮƽ��ֹ�ڷš�
{
	unsigned char	status_int = 0x00;
	unsigned int	retry = 0;
	unsigned char	reg[6];
	short	raw[3];
	float	st_out[3];
	uint8_t err = 0;
	if(enSensor & QMI8658_ACC_ENABLE)
	{
		qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);
		qmi8658_delay(2);
		qmi8658_write_reg(Qmi8658Register_Ctrl2, Qmi8658AccRange_8g|Qmi8658AccOdr_250Hz|0x80);
		status_int = 0;
		retry = 0;
		while(!(status_int & 0x01))
		{
			qmi8658_read_reg(Qmi8658Register_StatusInt, &status_int, 1);
			qmi8658_delay(1);
			//qmi8658_log("count=%d ",retry);
			if(retry++ > 5000)
			{
				qmi8658_log("wati int high timeout\n");
				break;
			}
		}
		qmi8658_write_reg(Qmi8658Register_Ctrl2, Qmi8658AccRange_8g|Qmi8658AccOdr_250Hz);
		retry = 0;
		status_int = 0x01;
		while((status_int & 0x01))
		{
			qmi8658_read_reg(Qmi8658Register_StatusInt, &status_int, 1);
			qmi8658_delay(1);
			//qmi8658_log("count=%d ",retry);
			if(retry++ > 5000)
			{
				qmi8658_log("wati int low timeout\n");
				break;
			}
		}
		qmi8658_read_reg(Qmi8658Register_Dvx_L, reg, 6);
		raw[0] = (short)((unsigned short)(reg[1]<<8) |( reg[0]));
		raw[1] = (short)((unsigned short)(reg[3]<<8) |( reg[2]));
		raw[2] = (short)((unsigned short)(reg[5]<<8) |( reg[4]));
		st_out[0] = (float)(raw[0]*1000.0f/2048);	// mg
		st_out[1] = (float)(raw[1]*1000.0f/2048);
		st_out[2] = (float)(raw[2]*1000.0f/2048);
		g_imu.st_out[0] = st_out[0];
		g_imu.st_out[1] = st_out[1];
		g_imu.st_out[2] = st_out[2];
		if((QFABS(st_out[0]) > 200) && (QFABS(st_out[1]) > 200) && (QFABS(st_out[2]) > 200))
		{
			//qmi8658_log("acc-selftest raw[%d	%d	%d] out[%f	%f	%f] Pass!\n", raw[0],raw[1],raw[2],st_out[0],st_out[1],st_out[2]);
		}
		else
		{
			err++;
			qmi8658_log("acc-selftest raw[%d	%d	%d] out[%f	%f	%f] Fail!\n", raw[0],raw[1],raw[2],st_out[0],st_out[1],st_out[2]);
		}
	}

	if(enSensor & QMI8658_GYR_ENABLE)
	{	
		qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);
		qmi8658_delay(2);
		qmi8658_write_reg(Qmi8658Register_Ctrl3, Qmi8658GyrRange_1024dps|Qmi8658GyrOdr_250Hz|0x80);
		status_int = 0;
		retry = 0;
		while(!(status_int & 0x01))
		{
			qmi8658_read_reg(Qmi8658Register_StatusInt, &status_int, 1);
			qmi8658_delay(1);
			if(retry++ > 5000)
			{
				qmi8658_log("wati int high timeout\n");
				break;
			}
		}
		qmi8658_write_reg(Qmi8658Register_Ctrl3, Qmi8658GyrRange_1024dps|Qmi8658GyrOdr_250Hz);
		retry = 0;
		status_int = 0x01;
		while((status_int & 0x01))
		{
			qmi8658_read_reg(Qmi8658Register_StatusInt, &status_int, 1);
			qmi8658_delay(1);
			if(retry++ > 5000)
			{
				qmi8658_log("wati int low timeout\n");
				break;
			}
		}
		qmi8658_read_reg(Qmi8658Register_Dvx_L, reg, 6);
		raw[0] = (short)((unsigned short)(reg[1]<<8) |( reg[0]));
		raw[1] = (short)((unsigned short)(reg[3]<<8) |( reg[2]));
		raw[2] = (short)((unsigned short)(reg[5]<<8) |( reg[4]));
		st_out[0] = (float)(raw[0]/16.0f);	// dps
		st_out[1] = (float)(raw[1]/16.0f);
		st_out[2] = (float)(raw[2]/16.0f);	
		g_imu.st_out[3] = st_out[0];
		g_imu.st_out[4] = st_out[1];
		g_imu.st_out[5] = st_out[2];
		if((QFABS(st_out[0]) > 300) && (QFABS(st_out[1]) > 300) && (QFABS(st_out[2]) > 300))
		{
			//qmi8658_log("gyr-selftest raw[%d	%d	%d] out[%f	%f	%f] Pass!\n", raw[0],raw[1],raw[2],st_out[0],st_out[1],st_out[2]);
		}
		else
		{
			err++;
			qmi8658_log("gyr-selftest raw[%d	%d	%d] out[%f	%f	%f] Fail!\n", raw[0],raw[1],raw[2],st_out[0],st_out[1],st_out[2]);
		}

		if(err > 0)
		{
			qmi8658_log("selftest Fail!\n")
		}
		else
		{
			qmi8658_log("selftest succeed!\n")
		}
	}
}

void qmi8658_get_hw_selftest_data(float out[6])
{
	out[0] = g_imu.st_out[0];
	out[1] = g_imu.st_out[1];
	out[2] = g_imu.st_out[2];
	out[3] = g_imu.st_out[3];
	out[4] = g_imu.st_out[4];
	out[5] = g_imu.st_out[5];
}
#endif

#if defined(QMI8658_DISABLE_SDX_SCX_PURES)
void disable_puRes(void)
{
	qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x02);//disable sdx,scx pull up resistors
	qmi8658_send_ctl9cmd(qmi8658_Ctrl9_Cmd_SetRpu);
}
#endif



unsigned char qmi8658_init(void)
{
	if(qmi8658_get_id() == 0x05)
	{	
#if defined(QMI8658_SYNC_SAMPLE_MODE)
		qmi8658_enable_AHB_clock(0);
		g_imu.cfg.syncSample = 1;
#endif

#if defined(QMI8658_DISABLE_SDX_SCX_PURES)
		disable_puRes();
#endif

#if defined(QMI8658_USE_AMD)||defined(QMI8658_USE_NO_MOTION)||defined(QMI8658_USE_SIG_MOTION)
		qmi8658_config_motion();
#endif
#if defined(QMI8658_USE_TAP)
		qmi8658_config_tap();
#endif
#if defined(QMI8658_USE_PEDOMETER)
		qmi8658_config_pedometer(125);
		qmi8658_enable_pedometer(1);
#endif
		qmi8658_config_reg(0);
		qmi8658_enableSensors(g_imu.cfg.enSensors);
#if defined(QMI8658_USE_AMD)
		qmi8658_enable_amd(1,qmi8658_Int1,1);
#endif

		qmi8658_dump_reg();
#if defined(GYRO_DYNAMIC_CALIBRATION)
		gyro_raw_static_deviation = 0.005f*57.29578f*g_imu.ssvt_g;
		accel_raw_static_deviation = 0.04f/9.8f*g_imu.ssvt_a;
#endif
		qmi8658_delay(300);
		return 1;
	}
	else
	{
		qmi8658_log("qmi8658_init fail\n");
		return 0;
	}
}

void qmi8658_powerdown(void)
{
#if defined(QMI8658_DISABLE_SDX_SCX_PURES)
	disable_puRes();
#endif
	qmi8658_write_reg(Qmi8658Register_Ctrl1,0x60);
	qmi8658_write_reg(Qmi8658Register_Ctrl2,0x0f);
	qmi8658_write_reg(Qmi8658Register_Ctrl3,0x0f);
	qmi8658_write_reg(Qmi8658Register_Ctrl7,0x00);
}
void qmi8658_wake_up(void)
{
	qmi8658_write_reg(Qmi8658Register_Ctrl1, 0x60|QMI8658_INT2_ENABLE|QMI8658_INT1_ENABLE);
	qmi8658_config_reg(0);
	qmi8658_enableSensors(g_imu.cfg.enSensors);
	qmi8658_delay(300);
}
