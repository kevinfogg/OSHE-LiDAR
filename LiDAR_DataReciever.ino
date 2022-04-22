// This code is for the recieving info board
// Last Modified: 4-22-2022

#include <Wire.h>

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // function that executes whenever data is received from writer
  Serial.begin(9600);
}

void receiveEvent() {
   
    int c = Wire.read(); // receive an int
    Serial.print(c);  //print the distance measured
    Serial.print(" , ");
   
   int x = Wire.read();
   Serial.println(1.8 * x);  //multiply the steps by 1.8 to make 360 degrees
}

void loop(){
}
