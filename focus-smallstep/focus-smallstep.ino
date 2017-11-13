#include <SPI.h>

//const int HotShoe_Pin = 8;
//const int HotShoe_Gnd = 9;
const int LogicVDD_Pin = 10;
const int Cam2Lens_Pin = 11;
const int Clock_Pin = 13; 
 
int ByteReceived;

int RetVal;
 
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
        

 
delay(100);     
RetVal=SPI.transfer(10);
//send_signal(0x4F);    

delay(100);     

//send_signal(0x50);
} 


void loop() {     

  if (Serial.available() > 0)
  {
    ByteReceived = Serial.read();
    Serial.print(ByteReceived);   
    Serial.print("        ");      
    Serial.print(ByteReceived, HEX);
    Serial.print("       ");     
    Serial.print(char(ByteReceived));

    
if(ByteReceived == '1') // upon shutter release     
{         

focus_2infinity();         

}

if(ByteReceived == '0') // upon shutter release     
{         

focus_2near();         

} 


Serial.println();

} 

}
 

 void focus_2infinity()
 {

//focus to infinity
  //    RetVal = SPI.transfer(7);
  //    delay(10);
      RetVal = SPI.transfer(6);
   //   delay(10);
      
    //  RetVal = SPI.transfer(8);
      
      delay(1000);
Serial.print(" ");        
Serial.print(" FOCUS TO INFINITY");
 
 }

void focus_2near()
 {

//focus to near

   //   RetVal = SPI.transfer(7);
   //   delay(10);
      RetVal = SPI.transfer(6);
   //   delay(10);
      
    //  RetVal = SPI.transfer(-8);

      delay(1000);
Serial.print(" ");     
Serial.print(" FOCUS TO NEAR");
 
 }
