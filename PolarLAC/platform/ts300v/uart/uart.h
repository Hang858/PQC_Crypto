#ifndef	_MINI_UART_H
#define	_MINI_UART_H



//void uart_init ( void );
//char uart_recv ( void );
//void uart_send ( char c );
//void uart_send_string(char* str);

void UART_Init(unsigned int baud_div);
void UART_SendChar(unsigned char data);
unsigned char UART_RcvChar(void);
void UART_SendData(unsigned char *buff, unsigned int num);
void UART_RcvData(unsigned char *buff, unsigned int num);
int Uart_demo(void);

#endif  /*_MINI_UART_H */
