#include "MPU6050DMP.h"
#include "stdio.h"
#include "math.h"
#include "clock_calendar.h"


/* Buffer of data to be received by I2C1 */
uint8_t IIC_Buffer_Rx[25];
/* Buffer of data to be transmitted by I2C1 */
uint8_t IIC_Buffer_Tx[25] = {0x5, 0x6,0x8,0xA};

//变量定义
long temperature;
struct int_param_s int_param;
int result;
unsigned short gyro_rate, gyro_fsr;
unsigned char accel_fsr;
static signed char gyro_orientation[9] = {1,  0,  0,
                                           0, 1,  0,
                                           0,  0,  1};

short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
unsigned long timestamp;
	
struct dmpEuler_s sEuler;
struct dmpGravity_s sGravity;
struct dmpQuaternion_s sQuaternion;
struct dmpYawPitchRoll_s sYawPitchRoll;
//变量定义结束

//**************************************
//初始化MPU6050
//**************************************
int InitMPU6050()
{
	Status rtn;
	//Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	IIC_Buffer_Tx[0]=PWR_MGMT_1;
	IIC_Buffer_Tx[1]=0x01;
  rtn=I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,2,Polling, MPU6050_Addr);
	
	//Single_WriteI2C(SMPLRT_DIV, 0x07);
	IIC_Buffer_Tx[0]=SMPLRT_DIV;
	IIC_Buffer_Tx[1]=0x31;
  rtn=I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,2,Polling, MPU6050_Addr);
	
	
	//Single_WriteI2C(MPU6050_CONFIG, 0x06);
	IIC_Buffer_Tx[0]=MPU6050_CONFIG;
	IIC_Buffer_Tx[1]=0x06;
  rtn=I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,2,Polling, MPU6050_Addr);
	
	//Single_WriteI2C(GYRO_CONFIG, 0x18);
	IIC_Buffer_Tx[0]=GYRO_CONFIG;
	IIC_Buffer_Tx[1]=0x18;
  rtn=I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,2,Polling, MPU6050_Addr);
	
	//Single_WriteI2C(ACCEL_CONFIG, 0x01);
	IIC_Buffer_Tx[0]=ACCEL_CONFIG;
	IIC_Buffer_Tx[1]=0x01;
  rtn=I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,2,Polling, MPU6050_Addr);
	
	
		//配置Aux I2C 为 bypass mode , 0x37的bit1=1 而且 0x6A的bit
	IIC_Buffer_Tx[0] = 0x37;
	IIC_Buffer_Tx[1] = 0x02;  //0x37 / INT_LEVEL INT_OPEN LATCH _INT_EN INT_RD _CLEAR FSYNC_ INT_LEVEL FSYNC_ INT_EN I2C _BYPASS _EN CLKOUT _EN
	result = I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,2,Polling, MPU6050_Addr);
	
	IIC_Buffer_Tx[0] = 0x6A;  //0x6A  / - FIFO_EN[6] I2C_MST_EN[5] I2C_IF_DIS[4] - FIFO_RESET[2] I2C_MST_RESET[1] SIG_COND_RESET[0]
	IIC_Buffer_Tx[1] = 0x00; 
	result = I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,2,Polling, MPU6050_Addr);
	
	return !rtn;
}

//**************************************
//读指定地址数据
//**************************************
uint16_t MPU6050_GetData(uint8_t REG_Address)
{
	Status rtn;
	char H,L;
	//H=Single_ReadI2C(REG_Address);
		IIC_Buffer_Tx[0]=REG_Address;
	  I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,1,Polling, MPU6050_Addr);
		rtn=I2C_Master_BufferRead(MPU6050_I2C,IIC_Buffer_Rx,1,Polling, MPU6050_Addr);	
		if(rtn == Success) H=IIC_Buffer_Rx[0];
		else H=0;
	//L=Single_ReadI2C(REG_Address+1);
		IIC_Buffer_Tx[0]=REG_Address+1;
	  I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,1,Polling, MPU6050_Addr);
		rtn=I2C_Master_BufferRead(MPU6050_I2C,IIC_Buffer_Rx,1,Polling, MPU6050_Addr);
		if(rtn == Success) L=IIC_Buffer_Rx[0];
		else L=0;
	return (H<<8)+L;   //合成数据
}

//**************************************
//连续读ax,ay,az,temperature,gx,gy,gz数据
//**************************************
int MPU6050_ReadRawData(struct MPU6050_RawData_s *s_IMUVar)
{
	Status rtn;
	char H,L;

	IIC_Buffer_Tx[0]=ACCEL_XOUT_H;
  I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,1,Polling, MPU6050_Addr);
	rtn=I2C_Master_BufferRead(MPU6050_I2C,IIC_Buffer_Rx,14,DMA, MPU6050_Addr);	
	if(rtn == Success) 
	{
		H=IIC_Buffer_Rx[0];
		L=IIC_Buffer_Rx[1];
		s_IMUVar->ax = (H<<8)+L;
		
		H=IIC_Buffer_Rx[2];
		L=IIC_Buffer_Rx[3];
		s_IMUVar->ay = (H<<8)+L;
		
		H=IIC_Buffer_Rx[4];
		L=IIC_Buffer_Rx[5];
		s_IMUVar->az = (H<<8)+L;
		
		H=IIC_Buffer_Rx[6];
		L=IIC_Buffer_Rx[7];
		s_IMUVar->temperature = (H<<8)+L;
		
		H=IIC_Buffer_Rx[8];
		L=IIC_Buffer_Rx[9];
		s_IMUVar->gx = (H<<8)+L;
		
		H=IIC_Buffer_Rx[10];
		L=IIC_Buffer_Rx[11];
		s_IMUVar->gy = (H<<8)+L;
		
		H=IIC_Buffer_Rx[12];
		L=IIC_Buffer_Rx[13];
		s_IMUVar->gz = (H<<8)+L;

	}
	return !rtn;
}


int stm32_i2c_write(unsigned char slave_addr,
                     unsigned char reg_addr,
                     unsigned char length,
                     unsigned char const *data)
{
    unsigned char i;
		Status rtn;
    if (!length)
        return 0;
		
		if(length > 24) return -1;
		
		IIC_Buffer_Tx[0] = reg_addr;
		for(i=0;i<length;i++)
		{
			IIC_Buffer_Tx[i+1]=data[i];
		}
		rtn=I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,length+1,Polling, slave_addr);
    if(rtn==Success)return 0;
		else return -1;
}


int stm32_i2c_read(unsigned char slave_addr,
                    unsigned char reg_addr,
                    unsigned char length,
                    unsigned char *data)
{
		Status rtn;
    if (!length)
        return 0;

		rtn=I2C_Master_BufferWrite(MPU6050_I2C, &reg_addr,1,Polling, slave_addr);
		if(rtn!=Success) return -1;
		if(length<2) rtn=I2C_Master_BufferRead(MPU6050_I2C, (uint8_t*)data,length,Polling, slave_addr);
		else rtn=I2C_Master_BufferRead(MPU6050_I2C, (uint8_t*)data,length,DMA, slave_addr);
    if(rtn==Success)
		{
			
			return 0;
		}
		else 
		{
			return -1;
		}
}

int stm32_get_clock_ms(unsigned long *count)
{
    if (!count)
        return 1;
    count[0] = GetTimeStamp_ms();
    return 0;
}
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

void mpu6050_run_self_test(void)
{
    int rtn;
    long gyro[3], accel[3];
		float sens;
		unsigned short accel_sens;

    rtn = mpu_run_self_test(gyro, accel);
    if (rtn == 0x3) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */

        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
//			printf("\r\nSelf Test Passed!\r\n");
    }
}

//
int dmpGetGravity(struct dmpGravity_s *v, struct dmpQuaternion_s *q) {
    v->x = 2 * (q -> x*q -> z - q -> w*q -> y);
    v->y = 2 * (q -> w*q -> x + q -> y*q -> z);
    v->z = q -> w*q -> w - q -> x*q -> x - q -> y*q -> y + q -> z*q -> z;
    return 0;
}

int dmpGetEuler(struct dmpEuler_s *data, struct dmpQuaternion_s *q) {
    data->psi = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);   // psi
    data->theta = -asin(2*q -> x*q -> z + 2*q -> w*q -> y);                              // theta
    data->phi = atan2(2*q -> y*q -> z - 2*q -> w*q -> x, 2*q -> w*q -> w + 2*q -> z*q -> z - 1);   // phi
    return 0;
}

int dmpGetYawPitchRoll(struct dmpYawPitchRoll_s *data, struct dmpQuaternion_s *q, struct dmpGravity_s *gravity) {
    // yaw: (about Z axis) 偏航角
    data->yaw = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);
    // pitch: (nose up/down, about Y axis) 俯仰角
    data->pitch = atan(gravity -> x / sqrt(gravity -> y*gravity -> y + gravity -> z*gravity -> z));
    // roll: (tilt left/right, about X axis) 横滚角
    data->roll = atan(gravity -> y / sqrt(gravity -> x*gravity -> x + gravity -> z*gravity -> z));
    return 0;
}

int MPU6050GetYawPitchRoll(struct dmpYawPitchRoll_s *var)
{
	if(mpu_set_dmp_state(1))
	{
		return 0;
	}
	do
	{
		result=dmp_read_fifo( gyro, accel, quat, &timestamp, &sensors, &more );
	}
	while(more);
	
	if(!result && (sensors & INV_WXYZ_QUAT))
	{
		
		sQuaternion.w=quat[0]/1073741824.0f;
		sQuaternion.x=quat[1]/1073741824.0f;
		sQuaternion.y=quat[2]/1073741824.0f;
		sQuaternion.z=quat[3]/1073741824.0f;

		//计算欧拉角
		//dmpGetEuler(&sEuler,&sQuaternion);
		
		//计算俯仰角
		
		dmpGetGravity(&sGravity,&sQuaternion);
		dmpGetYawPitchRoll(&sYawPitchRoll,&sQuaternion,&sGravity);
		
		var->yaw = sYawPitchRoll.yaw;
		var->pitch = sYawPitchRoll.pitch;
		var->roll = sYawPitchRoll.roll;
		
		return 1;
	}
	return 0;
}

int InitMPU6050DMP()
{
    /* Set up MSP430 hardware. */
    result = InitMPU6050();

    /* Set up gyro.
     * Every function preceded by mpu_ is a driver function and can be found
     * in inv_mpu.h.
     */
    result = mpu_init(&int_param);
		
		
		    /* If you're not using an MPU9150 AND you're not using DMP features, this
     * function will place all slaves on the primary bus.
     * mpu_set_bypass(1);
     */

    /* Get/set hardware configuration. Start gyro. */
    /* Wake up all sensors. */
//     result = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);  //注意！！！启用外部罗盘，这里要包含 INV_XYZ_COMPASS
		result = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    /* Push both gyro and accel data into the FIFO. */
    result = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    result = mpu_set_sample_rate(100);
    /* Read back configuration in case it was set improperly. */
    result = mpu_get_sample_rate(&gyro_rate);
    result = mpu_get_gyro_fsr(&gyro_fsr);
    result = mpu_get_accel_fsr(&accel_fsr);

    /* Initialize HAL state variables. */
//     memset(&hal, 0, sizeof(hal));
//     hal.sensors = ACCEL_ON | GYRO_ON;
//     hal.report = PRINT_QUAT;

    /* To initialize the DMP:
     * 1. Call dmp_load_motion_driver_firmware(). This pushes the DMP image in
     *    inv_mpu_dmp_motion_driver.h into the MPU memory.
     * 2. Push the gyro and accel orientation matrix to the DMP.
     * 3. Register gesture callbacks. Don't worry, these callbacks won't be
     *    executed unless the corresponding feature is enabled.
     * 4. Call dmp_enable_feature(mask) to enable different features.
     * 5. Call dmp_set_fifo_rate(freq) to select a DMP output rate.
     * 6. Call any feature-specific control functions.
     *
     * To enable the DMP, just call mpu_set_dmp_state(1). This function can
     * be called repeatedly to enable and disable the DMP at runtime.
     *
     * The following is a short summary of the features supported in the DMP
     * image provided in inv_mpu_dmp_motion_driver.c:
     * DMP_FEATURE_LP_QUAT: Generate a gyro-only quaternion on the DMP at
     * 200Hz. Integrating the gyro data at higher rates reduces numerical
     * errors (compared to integration on the MCU at a lower sampling rate).
     * DMP_FEATURE_6X_LP_QUAT: Generate a gyro/accel quaternion on the DMP at
     * 200Hz. Cannot be used in combination with DMP_FEATURE_LP_QUAT.
     * DMP_FEATURE_TAP: Detect taps along the X, Y, and Z axes.
     * DMP_FEATURE_ANDROID_ORIENT: Google's screen rotation algorithm. Triggers
     * an event at the four orientations where the screen should rotate.
     * DMP_FEATURE_GYRO_CAL: Calibrates the gyro data after eight seconds of
     * no motion.
     * DMP_FEATURE_SEND_RAW_ACCEL: Add raw accelerometer data to the FIFO.
     * DMP_FEATURE_SEND_RAW_GYRO: Add raw gyro data to the FIFO.
     * DMP_FEATURE_SEND_CAL_GYRO: Add calibrated gyro data to the FIFO. Cannot
     * be used in combination with DMP_FEATURE_SEND_RAW_GYRO.
     */
    result = dmp_load_motion_driver_firmware();
    result = dmp_set_orientation(
    inv_orientation_matrix_to_scalar(gyro_orientation));
//     dmp_register_tap_cb(tap_cb);
//     dmp_register_android_orient_cb(android_orient_cb);

    result = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | /* DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT |*/ DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL);
    result = dmp_set_fifo_rate(100);
    result = mpu_set_dmp_state(1);
		//启用运动检测，第一个参数检测限值，32mg增量，第二个参数时间单位毫秒，第三个参数滤波频率
		result = mpu_lp_motion_interrupt(640,1,2);
		mpu6050_run_self_test();

// 		mpu_set_bypass(1);

		return result;
}
//读取MPU6050 I2C Slave0 的数据
int MPU6050_Read_Ext_Sens_Data(uint8_t RegAddr,uint8_t *buff, uint8_t length)
{
	Status rtn;
	IIC_Buffer_Tx[0] = RegAddr;  
	rtn = I2C_Master_BufferWrite(MPU6050_I2C, IIC_Buffer_Tx,1,Polling, MPU6050_Addr);
	rtn = I2C_Master_BufferRead(MPU6050_I2C, buff,length,DMA, MPU6050_Addr);
	return rtn;
}
