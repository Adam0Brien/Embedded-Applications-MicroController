///////////////////////////////////////////////////////////////////////
//
// File Name:      BENG (ORD) Base board interface template
// Created by:     Student Name
// Course:         Course Name
// Date Created:   15/10/08
//
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
//
// Implements a simple master slave protocol for the PROJECT BASEBOARD:
// connect via rs232 9600 n even
// -------------------------------------------------------------------
// MESSAGE FORMAT:
// -------------------------------------------------------------------
// | MESSAGE TYPE | MESSAGE DATA | END OF MESSAGE |
// -------------------------------------------------------------------
//  MESSAGE TYPES: di for digital inputs
//                 do for digital outputs
//                 ai for analog inputs
//                 ld for LCD display
//                 cr can rx message (not implemented)
//                 ct can tx message (not implemented)
//
// -------------------------------------------------------------------
// Digital Inputs "di"
// -------------------
// MESSAGE STRUCTURE: "di" then channel number i.e "di1", "di2"
//
// DESCRIPTION:
// when the base board receives "di1" then it checks status of digital
// input number 1 i.e check the status of the switch on the base board
//
// -------------------------------------------------------------------
// Digital Outputs "do"
// -------------------
// MESSAGE STRUCTURE: "do" then channel number then channel value i.e "do11"
//                    "do10","do21"
// DESCRIPTION:
// when the base board receives "do11" it will turn digital output 1 on
// when the base board receives "do10" it will turn digital output 1 off
// when the base board receives "do21" it will turn digital output 2 on
// etc, etc..
//
// -------------------------------------------------------------------
// Analog Input "ai"
// -------------------
// MESSAGE STRUCTURE: "ai" then channel number i.e "ai1", "ai2"
//
// DESCRIPTION:
// when the base board receives "ai" then it checks status of analog
// input number 1 i.e reads the first analog (pot) and returns value
//
// -------------------------------------------------------------------
// LCD functions "ld"
// -------------------
// MESSAGE STRUCTURE:
//                     ld1,2,3 or 4 display text at line 1,2,3 or 4
//                     ---------------------------------------------
//                     "ld" then "line number" then "text" to display
//                     i.e "ld1hello world"
//                         "ld2testing display"
//
//                     ldc to clear display
//                     ---------------------------------------------
//                     "ld" then 'c' to clear display
//                     i.e  "ldc"
//
//                     ldb to move back one position
//                     ---------------------------------------------
//                     "ld" then 'b' to move back one position
//                     i.e  "ldb"
//
//                     ldp go x co ordinate y co rodinate
//                     ---------------------------------------------
//                     "ld" then 'p' then 'x co ord' then 'y co ord'
//                     i.e "ldp11" go to  line 1 position 1 (upper left is 1,1)
//                         "ldp31" go to line 3 position 1
//
//                     lds display text from current set x y co ordinate
//                     ---------------------------------------------
//                     "ld" then 's' then "text"
//                     i.e "ldshello" display from last x y position set
//                         "ldsgoodye" display from last x y position set
//
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Can transmit message "ct"
// -------------------------
// not implemented
//
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Can receive message "cr"
// -------------------------
// not implemented
//
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// END OF MESSAGE: This is a simple carriage return i.e enter
// -------------------------------------------------------------------
// -------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////
//Version      By   Date         Description
///////////////////////////////////////////////////////////////////////
//   000       JTB  18/11/08     initial version
//   001       JTB  9/11/09      added lcd functionality
///////////////////////////////////////////////////////////////////////

//preprocessor directives
//////////////////////////////////////////////////////////////////////
#include <16F877A.h>
#device ICD=TRUE
#device *=16

// setup clock speed
#use delay(clock=20000000)

//set up rs232 pins on pic
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

//microbit
//#use rs232(baud=9600, xmit=PIN_B1, rcv=PIN_B2)                 //communicate with microbit

//max233
#use rs232(baud=9600, xmit=pin_a3, rcv=PIN_a5)                   //communicate pic through putty
//#use fixed_io(a_outputs=PIN_A2, PIN_A3)


#include <stdio.h>
#include <stdlib.h>
#include "BENGLCD420.C"

// setup chip
////////////////////////////////////////////////
#fuses HS,NOLVP,NOWDT,NOPROTECT,NOBROWNOUT,PUT

//Constants
////////////////////////////////////////////////

//test codes enable/disable
///////////////////////////////////////////////
//#define TEST_COMMS

//message deifinitons
//////////////////////////////////////////////

//message error codes
#define BAD_MESSAGE_TYPE 1

//message type definitions
//message type location
#define MESSAGE_TYPE_BYTE_SIZE             2

//message types
#define DIGITAL_INPUT_COMMAND             "di"
#define DIGITAL_OUTPUT_COMMAND            "do"
#define ANALOG_INPUT_COMMAND              "ai"
#define ANALOG_OUTPUT_COMMAND             "ao"
#define LCD_DISPLAY_COMMAND               "ld"
#define CAN_TRAMSNIT_COMMAND              "ct"
#define CAN_RECEIVE_COMMAND               "cr"

//digital input command message data
#define DI_CHANNEL_NUMBER_BYTE             2

//digital output command message data
#define DO_CHANNEL_NUMBER_BYTE             2
#define DO_CHANNEL_VALUE_BYTE              3

//analog input command message data
#define AI_CHANNEL_NUMBER_BYTE             2

//LCD command command message data
#define LCD_LINE_COMMAND_BYTE              2

/////////////////////////////////////////////

//local function prototypes
/////////////////////////////////////////////
void initialise_board(void);
void startup_screen(void);
int8 check_message_type(char *arg1);
void send_error_message(int8);
void handle_message(char *arg1);
void local_monitoring(void); 
void execute_digital_input_command(char *arg);
void execute_digital_output_command(char *arg);
void execute_analog_input_command(char *arg);
void execute_lcd_command(char *arg1);
void progress_bar(int8);


//global variables
//////////////////
int8 counter = 0;
//structure templates
/////////////////////






































//////////////////////////////////////////////////////////////////////
// function name: main()
// Date Created:  15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
//
// START OF PROGRAM
//
// Checks if message on serial port, if true then handle
// then carry out local monitor. This is where
// di,ai and do are montored and displayed locally
// on the lcd.
//
///////////////////////////////////////////////////////////////////////
void main()
{
//global definitions
char message[30];        // array to store received message
int error_code = 0;      // holds the returns from message checking functions
int message_counter = 0; // holds number of messages sent to robot

//initialisation sequence
initialise_board();
while(0)
   {
    printf("Hello World \n\r");
    //delay_ms(1000);
   }

while(1)
   {

   if(kbhit())
      {
      //read in message from serial port
      gets(message);

      //check if valid message type i.e first two data bytes
      error_code = check_message_type(message);

      //if message type correct then process meesage
      if(error_code == 0) // i.e no error if = to 0
         {
         handle_message(message);
         }
      //else message type not valid so send back message type error
      else
         {
         send_error_message(BAD_MESSAGE_TYPE);
         }
      }//end of character received
    
   //local monitoring
   //local_monitoring();
   //progress_bar(16);
   //progress_bar(1);


   }//end of while(1)
}






























//----------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////
// LEVEL ONE: FUNCTION IMPLEMENTATIONS
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// function name: initialise_board
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
// initialises the following on the board
// digital output ports
// digital input ports
// analog inputs
//
// prints up version number
//
///////////////////////////////////////////////////////////////////////
void initialise_board(void)
{
//init lcd
lcd_init();

//initialise portb to be outputs
//SET_TRIS_B( 0x04 );

//initialse pin a4 to be an input
//SET_TRIS_a( 0xdf ); // 11011111

//initialise pin a0 as analog input and select ad channel 0
setup_adc(ADC_CLOCK_INTERNAL );
setup_adc_ports( AN0 );
set_adc_channel(0);


//send version info out on serial
printf("Base board 2022 \n\r");
printf("-----------------------------------------\n\r");
printf("Commands: ,ai1<enter>, di1<enter>\n\r");
printf("do11<enter>, do10<enter>, ld1hello<enter>\n\r");
printf("ld2goodbye<enter>, ldc<enter>\n\r");
printf("-------------waiting---------------------\n\r");

//startup lcd screen and animation
startup_screen();


}

//////////////////////////////////////////////////////////////////////
// Fnction name: check_message_type
// Aguments: first character in message
// Rturns: 0 if ok or 1 not a recognised type
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
// Checks first byte of message against d,s,l,o and c
// returns 0 if ok otherwise returns 1 if bad type
//
//
///////////////////////////////////////////////////////////////////////

int8 check_message_type(char *arg1)
{
//set up local copies for string compare functions
//set up local copies
char di_command[]  =  "di";
char do_command[] =   "do";
char ai_command[]   = "ai";
char ld_command[]   = "ld";

// check if message contains any valid message types
// uses string compare to check first two bytes of message i.e "di" against
// known commands

if(!strncmp(arg1, di_command, MESSAGE_TYPE_BYTE_SIZE))
   {
   return(0);
   }
else if(!strncmp(arg1, do_command, MESSAGE_TYPE_BYTE_SIZE))
   {
   return(0);
   }
else if(!strncmp(arg1, ai_command, MESSAGE_TYPE_BYTE_SIZE))
   {
   return(0);
   }
else if(!strncmp(arg1, ld_command, MESSAGE_TYPE_BYTE_SIZE))
   {
   return(0);
   }

else // invalide message type
   {
   return(BAD_MESSAGE_TYPE); // remember 1 from #define above
   }
}


//////////////////////////////////////////////////////////////////////
// Fnction name: handle_messae
// Aguments: pointer to message typed in by the user
// Rturns: none
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
// decides what to do based on first two char in the message typed in
// "di"  execute_digital_input_command
// "do"  execute_digital_output_command
// "ai"  execute_analog_inputs_command
//
///////////////////////////////////////////////////////////////////////
void handle_message(char *arg1)
{
//set up local copies for string compare functions
char di_command[]   = "di";
char do_command[]   = "do";
char ai_command[]   = "ai";
char ld_command[]   = "ld";


//handle message based on message type
if(!strncmp(arg1, di_command, MESSAGE_TYPE_BYTE_SIZE))
   {
   execute_digital_input_command(arg1);
   }//end of drive command

else if(!strncmp(arg1, do_command, MESSAGE_TYPE_BYTE_SIZE))
   {
   execute_digital_output_command(arg1);
   }

else if(!strncmp(arg1, ai_command, MESSAGE_TYPE_BYTE_SIZE))
   {
   execute_analog_input_command(arg1);
   }
else if(!strncmp(arg1, ld_command, MESSAGE_TYPE_BYTE_SIZE))
   {
   execute_lcd_command(arg1);

   }
else
   {
   //blank
   }

 // test code when enabled it prints out string received
#ifdef TEST_COMMS
printf("::string received:%s::      ",arg1);
#endif

}
//////////////////////////////////////////////////////////////////////
// Fnction name: send_error_message
// Aguments: int8 holds type of error message
// Returns: none
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
// printf type of message error ,example 1 bad message type
//
//
///////////////////////////////////////////////////////////////////////
void send_error_message(int8 arg1)
{

if(arg1 == BAD_MESSAGE_TYPE)
   {
   printf("NAK    \r");
   }
}



//////////////////////////////////////////////////////////////////////
// Function name: local_monitoring()
// Aguments: void
// Returns: void
// Date Created:   11/11/09
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
// student to complete
//
//
///////////////////////////////////////////////////////////////////////
void local_monitoring(void)
{

//student add code here
//simple example printing variable out on lcd

//display text in position 1 line 3
lcd_gotoxy( 1,3);

//display to lcd using printf, easiest way to display changing variables
printf(lcd_putc,"variable i = %3.0u     ",counter++);

}


//////////////////////////////////////////////////////////////////////
// Function name: progress_bar()
// Aguments: void
// Returns: void
// Date Created:   11/11/09
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
// student to complete
//
//
///////////////////////////////////////////////////////////////////////
void progress_bar(int8 arg)
{
int8 i;
//student add code here
//simple example printing variable out on lcd

//clear screen
lcd_putc('\f');

for(i=3;i<=arg+2;i++)
   {
   //display text in position 1 line 3
   lcd_gotoxy( i,2);

   // display square
   lcd_send_byte(1,0xff);

   //delay_ms
   delay_ms(200);
   }
//display to lcd using printf, easiest way to display changing variables
//printf(lcd_putc,"variable i = %3.0u     ",counter++);

}




















//-----------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////
// LEVEL TWO: FUNCTION IMPLEMENTATIONS
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Fnction name: execute_digital_input_command
// Aguments: pointer to message typed in by the user
// Rturns: none
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
// executes the specified drive command.
//
///////////////////////////////////////////////////////////////////////
void  execute_digital_input_command(char *arg)
{


//read digital input as per command
if(arg[DI_CHANNEL_NUMBER_BYTE] == '1')
   {
   if( input(PIN_A4) )
      {
      printf("di1:1  \r");
      }
    else
      {
      printf("di1:0  \r");
      }
   }

else if(arg[DI_CHANNEL_NUMBER_BYTE] == '2')
   {
   //not implemented
   }
else
   {
   }

}
//////////////////////////////////////////////////////////////////////
// Fnction name: execute_digital_output_command
// Aguments: pointer to message typed in by the user
// Rturns: none
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
//
//
///////////////////////////////////////////////////////////////////////
void execute_digital_output_command(char *arg)
{
if(arg[DO_CHANNEL_NUMBER_BYTE] == '1')
   {
   if(arg[DO_CHANNEL_VALUE_BYTE ] == '1')
      {
      output_low(PIN_B4);
      }
   else
      {
      output_high(PIN_B4);
      }
   }
else if(arg[DO_CHANNEL_NUMBER_BYTE] == '2')
   {
   if(arg[DO_CHANNEL_VALUE_BYTE ] == '1')
         {
         output_low(PIN_B5);
         }
      else
         {
         output_high(PIN_B5);
         }
   }     
else if(arg[DO_CHANNEL_NUMBER_BYTE] == '3')
   {
   if(arg[DO_CHANNEL_VALUE_BYTE ] == '1')
        {
        output_low(PIN_A5);
        }
      else
        {
        output_high(PIN_A5);
        }
   }
}

//////////////////////////////////////////////////////////////////////
// Fnction name: execute_analog_inputs_command
// Aguments: none
// Rturns: none
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
//
// measure object sensors and return values
// reads the object sensors and returns values into int8
// leftSensor and int8 rightSensor
//
///////////////////////////////////////////////////////////////////////
void execute_analog_input_command(char *arg)
{
int8 ad_value =0;

if(arg[AI_CHANNEL_NUMBER_BYTE] == '1')
   {
   //read adc (ai)
   ad_value = read_adc();

   //send data out
   printf("ai1:%3.0u\r",ad_value);
   }
else if(arg[AI_CHANNEL_NUMBER_BYTE] == '2')
   {
   //alarm code goes here
   }
else
   {
   //blank
   }

}









































//////////////////////////////////////////////////////////////////////
// Fnction name: execute_lcd_command
// Aguments: none
// Rturns: none
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
//
// LCD functions "ld"
// -------------------
// MESSAGE STRUCTURE:
//                     ld1,2,3 or 4 display text at line 1,2,3 or 4
//                     ---------------------------------------------
//                     "ld" then "line number" then "text" to display
//                     i.e "ld1hello world"
//                         "ld2testing display"
//
//                     ldc to clear display
//                     ---------------------------------------------
//                     "ld" then 'c' to clear display
//                     i.e  "ldc"
//
//                     ldb to move back one position
//                     ---------------------------------------------
//                     "ld" then 'b' to move back one position
//                     i.e  "ldb"
//
//
//                     ldp go x co ordinate y co rodinate
//                     ---------------------------------------------
//                     "ld" then 'p' then 'x co ord' then 'y co ord'
//                     i.e "ldp11" go to  line 1 position 1 (upper left is 1,1)
//                         "ldp31" go to line 3 position 1
//
//                     lds display text from current set x y co ordinate
//                     ---------------------------------------------
//                     "ld" then 's' then "text"
//                     i.e "ldshello" display from last x y position set
//                         "ldsgoodye" display from last x y position set
//
//
///////////////////////////////////////////////////////////////////////
void  execute_lcd_command(char *arg)
{
//lcd cursor location
byte position_x =0;
byte position_y =0;


//set position 23 to be Null char
// arg will hold message received from serial port
// i.e ld1hello world is stored as follows in array
//    _________________________________
//    |l|d|1|h|e|l|l|o| |w|o |r |l |d |
//    ---------------------------------
// arg|0|1|2|3|4|5|6|7|8|9|10|11|12|13|
//    ---------------------------------
//
// we need a null character at the end of string before we send text to lcd
// also note: text to display starts at arg[3]

arg[23] = NULL;


if(arg[LCD_LINE_COMMAND_BYTE] == '1')
   {
   //display text on lcd line 1
   lcd_gotoxy( 1,1);

   //note text to display starts at arg[3]
   printf(lcd_putc,"%s",arg+3);
   }
else if(arg[LCD_LINE_COMMAND_BYTE] == '2')
   {
   //display text on lcd line 2
   lcd_gotoxy( 1,2);
   printf(lcd_putc,"%s",arg+3);
   }
else if(arg[LCD_LINE_COMMAND_BYTE] == '3')
   {
   //display text on lcd line 3
   lcd_gotoxy( 1,3);
   printf(lcd_putc,"%s",arg+3);
   }
else if(arg[LCD_LINE_COMMAND_BYTE] == '4')
   {
   //display text on lcd line 4
   lcd_gotoxy( 1,4);
   printf(lcd_putc,"%s",arg+3);
   }
else if(arg[LCD_LINE_COMMAND_BYTE] == 'c')
   {
   //clear lcd display
   lcd_putc('\f');
   }
else if(arg[LCD_LINE_COMMAND_BYTE] == 'p')
   {
   //convery ascii values to byte value for goto function
   // ascii to int trick of the trade

   position_x = arg[3]- 48;
   position_y = arg[4]- 48;

   lcd_gotoxy(position_x,position_y);
   }
else if(arg[LCD_LINE_COMMAND_BYTE] == 'b')
   {
   //move cursor back one positon
   lcd_putc('\b');

   }
else if(arg[LCD_LINE_COMMAND_BYTE] == 's')
   {
   printf(lcd_putc,"%s",arg+3);
   }
 else
    {
    //blank
    }
}


//////////////////////////////////////////////////////////////////////
// function name: startup_screen
// Date Created:   15/10/08
///////////////////////////////////////////////////////////////////////
// Purpose:        Description of Program
// initialises the following on the board
// sets lcd display plus animation of space invader character
//
// see the following web site for bitmap generation
//   http://www.8051projects.net/lcd-interfacing/lcd-custom-character.php
//   note: scroll to bottom of the page above
//
///////////////////////////////////////////////////////////////////////
void startup_screen(void)
{
//bitmap design for custom icons
unsigned char unlocked_pattern[8] = {14,16,16,31,27,27,31,0};
unsigned char locked_pattern[8]   = {14,17,17,31,27,27,31,0};
unsigned char space_invader1[8]   = {0,17,14,27,21,14,31,17};
unsigned char space_invader2[8]   = {0,17,14,27,21,14,31,10};

int16 i;

//send down first custom pattern
LCD_build(0,locked_pattern);

//send down second custom pattern
LCD_build(1,unlocked_pattern);

//send down third custom pattern
LCD_build(2,space_invader1);

//send down fourth custom pattern
LCD_build(3,space_invader2);

//print out welcome info on lcd
lcd_gotoxy( 1,1);
printf(lcd_putc,"WIT Internet of Things");

//display fisrt custom pattern
lcd_gotoxy( 10,2);
lcd_send_byte(1,0x00);

//space invader animation
for(i=9;i<=11;i++)
   {
   //display third custom pattern
   lcd_gotoxy(i,3);
   lcd_send_byte(1,0x02);

   //delay to see effect
   delay_ms(400);

   //display fourth custom pattern
   lcd_gotoxy(i,3);
   lcd_send_byte(1,0x03);

   //delay to see effect
   delay_ms(400);

   //blank this position at x, y
   lcd_gotoxy(i,3);
   lcd_send_byte(1,0x20);
   }

//display second custom pattern
lcd_gotoxy( 10,2);
lcd_send_byte(1,0x01);

lcd_gotoxy( 9,3);
printf(lcd_putc,"2022");
lcd_gotoxy( 1,4);
printf(LCD_PUTC,__DATE__);
lcd_gotoxy(13,4);
printf(LCD_PUTC,__TIME__);

delay_ms(2000);
lcd_putc('\f');

lcd_gotoxy(8,2);
printf(lcd_putc,"Adam");
lcd_gotoxy(7,3);
printf(lcd_putc,"20093460");

//delay for a short time
delay_ms(3000);


//clear lcd display
lcd_putc('\f');
}


////end of file
///////////////


















//comment end of file
//////////////////////



