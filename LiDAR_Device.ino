// Last Modified: 3-02-2022
// Last Uploaded to Device: 2-28-2022
#include <SoftwareSerial.h>   //header file of software serial port
#include <Wire.h>             //include wire library for i2c communication
SoftwareSerial Serial1(6, 7); //define software serial port name as Serial1 and define pin6 as RX & pin7 as TX
 
int dist;                     //actual distance measurements of LiDAR
int strength;                 //signal strength of LiDAR
int check;                    //save check value
int i;
int uart[9];                   //save data measured by LiDAR
const int HEADER = 0x59;      //frame header of data package

// Define pin connections & motor's steps per revolution
const int dirPin = 4;
const int stepPin = 5;
const int stepsPerRevolution = 200;

int stepLocation;
 
 
void setup()
{
  Serial.begin(9600);         //set bit rate of serial port connecting Arduino with computer
  Serial1.begin(115200);      //set bit rate of serial port connecting LiDAR with Arduino
  
  // Declare pins as Outputs for NEMA
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  digitalWrite(dirPin, HIGH); //set direction to clockwise

  attachInterrupt(digitalPinToInterrupt(2),ledSensed, LOW);

   Wire.begin();       //set up pin 8 for i2c
}

void ledSensed() {
  stepLocation = 0;
}
 
void loop() {

  for(int x = 0; x < stepsPerRevolution; x++)
  {
    stepLocation++;
    digitalWrite(stepPin, HIGH);
    LiDARMeasurement(stepLocation);
    delayMicroseconds(750);   //best found so far to be delay of 750 micro sec
    digitalWrite(stepPin, LOW);
    LiDARMeasurement(stepLocation);
    delayMicroseconds(750);
  } //for loop
   
}

void LiDARMeasurement(int x){
    if (Serial1.available())                //check if serial port has data input
    {
      if (Serial1.read() == HEADER)        //assess data package frame header 0x59
      {
        uart[0] = HEADER;
        if (Serial1.read() == HEADER)      //assess data package frame header 0x59
        {
          uart[1] = HEADER;
          for (i = 2; i < 9; i++)         //save data in array
          {
            uart[i] = Serial1.read();
          }
          check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
          if (uart[8] == (check & 0xff))        //verify the received data as per protocol
          {
            dist = uart[2] + uart[3] * 256;     //calculate distance value
            Wire.beginTransmission(8); // transmit to device #8
            Wire.write(dist);
            Wire.write(x);
            Wire.endTransmission(); // 
          }
        }
      }
    } //if serial.avaliable
}
