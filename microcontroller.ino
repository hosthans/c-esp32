#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "DHT.h"


//Water sensor
#define POWER_PIN  17 // ESP32 pin GIOP17 connected to sensor's VCC pin
#define SIGNAL_PIN 36 // ESP32 pin GIOP36 (ADC0) connected to sensor's signal pin

int value = 0; // variable to store the sensor value




#define DHTPIN 4
#define DHTTYPE DHT11



DHT dht(DHTPIN, DHTTYPE);

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRow2 = 1;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


const char* ssid = "VentureNet";
const char* password = "VentureLabLan123!";
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();



//Array für diverse Werte
//1=Temp; 2=FeuchtigkeitL; 3=FeuchtigkeitE

double werte[3] = {  };


String messageStatic;


//soll durch Messwerte ersetzt werden
boolean durst = true;


//String messageStatic = "ich bin durstig";
String messageToScroll;

void watersensor(){
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  Serial.print("The water sensor value: ");
  Serial.println(value);

  if(value <= 500){
    messageStatic = "Wasser ausreichend -";
  } else if(value <= 100){
      messageStatic = "Wasser nachfüllen -";
  } else {
    messageStatic = "Wasserstand sehr gut -";
  }

  scrollText(0, messageStatic, 800, lcdColumns);
  delay(1000);
}




void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos = pos + 3) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void wificonnection(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting");
  }
  Serial.println("Connected");
  // initialize LCD
}


void setup(){
  Serial.begin(115200);
  pinMode(POWER_PIN, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW);
  wificonnection();
  Serial.println(F("DHTxx test!"));
  dht.begin();
  // initialize LCD
  dht.begin();
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  sensordata();
  // print static message
  lcd.print(messageStatic);
  watersensor();
  // print scrolling message
  scrollText(1, messageToScroll, 800, lcdColumns);
}

void sensordata(){
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  werte[1] = dht.readHumidity();
  // Read temperature as Celsius (the default)
  werte[0] = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(werte[0]) || isnan(werte[1])) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(werte[0], werte[1], false);

  Serial.print((String) werte[0] + " Temperatur");
  Serial.print((String) werte[1] + " LFKT");

  messageToScroll = "Temperatur " + (String) werte[0] + " Grad - Luftfeuchtigkeit " + (String) werte[1] + "% - " + "-";
  
}



//Blink without delay
