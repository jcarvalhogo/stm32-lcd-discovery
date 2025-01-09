#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./touch/bsp_touch_ft5x26.h"
#include "./touch/bsp_i2c_touch.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/palette.h"


/**
  * @brief   使用IIC进行数据传输
  * @param
  *		@arg i2c_ft_msg:数据传输结构体
  *		@arg n:数据传输结构体的个数
  * @retval  0:成功; 1:失败
  */
static int32_t I2C_FT_Transfer(struct i2c_ft_msg *msgs,uint32_t n)
{
	uint32_t i=0;
	int32_t ret=0;
	for(i=0;i<n;i++)
	{
		if(msgs[i].flags==I2C_FT_WR)
		{
			ret=I2C_WriteBytes(msgs[i].addr,msgs[i].buf,msgs[i].len);
		}
		else
		{
			ret=I2C_ReadBytes(msgs[i].addr,msgs[i].buf,msgs[i].len);
		}
		if(ret)
		{
			break;
		}
	}
	return ret;
}

/**
  * @brief   向IIC设备写入数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 要写入的数据寄存器的起始地址
  *		@arg buf[2~len-1]: 要写入的数据
  *		@arg len:    FT_ADDR_LENGTH + write bytes count（寄存器地址长度+写入的数据字节数）
  * @retval   0:成功，1:失败
  */
static int32_t FT5X26_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
	int32_t ret = -1;
	struct i2c_ft_msg ft_msg;
	ft_msg.addr  = client_addr;
	ft_msg.buf   = buf;
	ft_msg.flags = I2C_FT_WR;
	ft_msg.len   = len;
	ret = I2C_FT_Transfer(&ft_msg,1);
	if(ret)
	{
		printf("write 0x%02X regisrer failed!!",buf[0]);
	}
	return ret;
}

/**
  * @brief   从IIC设备中读取数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 读取数据寄存器的起始地址
  *		@arg buf[2~len-1]: 存储读出来数据的缓冲buffer
  *		@arg len:    FT_ADDR_LENGTH + read bytes count（寄存器地址长度+读取的数据字节数）
  * @retval   0:成功，1:失败
  */
static int32_t FT5X26_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
	int32_t ret = -1;
	struct i2c_ft_msg ft_msg[2];
	ft_msg[0].addr  = client_addr;
	ft_msg[0].flags = I2C_FT_WR;
	ft_msg[0].buf   = &buf[0];
	ft_msg[0].len   = FT_ADDR_LENGTH;
	
	ft_msg[1].addr  = client_addr;
	ft_msg[1].flags = I2C_FT_RD;
	ft_msg[1].buf   = &buf[FT_ADDR_LENGTH];
	ft_msg[1].len   = len-FT_ADDR_LENGTH;
	
	ret = I2C_FT_Transfer(ft_msg,2);
	if(ret)
	{
		printf("read 0x%02X regisrer failed!!",buf[0]);
	}
	return ret;
}

/*******************************************************
Function:
    Read chip id.
Output:
    read operation return.
       chip id.
*******************************************************/
uint16_t FT5X26_Read_ID(void)
{
	uint16_t ret = 0;
	uint8_t reg_buf[3]={FT_ID_G_LIB_VERSION,0,0};
	ret = FT5X26_I2C_Read(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+2);
	if(ret)
	{
		ret = 0xFFFF;
		printf("read chip id failed!!");
	}
	else
	{
		ret = reg_buf[1]<<8|reg_buf[2];
	}
	return ret;
}

/*******************************************************
Function:
    Initialize ftp.
Output:
    Executive outcomes.
        0: succeed, otherwise: failed
*******************************************************/
int32_t FT5X26_Init_Panel(void)
{
	uint16_t ret = 0;
	uint8_t reg_buf[2];
	I2C_Touch_Init();  //初始化GPIO和复位
	reg_buf[0]=FT_DEVIDE_MODE;
	reg_buf[1]=0;
	ret = FT5X26_I2C_Write(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1);  //进入正常的操作模式
	if(ret)
	{
		printf("set FT_DEVIDE_MODE failed!!");
		return ret;
	}
	reg_buf[0]=FT_ID_G_MODE;
	reg_buf[1]=1;
	ret = FT5X26_I2C_Write(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1);  //设置中断模式
	if(ret)
	{
		printf("set FT_ID_G_MODE failed!!");
		return ret;
	}
	reg_buf[0]=FT_ID_G_THGROUP;
	reg_buf[1]=22;
	ret = FT5X26_I2C_Write(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1);  //设置触摸有效值，越小越灵敏
	if(ret)
	{
		printf("set FT_ID_G_THGROUP failed!!");
		return ret;
	}
	reg_buf[0]=FT_ID_G_PERIODACTIVE;
	reg_buf[1]=12;
	ret = FT5X26_I2C_Write(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1); //设置激活周期，不能小于12，最大14
	if(ret)
	{
		printf("set FT_ID_G_PERIODACTIVE failed!!");
		return ret;
	}
	ret = FT5X26_Read_ID();
	if((ret != 0x3003)&&(ret != 0x0001)&&(ret != 0x0002))
	{
		printf("read chip id failed!!");
		return ret;
	}
	I2C_GTP_IRQEnable();
	return 0;
}

/**
  * @brief   用于处理或报告触屏检测到按下
  * @param
  *    @arg     id: 触摸顺序trackID
  *    @arg     x:  触摸的 x 坐标
  *    @arg     y:  触摸的 y 坐标
  * @retval 无
  */
/*用于记录连续触摸时(长按)的上一次触摸位置，负数值表示上一次无触摸按下*/
static int16_t pre_x[FT_MAX_TOUCH] ={-1,-1,-1,-1,-1};
static int16_t pre_y[FT_MAX_TOUCH] ={-1,-1,-1,-1,-1};

static void FT_Touch_Down(int32_t id,int32_t x,int32_t y)
{
  
    /* 处理触摸按钮，用于触摸画板 */
    Touch_Button_Down(x,y); 
	

    /*处理描绘轨迹，用于触摸画板 */
    Draw_Trail(pre_x[id],pre_y[id],x,y,&brush);
	
		/************************************/
		/*在此处添加自己的触摸点按下时处理过程即可*/
		/* (x,y) 即为最新的触摸点 *************/
		/************************************/
	
		/*prex,prey数组存储上一次触摸的位置，id为轨迹编号(多点触控时有多轨迹)*/
    pre_x[id] = x; pre_y[id] =y;
	
}


/**
  * @brief   用于处理或报告触屏释放
  * @param 释放点的id号
  * @retval 无
  */
static void FT_Touch_Up( int32_t id)
{
	

    /*处理触摸释放,用于触摸画板*/
    Touch_Button_Up(pre_x[id],pre_y[id]);

		/*****************************************/
		/*在此处添加自己的触摸点释放时的处理过程即可*/
		/* pre_x[id],pre_y[id] 即为最新的释放点 ****/
		/*******************************************/	
		/***id为轨迹编号(多点触控时有多轨迹)********/
	
	
    /*触笔释放，把pre xy 重置为负*/
	  pre_x[id] = -1;
	  pre_y[id] = -1;		
}

const u16 FT5X26_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};
/**
  * @brief   触屏处理函数，轮询或者在触摸中断调用
  * @param 无
  * @retval 无
  */
static void FT_TS_Work_Func(void)
{
		uint8_t  reg_buf[2] = {FT_REG_NUM_FINGER};
		uint8_t  data_buf[5] = {0};
    uint8_t  point_data[4*FT_MAX_TOUCH]={0};
    uint8_t  touch_num = 0;
    uint8_t  finger = 0;
    static uint16_t pre_touch = 0;
    static uint8_t pre_id[FT_MAX_TOUCH] = {0};

    uint8_t* coor_data = NULL;
    int32_t input_x = 0;
    int32_t input_y = 0;
    uint8_t id = 0;
 
    int32_t i  = 0;
		uint8_t j;
    int32_t ret = -1;

		ret = FT5X26_I2C_Read(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1); //读取触摸点状态
    if (ret < 0)
    {
        printf("read point num failed!!\n");
        return;
    }   
    finger = reg_buf[FT_ADDR_LENGTH];//获取触摸点个数
    touch_num = finger & 0x0f;//坐标点数
		if ((touch_num!=0)&&(touch_num<FT_MAX_TOUCH+1))//不止一个点
    {
        for(i=0;i<touch_num;i++)
				{
					data_buf[0] = FT5X26_TPX_TBL[i];
					FT5X26_I2C_Read(FT_ADDRESS,data_buf,FT_ADDR_LENGTH+4);
					memcpy(&point_data[i*4],&data_buf[FT_ADDR_LENGTH],4);
				}
    }
  
    if (pre_touch>touch_num)				//pre_touch>touch_num,表示有的点释放了
    {
        for (i = 0; i < pre_touch; i++)						//一个点一个点处理
         {
           for(j=0; j<touch_num; j++)
           {
               coor_data = &point_data[j*4];
               id = (coor_data[2]>>4)&0x0F;									//track id
							 if(pre_id[i] == id)
                break;

              if(j >= touch_num-1)											//遍历当前所有id都找不到pre_id[i]，表示已释放
              {
                 FT_Touch_Up( pre_id[i]);
              }
           }
       }
    }


    if (touch_num)
    {
        for (i = 0; i < touch_num; i++)						//一个点一个点处理
        {
            coor_data = &point_data[i*4];

            id = (coor_data[2]>>4)&0x0F;									//track id
            pre_id[i] = id;

            input_y  = (coor_data[0]&0x0F)<<8 | coor_data[1];	//x坐标
            input_x  = (coor_data[2]&0x0F)<<8 | coor_data[3];	//y坐标
            {
                FT_Touch_Down( id, input_x, input_y);//数据处理
            }
        }
    }
    else if (pre_touch)		//touch_ num=0 且pre_touch！=0
    {
      for(i=0;i<pre_touch;i++)
      {
          FT_Touch_Up(pre_id[i]);
      }
    }
    pre_touch = touch_num;
}

void FT_TouchProcess(void)
{
  FT_TS_Work_Func();
}
