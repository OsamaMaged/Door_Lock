#include "tm4c123gh6pm.h"
#include <stdint.h>


#define Switch1 0x01
#define Switch2 0x10

#define zero 0x3f
#define one 0x06
#define two 0x5b
#define three 0x4f
#define four 0x66
#define five 0x6d
#define six 0x7d
#define seven 0x07
#define eight 0x7f
#define nine 0x6f
#define A 0x77
#define B 0x7c
#define C 0x39
#define D 0x5e
#define E 0x79
#define F 0x71

#define RS 0x10 /* PORTC BIT5 mask */
#define RW 0x20 /* PORTA BIT6 mask */
#define EN 0x40 /* PORTA BIT7 mask */

void PORTS_Init(void);
void LCD_start(void);
void LCD_data(unsigned char);
void LCD_command(unsigned char);
unsigned char keypad_getkey(void); //get keypad press
unsigned char keypad_getchar(void); //prevents any non-idigit character
void delay(int);
void delayMs(int);
void delayUs(int);
void systick(int); //delay in ms
void append(char*,char);//makes a string from characters
void LCD_puts(const char*);//prints string directly to lcd
void Timer0_Init(void);
int Timer0A_countCapture(void);
void Timer0A_delaySec(void); //delay 1 sec
float timeinSecs(int);//convert timestamp into seconds