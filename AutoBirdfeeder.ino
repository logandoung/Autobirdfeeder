#include <Servo.h>
  #include "SD.h"
  #include "TMRpcm.h"
  #include "SPI.h"
  #define SD_ChipSelectPin 10
  TMRpcm tmrpcm;
  
  Servo gate;
  const byte foodstatus = 3; //low food LED indicator
  int weightsensor = 0;
  int counter = 0;
  void setup() {
    tmrpcm.speakerPin = 9;
    pinMode(foodstatus, OUTPUT);
    Serial.begin(9600);
    gate.attach(7);
  if (!SD.begin(SD_ChipSelectPin)) {
  Serial.println("SD fail");
    return;
          
  }
  }
  void loop() {
  gate.write(100); //gate open position
    counter++;
    int foodstate = analogRead(A1);
 
    digitalWrite(foodstatus, LOW);
    if (foodstate >= 600) //low food detected, assumed light is hitting photoresistor
    {
      digitalWrite(foodstatus, HIGH); //turn on low food LED
    }
    
    weightsensor = analogRead(A0);
    
    Serial.println(weightsensor);
              if (weightsensor > 200) // 180 is default reading with no weight on sensor
    {
      delay(500); //time to stabilize after bird lands
 
    }
    while (weightsensor > 350) //pest detected
    {
      gate.write(180); //close gate
     weightsensor = analogRead(A0); //check again if pest is still detected
    }
    
    
    if (counter == 2000) //how frequenct the bird sound will play
    {
 
    gate.detach(); //need to disable servo because incompatible with tmrpcm library
    delay(100); //time for servo to detach
    tmrpcm.setVolume(6); //setting the volume for the speaker
    tmrpcm.play("birds006.wav"); //plays the sound file with that name in the sd card
    delay(5000); //allow time for the sound file to play
      tmrpcm.disable(); //disable tmrpcm so that servo can be reattached
    gate.attach(7); //reattach servo
    counter = 0;
    }
    delay(1); //used to relate time to our counter value
  }
