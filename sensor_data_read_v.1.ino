#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SparkFunCCS811.h>
#include <ClosedCube_HDC1080.h>
#define CCS811_ADDR 0x5A    //Alternate I2C Address

// Ticker for watchdog

#include <Ticker.h>
Ticker secondTick;

CCS811 myCCS811(CCS811_ADDR);
ClosedCube_HDC1080 myHDC1080;


//MQTT  credentials

const char *mqtt_server = "broker.hivemq.com";
const int mqttPort = 1883;
int mqttTryCounter=0;
unsigned long previousMillis = 0;

//WiFiClient espClient;
//PubSubClient client(espClient);
//

//---------------Difining Sensor Pin----------------------------------------------//
int analogPin = A0;
int sensorValue;
int Temperature = 0;
int Humidity = 0;
int carbon_dioxide = 0;
int total_voc = 0;
int methane = 0;

//-------------------------Variable to store sensor data----------------------------//
int data1 = 0;
int data2 = 0;
int data3 = 0;
int data4 = 0;
int data5 = 0;
int did = 1002;

//int D8=0;
//int D7=0;
volatile int watchdogCount = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;


extern "C" {
#include "user_interface.h"
}



//--------------------Watchdog----------------------//

void ISRwatchdog() {
  watchdogCount++;
  if (watchdogCount == 300) {
    Serial.println("Watchdog bites!!");
    ESP.reset();
  }
}

void setup() {

  
  secondTick.attach(1, ISRwatchdog);// Attaching ISRwatchdog function
  Serial.begin(115200);
  myHDC1080.begin(0x40);
  delay(10);

  //It is recommended to check return status on .begin(), but it is not required.
  CCS811Core::status returnCode = myCCS811.begin();
  delay(10);
  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    Serial.println(".begin() returned with an error.");
    while (1);      //No reason to go further
  }
  pinMode(analogPin, INPUT);

//  client.setServer(mqtt_server,mqttPort);
  
//  client.setCallback(callback);
}

void loop(){

    watchdogCount = 0;
    
    if (myCCS811.dataAvailable()) {
    myCCS811.readAlgorithmResults();
  }
//  delay(250);
if (WiFi.status() != WL_CONNECTED){ 
  set_wifi();
  }
//  delay(250);
  
  sensor_data();
 
  
//  light_sleep();
    unsigned long currentMillis = millis();
    Serial.println("Current Millis");
    Serial.println(currentMillis);
    if ((unsigned long)(currentMillis - previousMillis) >= 50000) {
    deep_sleep();
    previousMillis=currentMillis;
  }
    
  
}


//---------------------------------------Read Sensor Data---------------------------------------//

int temp() {
  for (int i = 1; i <= 10; i++) {
    Temperature = Temperature + myHDC1080.readTemperature();
    delay(100);
  }
  Temperature = ((Temperature / 10) - 3);
  return Temperature ;
}

int hum() {
  for (int i = 1; i <= 10; i++) {
    Humidity = Humidity + myHDC1080.readHumidity();
    delay(100);
  }
  Humidity = (Humidity / 10);
  return Humidity;
}

int co2() {

  Serial.println("Inside co2");
  for (int i = 1; i <= 10; i++) {
    carbon_dioxide = carbon_dioxide + myCCS811.getCO2();
    

  }
  carbon_dioxide = carbon_dioxide / 10;
  return carbon_dioxide;
}

int tvoc() {
  Serial.println("Inside Tvoc");
  for (int i = 1; i <= 10; i++) {
    
    total_voc = total_voc + myCCS811.getTVOC();
    delay(100);
  }
  total_voc = total_voc / 10;
  return total_voc;
}

//------------------Figaro_TGS2611-------------------//

int tgsVal() {
  delay(5000);      //.........5sec delay..........//
  for (int i = 1; i <= 10; i++) {
    int raw = analogRead(analogPin);
    methane = pow(10, .00390625 * (raw - 0));
    delay(100);
  }
  methane = methane / 10;
  return methane;
}

//-----------------all-data------------------//

int sensor_data()
{ 
  Serial.println("reading data");
  data1 = temp(); 
  data2 = hum(); 
  data3 = co2();
  data4 = tvoc();
  data5 = tgsVal();
  

  String msg2 = "";
  msg2 = msg2 + "{\"DID\":" + did + "," + "\"TMP\":" + data1 + "," + "\"HUM\":" + data2 + "," + "\"CO2\":" + data3 + "," + "\"VOC\":" + data4 + "," + "\"CH4\":" + data5 + "}";
//  msg2 = msg2 + "{\"DID\":" + did + "," + "\"CO2\":" + data3 + "," + "\"VOC\":" + data4 + "," + "\"CH4\":" + data5 + "}";
  Serial.println(msg2);
  delay(200);       //........ 0.2 sec delay...........//


}



void deep_sleep(){
Serial.println("Device is going into DEEEEEEEP_Sleep");
  ESP.deepSleep(10 * 1000000); //sleep for 10 seconds
  delay (500);
}
void set_wifi() {
  delay(250);
  int tryCount = 0;
  Serial.println("");
  Serial.println("Connecting to WiFi");
  WiFi.begin("DataSoft_WiFi", "support123");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);        //........ 1 sec delay
    Serial.print(".");
    tryCount++;
    if (tryCount == 10) return loop(); //exiting loop after trying 10 times
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
  delay(250);
  }

