//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RCT6,����ԭ��MiniSTM32������,��Ƶ72MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/09
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//��ģ�����ֱ�Ӳ�������ԭ��MiniSTM32������TFTLCD��ۣ������ֶ�����
//STM32����������ָTFTLCD��������ڲ����ӵ�STM32����
//=========================================��Դ����================================================//
//     LCDģ��             TFTLCD�������        STM32��������
//      VDD       --->         5V/3.3              DC5V/3.3V          //��Դ
//      GND       --->          GND                  GND              //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ16λ��������
//     LCDģ��             TFTLCD�������        STM32��������
//     D0~D15     --->         D0~D15               PB0~PB15          //Һ����16λ���������źţ�8λģʽ��DB0~DB7��D0~D7��
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 				     TFTLCD�������        STM32�������� 
//       WR       --->          WR                   PC7             //Һ����д���ݿ����ź�
//       RD       --->          RD                   PC6             //Һ���������ݿ����ź�
//       RS       --->          RS                   PC8             //Һ��������/��������ź�
//       RST      --->          RST                  PC4             //Һ������λ�����ź�
//       CS       --->          CS                   PC9             //Һ����Ƭѡ�����ź�
//       BL       --->          BL                   PC10            //Һ������������ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��             TFTLCD�������        STM32�������� 
//      INI       --->          PEN                  PC1             //�����������ж��ź�
//      SDA       --->          MOSI                 PC3             //������IIC���������ź�
//      CRST      --->          TCS                  PC13            //��������λ�ź�
//      CLK       --->          CLK                  PC0             //������IIC����ʱ���ź�
**************************************************************************************************/			
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/		
#include "sys.h"
#include "exti.h"	

//�жϴ�����־λ
u8 touch_flag=0;   //0-û�д����жϣ�1-�����ж�

/*****************************************************************************
 * @name       :void Touch_EXTI_Init(void)
 * @date       :2020-05-13 
 * @function   :DInitialization  PC1 exti 
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Touch_EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOH_CLK_ENABLE();               //����GPIOHʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_7;               //PC13
  GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //�½��ش���
  GPIO_Initure.Pull=GPIO_PULLUP;				//����
  HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,2);   //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);         //ʹ���ж���13  
	touch_flag=0;
}

/*****************************************************************************
 * @name       :void EXTI1_IRQHandler(void)
 * @date       :2020-05-13 
 * @function   :PC1 Interrupt Processing Function 
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);  //�����жϴ����ú���
}

/*****************************************************************************
 * @name       :void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
 * @date       :2020-05-13 
 * @function   :EXTI line detection callback 
 * @parameters :GPIO_Pin:Specifies the port pin connected to corresponding EXTI line
 * @retvalue   :None
******************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_7)
	{
		touch_flag=1;
	}
}
