//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F429IGT6,����ԭ��Apollo STM32F4/F7������,��Ƶ180MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 FSMC
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
//��ģ�����ֱ�Ӳ�������ԭ��Apollo STM32F4/F7������TFTLCD��ۣ������ֶ�����
//STM32����������ָTFTLCD��������ڲ����ӵ�STM32����
//=========================================��Դ����================================================//
//     LCDģ��             TFTLCD�������        STM32��������
//      VDD       --->         5V/3.3              DC5V/3.3V          //��Դ
//      GND       --->          GND                  GND              //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ16λ��������
//     LCDģ��             TFTLCD�������        STM32��������
//      D0        --->          D0                   PD14        -|   
//      D1        --->          D1                   PD15         |  
//      D2        --->          D2                   PD0          | 
//      D3        --->          D3                   PD1          | 
//      D4        --->          D4                   PE7          |
//      D5        --->          D5                   PE8          |
//      D6        --->          D6                   PE9          |
//      D7        --->          D7                   PE10         |
//���ʹ��8λģʽ���������������û��ʹ��                         |===>Һ����16λ���������ź�
//      D8        --->          D8                   PE11         |
//      D9        --->          D9                   PE12         |
//      D10       --->          D10                  PE13         |
//      D11       --->          D11                  PE14         |
//      D12       --->          D12                  PE15         |
//      D13       --->          D13                  PD8          |
//      D14       --->          D14                  PD9          |
//      D15       --->          D15                  PD10        -|
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 				     TFTLCD�������        STM32�������� 
//       WR       --->          WR                   PD5             //Һ����д���ݿ����ź�
//       RD       --->          RD                   PD4             //Һ���������ݿ����ź�
//       RS       --->          RS                   PD13            //Һ��������/��������ź�
//       RST      --->          RST                ��λ����          //Һ������λ�����ź�
//       CS       --->          CS                   PD7             //Һ����Ƭѡ�����ź�
//       BL       --->          BL                   PB5             //Һ������������ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��             TFTLCD�������        STM32�������� 
//      INI       --->          PEN                  PH7             //�����������ж��ź�
//      SDA       --->          MOSI                 PI3             //������IIC����д�ź�
//      CRST      --->          TCS                  PI8             //��������λ�����ź�
//      CLK       --->          CLK                  PH6             //������IIC����ʱ���ź�
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
#include "lcd.h"
#include "mcu_lcd.h"
#include "rgb_lcd.h"

//������ɫ,������ɫ
u32 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  

_lcd_dev lcd_dev;

void LCD_Init(void)
{
	lcd_dev.id = RGBLCD_Read_ID();
	if(lcd_dev.id)
	{
		lcd_dev.type = RGB_LCD;
		RGBLCD_Init();
	}
	else
	{
		lcd_dev.type = MCU_LCD;
		MCULCD_Init();
		lcd_dev.id = MCULCD_Read_ID();
	}
}

u16 LCD_Read(void)
{
	if(lcd_dev.type == MCU_LCD)
	{
		return MCULCD_Read();
	}
	else
	{
		return 0;
	}
}

void LCD_Clear(u32 Color)
{
	if(lcd_dev.type == RGB_LCD)
	{
		RGBLCD_Clear(Color);
	}
	else if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Clear((u16)Color);
	}
}

void LCD_Set_Cursor(u16 Xpos, u16 Ypos)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Set_Cursor(Xpos, Ypos);
	}
	else
	{
		return;
	}
}

void LCD_Draw_Point(u16 x,u16 y, u32 color)
{
	if(lcd_dev.type == RGB_LCD)
	{
		RGBLCD_Draw_Point(x, y, color);		
	}
	else if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Draw_Point(x, y, (u16)color);
	}
}

u32 LCD_Read_Point(u16 x,u16 y)
{
	if(lcd_dev.type == RGB_LCD)
	{
		return RGBLCD_Read_Point(x,y);		
	}
	else if(lcd_dev.type == MCU_LCD)
	{
		return (u16)MCULCD_Read_Point(x,y);
	}
	return 0;
}

void LCD_Set_Windows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Set_Windows(xStar, yStar, xEnd, yEnd);
	}
	else
	{
		return;
	}
}

u16 LCD_RD_DATA(void)
{
	if(lcd_dev.type == MCU_LCD)
	{
		return MCULCD_RD_DATA();
	}
	else
	{
		return 0;
	}
}

void LCD_Write_Reg(u16 LCD_Reg, u16 LCD_RegValue)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Write_Reg(LCD_Reg, LCD_RegValue);
	}
	else
	{
		return;
	}
}

void LCD_WR_REG(u16 data)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_WR_REG(data);
	}
	else
	{
		return;
	}
}

void LCD_WR_DATA(u16 data)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_WR_DATA(data);
	}
	else
	{
		return;
	}
}

void LCD_Read_Reg(u16 LCD_Reg,u8 *Rval,int n)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Read_Reg(LCD_Reg, Rval, n);
	}
	else
	{
		return;
	}
}

void LCD_Write_RAM_Prepare(void)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Write_RAM_Prepare();
	}
	else
	{
		return;
	}
}

void LCD_Read_RAM_Prepare(void)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Read_RAM_Prepare();
	}
	else
	{
		return;
	}
}
   
void LCD_Write_Data_16Bit(u16 Data)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Write_Data_16Bit(Data);
	}
	else
	{
		return;
	}
}

u16 LCD_Read_Data_16Bit(void)
{
	if(lcd_dev.type == MCU_LCD)
	{
		return MCULCD_Read_Data_16Bit();
	}
	else
	{
		return 0;
	}
}

void LCD_Set_Direction(u8 direction)
{
	if(lcd_dev.type == RGB_LCD)
	{
		RGBLCD_Display_Dir(direction);		
	}
	else if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Set_Direction(direction);
	}
}

u16 LCD_Read_ID(void)
{
	if(lcd_dev.type == RGB_LCD)
	{
		return RGBLCD_Read_ID();		
	}
	else if(lcd_dev.type == MCU_LCD)
	{
		return MCULCD_Read_ID();
	}
	return 0;
}

void LCD_Set_PWM_BackLight(u8 pwm)
{
	if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Set_PWM_BackLight(pwm);
	}
	else
	{
		return;
	}
}

void LCD_Fill_Color(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{
	if(lcd_dev.type == RGB_LCD)
	{
		RGBLCD_Fill_color(sx, sy, ex, ey, color);		
	}
	else if(lcd_dev.type == MCU_LCD)
	{
		MCULCD_Fill_Color( sx, sy, ex, ey,(u16)color);
	}
}
