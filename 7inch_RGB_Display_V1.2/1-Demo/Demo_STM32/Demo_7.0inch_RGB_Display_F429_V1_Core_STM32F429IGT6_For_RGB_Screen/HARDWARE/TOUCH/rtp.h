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
#ifndef __RTP_H__
#define __RTP_H__
#include "sys.h"

  	
//������оƬ��������	   
#define PEN     PHin(7)    //T_PEN
#define DOUT    PGin(3)    //T_MISO
#define TDIN    PIout(3)   //T_MOSI
#define TCLK    PHout(6)   //T_SCK
#define TCS     PIout(8)   //T_CS  
     
void RTP_Write_Byte(u8 num);						//�����оƬд��һ������
u16 RTP_Read_AD(u8 CMD);							//��ȡADת��ֵ
u16 RTP_Read_XOY(u8 xy);							//���˲��������ȡ(X/Y)
u8 RTP_Read_XY(u16 *x,u16 *y);					//˫�����ȡ(X+Y)
u8 RTP_Read_XY2(u16 *x,u16 *y);					//����ǿ�˲���˫���������ȡ
void RTP_Drow_Touch_Point(u16 x,u16 y,u32 color);//��һ������У׼��
void RTP_Draw_Big_Point(u16 x,u16 y,u32 color);	//��һ�����
u8 RTP_Scan(u8 tp);								//ɨ��
void RTP_Save_Adjdata(void);						//����У׼����
u8 RTP_Get_Adjdata(void);						//��ȡУ׼����
void RTP_Adjust(void);							//������У׼
u8 RTP_Init(void);								//��ʼ��														 
void RTP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//��ʾУ׼��Ϣ
																 		  
#endif
