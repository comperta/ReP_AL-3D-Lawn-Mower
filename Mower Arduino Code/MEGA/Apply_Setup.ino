#ifdef DEBUG
  void Print_Mower_Status() {
    //Serial.print("Mower Stat....");
    if (Mower_Docked == 1) Serial.print(F("Docked:1|"));
    if (Mower_Parked == 1) Serial.print(F("Parked:1|"));
    if (Mower_Running == 1) Serial.print(F("Running:1|"));
    if (Manuel_Mode == 1) Serial.print(F("Manuel Mode:1|"));
    if (Mower_Parked_Low_Batt == 1) Serial.print(F("Park_Low_Batt:1|"));
    if (Mower_Error == 1) Serial.print(F("Mower Error:1|"));
    
  }
#endif

void Setup_Tilt_Tip_Safety() {
  if (Angle_Sensor_Enabled == 1)     pinMode(Tilt_Angle, INPUT);//define Data input pin input pin
  if (Tip_Over_Sensor_Enabled == 1)  pinMode(Tilt_Orientation, INPUT);//define Data input pin input
  }

void Prepare_Mower_from_Settings() {

  if (Use_Charging_Station == 1) {
     Mower_Docked  = 1;
     Mower_Parked  = 0;
     Mower_Running = 0;
     }


if (Use_Charging_Station == 0) {
    Mower_Docked  = 0;
    Mower_Parked  = 1;
    Mower_Running = 0;
    }
 }


void Setup_QMC5883L_Compass() {
    if (Compass_Activate == 1) {
        Serial.println("Setting up QMC5883L Compass - Mode 3");
        Compass_Detected = 4;
        Wire.begin();
        compass2.init();
        compass2.setSamplingRate(50);
        }
    } 

void Setup_Manuel_QMC5883_Compass_2() {
    if (Compass_Activate == 1) {
        Wire.begin();
        //Initialize Serial and I2C communications
        Serial.println("Setting up QMC5883 Compass - Mode 2");
    
        //Put the QMC5883L IC into the correct operating mode:
        Wire.beginTransmission(QMC5883_ADDRESS); //Open communication with QMC5883L.    
        Wire.write(0x09); //Select mode register.         (0x0B)? (0x09)
        Wire.write(0x1D); //Continuous measurement mode.  (0x0D)? (0x1D)

        Wire.endTransmission();
        Compass_Detected = 3;
        }
    }


void Setup_Manuel_QMC5883_Compass() {
  //Enter Manual Code here
  Serial.println(F("No Manuel Code Written - Use DF Robot and GY282 Compass"));

}

byte i2c_write(byte device, byte addr, byte value) {
  byte n;
  Wire.beginTransmission(device);
  n = Wire.write(addr);
  if (n != 1) Serial.println("Wire.write(addr) failed");
  n = Wire.write(value);
  if (n != 1) Serial.println("Wire.write(value) failed");
  n = Wire.endTransmission(true);
  if (n != 0)     {
    char msg[100];
    sprintf(msg, "sendHMC5883 i2c[%02x:%02x]=%02x returns %d", device, addr, value,n);
    Serial.println(msg);
  }
  return n;
}

void Setup_DFRobot_QMC5883_HMC5883L_Compass() {
 
  if (Compass_Activate == 1) {

        /*Setup Compass
        *************************************************************************/
      
        int Compass_Attempt = 0;
        int Compass_Found = 0;
      
        //HMC5883 Compass
        while ((!compass.begin()) && (Compass_Attempt <= 5)){
          #ifdef DEBUG
            Serial.println(F("No HMC or QMC5883 sensor found, check wiring!"));
          #endif
            delay(500);
            Compass_Attempt = Compass_Attempt + 1;
            }
            if (compass.isHMC()) {
              #ifdef DEBUG
                Serial.println(F("Initialize DF Robot HMC5883 Compass"));
              #endif
                Compass_Detected = 1;                        // HMC5883
                       //compass.setRange(HMC5883L_RANGE_1_3GA);
                        //compass.setMeasurementMode(HMC5883L_CONTINOUS);
                        //compass.setDataRate(HMC5883L_DATARATE_15HZ);
                        //compass.setSamples(HMC5883L_SAMPLES_8);
                        //Compass_Found = 1;
                }
            
             //QMC5883 Compass
              else if (compass.isQMC()) {
                #ifdef DEBUG
                  Serial.println(F("Initialising DF Robot QMC5883 Compass"));
                #endif
                Compass_Detected = 2;                        // HMC5883
                  compass.setRange(QMC5883_RANGE_2GA);
                  compass.setMeasurementMode(QMC5883_CONTINOUS); 
                  compass.setDataRate(QMC5883_DATARATE_50HZ);
                  compass.setSamples(QMC5883_SAMPLES_8);
                }
            
          
            // Escape the loop if no compass is found but compass is activated in the settings
            if ((Compass_Attempt > 5) && (Compass_Found == 0)) {
              #ifdef DEBUG
                Serial.println("No Valid Compass Found");
              #endif
              Compass_Activate = 0;
              #ifdef DEBUG
                Serial.println("Compass Deactivated");
              #endif
              delay(3000);
            }
          //Wire.endTransmission(); 
          }
        
      
      
        if (Compass_Activate == 0) {
            #ifdef DEBUG
              Serial.println(F("Compass Switched off - Select 1 in setup to switch on.")); 
            #endif
            }
       
      }


void Setup_Gyro() {

        if (GYRO_Enabled == 1) {
        #ifdef DEBUG
          Serial.println("GYRO GY-521 Activated");
        #endif
        Wire.begin();
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x6B);
        Wire.write(0);
        Wire.endTransmission(true);  
        }
    }


void Setup_Relays() {
  #ifdef DEBUG
    Serial.println(F("Setup Relays"));
  #endif
  pinMode(Relay_Motors, OUTPUT);
  delay(5);
  Turn_Off_Relay();
  delay(5);
  }

void Setup_Motor_Pins() {
  #ifdef DEBUG
    Serial.println(F("Setup Motor Pins"));
  #endif  
  pinMode(L_EN, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(RPWM, OUTPUT);
  }


void  Turn_On_Relay() {
  #ifdef DEBUG
   Serial.print(F("Relay:ON|"));
   #endif
   if (PCB == 0) digitalWrite(Relay_Motors, LOW);                         // Turn of the relay for the main battery power
   if (PCB == 1) digitalWrite(Relay_Motors, HIGH); 
   }

void  Turn_Off_Relay() {
   #ifdef DEBUG
   Serial.print(F("Relay:Off|"));
   #endif
   if (PCB == 0) (digitalWrite(Relay_Motors, HIGH));                         // Turn of the relay for the main battery power
   if (PCB == 1) (digitalWrite(Relay_Motors, LOW));    
   }

void Setup_Membrane_Buttons() {
  #ifdef DEBUG
  Serial.println(F("Setup Membrane Keys"));
  #endif
  pinMode(Start_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Plus_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Minus_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Stop_Key, INPUT_PULLUP);            // set pin as input  
  }

void Setup_Bumper_Bar() {
  pinMode(Bumper_Switch_Frnt_RH, INPUT_PULLUP); 
  pinMode(Bumper_Switch_Frnt_LH, INPUT_PULLUP); 
}

void Setup_ADCMan() {
  #ifdef DEBUG
  Serial.println(F("Setting up ADCMAN"));
  #endif

  if (Perimeter_Wire_Enabled == 1) {
      // Wire Sensor
      #ifdef DEBUG
      Serial.println(F("Setting up Wire Sensor"));
      #endif
      ADCMan.init();
      ADCMan.setCapture(pinPerimeterLeft, 1, 0);
      perimeter.setPins(pinPerimeterLeft, pinPerimeterRight);
      perimeter.useDifferentialPerimeterSignal = true; 
      perimeter.speedTest(); 
      }

  if (GPS_Enabled == 1) {
      // GPS Fence Sensor 
      #ifdef DEBUG
      Serial.println(F("Setting up GPS Fence Sensor"));
      #endif
      pinMode(GPS_Fence_Signal_Pin, INPUT);
      pinMode(GPS_Lock_Pin, INPUT);
      ADCMan.init();
      ADCMan.setCapture(GPS_Fence_Signal_Pin, 1, 1);
      ADCMan.setCapture(GPS_Lock_Pin, 1, 1);
      }

  ADCMan.run();
}

    
  
