// Script by Matt, June 2015
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 53
const int maxLEDs = 1056;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(maxLEDs, PIN, NEO_GRB + NEO_KHZ800);

int brightness[maxLEDs];
int LEDsArray[] = {24,25,26,27,38,40,40,40,40,37,36,35,35,34,34,34,34,34,35,35,36,37,40,40,40,40,38,27,26,25,24};
// int Rgb[1056][3];

int stripNum; // This is which strip the LED is on
int rowNum; //This is how far up the LED is
int LEDNum; // This is the LED ID

///SNAAAAAAAAAAAAAAAAAAAAAAKKKKKKKKEEEEEEEEEEEEE
int angle=0;
int snakesize=5;
int aTime=0;
int headx[100];
int heady[100];
int  applex=(round(random(31))+1)*10;
int  appley=(round(random(40))+1)*10;
boolean redo=true;
boolean stopgame=false;
char val; // Data received from the serial port
int ledPin = 13; // Set the pin to digital I/O 13
float snakeSpeed = 3.0;
void Move() {
    
     if (stopgame)
  {
    //do nothing because of game over (stop playing)
  }
  else
  {
     //draw stationary stuff
  aTime+=1;

  ShineOn(applex, appley, 0, true); //The apple

     if ((aTime % 2)==0)
  {
   
    LEDdisplay();
    travel();
    checkdead();
  }
  }
  }  

void travel()
{
  for(int i=snakesize;i>0;i--)
  {
    if (i!=1)
    {
      //shift all the coordinates back one array
      headx[i]=headx[i-1];
      heady[i]=heady[i-1];
    }
    else
    {
      //move the new spot for the head of the snake, which is
      //always at headx[1] and heady[1].
      switch(angle)
      {
        case 0:
        headx[1]+=10;
        break;
        case 90:
        heady[1]-=10;
        break;
        case 180:
        headx[1]-=10;
        break;
        case 270:
        heady[1]+=10;
        break;
      }
    }
  }
  
}
void LEDdisplay()
{
  //is the head of the snake eating the apple?
  if (headx[1]==applex && heady[1]==appley)
  {
    //grow and spawn the apple somewhere away from the snake
    //(currently some of the code below might not be working, but the game still works.)
    snakesize+=round(random(3)+1);
    redo=true;
    while(redo)
    {
      applex=(round(random(31))+1)*10;
      appley=(round(random(40))+1)*10;
      snakeSpeed -=0.1;
      for(int i=1;i<snakesize;i++)
      {
        
        if (applex==headx[i] && appley==heady[i])
        {
          redo=true;
        }
        else
        {
          redo=false;
          i=1000;
        }
      }
    }
  }
  
         ShineOn(headx[1], heady[1], 255, false); //The head of the snake
         ShineOn(headx[snakesize], heady[snakesize], 0, false); //The tail of the snake

    // You wont need to insert the exact position for arduino, just have the right ID
   

}

void checkdead()
{
  for(int i=2;i<=snakesize;i++)
  {
    //is the head of the snake occupying the same spot as any of the snake chunks?
    if (headx[1]==headx[i] && heady[1]==heady[i])
    {
      stopgame=true;
    }
    //is the head of the snake hitting the walls?
    if (headx[1]>=320)
    {
      headx[1] = 0;
    }
    if(heady[1]>=410)
    {
      heady[1]= 0;
    }
    if(headx[1]<0)
    {
      headx[1] = 320-10;
    }
    if(heady[1]< 0)
    {
      heady[1]= 410 - 10;
    }
    
 
  }
}
void restart()
{
  //by pressing shift, all of the main variables reset to their defaults.
  headx[1]=200;
  heady[1]=200;
for(int i = 0; i < maxLEDs; i++){
   brightness[i] = 0;
 }
  
  for(int i=2;i<1000;i++)
  {
    headx[i]=0;
    heady[i]=0;
  }
  stopgame=false;
  applex=(round(random(31))+1)*10;
  appley=(round(random(40))+1)*10;
  snakesize=5;
  aTime=0;
  angle=0;
  redo=true;
}
int sinecolor(float percent)
{
  float slime = (sin(radians((((millis() +(255*percent))/255))*360)))*255;
 // int slime=(int)(sin(radians((((millis() +(255*percent)) % 255)/255)*360)))*255;
 // int slime = 255;
 Serial.print(slime);
  return slime;
}


void setup()
{
  pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
  Serial.begin(9600);
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
 
 for(int i = 0; i < maxLEDs; i++){
   brightness[i] = 0;
 }
 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


// int birdNum = 1;

void loop()
{
  
 // for (int i=0; i<birdNum; i++) {
 //   Birdy = ( BirdObject ) Birds.get(i);
 keyPressed();
     Move();
     
  strip.show();
}

void ShineOn(int pX, int pY, int bright, boolean apple){
 
  ///// Make sure to get LED brightness before we go any further!
   stripNum = (int)(map(pX, 0, 320, 0, 31));
     rowNum = (int)(map(pY, 0, 410, 0, 40));


 for(int n = 0; n < stripNum; n++){
    LEDNum += LEDsArray[n];
  }
   LEDNum += rowNum;
   if(rowNum < LEDsArray[stripNum]){
    brightness[LEDNum] = bright;
  //  stripNumArr[LEDNum] = stripNum;
  //  rowNumArr[LEDNum] = rowNum;    
    // You wont need to insert the exact position for arduino, just have the right ID
  
   ///// Make sure to get LED brightness before we go any further!
  int r ;
  int b ;
  int g ;
  if(apple){
   r = 255;
   g = 0;
   b = 0;
  }
  else if(brightness[LEDNum] > 0){
//   r = sinecolor(0.5);
//   b = sinecolor(1);
//   g = sinecolor(1);
   
 r = 0;
 g = 255;
 b = 0;
   // r = 235;
   // g = 200;
   // b = 20;
  }
  else {
     g = 0;
  r = 0;
  b = 0;
  }
  // This would just be the ID number
  strip.setPixelColor(LEDNum, r, g, b);
 }
  else if(apple)
  {
    applex=(round(random(31))+1)*10;
      appley=(round(random(40))+1)*10;
  } 
  
  LEDNum = 0;
  
  
  }

//controls:
void keyPressed()
{
 if (Serial.available()) 
   { // If data is available to read,
     val = Serial.read(); // read it and store it in val
   }
   if (val == 'R' && angle!=180 && (headx[1]+10)!=headx[2]) 
   { 
     digitalWrite(ledPin, HIGH); // turn the LED on
      angle=0;
   } 
   if (val == 'L' && angle!=0 && (headx[1]-8)!=headx[2]) {
     digitalWrite(ledPin, LOW); // otherwise turn it off
     angle=180;
   }
    if (val == 'U' && angle!=90 && (heady[1]+10)!=heady[2]) 
   { 
     digitalWrite(ledPin, HIGH); // turn the LED on
     angle=270;
   } 
   if (val == 'D' && angle!=270 && (heady[1]-10)!=heady[2]) {
     digitalWrite(ledPin, LOW); // otherwise turn it off
     angle=90;
   }
   delay(10); // Wait 10 milliseconds for next reading
   
}
