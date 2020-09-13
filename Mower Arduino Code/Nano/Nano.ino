#include <SoftwareSerial.h>
//#define DEBUG    //uncomment to activate
//#define TEST     //uncomment to activate

SoftwareSerial mySerial(10, 11);  // RX, TX

//Amp Sensor Variables.
int RawValueAmp= 0;
int RawValueVolt = 0;
int RawWheelAmp = 0;
int Raining = 5;
int Charging = 0;


// Transmission of the raw sensor values to the Arduino MEGA
float AmpsTX; 
float VoltsTX;
float WheelAmpsTX;
bool  Test;
int   Wheel_Blocked;
float AmpLimit = 1.0;

void setup(){ 
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println();
    Serial.println("debug mode on");
  #endif
 mySerial.begin(1200);


 
 
 pinMode(A1, INPUT);
 pinMode(A2, INPUT);
 pinMode(A3, INPUT);
 pinMode(A6, INPUT);


  #ifdef TEST
    RawValueAmp = 510;
    RawValueVolt = 500;
  #endif
}

void Calculate_Volt_Amp() {


// Calculate Amp Value from Charging Pins
 int mVperAmp = 185;
 int ACSoffset = 2500; 
 double VoltageAmp = 0;
 double Amps = 0;
 VoltageAmp = (RawValueAmp / 1024.0) * 5000; // Gets you mV
 AmpsTX =  ((VoltageAmp - ACSoffset) / mVperAmp);

 VoltageAmp = (RawWheelAmp / 1024.0) * 5000; // Gets you mV
 WheelAmpsTX =  ((VoltageAmp - ACSoffset) / mVperAmp);


// Calculate Voltage Sensor Value from Battery
 
 float vout = 0.0;
 float R1 = 30000;      // 30000 Mower 2    Mower 1 30000
 float R2 = 7500;       // 7300 Mower 2     Mower 1 7500
 vout = (RawValueVolt * 5.0) / 1024.0; // see text
 VoltsTX = vout / (R2/(R1+R2)); 

}

 
void TX_Volts()  {  
  mySerial.print(VoltsTX);
  mySerial.println("\j");
  Serial.print("VoltsTX = ");
  Serial.print(VoltsTX);
  Serial.print("|");
}

void TX_Raw_Value_Volts()  {  
  mySerial.print(RawValueVolt);
  mySerial.println("\j");
}

void TX_Charge()  {
  mySerial.print(Charging);
  mySerial.println("\q");
}

void TX_Raw_Value_Charge()  {
  mySerial.print(RawValueAmp);
  mySerial.println("\q");
}

void TX_Raining()  {
  mySerial.print(Raining);
  mySerial.println("\w");
}

void TX_Wheel_Blocked()  {
  mySerial.print(RawWheelAmp);
  //mySerial.print(Wheel_Blocked);
  mySerial.println("\s");
}

void loop(){

 //Read Amp and Volt Raw Number Sensor Values 
  #ifdef TEST
    RawValueAmp = RawValueAmp + 1;
    if (RawValueAmp > 550) RawValueAmp = 500;
    RawValueVolt = RawValueVolt + 1;
    if (RawValueVolt > 550) RawValueVolt = 500;
  #else
    RawValueAmp = analogRead(A1);
    RawValueVolt = analogRead(A2);
    Raining = analogRead(A3);
    RawWheelAmp = analogRead(A6);
  #endif
 #ifdef DEBUG
 Serial.print("VoltsTX Raw = ");
 Serial.print(RawValueVolt);
 Serial.print("|");
 Serial.print("AmpsTX Raw = ");
 Serial.print (RawValueAmp);
 Serial.print("|");
 Serial.print("Wheel Raw = ");
 Serial.print (RawWheelAmp);
 Serial.print("|");
#endif
 Calculate_Volt_Amp();
 #ifdef DEBUG
 Serial.print("VoltsTX = ");
 Serial.print(VoltsTX);
 Serial.print("|");
#endif
 if (AmpsTX < 0.4) Charging = 0;
 if (AmpsTX > 0.4) Charging = 4;
  #ifdef DEBUG
 Serial.print("Charging = ");  
 Serial.print(Charging);
 Serial.print("|");

 Serial.print("AmpsTX = ");
 Serial.print (AmpsTX);
 Serial.print("|");

 Serial.print("Rain Sensor Raw = ");
 Serial.print (Raining);
 Serial.print("|");
#endif
if (Raining < 100) Raining = 0;
if (Raining >= 100) Raining = 1;

#ifdef DEBUG
Serial.print( "Raining = ");
Serial.print(Raining);
Serial.print("|");

Serial.print( "Wheel Amps = ");
Serial.print(WheelAmpsTX);
Serial.print("|");
#endif
if (WheelAmpsTX > AmpLimit) {
  #ifdef DEBUG
  Serial.println("!! Wheel Blocked !!");
  #endif
  Wheel_Blocked = 4;
  }
if (WheelAmpsTX <= AmpLimit) {
  Wheel_Blocked = 0;
  }

 #ifdef DEBUG
 Serial.println("");
 #endif
 
 //TX_Charge();
 TX_Raw_Value_Charge(); 
 delay(5); 
 //TX_Volts();
 TX_Raw_Value_Volts(); 
 delay(5);

 TX_Raining();
 delay(5);

 TX_Wheel_Blocked();

    
} 
