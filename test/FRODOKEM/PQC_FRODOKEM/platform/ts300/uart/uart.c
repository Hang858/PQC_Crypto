/*
 * ts300v_Uart.c
 *
 *  Created on: 2026年7月2日
 *      Author: Administrator
 */
#include <stdio.h>
#include "smartchip_sdk_soc.h"
#include <stdio.h>
#include "smartchip_sdk_soc.h"
unsigned char data[8]={0};
#define DW_UART
//#define XT_UART

#ifdef XT_UART

#define UART_TX 0x40000000+0x00
#define UART_RX 0x40000000+0x04
#define UART_ST 0x40000000+0x08
#define UART_CT 0x40000000+0x0C
#define UART_IN 0x40000000+0x10
#define UART_BA 0x40000000+0x14

void UART_Init(uint32_t baud_div) {

	// 1. 设置分频系数
    //*(volatile unsigned int *)(UART_BA) = baud_div;
    *(volatile unsigned int *)(UART_BA) = baud_div;

    *(volatile unsigned int *)(UART_CT) |= 0x200;//清接收fifo
    *(volatile unsigned int *)(UART_CT) |= 0x100;//清发送fifo

	// 2. 配置控制寄存器 (例如：使能TX/RX，具体看芯片手册)
     //*(volatile unsigned int *)(UART_CT) = 0x01;
    *(volatile unsigned int *)(UART_CT) |= 0x03;// 使能接收和发送

    // 3. 清除可能存在的中断状态
    *(volatile unsigned int *)(UART_IN) = 0x1F;

}
void UART_SendChar(uint8_t data)
{
    // 等待发送缓冲区/FIFO 有空位
    while (!(*(volatile unsigned int *)(UART_ST) & 0x20)){
		// 等待
	};

    // 写入发送寄存器
    *(volatile unsigned int *)(UART_TX) = data;
}
void UART_SendData(unsigned char *buff, unsigned int num)
{

    unsigned int i;
    for(i = 0; i < num; i++)
     {
		// 等待发送缓冲区/FIFO 有空位
		while (!(*(volatile unsigned int *)(UART_ST) & 0x20))
		{
			// 等待
		};

		// 写入发送寄存器
		*(volatile unsigned int *)(UART_TX) = buff[i];
     }
}
uint8_t UART_RcvChar(void)
{
    // 等待接收fifo满标
    while ((*(volatile unsigned int *)(UART_ST) & 0x02))
    {
		// 等待
	};
    // 取出接收数据寄存器内容
    return *(volatile unsigned int *)(UART_RX);
}
void UART_RcvData(unsigned char *buff, unsigned int num)
{

    unsigned int i;
    for(i = 0; i < num; i++)
     {
		// 等待接收fifo满标志
		while ((*(volatile unsigned int *)(UART_ST) & 0x02))
		{
		// 等待
		};

		 // 取出接收数据寄存器内容
		 buff[i] = *(volatile unsigned int *)(UART_RX);
     }
}

int Uart_demo(void)
{
	UART_Init(2083);//波特率9600
	UART_SendData("Uart is ok!",11);
	while(1)
	{
		UART_RcvData(data,8);
		UART_SendData(data,8);
	}
}
#endif
#ifdef DW_UART
#define UART_REG_BASE_ADDR		(0x40046000)
//===================================DW_UART 内核==========================//
#define UART_RBR UART_REG_BASE_ADDR+0x00 //复用寄存器Receive Buffer Register(R)/Transmit Holding Register(W)/Divisor Latch (Low)(R/W)
#define UART_THR UART_REG_BASE_ADDR+0x00
#define UART_DLL UART_REG_BASE_ADDR+0x00 // 需设置DLAB=1
#define UART_IER UART_REG_BASE_ADDR+0x04 //复用寄存器Divisor Latch (High)(R/W)/Interrupt Enable Register(R/W)
#define UART_DLH UART_REG_BASE_ADDR+0x04 // 需设置DLAB=1
#define UART_IIR UART_REG_BASE_ADDR+0x08 //复用寄存器Interrupt Identification Register(R)/FIFO Control Register(W)
#define UART_FCR UART_REG_BASE_ADDR+0x08
#define UART_LCR UART_REG_BASE_ADDR+0x0C  //Line Control Register
#define UART_MCR UART_REG_BASE_ADDR+0x10  //Modem Control Register
#define UART_LSR UART_REG_BASE_ADDR+0x14  //Line Status Register
#define UART_MSR UART_REG_BASE_ADDR+0x18  //Modem Status Register
#define UART_SCR UART_REG_BASE_ADDR+0x1C  //Scratchpad Register


#define UART_USR UART_REG_BASE_ADDR+0x7C  //UART Status Register
#define LSR_THRE 0x20 //Transmit Holding Register Empty bit.
#define LSR_DR 0x01 //Data Ready bit
#define USR_BUSY 0x01
void writeb(unsigned char val, unsigned long addr)
{
    // volatile 防止编译器优化
    *(volatile unsigned char *)addr = val;
}
unsigned char readb(unsigned long addr)
{
    return *(volatile unsigned char *)addr;
}
void UART_Init(unsigned int baud_div) {

     //unsigned int divisor = uart_clock / (baud_rate * 16);
	 unsigned int divisor = 20000000 / (baud_div * 16);
	    // 1. 禁用所有中断
	    writeb(0, UART_IER);

	   // while (readb(UART_USR) & USR_BUSY);


	    // 2. 设置DLAB=1，配置波特率除数
	    writeb(0x80, UART_LCR);           // 设置DLAB=1
	    writeb(divisor & 0xFF, UART_DLL); // 除数低字节
	    writeb((divisor >> 8) & 0xFF, UART_DLH); // 除数高字节

	    // 3. 配置串行数据格式（8位数据、1停止位、无校验）
	    writeb(0x03, UART_LCR);           // DLAB=0, WLS=8位

	    // 4. 配置FIFO（使能FIFO，清空FIFO，设置触发阈值）
	    writeb(0xC7, UART_FCR);           // FIFO使能+清空+14字节阈值

	    // 5. 按需使能中断
	    // writeb(IER_ERBFI | IER_ELSI, UART_IER);
}
void UART_SendChar(char ch) {
    // 等待THRE为1（发送保持寄存器为空）
    while (!(readb(UART_LSR) & LSR_THRE));
    writeb(ch, UART_THR);
}
char UART_RcvChar(void) {
    // 等待DR为1（接收数据就绪）
    while (!(readb(UART_LSR) & LSR_DR));
    return readb(UART_RBR);
}
void UART_RcvData(unsigned char *buff, unsigned int num)
{

    unsigned int i;
    for(i = 0; i < num; i++)
     {
    	 // 等待DR为1（接收数据就绪）
    	    while (!(readb(UART_LSR) & LSR_DR));

		 // 取出接收数据寄存器内容
		 buff[i] = *(volatile unsigned int *)(UART_RBR);
     }
}
void UART_SendData(unsigned char *buff, unsigned int num)
{

    unsigned int i;
    for(i = 0; i < num; i++)
     {
    	// 等待THRE为1（发送保持寄存器为空）
    	    while (!(readb(UART_LSR) & LSR_THRE));
		// 写入发送寄存器
		*(volatile unsigned int *)(UART_THR) = buff[i];
     }
}

void UART_SendString(unsigned char *str) {
	UART_SendData(str, strlen(str));
}

//int Uart_demo(void)
//{
//	UART_Init(9600);
//	UART_SendData("Uart is ok!",11);
//	while(1)
//	{
////		char c = UART_RcvChar();
////		UART_SendChar(c); /* Echo back */
//		UART_RcvData(data,8);
//		UART_SendData(data,8);
//	}
//}
#endif

