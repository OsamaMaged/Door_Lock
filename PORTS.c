#include "PORTS.h"

void PORTS_Init(void)
{
  uint32_t dummy;
  SYSCTL_RCGCGPIO_R |= 0x20;
  SYSCTL_RCGCGPIO_R |= (0x02|0x01|0x04|0x10);
  dummy= SYSCTL_RCGCGPIO_R;
  
  GPIO_PORTF_LOCK_R =0x4c4f434b;
  GPIO_PORTF_CR_R = 0x01f;
  
  GPIO_PORTF_DIR_R &= ~0x11;
  GPIO_PORTF_DIR_R |= 0x0e;
  GPIO_PORTF_DEN_R |= 0x1f;
  GPIO_PORTF_PUR_R |= (0x01 | 0x10);
  
  //D* pins at port b
  GPIO_PORTB_DIR_R |= 0xff;
  GPIO_PORTB_DEN_R = 0xff;
  
  //RS RW E pins at A4 A5 A6, A7
  GPIO_PORTA_DIR_R |= 0xf0;
  GPIO_PORTA_DEN_R = 0xf0;
  
  //Keypad
  //columns
  GPIO_PORTE_DIR_R &= ~0x0f;
  GPIO_PORTE_DEN_R = 0x0f;
  GPIO_PORTE_PUR_R = 0x0f;
  
  //rows
  GPIO_PORTC_DIR_R = 0xf0;
  GPIO_PORTC_DEN_R = 0xf0;
  GPIO_PORTC_ODR_R= 0xf0;
  
}

unsigned char keypad_getkey()
{
  const unsigned char keymap[4][4] = {
    {'1', '2', '3', 'A'},
    { '4', '5', '6', 'B'},
    { '7', '8', '9', 'C'},
    { '*', '0', '#', 'D'},
  };
  
  int row;
  /* check to see any key pressed first */
  GPIO_PORTC_DATA_R = 0; /* enable all rows */
  switch (GPIO_PORTE_DATA_R & 0x0f) case 0x0f: return 0; /* no key pressed */
  
  while (1)
  {
    row = 0;
    GPIO_PORTC_DATA_R = 0xE0; /* enable row 0 */
    delay(2000); /* wait for signal to settle */
    switch(GPIO_PORTE_DATA_R & 0x0f)
    {
    case 0x0E: return keymap[row][0]; /* key in column 0 */
    case 0x0D: return keymap[row][1]; /* key in column 1 */
    case 0x0B: return keymap[row][2]; /* key in column 2 */
    case 0x07: return keymap[row][3]; /* key in column 3 */   
    }
    
    row = 1;
    GPIO_PORTC_DATA_R = 0xD0; /* enable row 1 */
    delay(2000); /* wait for signal to settle */
    switch(GPIO_PORTE_DATA_R & 0x0f)
    {
    case 0x0E: return keymap[row][0]; /* key in column 0 */
    case 0x0D: return keymap[row][1]; /* key in column 1 */
    case 0x0B: return keymap[row][2]; /* key in column 2 */
    case 0x07: return keymap[row][3]; /* key in column 3 */   
    }
    
    row = 2;
    GPIO_PORTC_DATA_R = 0xB0; /* enable row 2 */
    delay(2000); /* wait for signal to settle */
    switch(GPIO_PORTE_DATA_R & 0x0f)
    {
    case 0x0E: return keymap[row][0]; /* key in column 0 */
    case 0x0D: return keymap[row][1]; /* key in column 1 */
    case 0x0B: return keymap[row][2]; /* key in column 2 */
    case 0x07: return keymap[row][3]; /* key in column 3 */   
    }
    
    row = 3;
    GPIO_PORTC_DATA_R = 0x70; /* enable row 3 */
    delay(2000); /* wait for signal to settle */
    switch(GPIO_PORTE_DATA_R & 0x0f)
    {
    case 0x0E: return keymap[row][0]; /* key in column 0 */
    case 0x0D: return keymap[row][1]; /* key in column 1 */
    case 0x0B: return keymap[row][2]; /* key in column 2 */
    case 0x07: return keymap[row][3]; /* key in column 3 */   
    }
  }
  
}

unsigned char keypad_getchar(){
  char key=keypad_getkey();
  if(key == 'A' || key == 'B' || key == 'C' || key == 'D' || key == '*' || key == '#')
    return 0;
  return key;
}

void LCD_command(unsigned char command)

{
  
  GPIO_PORTA_DATA_R = 0;
  /* RS = 0, R/W = 0 */
  
  GPIO_PORTB_DATA_R = command;
  
  GPIO_PORTA_DATA_R = EN; //enable
  /* pulse E */
  
  delayUs(1);
  /* Enable pulse Width */
  
  GPIO_PORTA_DATA_R = 0;
  
  if (command < 4)
    
    delayMs(2);
  /* command 1 and 2 needs up to 1.64ms */
  
  else
    
    delayUs(40); /* all others 40 us */
  
}
void LCD_data(unsigned char data)

{
  
  GPIO_PORTA_DATA_R = RS; /* RS = 1, R/W = 0 */ //C4
  // delayUs(1000);
  GPIO_PORTB_DATA_R = data;
  
  GPIO_PORTA_DATA_R = EN | RS; /* pulse E */
  
  delayUs(1);
  
  GPIO_PORTA_DATA_R = 0;
  
  delayUs(40);
}

void LCD_start()
{
  delayMs(20); /* initialization sequence */// Wait >15 ms after power is applied
  
  LCD_command(0x30);
  // command 0x30 = Wake up
  
  delayMs(5);
  // must wait 5ms, busy flag not available
  
  LCD_command(0x30);
  // command 0x30 = Wake up #2
  
  delayUs(100);
  // must wait 100us, busy flag not available
  
  LCD_command(0x30);
  // command 0x30 = Wake up #3
  
  delayUs(100);
  // must wait 100us, busy flag not available
  
  // Prepare LCD Operation and Function
  
  LCD_command(0x38); /* set 8-bit data, 2-line, 5x7 font */
  
  LCD_command(0x06); /* move cursor right */
  
  LCD_command(0x01); /* clear screen, move cursor to home */
  
  LCD_command(0x0F); /* turn on display, cursor blinking */}


void delayMs(int n)
{
  int i, j;
  for(i = 0 ; i < n; i++)
    for(j = 0; j < 3180; j++)
    {} /* do nothing for 1 ms */
}
/* delay n microseconds (16 MHz CPU clock) */
void delayUs(int n)
{
  int i, j;
  for(i = 0 ; i < n; i++)
    for(j = 0; j < 3; j++)
    {} /* do nothing for 1 us */
}


void delay(int time)
{
  int delay=0;
  while(delay<time)
  {delay++;}
}

void systick(int n) {
  /* Configure SysTick */
  uint32_t counts = n*1000*16-1; //counts for milliseconds
  NVIC_ST_CTRL_R = 0;
  NVIC_ST_RELOAD_R = counts;   /* reload reg. with max value */
  NVIC_ST_CTRL_R = 5;         /* enable it, no interrupt, use system clock */
  while ((NVIC_ST_CTRL_R & 0x10000) == 0);         /* if COUNT (D16 of CTRL reg.) flag is set : Reading COUNT FLAG will reset it back*/
}



void append(char* s, char c) {
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}

void LCD_puts(const char *s)
{
  char c;
  while ( (c = *s++) ) {
    LCD_data(c);
  }
}

