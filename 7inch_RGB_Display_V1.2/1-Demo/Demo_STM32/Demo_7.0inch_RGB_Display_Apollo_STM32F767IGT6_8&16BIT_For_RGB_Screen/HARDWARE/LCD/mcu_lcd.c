//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32H743IIT6,����ԭ��Apollo STM32F4/F7������,��Ƶ400MHZ������12MHZ
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
#include "mcu_lcd.h"
#include "stdlib.h"
#include "delay.h"
#include "lcd.h"	 

SRAM_HandleTypeDef SRAM_Handler;    //SRAM���(���ڿ���LCD)
	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_mculcd_dev mculcd_dev;

u16 MCULCD_Read(void)
{
	vu16 data;			//��ֹ���Ż�
	data=MCULCD->MCULCD_RAM;	
	return data;
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void MCULCD_WR_REG(vu16 data)
{ 
	data=data;		//ʹ��-O2�Ż���ʱ��,����������ʱ
	MCULCD->MCULCD_REG=data;//д��Ҫд�ļĴ������	 
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u16 data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void MCULCD_WR_DATA(vu16 data)
{
	data=data;			//ʹ��-O2�Ż���ʱ��,����������ʱ
	MCULCD->MCULCD_RAM=data;	
}

/*****************************************************************************
 * @name       :u16 LCD_RD_DATA(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/
u16 MCULCD_RD_DATA(void)
{
	return MCULCD_Read();
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void MCULCD_Write_Reg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	MCULCD->MCULCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	MCULCD->MCULCD_RAM = LCD_RegValue;//д������	    		 
}	   

/*****************************************************************************
 * @name       :u16 LCD_ReadReg(u16 LCD_Reg)
 * @date       :2018-11-13 
 * @function   :read value from specially registers
 * @parameters :LCD_Reg:Register address
 * @retvalue   :read value
******************************************************************************/
void MCULCD_Read_Reg(u16 LCD_Reg,u8 *Rval,int n)
{
	MCULCD_WR_REG(LCD_Reg); 
	while(n--)
	{		
		*(Rval++) = MCULCD_RD_DATA();
		delay_us(5);	
	}
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void MCULCD_Write_RAM_Prepare(void)
{
	LCD_WR_REG(mculcd_dev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void LCD_ReadRAM_Prepare(void)
 * @date       :2018-11-13 
 * @function   :Read GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void MCULCD_Read_RAM_Prepare(void)
{
	MCULCD_WR_REG(mculcd_dev.rramcmd);
}

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void MCULCD_Write_Data_16Bit(u16 Data)
{
#if MCULCD_USE8BIT_MODEL	
	 MCULCD->MCULCD_RAM = (Data>>8)&0xF8;
	 MCULCD->MCULCD_RAM = (Data>>3)&0xFC;
	 MCULCD->MCULCD_RAM = Data<<3;
#else
	 MCULCD->MCULCD_RAM = Data;  //дʮ��λ��ɫֵ
#endif
}

u16 Color_To_565(u8 r, u8 g, u8 b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

/*****************************************************************************
 * @name       :u16 Lcd_ReadData_16Bit(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/	
u16 MCULCD_Read_Data_16Bit(void)
{
	u16 r=0;
#if MCULCD_USE8BIT_MODEL	
	u16 g=0,b=0;
#endif
	//16bit: red green and blue data
	//8bit:red data
	r = MCULCD_RD_DATA();
	delay_us(1);//��ʱ1us
#if MCULCD_USE8BIT_MODEL
	//8bit:green data	
	g = MCULCD_RD_DATA();
	delay_us(1);//��ʱ1us
	//8bit:blue data
	b = MCULCD_RD_DATA();
	delay_us(1);//��ʱ1us	
	r=Color_To_565(r, g, b);
#endif
	return r;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void MCULCD_Draw_Point(u16 x,u16 y, u16 color)
{
	MCULCD_Set_Cursor(x,y);//���ù��λ�� 
	MCULCD_Write_Data_16Bit(color); 
}

/*****************************************************************************
 * @name       :u16 LCD_ReadPoint(u16 x,u16 y)
 * @date       :2018-11-13 
 * @function   :Read a pixel color value at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :the read color value
******************************************************************************/	
u16 MCULCD_Read_Point(u16 x,u16 y)
{
	u16 color;
	if(x>=mculcd_dev.width||y>=mculcd_dev.height)
	{
		return 0;	//�����˷�Χ,ֱ�ӷ���	
	}
	MCULCD_Set_Cursor(x,y);//���ù��λ�� 
	MCULCD_Read_RAM_Prepare();
	color = MCULCD_Read_Data_16Bit();
	return color;
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void MCULCD_Clear(u16 Color)
{
  unsigned int i;//,m;  
	u32 total_point=mculcd_dev.width*mculcd_dev.height;
	MCULCD_Set_Windows(0,0,mculcd_dev.width-1,mculcd_dev.height-1);   
	for(i=0;i<total_point;i++)
	{ 
#if MCULCD_USE8BIT_MODEL	
	 MCULCD->MCULCD_RAM = (Color>>8)&0xF8;
	 MCULCD->MCULCD_RAM = (Color>>3)&0xFC;
	 MCULCD->MCULCD_RAM = Color<<3;
#else
		MCULCD->MCULCD_RAM = Color; //дʮ��λ��ɫֵ
#endif
	}
} 

/*****************************************************************************
 * @name       :void LCD_PWM_BackLightSet(u8 pwm)
 * @date       :2020-05-13
 * @function   :Set the backlight brightness from PWM
 * @parameters :pwm:pwm value 0~100, The greater the value, the brighter
 * @retvalue   :None
******************************************************************************/
void MCULCD_Set_PWM_BackLight(u8 pwm)
{	
	MCULCD_WR_REG(0xBE);	//����PWM���
	MCULCD_WR_DATA(0x05);	//1����PWMƵ��
	MCULCD_WR_DATA(pwm*2.55);//2����PWMռ�ձ�
	MCULCD_WR_DATA(0x01);	//3����C
	MCULCD_WR_DATA(0xFF);	//4����D
	MCULCD_WR_DATA(0x00);	//5����E
	MCULCD_WR_DATA(0x00);	//6����F
}

/*****************************************************************************
 * @name       :void LCD_MPU_Config(void)
 * @date       :2018-12-18 
 * @function   :Configure the region of the MPU, and configure 
								the external SRAM area to be in write-through mode.
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void MCULCD_MPU_Config(void)
{	
	MPU_Region_InitTypeDef MPU_Initure;

	HAL_MPU_Disable();							//����MPU֮ǰ�ȹر�MPU,��������Ժ���ʹ��MPU	
	//�ⲿSRAMΪregion0����СΪ2MB��������ɶ�д
	MPU_Initure.Enable=MPU_REGION_ENABLE;	    //ʹ��region
	MPU_Initure.Number=MCULCD_REGION_NUMBER;		//����region���ⲿSRAMʹ�õ�region0
	MPU_Initure.BaseAddress=MCULCD_ADDRESS_START;	//region����ַ
	MPU_Initure.Size=MCULCD_REGION_SIZE;			//region��С
	MPU_Initure.SubRegionDisable=0X00;
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;
	MPU_Initure.AccessPermission=MPU_REGION_FULL_ACCESS;	//��region�ɶ�д
	MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;	//�����ȡ�������е�ָ��
	MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;
	MPU_Initure.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
	MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;
	HAL_MPU_ConfigRegion(&MPU_Initure);
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);     //����MPU
}

/*****************************************************************************
 * @name       :void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
 * @date       :2018-12-18 
 * @function   :SRAM underlying driver, clock enable, pin assignment
								This function will be called by HAL_SRAM_Init ()
 * @parameters :hsram:SRAM handle
 * @retvalue   :None
******************************************************************************/
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_FMC_CLK_ENABLE();				//ʹ��FMCʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIODʱ��
	__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOEʱ��
	
	//��ʼ��PD0,1,4,5,7,8,9,10,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//����
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;	//����
	GPIO_Initure.Alternate=GPIO_AF12_FMC;	//����ΪFMC
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	//��ʼ��PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void MCULCD_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		FMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
    FMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
		__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
    GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_8;           //PB5,������� //PB8 ��λ
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
    MCULCD_MPU_Config();                       //ʹ��MPU����LCD����
    SRAM_Handler.Instance=FMC_NORSRAM_DEVICE; //BANK1    
    SRAM_Handler.Extended=FMC_NORSRAM_EXTENDED_DEVICE;         
              
        
    SRAM_Handler.Init.NSBank=FMC_NORSRAM_BANK1;     //ʹ��NE1
    SRAM_Handler.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;  //����     ��������
    SRAM_Handler.Init.MemoryType=FMC_MEMORY_TYPE_SRAM;              //SRAM
#if MCULCD_USE8BIT_MODEL 
    SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_8; //8λ���ݿ��
#else
		SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //16λ���ݿ��
#endif
    SRAM_Handler.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE; //�Ƿ�ʹ��ͻ������,����ͬ��ͻ���洢����Ч,�˴�δ�õ�
    SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;    //�洢��дʹ��
    SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;        //��дʹ�ò�ͬ��ʱ��

        
    //FSMC��ʱ����ƼĴ���
    FSMC_ReadWriteTim.AddressSetupTime=0x15;    //��ַ����ʱ��(ADDSET)Ϊ17��HCLK 1/216M=4.6ns*17=78ns
    FSMC_ReadWriteTim.AddressHoldTime=0x00;
    FSMC_ReadWriteTim.DataSetupTime=0x80;       //���ݱ���ʱ��(DATAST)Ϊ85��HCLK	=4.6*85=391ns
    FSMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //ģʽA
    //FSMCдʱ����ƼĴ���
    FSMC_WriteTim.AddressSetupTime=0x15;        //��ַ����ʱ��(ADDSET)Ϊ21��HCLK=96ns
    FSMC_WriteTim.AddressHoldTime=0x00;
    FSMC_WriteTim.DataSetupTime=0x15;           //���ݱ���ʱ��(DATAST)Ϊ4.6ns*21��HCLK=96ns
    FSMC_WriteTim.AccessMode=FMC_ACCESS_MODE_A;     //ģʽA
		
    HAL_SRAM_Init(&SRAM_Handler,&FSMC_ReadWriteTim,&FSMC_WriteTim);	
  	 							    
}

void MCULCD_FSMC_Write_Time_Set(uint32_t AST, uint32_t DST)
{
		FMC_Bank1E->BWTR[0]&=~(0XF<<0);	//��ַ����ʱ��(ADDSET)���� 	 
		FMC_Bank1E->BWTR[0]&=~(0XF<<8);	//���ݱ���ʱ������
		FMC_Bank1E->BWTR[0]|=AST<<0;	 	//��ַ����ʱ��(ADDSET)Ϊ5��HCLK =21ns  	 
		FMC_Bank1E->BWTR[0]|=DST<<8; 		//���ݱ���ʱ��(DATAST)Ϊ4.6ns*4��HCLK=21ns
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void MCULCD_RESET(void)
{
	MCULCD_RST(0);
	delay_ms(100);	
	MCULCD_RST(1);
	delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void MCULCD_Init(void)
{  
	MCULCD_GPIO_Init();//LCD GPIO��ʼ��	
	delay_ms(100);
	//LCD_RESET();    //�����ʹ�ÿ����帴λ���ţ�����ô˸�λ����
//************* SSD1963��ʼ��**********//	
	MCULCD_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
	MCULCD_WR_DATA(0x1D);		//����1 
	MCULCD_WR_DATA(0x02);		//����2 Divider M = 2, PLL = 300/(M+1) = 100MHz
	MCULCD_WR_DATA(0x04);		//����3 Validate M and N values   
	delay_us(100);
	MCULCD_WR_REG(0xE0);		// Start PLL command
	MCULCD_WR_DATA(0x01);		// enable PLL
	delay_ms(10);
	MCULCD_WR_REG(0xE0);		// Start PLL command again
	MCULCD_WR_DATA(0x03);		// now, use PLL output as system clock	
	delay_ms(12);  
	MCULCD_WR_REG(0x01);		//��λ
	delay_ms(10);
		
	MCULCD_WR_REG(0xE6);		//��������Ƶ��,33Mhz
	MCULCD_WR_DATA(0x2F);
	MCULCD_WR_DATA(0xFF);
	MCULCD_WR_DATA(0xFF);
		
	MCULCD_WR_REG(0xB0);		//����LCDģʽ
	MCULCD_WR_DATA(0x20);		//24λģʽ
	MCULCD_WR_DATA(0x00);		//TFT ģʽ 
	
	MCULCD_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);//����LCDˮƽ����
	MCULCD_WR_DATA(SSD_HOR_RESOLUTION-1);		 
	MCULCD_WR_DATA((SSD_VER_RESOLUTION-1)>>8);//����LCD��ֱ����
	MCULCD_WR_DATA(SSD_VER_RESOLUTION-1);		 
	MCULCD_WR_DATA(0x00);		//RGB���� 
		
	MCULCD_WR_REG(0xB4);		//Set horizontal period
	MCULCD_WR_DATA((SSD_HT-1)>>8);
	MCULCD_WR_DATA(SSD_HT-1);
	MCULCD_WR_DATA(SSD_HPS>>8);
	MCULCD_WR_DATA(SSD_HPS);
	MCULCD_WR_DATA(SSD_HOR_PULSE_WIDTH-1);
	MCULCD_WR_DATA(0x00);
	MCULCD_WR_DATA(0x00);
	MCULCD_WR_DATA(0x00);
	MCULCD_WR_REG(0xB6);		//Set vertical period
	MCULCD_WR_DATA((SSD_VT-1)>>8);
	MCULCD_WR_DATA(SSD_VT-1);
	MCULCD_WR_DATA(SSD_VPS>>8);
	MCULCD_WR_DATA(SSD_VPS);
	MCULCD_WR_DATA(SSD_VER_FRONT_PORCH-1);
	MCULCD_WR_DATA(0x00);
	MCULCD_WR_DATA(0x00);
		
	MCULCD_WR_REG(0xF0);	//����SSD1963��CPU�ӿ�Ϊ16bit  
#if MCULCD_USE8BIT_MODEL 
	MCULCD_WR_DATA(0x00);	//8-bit(565 format) data for 16bpp 
#else
	MCULCD_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp 
#endif
	MCULCD_WR_REG(0x29);	//������ʾ
	//����PWM���  ����ͨ��ռ�ձȿɵ� 
	MCULCD_WR_REG(0xD0);	//�����Զ���ƽ��DBC
	MCULCD_WR_DATA(0x00);	//disable
	
	MCULCD_WR_REG(0xBE);	//����PWM���
	MCULCD_WR_DATA(0x05);	//1����PWMƵ��
	MCULCD_WR_DATA(0xFE);	//2����PWMռ�ձ�
	MCULCD_WR_DATA(0x01);	//3����C
	MCULCD_WR_DATA(0x00);	//4����D
	MCULCD_WR_DATA(0x00);	//5����E 
	MCULCD_WR_DATA(0x00);	//6����F 
		
	MCULCD_WR_REG(0xB8);	//����GPIO����
	MCULCD_WR_DATA(0x03);	//2��IO�����ó����
	MCULCD_WR_DATA(0x01);	//GPIOʹ��������IO���� 
	MCULCD_WR_REG(0xBA);
	MCULCD_WR_DATA(0X01);	//GPIO[1:0]=01,����LCD����
		
	MCULCD_Set_PWM_BackLight(100);//��������Ϊ����
	
	//��������дʱ����ƼĴ�����ʱ�� 
	MCULCD_FSMC_Write_Time_Set(5, 5);
  MCULCD_Set_Direction(USE_HORIZONTAL);//����LCD��ʾ����
	MCULCD_LED(1);//��������	 
	MCULCD_Clear(WHITE);//��ȫ����ɫ
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void MCULCD_Set_Windows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	switch(mculcd_dev.dir)
	{
		case 0:
			xStar=xStar+xEnd;
			xEnd=xStar-xEnd;
			xStar=xStar-xEnd;
			xStar=mculcd_dev.width-xStar-1;
			xEnd=mculcd_dev.width-xEnd-1;
		break;
		case 2:
			yStar=yStar+yEnd;
			yEnd=yStar-yEnd;
			yStar=yStar-yEnd;
			yStar=mculcd_dev.height-yStar-1;
			yEnd=mculcd_dev.height-yEnd-1;
		break;
		case 3:
			xStar=xStar+xEnd;
			xEnd=xStar-xEnd;
			xStar=xStar-xEnd;
			xStar=mculcd_dev.width-xStar-1;
			xEnd=mculcd_dev.width-xEnd-1;
			yStar=yStar+yEnd;
			yEnd=yStar-yEnd;
			yStar=yStar-yEnd;
			yStar=mculcd_dev.height-yStar-1;
			yEnd=mculcd_dev.height-yEnd-1;
		break;
		default:
		break;
	}
	LCD_WR_REG(mculcd_dev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(mculcd_dev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	MCULCD_Write_RAM_Prepare();	//��ʼд��GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void MCULCD_Set_Cursor(u16 Xpos, u16 Ypos)
{	  	    			
	MCULCD_Set_Windows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void MCULCD_Set_Direction(u8 direction)
{ 
	mculcd_dev.wramcmd=0x2C;
	mculcd_dev.rramcmd=0x2E;
	mculcd_dev.dir=direction%4;
	switch(mculcd_dev.dir){		  
		case 0:	
			mculcd_dev.setxcmd=0x2B;
			mculcd_dev.setycmd=0x2A;
			mculcd_dev.width=LCD_W;
			mculcd_dev.height=LCD_H;		
			MCULCD_Write_Reg(0x36,(0<<3)|(1<<7)|(0<<6)|(1<<5));
		break;
		case 1:
			mculcd_dev.setxcmd=0x2A;
			mculcd_dev.setycmd=0x2B;
			mculcd_dev.width=LCD_H;
			mculcd_dev.height=LCD_W;
			MCULCD_Write_Reg(0x36,(0<<3)|(0<<7)|(0<<6)|(0<<5));
		break;
		case 2:
			mculcd_dev.setxcmd=0x2B;
			mculcd_dev.setycmd=0x2A;		
			mculcd_dev.width=LCD_W;
			mculcd_dev.height=LCD_H;	
			MCULCD_Write_Reg(0x36,(0<<3)|(0<<7)|(1<<6)|(1<<5));
		break;
		case 3:
			mculcd_dev.setxcmd=0x2A;
			mculcd_dev.setycmd=0x2B;
			mculcd_dev.width=LCD_H;
			mculcd_dev.height=LCD_W;
			MCULCD_Write_Reg(0x36,(0<<3)|(1<<7)|(1<<6)|(0<<5));
		break;	
		default:break;
	}		
	lcd_dev.width=mculcd_dev.width;
	lcd_dev.height=mculcd_dev.height;
	lcd_dev.dir=mculcd_dev.dir;
}	 

/*****************************************************************************
 * @name       :u16 LCD_Read_ID(void)
 * @date       :2018-11-13 
 * @function   :Read ID
 * @parameters :None
 * @retvalue   :ID value
******************************************************************************/ 
u16 MCULCD_Read_ID(void)
{
	u8 val[3] = {0};
	MCULCD_Read_Reg(0xA1,val,3);
	return (val[1]<<8)|val[2];
}

void MCULCD_Fill_Color(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
	u16 i,j;			
	u16 width=ex-sx+1; 		//�õ����Ŀ��
	u16 height=ey-sy+1;		//�߶�
	MCULCD_Set_Windows(sx,sy,ex,ey);//������ʾ����
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)		
		{
			#if MCULCD_USE8BIT_MODEL	
			 MCULCD->MCULCD_RAM = (color>>8)&0xF8;
			 MCULCD->MCULCD_RAM = (color>>3)&0xFC;
			 MCULCD->MCULCD_RAM = color<<3;
			#else
				MCULCD->MCULCD_RAM = color; //дʮ��λ��ɫֵ
			#endif 
		}
	}
	MCULCD_Set_Windows(0,0,mculcd_dev.width-1,mculcd_dev.height-1);//�ָ���������Ϊȫ��
}

