

#include "all.h"

#include "gsm_code.h"
#include "gsm_base.h"
#include "usart.h"




void printf_hex(u8 *buf, int len)
{
	int i;
	u8 tmp; 
	for(i = 0; i < len; i ++)
	{
		tmp = (buf[i] >> 4);
		if(tmp > 9)
		{
			printf_c((tmp - 10) + 'A');
		}
		else
		{
			printf_c(tmp + '0');
		}

		tmp = (buf[i] & 0x0f);
		if(tmp > 9)
		{
			printf_c((tmp - 10) + 'A');
		}
		else
		{
			printf_c(tmp + '0');
		}

		printf_c(' ');
	}
}




/*******************************************************************************
* ������ : CLR_Buf2
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //��������������
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��

	clean_delay_uart();
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}



/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,u8 wait_time)         
{

	u8 i;
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf2(); 
  i = 0;
	
	b = c;							//���ַ�����ַ��b

	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					gsm_base_send(*b);//UART2_SendData(*b);
				}
				UART2_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
				UART1_SendData(Uart2_Buf, First_Int);
		   }
    	}
 	  	else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf2(); 
}




/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

int Second_AT_Command_Recv(char *b,char *a, char *buf, u8 wait_time)         
{

	u8 i;
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf2(); 
  i = 0;
	
	b = c;							//���ַ�����ַ��b

	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					gsm_base_send(*b);//UART2_SendData(*b);
				}
				UART2_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
		   }
    	}
 	  	else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	//CLR_Buf2(); 
	memcpy(buf, Uart2_Buf, First_Int);
	return First_Int;
}





/*******************************************************************************
* ������ : Second_AT_Command_Try
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S) ���ӳ�ʱ�˳�����
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

u8 Second_AT_Command_Try(char *b,char *a,u8 wait_time)         
{
	u8 i;
	u8 try_num = 0;
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf2(); 
  	i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			//printf_s("recv buf: \r\n");
			//printf_s(Uart2_Buf);
			//printf_s("\r\n");
			
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
				//printf_s("send ...\r\n");
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					gsm_base_send(*b);//UART2_SendData(*b);
				}
				//printf_s("send ok ...\r\n");
				UART2_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;

				try_num ++;
				
				//UART1_SendData(Uart2_Buf, First_Int);
				//printf_s("\r\n");
				if(try_num > 30)
				{
					Timer0_start = 0;
					printf_s("try exit ...\r\n");
					return 0;
				}
		   }
    	}
 	  	else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf2(); 

	return 1;
}



/*******************************************************************************
* ������ : Second_AT_Data
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)  ���ӳ�ʱ�˳�����
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Second_AT_Data(char *b,char *a, u32 len, u8 wait_time)         
{
	u32 i, j;
	u8 try_num = 0;
	
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf2(); 
  	i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			//printf_s("recv buf: \r\n");
			//printf_s(Uart2_Buf);
			//printf_s("\r\n");
			
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
				for (j = 0; j < len; j++)
				{
					//printf("%s %d %d %d\r\n", __FILE__, __LINE__, len, j);
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					gsm_base_send(*b);//UART2_SendData(*b);
					b++;
				}
				//printf("%s %d %d %d\r\n", __FILE__, __LINE__, len, j);
				//UART2_SendLR();	
				gsm_base_send(0x1A);//UART2_SendData(*b);
				
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;

				try_num ++;

				if(try_num > 3)
				{
					Timer0_start = 0;
					printf_s("try exit ...\r\n");
					return ;
				}
		   }
    	}
 	  	else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf2(); 
}


//��������
void uart_send_data(char *buf, u32 len)
{
	char *b = buf;
	u32 j;
	for (j = 0; j < len; j++)
	{
		//printf("%s %d %d %d\r\n", __FILE__, __LINE__, len, j);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		gsm_base_send(*b);//UART2_SendData(*b);
		b++;
	}
}


/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u32 i;
	u32 k;
	i = 0;
	CLR_Buf2();
  while(i == 0)        			
	{
		CLR_Buf2();        
		UART2_SendString("AT+CREG?");
		UART2_SendLR();
		Delay_nMs(5000);

	    for(k=0;k<Buf2_Max;k++)      			
    	{
			if(Uart2_Buf[k] == ':')
			{
				if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '5'))
				{
					i = 1;
					UART1_SendLR();
				  break;
				}
			}
		}
		UART1_SendString("ע����.....");
	}
}

/*******************************************************************************
* ������ : Set_ATE0
* ����   : ȡ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3);								//ȡ������		
}



