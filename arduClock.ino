//Arduino based digital clock done using RTC module and OLED display by BD
//Time is in PST(Pacific Standard Time)
//Digital clock also displays current Date
//Necessary Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define SCREEN_WIDTH 128        //constant for screen width
#define SCREEN_HEIGHT 64        //constant for screen height
#define SCREEN_I2C_ADDRESS 0x3C //constant for screen address
#define OLED_RESET_PIN -1       //constant for reset button

Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN); //creates object for display functionality while passing in necessary attributes
RTC_DS3231 realTimeClock; //creates object for rtc(real time clock) functionality

int year, //integer variables to hold necessary information for clock
      month,
      day,
      hour,
      minutes,
      seconds,
      convertedHour;

String AMorPM; //string variable which holds AM or PM


void setup() {

  screen.begin(SSD1306_SWITCHCAPVCC,SCREEN_I2C_ADDRESS); //intializes screen
  realTimeClock.begin(); //initializes the real time clock module
  
  //realTimeClock.adjust(DateTime(2025,4,27,0,0,0)); //sets time of module to March 4,2025 and 12:00:00 AM [setting time is based on 24 hours system] (upload once then comment out so RTC module keeps track of time)
} 

void loop() {

  DateTime information = realTimeClock.now(); //retreives current time from rtc module and stores it into information object

  //retrieves and stores necessary imformation from information object using built in functions from RTC library
  year = information.year();
  month = information.month();
  day = information.day();
  hour = information.hour();
  minutes = information.minute();
  seconds = information.second();


  if(hour >= 12){ //if the current hour (based on 24 hour system is >=12) set the clock to PM
    AMorPM = "PM";

    if (hour > 12){ //converts the hour from the 24 hour system to a 12 the 12 hour system and stores it into variable
      convertedHour = (hour - 12);
    }

    else if(hour == 12){ //special case when the clock hits the 12'th hour meaning it's 12pm
      convertedHour = 12;
    }

  }

  else{ //if the current hour (based on 24 hour system) is < 12 set the clock to AM
    AMorPM = "AM";

    if (hour == 0){ //converts the 24 hour system of 0:00:00 to 12 hour system (12:00 AM)
      convertedHour = 12;
    }

    else{ //if given hour is in between 0 and 12 just leave it as is
      convertedHour = hour;
    }

  }

  //Screen functions to display information
  //Top Border of Screen
  screen.clearDisplay();                //clears display so it refereshes each time it loops
  screen.setTextSize(1);                //sets text size
  screen.setTextColor(WHITE);           //sets text color
  screen.setCursor(0,0);                //sets cursors postion on display to start inserting text
  screen.print("===================="); 

  //Time Part of Screen
  screen.setCursor(0,15);               
  screen.setTextSize(2);                
  screen.print(zeroAdder(convertedHour));        //prints information on screen while using zero adder function to add a zero in front if needed
  screen.print(":");
  screen.print(zeroAdder(minutes));     
  screen.print(":");
  screen.print(zeroAdder(seconds));     

  //Pm-Am part of screen
  screen.setCursor(95,19);              
  screen.setTextSize(1);                
  screen.print("[");
  screen.print(AMorPM);
  screen.print("]");

  //Date Part of Screen
  screen.setCursor(0,37);               
  screen.setTextSize(2);               
  screen.print(zeroAdder(month));       
  screen.print("/");
  screen.print(zeroAdder(day));         
  screen.print("/");
  screen.print(year);   

  //Bottom Border of Screen
  screen.setCursor(0,59);               
  screen.setTextSize(1);                
  screen.print("====================");

  screen.display();//displays text on screen
}


//function that adds a 0 infront of a number if less than 10 (purely for eligibilty and neatness)
int zeroAdder(int x){

  if (x < 10){ //if given number is < 10 it prints a zero on the same line as it
    screen.print("0");
  }
  return(x); //returns x with the 0 now in front

}
