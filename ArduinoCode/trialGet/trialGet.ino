#include <SoftwareSerial.h>

//connect UTXD of ESP to D10 of Arduino and URXD of ESP to D11 of Arduino
//PD of ESP should be put to Vcc
//set serial emulator to pin 10 and 11:
SoftwareSerial ESP8266(10,11);

//wifi name and ssid to connect to:
String nomWifi="*****";
String ssid="*****";


void initializeWifi(){
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
}

void performGetRequest(String request){
       int slash = request.indexOf('/');
     
     //grab the domain
     String domain;
     if(slash>0){  
       domain = request.substring(0,slash);
     }else{
       domain = request;
     }

     //get the path
     String path;
     if(slash>0){  
       path = request.substring(slash);   
     }else{
       path = "/";          
     }
     

  //set on channel 4, TCP connection to google.com on port 80:
  ESP8266.println("AT+CIPSTART=4,\"TCP\",\""+domain+"\",80");
  //should get 4 connect and ok:
  recoitDuESP8266(1000);
  
  //give amount of bytes to send to channel 4 (18 here):
  String getCommand="GET "+path+" HTTP/1.0\r\n\r\n";
  ESP8266.print("AT+CIPSEND=4,");
  //need to provide length apart else get issue with string concatenation:
  ESP8266.println(getCommand.length());
  //should get ok and a prompt
  recoitDuESP8266(1000);
  //send the request not via println as CR/LN already in string to get correct length:
  ESP8266.print(getCommand);
  //should get received 18 bytes and mentioned send ok and then receive the response and then 4,closed
  recoitDuESP8266(5000);
}

void setup() {
  initializeWifi();
  
  
  String request="google.com";
performGetRequest(request);  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void recoitDuESP8266(const int timeout){
  long int time = millis();
  Serial.println("response:");
  while ((time+timeout)>millis()){
    
    while(ESP8266.available()){
        char c=ESP8266.read();
        Serial.print(c);
        
    }
  }
  Serial.println("");
  

}
