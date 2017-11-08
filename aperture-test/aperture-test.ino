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

aperture_fullopen();
//focus_infinity();
         
delay(1000);
Serial.print(" ");
//Serial.print(char(RetVal));         
Serial.print(" OPEN APERTURE FULLY");


}

if(ByteReceived == '0') // upon shutter release     
{         

aperture_closestep();
//send_signal(0xA0); // move focus back to nearest         
delay(1000);     
Serial.print(" CLOSE APERTURE ONE STOP");
} 


Serial.println();

} 

}
 

 void aperture_closestep()
 {

//close aperture
      RetVal = SPI.transfer(7);
      delay(10);
      
      RetVal = SPI.transfer(19);
      delay(10);

      RetVal = SPI.transfer(4);
//close aperture
 
 }

void aperture_fullopen()
 {

//open aperture
      RetVal = SPI.transfer(19);
      delay(30);
      
      RetVal = SPI.transfer(128);
      delay(10);

//open aperture
 
 }

void focus_infinity()
{
  
RetVal=SPI.transfer(0x0A); // move focus to infinity

}

//void send_signal(byte signal) // SPI command generator 
//{     
//unsigned int i;     
//for(i = 0; i < 16; i++)     
//{         

//digitalWrite(Clock_Pin, i & 1);         

//if(i % 2 == 0)         
//{             
//digitalWrite(Cam2Lens_Pin, (signal >> (i / 2)) & 1);
//}    
//} 
//} 
