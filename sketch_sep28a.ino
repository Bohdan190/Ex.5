#include <Wire.h> 
#include <Adafruit_Sensor.h> 
#include <Adafruit_BME280.h> 
#include <SPI.h> 
 
#define SEALEVELPRESSURE_HPA (1013.25) 
 
Adafruit_BME280 bme;  
 
 
 
const byte button = 34; 
volatile bool state = LOW; 
 
const int led1=23;  
const int led2=22;  
const int led3=21;  
const int led4=4;  
 
 
void printValues()  
{ 
  Serial.print("Temperature = "); 
  int temp=bme.readTemperature(); 
  Serial.print(temp); 
  Serial.println(" *C"); 
 
  Serial.print("Humidity = "); 
  Serial.print(bme.readHumidity()); 
  Serial.println(" %"); 
   
  Serial.print("Pressure = "); 
  Serial.print(bme.readPressure() / 100.0F); 
  Serial.println(" hPa"); 
 
  Serial.println(); 
} 
 
void tempSwitch() 
{  
  int tempState = map(bme.readTemperature(), 24, 32, 0, 2);
  
        
    switch (tempState) 
    { 
 
      case 0: 
 
        Serial.println("Cold"); 
        digitalWrite(led1, HIGH);  
        digitalWrite(led4, LOW); 
 
        break; 
 
      case 1: 
 
        Serial.println("Medium"); 
        digitalWrite(led1, LOW);  
        digitalWrite(led4, LOW); 
        printValues(); 
         
        break;         
 
      case 2: 
 
        Serial.println("Hot"); 
        digitalWrite(led1, HIGH);    
        digitalWrite(led4, HIGH);  
 
        break; 
         
      default: 
      {} 
     
    } 
} 
 
void ISR_button_pressed(void)  
{ 
    if (!state) //set flag to low if flag is true 
    { 
        state = true; //set flag 
        detachInterrupt( digitalPinToInterrupt(button) ); 
         
    } 
} 
 
 
 
void setup()  
{ 
 
  Serial.begin(115200); 
   
  Serial.println(F("BME280 test")); 
 
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT); 
  pinMode(led3, OUTPUT); 
  pinMode(led4, OUTPUT); 
   
  pinMode( button, INPUT_PULLUP ); 
  attachInterrupt(digitalPinToInterrupt(button), ISR_button_pressed, FALLING); 
   
  bool status; 
 
  status = bme.begin(0x76);   
  if (!status)  
  { 
    Serial.println("Could not find a valid BME280 sensor, check wiring!"); 
    while (1); 
  } 
 
} 
 
 
void loop()  
{ 
    if (state) //if an interrup has occured 
    { 
      tempSwitch(); 
      delay(500); 
         
        if (digitalRead(button)) //if button is released let ISR set flag. 
        { 
 
            attachInterrupt(digitalPinToInterrupt(button), ISR_button_pressed, FALLING ); 
            state = false; //reset interrupt flag 
             
        } 
    } 
}