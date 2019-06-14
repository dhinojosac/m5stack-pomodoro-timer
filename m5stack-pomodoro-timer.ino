        
/*
 * Project           : M5STACK CORE Pomodoro
 * Author            : Diego Hinojosa Cordova
 * Date created      : 12-Jun-2019
 *
*/
#include <M5Stack.h>

#define TFT_GREY 0x5AEB
#define LCD M5.Lcd
#define width 320
#define BUT_POS_A  1*(width/3)-60
#define BUT_POS_B  2*(width/3)-60
#define BUT_POS_C  3*(width/3)-70
#define TEXT_HEIGHT 16

uint32_t targetTime = 0;                    // for next 1 second timeout
uint8_t is_stop = false;
uint8_t is_start = false;
static uint8_t conv2d(const char* p);       // Forward declaration needed for IDE 1.6.x

//Pomodoro time
int8_t hh = 00, mm = 25, ss = 59; // Get H, M, S from compile time

byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;

void setup(void) {
  //Serial.begin(115200);
  M5.begin();
  // M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);
  
  M5.Lcd.setTextSize(2);
  LCD.setCursor(BUT_POS_A+ 8, 30);
  LCD.printf("Pomodoro Timer v0.1");
  M5.Lcd.setTextSize(1);
  LCD.setCursor(BUT_POS_A+70, 50);
  LCD.printf("dhinojosac 2019");
  
  M5.Lcd.setTextSize(2);
  LCD.setCursor(BUT_POS_A, 215);
  LCD.printf("Start");

  LCD.setCursor(BUT_POS_C-10, 215);
  LCD.printf("Reset");

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);


  
  targetTime = millis() + 100; 
}

void loop() {

  //Buttons
  if(M5.BtnA.wasPressed())
  {
    //Check state
    LCD.fillRect(BUT_POS_A, 215,320,TEXT_HEIGHT, TFT_BLACK);
    LCD.setTextColor(TFT_WHITE);
    LCD.setTextSize(2);
    LCD.setCursor(BUT_POS_A, 215);
    if(is_start==false)
    {
      LCD.printf("Stop");
      LCD.setCursor(BUT_POS_C-10, 215);
      LCD.printf("Reset");
      is_start = true;
    }else{
      LCD.printf("Start");
      LCD.setCursor(BUT_POS_C-10, 215);
      LCD.printf("Reset");
      is_start = false;
    }
  }

  if(M5.BtnC.wasPressed())
  {
    LCD.fillRect(BUT_POS_A, 215, 150,TEXT_HEIGHT, TFT_BLACK);
    LCD.setTextColor(TFT_WHITE);
    LCD.setTextSize(2);
    LCD.setCursor(BUT_POS_A, 215);
    LCD.printf("Start");
    ss=59;
    mm=25;
    is_start=false;
  }

  
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);

  if((ss==0) && (mm==0) &&  (hh==0))
  {
    M5.Speaker.tone(661, 200); 
    is_stop = true;
    mm=25;
    ss=59;
  }

  if(is_stop == true)
  {
    while(1);
  }
  
 
    if (targetTime < millis()) {
      // Set next update for 1 second later
      targetTime = millis() + 100;
  
      // Adjust the time values by adding 1 second
     if(is_start == true){
        ss--;              // Advance second
     }
     if (ss < 0) {    // Check for roll-over
        ss = 59;          
        omm = mm;        // Save last minute time for display update
        mm--;            // Advance minute
        if (mm < 0) {   // Check for roll-over
          mm = 59;
          hh--;          // Advance hour
          if (hh < 0) { // Check for 24hr roll-over (could roll-over on 13)
            hh = 23;      // 0 for 24 hour clock, set to 1 for 12 hour clock
          }
        }
      }
      
      
  
      // Update digital time
      int xpos = 0;
      int ypos = 85; // Top left corner ot clock text, about half way down
      int ysecs = ypos + 24;
  
      if (omm != mm) { // Redraw hours and minutes time every minute
        omm = mm;
        // Draw hours and minutes
        if (hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
        xpos += M5.Lcd.drawNumber(hh, xpos, ypos, 8);             // Draw hours
        xcolon = xpos; // Save colon coord for later to flash on/off later
        xpos += M5.Lcd.drawChar(':', xpos, ypos - 8, 8);
        if (mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
        xpos += M5.Lcd.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
        xsecs = xpos; // Sae seconds 'x' position for later display updates
      }
      if (oss != ss) { // Redraw seconds time every second
        oss = ss;
        xpos = xsecs;
  
        if (ss % 2) { // Flash the colons on/off
          M5.Lcd.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
          M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
          xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
          M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);    // Set colour back to yellow
        }
        else {
          M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
          xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
        }
  
        //Draw seconds
        if (ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ysecs, 6); // Add leading zero
        M5.Lcd.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
      }
    }
    
    M5.update();
}
