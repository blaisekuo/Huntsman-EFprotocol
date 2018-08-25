#include <SPI.h>

const int LogicVDD_Pin = 10;
const int Cam2Lens_Pin = 11;
const int Clock_Pin = 13; 
 
int ByteReceived;

int RetVal;

int x,y,offset;

int stepsize = 500;
 
void setup() // initialization 
{     

delay(300);
  CLKPR = B10000000;
  CLKPR = B00000001;
  
  Serial.begin(38400);  
  Serial.println("--- Start Serial Monitor SEND_RCVE ---");
    Serial.println(" Type in Box above, . ");
  Serial.println("(Decimal)(Hex)(Character)");  
  Serial.println(); 

pinMode(LogicVDD_Pin, OUTPUT);     
digitalWrite(LogicVDD_Pin, HIGH); 
 
pinMode(Cam2Lens_Pin, OUTPUT); 
 
pinMode(Clock_Pin, OUTPUT);     
digitalWrite(Clock_Pin, HIGH); 

//SPI.begin();
//SPI.setBitOrder(MSBFRIST);
//SPI.setClockDivider(SPI_CLOCK_DIV128);
//SPI.setDataMode(SPI_MODE3);
SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV128,MSBFIRST,SPI_MODE3));
        
InitLens();

}

void InitLens()
{
  SPI.transfer(0x0A);
  delay(50);
  SPI.transfer(0x00);
  delay(50);
  SPI.transfer(0x0A);
  delay(50);
  SPI.transfer(0x00);
  delay(50);
}

void loop() {     

  if (Serial.available() > 0)
  {
    ByteReceived = Serial.read();
    Serial.print("command sent: ");
    Serial.print(char(ByteReceived));
    Serial.println();   
    //Serial.print("        ");      
    //Serial.print(ByteReceived, HEX);
    //Serial.print("       ");     
    //Serial.print(char(ByteReceived));

    
if(ByteReceived == 'I') // upon shutter release     
{         

      InitLens();
      SPI.transfer(0x06);
      Serial.print(" FOCUS TO INFINITY");
      delay(50);
}

if(ByteReceived == 'Z') // upon shutter release     
{         

      InitLens();
      SPI.transfer(0x05);
      Serial.print(" FOCUS TO ZERO");
      delay(50);
} 

if(ByteReceived == '0') // upon shutter release     
{         

InitLens();

      offset = stepsize;
      x = highByte(offset);
      y = lowByte(offset);

      SPI.transfer(68);
      delay(50);
      SPI.transfer(x);
      delay(50);
      SPI.transfer(y);
      delay(50);     
      SPI.transfer(0);     
      Serial.print(" MOVE LENS +50");
      delay(50);      

}

if(ByteReceived == '1') // upon shutter release     
{         

InitLens();

      offset = stepsize * (-1);
      x = highByte(offset);
      y = lowByte(offset);

      SPI.transfer(68);
      delay(50);
      SPI.transfer(x);
      delay(50);
      SPI.transfer(y);
      delay(50);     
      SPI.transfer(0);     
      Serial.print(" MOVE LENS -50");
      delay(50);      

}

Serial.println();

} 

}
