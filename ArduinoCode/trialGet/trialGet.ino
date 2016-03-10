#include <SoftwareSerial.h>

//connect UTXD of ESP to D10 of Arduino and URXD of ESP to D11 of Arduino
//PD of ESP should be put to Vcc
//set serial emulator to pin 10 and 11:
SoftwareSerial ESP8266(10,11);

//wifi name and ssid to connect to:
String nomWifi="********";
String ssid="***********";

void sendRequestTrial1(){

      
     String startcommand = "AT+CIPSTART=\"TCP\",\"www.google.fr\", 80";
     
     ESP8266.println(startcommand);
      delay(2000);
     //the sendcommand string is empty as soon as there is a slash in the string
     String sendcommand = "GET /analytics HTTP/1.1\r\nHost: www.google.com\r\n\r\n";//works for most cases
      
     
     //debug the command
     ESP8266.print("AT+CIPSEND=");
     ESP8266.println(sendcommand.length());
     delay(1000);
     if (ESP8266.find(">")){
     Serial.print("send command:");
     Serial.println(sendcommand);
      ESP8266.println(sendcommand);
     }
     recoitDuESP8266(5000);
  
}


void setup() {
  //set serial to see in serial monitor results:
  Serial.begin(9600);
  //set serial emulator to 115200 bauds:
  ESP8266.begin(115200);
  //change ESP to run in 9600 bauds as emulator cannot go so high for reception (not needed if no AT+RST command):
  ESP8266.println("AT+CIOBAUD=9600");
  recoitDuESP8266(2000);
  //so now change to 9600 for rest of communication:
  ESP8266.begin(9600);

  //enable ESP as station and access point:
  ESP8266.println("AT+CWMODE=3");
  //should get OK as response
  recoitDuESP8266(2000);

  //connect to wifi:
  ESP8266.println("AT+CWJAP=\""+nomWifi+"\",\""+ssid+"\"");
  //should get wifi connected and then got IP:
  recoitDuESP8266(10000);
  //to see IP obtained (optional):
//  ESP8266.println("AT+CIFSR");
//  recoitDuESP8266(5000);

//enable multiple connection:
  ESP8266.println("AT+CIPMUX=1");
  //should get OK:
  recoitDuESP8266(1000);
  //set on channel 4, TCP connection to google.com on port 80:
  ESP8266.println("AT+CIPSTART=4,\"TCP\",\"google.com\",80");
  //should get 4 connect and ok:
  recoitDuESP8266(1000);
  //give amount of bytes to send to channel 4 (18 here):
  ESP8266.println("AT+CIPSEND=4,18");
  //should get ok and a prompt
  recoitDuESP8266(1000);
  //send the request (the 18 bytes), the first slash is the path to resource:
  ESP8266.println("GET / HTTP/1.0");
  //send second carriage return:
  ESP8266.println("");
  //should get received 18 bytes and mentioned send ok and then receive the response and then 4,closed
  recoitDuESP8266(5000);
  //sendRequestTrial1();

}

void loop() {
  // put your main code here, to run repeatedly:

}

void recoitDuESP8266(const int timeout){
  long int time = millis();
  String response="";
  while ((time+timeout)>millis()){
    
    while(ESP8266.available()){
        char c=ESP8266.read();
        response+=c;
    }
  }
  Serial.println("response:"+response);

}
