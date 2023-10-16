
#include <WiFi.h>


const char* ssid = "";
const char* password = "!";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  //Connect to WiFi
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting");
  }

  //If connected
  Serial.println("Connected");

  

}

void loop() {
  // put your main code here, to run repeatedly:
  

}
