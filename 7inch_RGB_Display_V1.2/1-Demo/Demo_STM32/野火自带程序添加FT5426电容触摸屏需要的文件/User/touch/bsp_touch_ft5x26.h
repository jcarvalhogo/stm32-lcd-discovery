#ifndef __BSP_TOUCH_FT5X26_H__
#define __BSP_TOUCH_FT5X26_H__

#include "stm32f4xx.h"

//IIC���豸��ַ
#define FT_ADDRESS 0x70

/* ��ʾ������ */ 
#define I2C_FT_RD		1
#define I2C_FT_WR		0	
 /*
 * �洢I2CͨѶ����Ϣ
 * @addr��  ���豸��I2C�豸��ַ	
 * @flags: ���Ʊ�־
 * @len��  ��д���ݵĳ���
 * @buf��  �洢��д���ݵ�ָ��
 **/
struct i2c_ft_msg {
	uint8_t addr;		/*���豸��I2C�豸��ַ */
	uint16_t flags;	/*���Ʊ�־*/
	uint16_t len;		/*��д���ݵĳ���			*/
	uint8_t *buf;		/*�洢��д���ݵ�ָ��	*/
};

//FT5X26 ���ּĴ�������
#define FT_DEVIDE_MODE 				0x00   		//FT5X26ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER   	0x02		  //����״̬�Ĵ���

#define FT_TP1_REG 						0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 						0X09		  //�ڶ������������ݵ�ַ
#define FT_TP3_REG 						0X0F		  //���������������ݵ�ַ
#define FT_TP4_REG 						0X15		  //���ĸ����������ݵ�ַ
#define FT_TP5_REG 						0X1B		  //��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1		  //�汾		
#define FT_ID_G_MODE 			    0xA4   		//FT5X26�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			  0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���

#define FT_ADDR_LENGTH        1    //�Ĵ�����ַ����
#define FT_MAX_TOUCH          5    //���������

void FT_TouchProcess(void);
int32_t FT5X26_Init_Panel(void);
uint16_t FT5X26_Read_ID(void);

#endif
