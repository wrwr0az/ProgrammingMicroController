#include <stdint.h>
#include <stdio.h>
unsigned char sec, min1, hr;
unsigned char dday1, day1, month1, year1;
 // LCD module connections
 sbit LCD_RS at RE0_bit;
 sbit LCD_EN at RE2_bit;
 sbit LCD_D4 at RD4_bit;
 sbit LCD_D5 at RD5_bit;
 sbit LCD_D6 at RD6_bit;
 sbit LCD_D7 at RD7_bit;
 sbit LCD_RS_Direction at LATE0_bit;
 sbit LCD_EN_Direction at LATE2_bit;
 sbit LCD_D4_Direction at LATD4_bit;
 sbit LCD_D5_Direction at LATD5_bit;
 sbit LCD_D6_Direction at LATD6_bit;
 sbit LCD_D7_Direction at LATD7_bit;
 // End LCD module connections

unsigned short Wr_Time(char sec, char min, char hr) {
Soft_I2C_Init(); // initialize full master mode
Soft_I2C_Start(); // issue start signal
Soft_I2C_Write(0xD0); // address DS3232
Soft_I2C_Write(0); // start from word at address (REG0)
sec.B7 = 1; // Pause counter by setting the MSB of sec
Soft_I2C_Write(sec); // write sec to REG0.
Soft_I2C_Write(min); // write minutes word to (REG1)
Soft_I2C_Write(hr); // write hours word (24-hours mode)(REG2)
Soft_I2C_Stop(); // issue stop signal
Soft_I2C_Start(); // issue start signal
Soft_I2C_Write(0xD0); // address DS1307
Soft_I2C_Write(0); // start from word at address 0
Soft_I2C_Write(0); // write 0 to REG0 (enable counting + 0 sec)
Soft_I2C_Stop(); // issue stop signal
}//~!

unsigned short Wr_Date(char dday1, char day1, char month1, char year1) {
Soft_I2C_Init(); // initialize full master mode
Soft_I2C_Start(); // issue start signal
Soft_I2C_Write(0xD0); // address DS3232
Soft_I2C_Write(3);
Soft_I2C_Write(dday1);
Soft_I2C_Write(day1);
Soft_I2C_Write(month1);
Soft_I2C_Write(year1);
Soft_I2C_Stop(); // issue stop signal
}


 unsigned short read_ds1307(unsigned short address)
{
  unsigned short read_data;

  Soft_I2C_Start();
  Soft_I2C_Write(0xD0);
  Soft_I2C_Write(address);
  Soft_I2C_Start();
  Soft_I2C_Write(0xD1);
  read_data=Soft_I2C_Read(0);
  Soft_I2C_Stop();
  return(read_data);
}



unsigned char MSB(unsigned char x)           //Display Most Significant Bit of BCD number
{
  return ((x >> 4) + '0');
}

unsigned char  LSB(unsigned char x)          //Display Least Significant Bit of BCD number
{
  return ((x & 0x0F) + '0');
}

//Global Variables:

int second;
int minute;
int hour;
int hrr;
int day;
int dday;
int month;
int year;


char time[] = "00:00:00 ";
char date[] = "00-00-2000";
char daay[4] ;
// Software I2C connections

sbit Soft_I2C_Scl           at RC3_bit;
sbit Soft_I2C_Sda           at RC4_bit;
sbit Soft_I2C_Scl_Direction at TRISC3_bit;
sbit Soft_I2C_Sda_Direction at TRISC4_bit;
// End Software I2C connections

 char Temp[5];
 int j;

 void main(void) {

 TRISE = 0x00;
 TRISD = 0x00;
 TRISA.B0 = 1;  // Temp
 OSCCON = 0x73; // 8 MHz Internal Oscillator

 ADCON1 = 0x0E; // Digital I/O
 ADCON2 = 0x97;
 CMCON = 0x07; // Disable Comparator

 TRISD.B1 = 0; // PORTE.B1 is Output for R/W
 PORTD.B1 = 0; // R/W is 0 for writing to LCD

  // Select Time
  sec = 0x00; // Seconds = 0
  min1 = 0x10; // Minutes = 10
  hr = 0x15; // Hour = 13hr, ie 1 pm

  //Select Date
  //first step you should set the time then send the data to RTC by clear comment from line 133 then load the code to microcontroller
  // after that comment line 133 and reload the code
  dday1 = 0x4; // 0 for monday, 2 for tuesday, 3 for wednesday .. continue until 7
  day1 = 0x19; // from 0x01 to 0x30
  month1 = 0x12; // from 0x01 to 0x12 
  year1 = 0x19; // 0x19 means 2019
  
  // ** write date to RTC
  Wr_Date(dday1, day1, month1, year1);

 // ** Write time to RTC
//Wr_Time(sec, min1, hr);

Lcd_Init(); // Initialize LCD
Lcd_Cmd(_LCD_CLEAR); // Clear display
Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
 //Lcd_out(1,1,"Time: ");
 //Lcd_Out(2,1,"Temp:");

 while (1){

   /* TIME and DATE CODE */
      second = read_ds1307(0);
      minute = read_ds1307(1);
      hour = read_ds1307(2);
      // date
      dday = read_ds1307(3);
      day = read_ds1307(4);
      month = read_ds1307(5);
      year = read_ds1307(6);
      time[0] = MSB(hour);
      time[1] = LSB(hour);
      time[3] = MSB(minute);
      time[4] = LSB(minute);
      time[6] = MSB(second);
      time[7] = LSB(second);
      // get date
      date[0] = MSB(day);
      date[1] = LSB(day);
      date[3] = MSB(month);
      date[4] = LSB(month);
      date[8] = MSB(year);
      date[9] = LSB(year);
      //daay[0] = LSB(dday);
      switch(dday) {
       case 0:
       daay[0] = 'M';
       daay[1] = 'O';
       daay[2] = 'N';
       break;
       case 2:
       daay[0] = 'T';
       daay[1] = 'U';
       daay[2] = 'E';
       break;
       case 3:
       daay[0] = 'W';
       daay[1] = 'E';
       daay[2] = 'D';
       break;
       case 4:
       daay[0] = 'T';
       daay[1] = 'H';
       daay[2] = 'U';
       break;
       case 5:
       daay[0] = 'F';
       daay[1] = 'R';
       daay[2] = 'I';
       break;
       case 6:
       daay[0] = 'S';
       daay[1] = 'A';
       daay[2] = 'T';
       break;
       case 7:
       daay[0] = 'S';
       daay[1] = 'U';
       daay[2] = 'N';
       break;
      }
     //Lcd_out(1, 6, time);
       Lcd_out(1, 1, time);
       Lcd_out(2, 1, daay);
       Lcd_out(2, 6, date);


   /* TEMP CODE */
      ADCON0= 0b00000011;
      while (ADCON0.GO);

      j=ADRESL;
      j = j*0.48876;
      IntToStr (j,Temp);

      //Lcd_Out(2,8,Temp);
      Lcd_Out(1,9,Temp);
      Lcd_out(1,15,"C");
      Delay_ms(500);

  }
}