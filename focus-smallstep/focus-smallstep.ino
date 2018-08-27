#include <SPI.h>

const int LogicVDD_Pin = 10;
const int Cam2Lens_Pin = 11;
const int Clock_Pin = 13; 
 
int ByteReceived;

int RetVal;

int x,y,offset;

int stepsize = 50;
 
void setup() // initialization 
{     

delay(300);
//arduino clock is 16 mhz
//following lines half the clock speed to 8 Mhz
//SPI_CLOCK further down, divides it more to the 62 to 500 khz range needed
//for communicating with cannon lenses
//for 500 khz, comment out the CLKPR lines and change to SPI_CLOCK_DIV32
//  CLKPR = B10000000;
//  CLKPR = B00000001;
  
  Serial.begin(115200);  
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
// for (16/2)/128 - enable the two CLKPR lines 
//SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV128,MSBFIRST,SPI_MODE3));
// for 16/32, resulting clock speed of 500 khZ
SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32,MSBFIRST,SPI_MODE3));
        

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

  //extra unblocks
  //SPI.transfer(0x09);
  //delay(50);
  //SPI.transfer(0x00);
  //delay(50);
  //SPI.transfer(0x0E);
  //delay(50);
  //SPI.transfer(0x00);
  //delay(50);
  //SPI.transfer(0x0F);
  //delay(50);
  //SPI.transfer(0x00);
  //delay(50);
  //SPI.transfer(0x0A);
  //delay(50);
  //SPI.transfer(0x00);
  //delay(50);

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
      // for stm?
      //SPI.transfer(0x0C);
      //delay(50);
      SPI.transfer(0x06);
      Serial.print(" FOCUS TO INFINITY");
      delay(50);
}

if(ByteReceived == 'Z') // upon shutter release     
{         

      InitLens();
      // for stm?
      //SPI.transfer(0x0C);
      //delay(50);
      SPI.transfer(0x05);
      Serial.print(" FOCUS TO ZERO");
      delay(50);
} 

if(ByteReceived == '0') // small step on motor     
{         

InitLens();

      offset = stepsize;
      x = highByte(offset);
      y = lowByte(offset);

      SPI.transfer(0x44);
      delay(50);
      SPI.transfer(x);
      delay(50);
      SPI.transfer(y);
      delay(50);     
      SPI.transfer(0);     
      Serial.print(" MOVE LENS ");
      Serial.print(offset);
      delay(50);      

}

if(ByteReceived == '1') //small step on motor     
{         

InitLens();

      offset = stepsize * (-1);
      x = highByte(offset);
      y = lowByte(offset);

      SPI.transfer(0x44);
      delay(50);
      SPI.transfer(x);
      delay(50);
      SPI.transfer(y);
      delay(50);     
      SPI.transfer(0);     
      Serial.print(" MOVE LENS ");
      Serial.print(offset);
      delay(50);      

}

if(ByteReceived == 'A') // upon shutter release     
{         

InitLens();

      SPI.transfer(0x07);
      delay(50);
      SPI.transfer(0x13);
      delay(50);
      SPI.transfer(0x05);
      delay(50);     
      SPI.transfer(0x08);     
      delay(50);     
      SPI.transfer(0x00);     
      Serial.print(" APERTURE 5");
      delay(50);      

}

Serial.println();

} 

}
