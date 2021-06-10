#include "tm4c123gh6pm.h"
#include "PORTS.h"
#include <stdio.h>
int main(){
  PORTS_Init();
  int retry=0;
start:
  LCD_start();
   GPIO_PORTF_DATA_R &=~0x08;
  unsigned char key;
  LCD_puts("Enter Password");
  LCD_command(0xc0);
  int i=0;
  int press=1;
  char password[6]={'1','2','3','A','B','C'}; //correct password
  int correct=1;
  while(1)
  {
    
    while(press) //if a key is pressed
    {
      key = keypad_getkey();
      systick(300);
      if (i==6)
      {press=0;
      break;
      }
      if(key!=0)
      { 
        if(password[i]!=key)
        {
          correct = 0;
        }
        i++;
        LCD_data('*');
        
      }
    }
    
    if(correct)
    {
      press=1;
      GPIO_PORTF_DATA_R |= 0x08;
      retry=0;
      LCD_start();
      LCD_puts("ACCESS GRANTED");
      systick(100);
      GPIO_PORTA_DATA_R |= 0x80; //open lock
      delayMs(20000);//wait for 20 secs and then automtically lock the door
      GPIO_PORTA_DATA_R &=~0x80;
          while(press) //if a key is pressed
    {
      key=key_getkey();
      if(key=='*')
      {
        press=0;

      }   
      else goto start;
    }
    }
    else
    {
      if(retry<2)
      {
        LCD_start();
        LCD_puts("WRONG PASSWORD");
        LCD_command(0xc0);
        LCD_puts("TRY AGAIN");
        retry++; 
        delayMs(2500);
      }
      else{
        LCD_start();
        LCD_puts("ACCESS FAILED");
        while(GPIO_PORTF_DATA_R & Switch2==Switch2){    
        GPIO_PORTF_DATA_R=0x02;
        systick(2000);
        GPIO_PORTF_DATA_R=0x00;
        systick(2000);
        
      }
    }
    goto start;
    
    //  int eh=strcmp(input,password);
    //      if(eh==0 && (strlen(input)==6))
    //      {
    //        retry=0;
    //        LCD_start();
    //        LCD_puts("ACCESS GRANTED");
    //        systick(100);
    //        GPIO_PORTA_DATA_R |= 0x80; //open lock
    //        delayMs(200000);//wait for 20 secs and then automtically lock the door
    //        GPIO_PORTA_DATA_R &=~0x80;
    //        break;
    //      }
    
    //      else if (strcmp(input,password)!=0&& (strlen(input)==6))
    //      {
    //        if(retry<2&&strcmp(input,password)!=0&& (strlen(input)==6))
    //        {
    //        LCD_start();
    //        LCD_puts("WRONG PASSWORD");
    //        LCD_command(0xc0);
    //        LCD_puts("TRY AGAIN");
    //        input[0]=0;
    //        retry++; 
    //        }
    //        else
    //        {
    //          LCD_start();
    //          LCD_puts("ACCESS FAILED");
    //          input[0]=0;
    //          
    //    while(GPIO_PORTF_DATA_R & Switch2==Switch2){
    //      
    //  //  case Switch1: //start countdown
    //          
    //    //      LCD_puts("Enter Password");
    //    //      LCD_command(0xc0);
    //   // } 
    //        GPIO_PORTF_DATA_R=0x02;
    //          systick(2000);
    //          GPIO_PORTF_DATA_R=0x00;
    //          systick(2000);
    //          
    //         }
    //   
    //    goto start;
    //          
    //     
    //      
    //      
    //      }
    //      
    //      
    //      
    //      systick(100);
    // //switch
    //  }
    }}}