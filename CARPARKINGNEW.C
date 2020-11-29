#include<reg51.h>
#define lcdport P2
sbit senin=P1^0;  //assign ports inputs cehcking which door is opened
sbit senout=P1^1;
sbit door1_a=P3^4; //input for open door
sbit door1_b=P3^5;
sbit door2_a=P3^6;	//input for close door
sbit door2_b=P3^7;
sbit rs=P3^0; //connected to lcd
sbit rw=P3^1; 
sbit en=P3^2;
sbit led=P1^7;
sbit pulse=P3^5;
int k,p;
unsigned int count=8;//counter variable
unsigned int digit[5];
void entry_door_open();
void exit_door_open();  //declaration of functions used
void entry_door_close();
void exit_door_close();
void exit_door_stop();
void entry_door_stop();
void lcdcmd(char);//Function to send command to LCD
void lcdint();
void lcddata(char);//Function to send data to LCD, character by character(1 byte size)
void lcdstring(char *);
void delay(unsigned int);
void convert(unsigned int);
void ldelay(unsigned int);
            
void main()
{
        lcdint();
        led=0;
        senin=0;//in door closed
        senout=0;//both doors closed
        lcdstring("AUTOMATIC CAR");//lcd display
        lcdcmd(0xc0);
        lcdstring("PARKING SYSTEM");
        delay(50000);//delay 
        lcdcmd(0x01);
        lcdstring("parking left:");
        lcdcmd(0x8d);
        convert(count);//as lcd understands string only converting int to count
        while(1)
        {
        lcdcmd(0x8d);
        led=1;//led glows
        ldelay(10);
        if(senin==1)//as a car enters
        {
        count=count-1;//counter decreases
        convert(count);//converting int to string
        entry_door_open();//open door function is called
        while(senin==1);//delay when door is opened
        delay(60000);
        delay(60000);
        entry_door_close();//closing door
        entry_door_stop();
        delay(5000);
       }
       else if(senout==1)//if out door is opened
       {
        count=count+1;//counter increases
        convert(count);
        exit_door_open();//door is open 
        while(senout==1);//delay
        delay(60000);
        delay(60000);
        exit_door_close();
        exit_door_stop();
        delay(5000);
        }
          }
            }
void convert(unsigned int value)
{
  led=0;      
  for(k=0;k<2;k++)
  {
        digit[k]=value%10;
        value=value/10;
        }  
        for(p=1;p>=0;p--)
        {
				lcddata(digit[p]+48);//ascii value converting .
        ldelay(1);
        }
        if (count==0)//if count is zero then NIL
        {
        lcdcmd(0x8d);
        lcdstring("NIL");
        }
      
        ldelay(10);
        }
 void delay(unsigned int x)
 {
        unsigned int i;
        for(i=0;i<x;i++);
} 
 void lcdint()
{
         lcdcmd(0x38);//8-bit data initialization
         delay(500);
         lcdcmd(0x01);//clear LCD screen
         delay(500);
         lcdcmd(0x0c);//Display switch command to turn on display and turn off cursor
         delay(500);
         lcdcmd(0x80);//Sets the cursor to the beginng of first line
         delay(500);
         lcdcmd(0x0e);
         delay(500);
    }
 
void lcdcmd(char value)
       {
         lcdport = value;
         rw=0;//read & write pin
         rs=0;//register select pin
         en=1;//enable
         delay(500);
         en=0;
       }   
void lcdstring(char *p)
      {
         while(*p!='\0')
          {
            lcddata(*p);
            delay(2000);
            p++;
          }
      }
 void lcddata(char value)
{
         lcdport=value;
         rs=1;//read & write pin
         rw=0;//register select pin
         en=1; //enable
         delay(500);
         en=0;
}
void ldelay(unsigned int x)
          {
               unsigned int i,j;
           for (i=0;i<=x;i++)
           for (j=0;j<=1275;j++);
       }
void entry_door_open()
{
lcdcmd(0xc0);
lcdstring("               ");
lcdcmd(0xc0);
lcdstring("opening door");
door1_a=1;
door1_b=0;
delay(60000);
delay(60000);
}
void entry_door_close()
{
lcdcmd(0xc0);
lcdstring(" ");
lcdcmd(0xc0);
lcdstring("clossing door");
door1_a=0;
door1_b=1;
delay(60000);
delay(60000);
}
void entry_door_stop()
{
lcdcmd(0xc0);
lcdstring("  ");
lcdcmd(0xc0);
lcdstring("door closed");
door1_a=0;
door1_b=0;
delay(60000);
delay(60000);
lcdcmd(0xc0);
lcdstring("");
}
void exit_door_open()
{
lcdcmd(0xc0);
lcdstring("");
lcdcmd(0xc0);
lcdstring("opening door");
door2_a=0;
door2_b=1;
delay(60000);
delay(60000);
}
void exit_door_close()
{
lcdcmd(0xc0);
lcdstring("");
lcdcmd(0xc0);
lcdstring("clossing door");
door2_a=1;
door2_b=0;
delay(60000);
delay(60000);
}
void exit_door_stop()
{
lcdcmd(0xc0);
lcdstring("");
lcdcmd(0xc0);
lcdstring("door closed");
door2_a=0;
door2_b=0;
delay(60000);
delay(60000);
lcdcmd(0xc0);
lcdstring("");
}
