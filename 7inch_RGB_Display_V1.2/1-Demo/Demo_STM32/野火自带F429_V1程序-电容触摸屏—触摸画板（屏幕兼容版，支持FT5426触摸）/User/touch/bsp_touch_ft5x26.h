#ifndef __BSP_TOUCH_FT5X26_H__
#define __BSP_TOUCH_FT5X26_H__

#include "stm32f4xx.h"

//IIC从设备地址
#define FT_ADDRESS 0x70

/* 表示读数据 */ 
#define I2C_FT_RD		1
#define I2C_FT_WR		0	
 /*
 * 存储I2C通讯的信息
 * @addr：  从设备的I2C设备地址	
 * @flags: 控制标志
 * @len：  读写数据的长度
 * @buf：  存储读写数据的指针
 **/
struct i2c_ft_msg {
	uint8_t addr;		/*从设备的I2C设备地址 */
	uint16_t flags;	/*控制标志*/
	uint16_t len;		/*读写数据的长度			*/
	uint8_t *buf;		/*存储读写数据的指针	*/
};

//FT5X26 部分寄存器定义
#define FT_DEVIDE_MODE 				0x00   		//FT5X26模式控制寄存器
#define FT_REG_NUM_FINGER   	0x02		  //触摸状态寄存器

#define FT_TP1_REG 						0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 						0X09		  //第二个触摸点数据地址
#define FT_TP3_REG 						0X0F		  //第三个触摸点数据地址
#define FT_TP4_REG 						0X15		  //第四个触摸点数据地址
#define FT_TP5_REG 						0X1B		  //第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		  //版本		
#define FT_ID_G_MODE 			    0xA4   		//FT5X26中断模式控制寄存器
#define FT_ID_G_THGROUP			  0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器

#define FT_ADDR_LENGTH        1    //寄存器地址长度
#define FT_MAX_TOUCH          5    //最大触摸点数

void FT_TouchProcess(void);
int32_t FT5X26_Init_Panel(void);
uint16_t FT5X26_Read_ID(void);

#endif
