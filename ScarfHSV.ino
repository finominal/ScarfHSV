#include "FastLED.h"
#include <EEPROM.h>

byte eepromAddress = 0;

float movement = 0;
float movementFactor = 0.031;

float brightness = 120;
float size = 10;

float mapMin = -100;
float mapMax = 100;

const int DATAPIN = 12;
const int LEDCOUNT = 50;

CRGB leds[LEDCOUNT];
CRGB ledsBuffer[LEDCOUNT];

float r,g,b,shade;
int program;
 
void setup()
{
  Serial.begin(115200);
  
  FastLED.addLeds<WS2812B, DATAPIN, GRB>(leds, LEDCOUNT);
  
  Serial.println("*** RESET ***");
  
  movement = random(0,20)/0.7; //randomize start pattern
  GetProgram();
  
}

void loop()
{
  Plasma();
}

void GetProgram()
{
  byte storedProg = EEPROM.read(eepromAddress);
  Serial.print("Program Found: "); Serial.println(storedProg); 
  
  if(storedProg >= 6)
  {
    EEPROM.write(eepromAddress, 0);
  }
  else
  {
    storedProg++;
    EEPROM.write(eepromAddress, storedProg);
  }
  
  program = EEPROM.read(eepromAddress);
  Serial.print("Program Running: "); Serial.println(program); 
}

void Plasma()
{
 
 switch(program)
 {
   case 0:
    Rainbow();
    break;
  case 1:
    RainbowMorphing();
    break;
  case 2:
    RedBlue();
    break;
  case 3:
    RedGreen();
    break;
  case 4:
    BlueGreen();
    break;
  case 5:
    AquaCentred();
    break;
  case 6:
    RedMorphing();
    break;
 }
    //SHOW THE ANIMATION FRAME
    FastLED.show();
  
  
    /*
    Serial.print("Shade="); Serial.println(shade);
    Serial.print("MappedValue=") ; Serial.println(mapLed(shade));
    Serial.print("r="); Serial.print(leds[LEDCOUNT-1].r); Serial.print(" ");
    Serial.print("g="); Serial.print(leds[LEDCOUNT-1].g); Serial.print(" ");
    Serial.print("b="); Serial.print(leds[LEDCOUNT-1].b); Serial.print(" ");
    Serial.println();
    */
}

void Rainbow()
{
  for(int i = 0; i<LEDCOUNT; i++)
  {
    //memset(leds, 0, LEDCOUNT * 3);
  
    shade = SinVerticle(i,0,size)
            + SinRotating(i,0,size) 
            + SinCircle(i,0, size) 
            ;
            
    leds[i] = CHSV(mapLed(shade), 255, brightness);
    //leds[i].g += leds[i].r*abs(sin(movement));
    //leds[i].b += leds[i].r*abs(cos(movement/3));
    
    //leds[i].r = map( sin(shade*PI)*100, mapMin, mapMax, 0, brightness);
    //leds[i].g = 0; //map( sin(shade*PI+2*PI*sin(movement/23))*100, mapMin, mapMax, 0, brightness);
    //leds[i].b = map( sin(shade*PI+4*PI*sin(movement/20))*100, mapMin, mapMax, 0, brightness);  
  }
  movement+=movementFactor;
}

void RainbowMorphing()
{
  CRGB buffer;
  for(int i = 0; i<LEDCOUNT; i++)
  {
    //memset(leds, 0, LEDCOUNT * 3);
  
    shade = //SinVerticle(i,0,size)
            + SinRotating(i,0,size) 
            + SinCircle(i,0, size) 
            ;
            
    leds[i] = CHSV(mapLed(shade), 255, brightness);
    
    int mg = 86 * abs(sin(movement/2));
    int mb = 86 * abs(cos(movement/4));
    //Serial.print(mg);Serial.print(" ");Serial.println(mb);
    
    //shift the blue
    buffer  = CHSV(mapLed(shade)-mg, 255, brightness);
    leds[i].b=buffer.b;

    //shift the grenn
    buffer = CHSV(mapLed(shade)+mb, 255, brightness);
    leds[i].g = buffer.g;
 }

  movement+=movementFactor;
}

void RedGreen()
{
  int s = size *.7;
  for(int i = 0; i<LEDCOUNT; i++)
  {
    //memset(leds, 0, LEDCOUNT * 3);
  
    shade = SinVerticle(i,0,s)
            + SinRotating(i,0,s) 
            + SinCircle(i,0, s) 
            ;
    
    leds[i] = CHSV(mapLed(shade), 255, brightness);
    leds[i].b = 0;
    leds[i].g += leds[i].r*abs(sin(movement/5));
  
  }
  movement+=movementFactor;
}


void RedBlue()
{
  for(int i = 0; i<LEDCOUNT; i++)
  {
    shade = //SinVerticle(i,0,size)
            + SinRotating(i,0,size) 
            + SinCircle(i,0, size) 
            ;

    leds[i] = CHSV(mapLed(shade), 255, brightness);
    leds[i].g=0;
  }
  movement+=movementFactor;
}

void BlueGreen()
{
  int s = size *.7;
  for(int i = 0; i<LEDCOUNT; i++)
  {
    shade = //SinVerticle(i,0,size)
            //+ SinRotating(i,0,size) 
            + SinCircle(i,0, s) 
            ;
    
    leds[i] = CHSV(mapLed(shade), 255, brightness);
    leds[i].r=0;
   
    
   }
   movement+=movementFactor;
}

void RedMorphing()
{
  for(int i = 0; i<LEDCOUNT; i++)
  {
    shade = //SinVerticle(i,0,size)
            + SinRotating(i,0,size) 
            + SinCircle(i,0, size) 
            ;

      leds[i] = CHSV(mapLed(shade), 255, brightness);
      leds[i].g = 0;
      leds[i].b = leds[i].r*abs(sin(movement/2));
   }
   movement+=movementFactor;
}


void AquaCentred()
{
 
  for(int i = 0; i<LEDCOUNT; i++)
  {
  shade = //SinVerticle(i,0,size)
            //+ SinRotating(i,0,size) 
            + SinCircleCentred(i,0, size/2)
            ;
   leds[i] = CHSV(mapLed(shade), 255, brightness);
   leds[i].r = 0;
   leds[i].g = leds[i].b;
    
    }
    movement+=movementFactor/2;//bit faster5
}


float SinVerticle(float x, float y, float size)
{
  return sin(x / size + movement);
}
 
float SinRotating(float x, float y, float size)
{
  return sin( (x * sin(movement/2 ) + y * cos(movement/5)) /size ) ;
}
 
float SinCircle(float x, float y, float size)
{
  float cx = LEDCOUNT * sin(movement/7);
  float cy = LEDCOUNT * cos(movement/2);
  float dist = sqrt(sq(cy-y) + sq(cx-x));
  
  return sin((dist/size ) + (movement) );
}

float SinCircleCentred(float x, float y, float size)
{
  float cx = (LEDCOUNT/2)-.5;
  float cy = 0;
  float dist = sqrt(sq(cy-y) + sq(cx-x));
  
  return sin((dist/size ) + (movement) );
}

void SelfCorrectMapping()
{
    //self correct mapping values to use maximum led resolution
    if(shade*100 < mapMin) mapMin = shade*100;
    if(shade*100 > mapMax) mapMax = shade*100;
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    int inByte = Serial.read(); 
    Serial.print("DataRecieved: "); Serial.println(inByte);
    inByte -= 48;
    Serial.print("DataModified: "); Serial.println(inByte);
     if(inByte <=6)
     {
       program = inByte;
       Serial.print("Program Set as "); Serial.println(inByte);
     }
  } 
}
  
int mapLed(float s)
{
  return map(shade*100 ,mapMin, mapMax, 0, 255);
}




