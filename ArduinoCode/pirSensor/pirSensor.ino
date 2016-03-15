int value=0;
int pirPIN=7;

void initalizePIR(){
  int calibrationTime=10;
  pinMode(pirPIN, INPUT);
  digitalWrite(pirPIN, HIGH);
 
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
    }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

void setup() {
  //set serial to see in serial monitor results:
  Serial.begin(9600);
  initalizePIR();
  
}

void loop() {
  if (digitalRead(pirPIN) == HIGH){
    Serial.println("PIR sensor is high");
    value=value+1;
    
  }
  else {
//        Serial.println("PIR sensor is low");

    if (value>0){
      value=value-1;
    }
  }
//    Serial.print("value is ");
//    Serial.println(value);
    
 delay(50); 

}
