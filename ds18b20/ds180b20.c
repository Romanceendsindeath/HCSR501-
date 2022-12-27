#include "DS180b20.h"
#include "delay.h"

/**
  * @brief  ����DQ��Ӧ������λ�������ģʽ
  * @param   �� 
  * @retval  ��
  */
static void DS18B20_SetDQMode_OUT()
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = PINDQ;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(GPIODQ, &GPIO_InitStructure);	
}
/**
  * @brief  ����DQ��Ӧ������Ϊ����ģʽ
  * @param   ��  
  * @retval  ��
  */
static void DS18B20_SetDQMode_IPU()
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = PINDQ;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(GPIODQ, &GPIO_InitStructure);	
}
/**
  * @brief  ����DQ��Ӧ��GPIO����
  * @param   ��
  * @retval  ��
  */
void DS18B20_DQGPIOConfig(void)
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd(DQ_GPIO_CLK, ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = PINDQ;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(GPIODQ, &GPIO_InitStructure);	
}
/**
  * @brief  ����DQ����λDS18B20
  * @param   
  *		@arg  
  *		@arg  
  * @retval  ��
  */
void delay_us(u32 nus);
void DS18B20_Init(void)
{
	/* ��ʼ��GPIO */
	DS18B20_DQGPIOConfig();
	/* ��λ */
	DS18B20_Rst();
}
/**
  * @brief  ��λDS18B20
  * @param   ��
  * @retval  ��
  */
void DS18B20_Rst(void)
{	
	uint8_t dqStatus = GPIO_ReadInputDataBit(GPIODQ,PINDQ);

	/*  ���������ٴ����ߵ��ߵ�ƽ��1��������� */
	/* ��ʼ��GPIO */
	DS18B20_DQGPIOConfig();
	DS18B20_SetDQMode_OUT();
	/* �Ƚ��������øߵ�ƽ��1�� */
	DQ_H;
	/* ��ʱ����ʱ��Ҫ��Ĳ��Ǻ��ϸ񣬵��Ǿ����ܵĶ�һ�㣩 */
	delay_us(1);
	/* �����������͵�ƽ��0���� */
	DQ_L;
	/* ��ʱ750΢�루��ʱ���ʱ�䷶Χ���Դ�480��960΢�룩 */
	delay_us(750);
	/* �����������ߵ�ƽ��1�� */
	DQ_H;
	/* ��ʱ�ȴ��������ʼ���ɹ�����15��60΢��ʱ��֮�ڲ���һ����
	DS18B20�����صĵ͵�ƽ��0�����ݸ�״̬������ȷ�����Ĵ��ڣ�
	����Ӧע�ⲻ�����޵Ľ��еȴ�����Ȼ��ʹ���������ѭ����
	����Ҫ���г�ʱ���ƣ��� */
	/* ��ʱʱ�䲻Ҫ���õ�̫�� ����70us�������ʼ��ʧ�ܣ�����ȡ���������¶� */
	delay_us(500);
	
	DQ_H;
}
/**
  * @brief  ͨ��DQ��DS18B20��ȡһ���ֽڣ�Byte��
  * @param   ��
  * @retval  ��
  */
uint8_t DS18B20_ReadByte(void)
{
	uint8_t data = 0x00;
	int i=0;
	for(i=0;i<8;i++)
	{
		DS18B20_SetDQMode_OUT();
		/* �����������ߡ�1�� */
		DQ_H;
		/* ��ʱ2΢�� */
		delay_us(2);
		/* �����������͡�0�� */
		DQ_L;
		/* ��ʱ3΢�� */
		delay_us(3); 
		data>>=1;
		/* �����������ߡ�1�� */
		DQ_H;
		DS18B20_SetDQMode_IPU();
		/* ��ʱ5΢�� */
		delay_us(5);
		/* �������ߵ�״̬�õ�1��״̬λ�����������ݴ��� */
		if(GPIO_ReadInputDataBit(GPIODQ,PINDQ))
			data|=0x80;
		/* ��ʱ60΢�� */
		delay_us(60);
	}
	return data;
}
/**
* @brief  ͨ��DQ��DS18B20����һ���ֽڣ�Byte��
  * @param   ��
  * @retval  ��
  */
void DS18B20_WriteByte(uint8_t data)
{
	int i=0;
	for(i=0;i<8;i++){
		/* ���������õ͵�ƽ��0�� */
		DQ_L;
		/* ��ʱȷ����ʱ��Ϊ15΢�� */
		delay_us(15);
		/* ���ӵ�λ����λ��˳�����ֽڣ�һ��ֻ����һλ�� */
		if(data&0x01){
			GPIO_WriteBit(GPIODQ,PINDQ,Bit_SET);		
		}else{
			GPIO_WriteBit(GPIODQ,PINDQ,Bit_RESET);
		}
		/* ��ʱʱ��Ϊ45΢�� */
		delay_us(45);
		/* �������������ߵ�ƽ */
		DQ_H;
		data>>=1;
	}
}
/**
  * @brief  ��ȡ�¶ȼĴ�����ֵ��ת��Ϊ�¶�ֵ����
  * @param   ��
  * @retval  ��
  */
float DS18B20_GetTemp(void)
{
	uint8_t tempL = DS18B20_ReadByte();
	uint8_t tempH = DS18B20_ReadByte();
	float temp;
	/* ���� */
	DS18B20_Rst();
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(CONVERTT);
	DS18B20_Rst();
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(READSCRATCHPAD);
	if(tempH>0x7f)
	{
		tempL = ~tempL;
		tempH = ~tempH+1;
	}
	/* �����¶�ֵ */
	temp = ((tempH<<4)|(tempL>>4))+
	(float)(tempL&0x0f)*0.0625; 
	return temp;
}

