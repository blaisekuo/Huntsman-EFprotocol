#include <SPI.h>
#include <EEPROM.h>
#include <math.h>

int RetVal;
int focuserPosition, targetPos, apValue, offset, apAddr, x, y;    
//float Temp, Vpower, Vadc, Rt, Rm, Ro, betta;
String targetStr, apStr, gStr;
boolean IsMoving, IsFirstConnect;
char inStr[6];

void InitLens()
{
  delay(100);
  RetVal = SPI.transfer(0x0A);
  delay(100);
  //SPI.transfer(0x00);
  //delay(100);
  //SPI.transfer(0x0A);
  //delay(100);
  //SPI.transfer(0x00);
  //delay(100);
}

void setup()
{

delay(300);
  CLKPR = B10000000;
  CLKPR = B00000001;
  
  apAddr = 0;
  focuserPosition = 5000;
  IsMoving = false;
  IsFirstConnect = true;
  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  //digitalWrite(13, LOW);
  //pinMode(2, OUTPUT);
  //digitalWrite(2, HIGH); //pull-up pin for temp sensor
  
  //Temperature init
  //Rm = 7400; //Ohm
  //Ro = 3000; //Ohm
  //Vpower = 4.73; //Volts
  //betta = 3988.0;
  
  Serial.begin(38400);
  
//  SPI.begin();
//  SPI.setBitOrder(MSBFIRST);
//  SPI.setClockDivider(SPI_CLOCK_DIV128); 
//  SPI.setDataMode(SPI_MODE3);  
SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV128,MSBFIRST,SPI_MODE3));
  //digitalWrite(12, HIGH);
  
  InitLens();
}

void loop()
{
  if (Serial.available() > 0) 
  {         
    Serial.readBytesUntil('#', inStr, 6);
    
    // Get Position
    if (inStr[0] == 'P')
    {
      Serial.print(focuserPosition);
      Serial.print("#");
    }
    
    // Get Temperature
    //if (inStr[0] == 'T')
    //{
    //  Vadc = (analogRead(A7) / 1023.0) * 5;
    //  Rt = (Rm * Vadc) / (Vpower - Vadc);
    //  Temp = (betta / (log(Rt / Ro) + (betta / 298.0))) - 273.0;
      
    //  Serial.print(int(Temp * 100));
    //  Serial.print("#");
    //}
    
    // Move
    if (inStr[0] == 'M')
    {
      targetStr = inStr;
      targetPos = (targetStr.substring(1, 5)).toInt();
      offset = targetPos - focuserPosition;

      x = highByte(offset);
      y = lowByte(offset); 

      InitLens();
      
      IsMoving = true;
      RetVal = SPI.transfer(68);
      delay(30);
      RetVal = SPI.transfer(x);
      delay(30);
      RetVal = SPI.transfer(y);
      delay(30);
      RetVal = SPI.transfer(0);
      delay(100);
      IsMoving = false;
      
      focuserPosition = targetPos;
    }
    
    // IsMoving
    if (inStr[0] == 'I')
    {
      if (IsMoving == true)
      {
        Serial.print("true#");
      }
      else
      {
        Serial.print("false#");
      }
    }
    
    // Set Aperture
    if (inStr[0] == 'A')
    {
      apStr = inStr;
      apValue = (apStr.substring(1, 3)).toInt();
      
      if (apValue != EEPROM.read(apAddr))
      { 
        InitLens();
        
        SPI.transfer(0x07);
        delay(10);
        SPI.transfer(0x13);
        delay(10);
        SPI.transfer((apValue - EEPROM.read(apAddr)) * 3);
        delay(100);
        SPI.transfer(0x08);
        delay(10);
        SPI.transfer(0x00);
        delay(10);
        
        EEPROM.write(apAddr, apValue);
      }
    }
  }  
}
