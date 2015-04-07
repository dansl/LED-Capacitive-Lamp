#include <CapacitiveSensor.h>
#include <math.h>

/*
 * Dan Long 2014
 */

///// LED STUFF /////
const int LEDRed = 6; //Make sure the Pin is set to a PWM Pin
const int LEDBlue = 9;
const int LEDGreen = 10;
const int LEDWhite = 11;

const int MaxLEDBrightness = 255;

int LEDBrightnessAndDirection[4][2] = {
  {0,1}, //Red
  {0,1}, //Green
  {0,1}, //Blue
  {0,1}  //white
};

int LEDWhiteBrightnessSettings[5] = {0,64,128,192,255};
int CurWhiteSetting = 0;

int CurColor = 0; 
int PrevColor = -1;
    
int FadeSpeed = 10; //Speed that the light will fade in miliseconds
boolean RainbowMode = true;
///// END/////

///// CAPACITIVE STUFF/////
CapacitiveSensor   Main_CS = CapacitiveSensor(13,12);        // 10M resistor between pins 13 & 12, pin 12 is sensor pin

int TapHoldCounter[2]; //Stores the value of the capacitive connection, and if we are testing for a tap or not.
int TapDelay = 40; //amount of loops to wait till we determin if it was a hold.

boolean DblTapCounterStart = false;
int DblTapTime = 1000 / FadeSpeed; //Divide by fade speed, because that is whats delaying the loop
int DblTapCounter = 0;

boolean WaitForStopHold = false;
///// END /////


boolean LampPower = true;

void setup() {
  
   Serial.begin(9600); //Run Serial on 9600
   
   //Setup LEDs on proper PINs
   pinMode(LEDRed, OUTPUT);
   pinMode(LEDBlue, OUTPUT);
   pinMode(LEDGreen, OUTPUT);
   pinMode(LEDWhite, OUTPUT);
}

void loop() {
  //For testing the performance of our program
  long start = millis();
  
  long MainCap =  Main_CS.capacitiveSensor(30); //Inialize Capacitive Sensor
  
  int TouchMode = checkTapHold(MainCap); //Detect Tap vs Hold
  
  ///// Double Tap checker //////
  if(!WaitForStopHold){
    if(TouchMode == 1){
      Serial.println("Tapped");
      RainbowMode = !RainbowMode;
    }else if(TouchMode == 2){
      Serial.println("Hold");
      LampPower = !LampPower;
      WaitForStopHold = true;
    }
  
    if(DblTapCounterStart){
      DblTapCounter++;
      
      if(TouchMode == 1){
        Serial.println("Double Tapped");
        CurWhiteSetting++;
        CurWhiteSetting %= 5;
        DblTapCounterStart = false;
        DblTapCounter = 0;
        TouchMode = 0;
      }
      
      if(DblTapCounter > DblTapTime){
        DblTapCounterStart = false;
        DblTapCounter = 0;
      }
    }
    
    if(TouchMode == 1){
      DblTapCounterStart = true;
    }
  }else{
    //This is so if they turn off the lamp, we want to wait till they let go before testing for taps again
    if(TouchMode == 0){
      WaitForStopHold = false;
    }
  }
  //////// END ///////
   
  if(RainbowMode){
    ///// CYCLE THROUGH ALL COLORS /////
    LEDBrightnessAndDirection[CurColor][1] = SetLEDDirection(LEDBrightnessAndDirection[CurColor][0], LEDBrightnessAndDirection[CurColor][1]);
    LEDBrightnessAndDirection[CurColor][0] += LEDBrightnessAndDirection[CurColor][1];
    
    if(LEDBrightnessAndDirection[CurColor][0] >= MaxLEDBrightness){
      LEDBrightnessAndDirection[CurColor][0] = 255;
      PrevColor = CurColor;
      CurColor++;
      CurColor %= 3;
    }
    
    if(PrevColor != -1){
      LEDBrightnessAndDirection[PrevColor][1] = SetLEDDirection(LEDBrightnessAndDirection[PrevColor][0], LEDBrightnessAndDirection[PrevColor][1]);
      LEDBrightnessAndDirection[PrevColor][0] += LEDBrightnessAndDirection[PrevColor][1];
    } 
  }
  //////// END ////////
  
  if(LampPower == true){
    analogWrite(LEDRed, LEDBrightnessAndDirection[0][0]);
    analogWrite(LEDGreen, LEDBrightnessAndDirection[1][0]);
    analogWrite(LEDBlue, LEDBrightnessAndDirection[2][0]);
    analogWrite(LEDWhite, LEDWhiteBrightnessSettings[CurWhiteSetting]);
  }else{
    analogWrite(LEDRed, 0);
    analogWrite(LEDGreen, 0);
    analogWrite(LEDBlue, 0);
    analogWrite(LEDWhite, 0);
  }
  
  //Serial.println(millis() - start);// check on performance in milliseconds 
  
  delay(FadeSpeed);
}

int SetLEDDirection(int CurBrightness, int CurDirection){
  CurDirection = CurBrightness >= MaxLEDBrightness ? -1 : CurDirection;
  CurDirection = CurBrightness <= 0 ? 1 : CurDirection;
  
  return CurDirection;
}

//pass the capacitive info, and a slot to save the data into. Returns 1 if tapped, 2 if held, 0 if nothing
int checkTapHold(int capTest){
  int value = 0;
  
  if(TapHoldCounter[0] > 0){
    TapHoldCounter[1] = 1;
    if(TapHoldCounter[0] > TapDelay){
      TapHoldCounter[1] = 0;
      value = 2; //Holding
    }
  }
  
  if(capTest > 0){
    TapHoldCounter[0]++;
  }else{
    TapHoldCounter[0] = 0;
  }
  
  if(TapHoldCounter[0] == 0 && TapHoldCounter[1] == 1){ 
    TapHoldCounter[1] = 0;
    value = 1;//Successful Tap
  }
  
  return value;
}
