#include<reg51.h>

/*Motor Connection Declaration*/
sbit doorp=P2^0;	//Opening Motor Pin	(P2.0) clockwise
sbit doorn=P2^1;	//Closing Motor Pin (P2.1) anti-clockwise

/*LCD Connection Declaration*/
sbit rs=P2^5;			//Register Select Pin (P2.5)
sbit rw=P2^6;			//Read & write Pin (P2.6)
sbit en=P2^7;			//Enable Pin (P2.7)

void lcddat(unsigned char);		//Function to send data to LCD, character by character(1 byte size)
void lcdcmd(unsigned char);		//Function to send command to LCD
void lcddis(unsigned char *);	//Function to display string in LCD

void lcd_init();							//LCD initialization
void serial_init();						//Serial Initialization for serial communication b/w RFID reader & LCD
void check();									//Function to Check if reg. no. is present in database or not
void convert(unsigned int);		//Convert String Count to Integer Count

/*Delay functions for LCD and Motor*/
void delay();
void mdelay();

unsigned char rfid[10],v1;		//Variable declaration, rfid[10] is 1-D array of size 10

/* MAIN function declaration */
void main()
{
	doorp=doorn=0;							//Both the motors opening & closing is initialized to 0
	serial_init();							//Serial Initialization function is called
	lcd_init();									//LCD initialization function is called
	lcddis("RFID BASED CAR");		
	lcdcmd(0xc0);								//0xc0 forces cursor to the beginning of second line
	lcddis("PARKING SYSTEM");
	delay();
	delay();
	lcdcmd(0x01);								//0x01 clears the LCD
	lcddis("MADE BY");
	lcdcmd(0xc0);
	lcddis("TEAM 26");
	delay();
	delay();
	while(1)
	{
		lcdcmd(0x01);								//LCD is cleared
		lcddis("SWIPE YOUR CARD");	
		//for loop is run for the no. of characters in the password one will enter
		for(v1=0;v1<9;v1++)
		{
			while(RI==0);			//wait untill data is received from rfid reader to 8051
												//After data is received it is saved in the SBUF register
			rfid[v1]=SBUF;		//SBUF character is saved in rfid[v1]
			RI=0;							//RI is forced to 0 to allow next received chracter byte to be placed in SBUF
			SBUF=rfid[v1];		//Re-Transmitting back to SBUF in order to see what we are typing
			while(TI==0);			
			TI=0;
		}
		check();						//Check function is called
	}
}

/*
	Check Function
	It matches the reg. no. entered in the terminal with one present here
*/
void check()
	{
		if(rfid[0]=='1'&&rfid[1]=='8'&&rfid[2]=='B'&&rfid[3]=='E'&&rfid[4]=='C'&&rfid[5]=='2'&&rfid[6]=='0'&&rfid[7]=='2'&&rfid[8]=='0')
		{
			lcdcmd(0x01);										//0x01 clears the LCD contents
			lcddis("ASSOCIATION MEMBER");		//String is displayed in the LCD
			lcdcmd("0xc0");									//0xc0 forces the cursor to beginning of second line
			lcddis("SHUBHAM ANIKET");
			delay();												//Delay function is called
			delay();
			lcdcmd(0x01);										//LCD is cleared
			doorp=1;doorn=0;								//Opening door is turned high
			lcddis("DOOR OPENING");
			lcdcmd(0xc0);										//Forcing cursor to beginning of second line
			lcddis("ALLOW INSIDE");
			mdelay();
			doorp=0;doorn=0;								//Both the motors are now turned off
			delay();
			lcdcmd(0x01);										//Clears the LCD
			doorp=0;doorn=1;								//Closing motor is turned high
			lcddis("DOOR CLOSING");
			mdelay();
			doorp=0;doorn=0;								//Both the motors are turned off again
		}
		else
		{
			lcdcmd(0x01);										//LCD is cleared
			lcddis("OUTSIDE PERSON");				
			lcdcmd(0xc0);										//Forcing cursor to second line 
			lcddis("NO SLOT FOR YOU");
			mdelay();												
		}
	}
	
	/*LCD initialization function*/
	void lcd_init()
	{
		lcdcmd(0x38);			//8-bit data initialization
		lcdcmd(0x01);			//Clear LCD Screen
		lcdcmd(0x10);			//Shift cursor position to right
		lcdcmd(0x0c);			//Display switch command to turn on display and turn off cursor
		lcdcmd(0x80);			//Sets the cursor to the beginng of first line
	}
	
	/*LCD command function*/
	void lcdcmd(unsigned char val)
	{
		P1=val;
		rs=0;
		rw=0;
		en=1;
		delay();
		en=0;
	}
	
	/*LCD Data Function*/
	void lcddat(unsigned char val)
	{
		P1=val;
		rs=1;
		rw=0;
		en=1;
		delay();
		en=0;
	}
	
	/*Delay Function in ms*/
	void delay()
	{
		unsigned int v5;
		for(v5=0;v5<6000;v5++);
	}
	
	/*LCD Display Function*/
	void lcddis(unsigned char *s)
	{
		unsigned char w;					//Temp. variable
		for(w=0;s[w]!='\0';w++)		//Sending character by character untill s[w] hits null character
		{
			lcddat(s[w]);
		}
	}
	
	/*Serial Initialization Function*/
	void serial_init()
	{
		SCON=0x50;				//SCON value is loaded with 50H 
		TMOD=0x20;				//Timer 1, Mode 2 selected
		TH1=-3;						//9600 baud rate 
		TR1=1;						//Timer 1 is started
	}
	
	void mdelay()
	{
		unsigned int v6,v7;
		for(v6=0;v6<2;v6++)
		{
			for(v7=0;v7<60000;v7++);
		}
	}