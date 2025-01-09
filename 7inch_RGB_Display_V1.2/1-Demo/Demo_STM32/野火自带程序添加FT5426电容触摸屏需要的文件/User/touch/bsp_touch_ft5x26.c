#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./touch/bsp_touch_ft5x26.h"
#include "./touch/bsp_i2c_touch.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/palette.h"


/**
  * @brief   ʹ��IIC�������ݴ���
  * @param
  *		@arg i2c_ft_msg:���ݴ���ṹ��
  *		@arg n:���ݴ���ṹ��ĸ���
  * @retval  0:�ɹ�; 1:ʧ��
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
  * @brief   ��IIC�豸д������
  * @param
  *		@arg client_addr:�豸��ַ
  *		@arg  buf[0~1]: Ҫд������ݼĴ�������ʼ��ַ
  *		@arg buf[2~len-1]: Ҫд�������
  *		@arg len:    FT_ADDR_LENGTH + write bytes count���Ĵ�����ַ����+д��������ֽ�����
  * @retval   0:�ɹ���1:ʧ��
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
  * @brief   ��IIC�豸�ж�ȡ����
  * @param
  *		@arg client_addr:�豸��ַ
  *		@arg  buf[0~1]: ��ȡ���ݼĴ�������ʼ��ַ
  *		@arg buf[2~len-1]: �洢���������ݵĻ���buffer
  *		@arg len:    FT_ADDR_LENGTH + read bytes count���Ĵ�����ַ����+��ȡ�������ֽ�����
  * @retval   0:�ɹ���1:ʧ��
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
	I2C_Touch_Init();  //��ʼ��GPIO�͸�λ
	reg_buf[0]=FT_DEVIDE_MODE;
	reg_buf[1]=0;
	ret = FT5X26_I2C_Write(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1);  //���������Ĳ���ģʽ
	if(ret)
	{
		printf("set FT_DEVIDE_MODE failed!!");
		return ret;
	}
	reg_buf[0]=FT_ID_G_MODE;
	reg_buf[1]=1;
	ret = FT5X26_I2C_Write(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1);  //�����ж�ģʽ
	if(ret)
	{
		printf("set FT_ID_G_MODE failed!!");
		return ret;
	}
	reg_buf[0]=FT_ID_G_THGROUP;
	reg_buf[1]=22;
	ret = FT5X26_I2C_Write(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1);  //���ô�����Чֵ��ԽСԽ����
	if(ret)
	{
		printf("set FT_ID_G_THGROUP failed!!");
		return ret;
	}
	reg_buf[0]=FT_ID_G_PERIODACTIVE;
	reg_buf[1]=12;
	ret = FT5X26_I2C_Write(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1); //���ü������ڣ�����С��12�����14
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
  * @brief   ���ڴ���򱨸津����⵽����
  * @param
  *    @arg     id: ����˳��trackID
  *    @arg     x:  ������ x ����
  *    @arg     y:  ������ y ����
  * @retval ��
  */
/*���ڼ�¼��������ʱ(����)����һ�δ���λ�ã�����ֵ��ʾ��һ���޴�������*/
static int16_t pre_x[FT_MAX_TOUCH] ={-1,-1,-1,-1,-1};
static int16_t pre_y[FT_MAX_TOUCH] ={-1,-1,-1,-1,-1};

static void FT_Touch_Down(int32_t id,int32_t x,int32_t y)
{
  
    /* ��������ť�����ڴ������� */
    Touch_Button_Down(x,y); 
	

    /*�������켣�����ڴ������� */
    Draw_Trail(pre_x[id],pre_y[id],x,y,&brush);
	
		/************************************/
		/*�ڴ˴�����Լ��Ĵ����㰴��ʱ������̼���*/
		/* (x,y) ��Ϊ���µĴ����� *************/
		/************************************/
	
		/*prex,prey����洢��һ�δ�����λ�ã�idΪ�켣���(��㴥��ʱ�ж�켣)*/
    pre_x[id] = x; pre_y[id] =y;
	
}


/**
  * @brief   ���ڴ���򱨸津���ͷ�
  * @param �ͷŵ��id��
  * @retval ��
  */
static void FT_Touch_Up( int32_t id)
{
	

    /*�������ͷ�,���ڴ�������*/
    Touch_Button_Up(pre_x[id],pre_y[id]);

		/*****************************************/
		/*�ڴ˴�����Լ��Ĵ������ͷ�ʱ�Ĵ�����̼���*/
		/* pre_x[id],pre_y[id] ��Ϊ���µ��ͷŵ� ****/
		/*******************************************/	
		/***idΪ�켣���(��㴥��ʱ�ж�켣)********/
	
	
    /*�����ͷţ���pre xy ����Ϊ��*/
	  pre_x[id] = -1;
	  pre_y[id] = -1;		
}

const u16 FT5X26_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};
/**
  * @brief   ��������������ѯ�����ڴ����жϵ���
  * @param ��
  * @retval ��
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

		ret = FT5X26_I2C_Read(FT_ADDRESS,reg_buf,FT_ADDR_LENGTH+1); //��ȡ������״̬
    if (ret < 0)
    {
        printf("read point num failed!!\n");
        return;
    }   
    finger = reg_buf[FT_ADDR_LENGTH];//��ȡ���������
    touch_num = finger & 0x0f;//�������
		if ((touch_num!=0)&&(touch_num<FT_MAX_TOUCH+1))//��ֹһ����
    {
        for(i=0;i<touch_num;i++)
				{
					data_buf[0] = FT5X26_TPX_TBL[i];
					FT5X26_I2C_Read(FT_ADDRESS,data_buf,FT_ADDR_LENGTH+4);
					memcpy(&point_data[i*4],&data_buf[FT_ADDR_LENGTH],4);
				}
    }
  
    if (pre_touch>touch_num)				//pre_touch>touch_num,��ʾ�еĵ��ͷ���
    {
        for (i = 0; i < pre_touch; i++)						//һ����һ���㴦��
         {
           for(j=0; j<touch_num; j++)
           {
               coor_data = &point_data[j*4];
               id = (coor_data[2]>>4)&0x0F;									//track id
							 if(pre_id[i] == id)
                break;

              if(j >= touch_num-1)											//������ǰ����id���Ҳ���pre_id[i]����ʾ���ͷ�
              {
                 FT_Touch_Up( pre_id[i]);
              }
           }
       }
    }


    if (touch_num)
    {
        for (i = 0; i < touch_num; i++)						//һ����һ���㴦��
        {
            coor_data = &point_data[i*4];

            id = (coor_data[2]>>4)&0x0F;									//track id
            pre_id[i] = id;

            input_y  = (coor_data[0]&0x0F)<<8 | coor_data[1];	//x����
            input_x  = (coor_data[2]&0x0F)<<8 | coor_data[3];	//y����
            {
                FT_Touch_Down( id, input_x, input_y);//���ݴ���
            }
        }
    }
    else if (pre_touch)		//touch_ num=0 ��pre_touch��=0
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
