#include <Arduino.h>
#include "secrets.h"
#include <ESP8266WiFi.h>     //  WIFI
#include <Ticker.h>          //  WiFi
#include <AsyncMqttClient.h> //  MQTT
#include <Sensors.h>
#include <Wire.h>            //  I2C
#include "mqttNodeRed.h"
#include <OneWire.h>
#include <DallasTemperature.h>


AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

unsigned long previousMillis = 0;       // Stores last time ALL mqtt data was published
const long mqttPubInterval = 30000;     // ms.Interval at which to publish sensor readings via MQTT

/* FUNCTIONS */

/*  MQTT  */
void connectToMqtt()
{
  mqttClient.connect();
  Serial.println(">>>>>> Connecting to MQTT...");
}

void onMqttConnect(bool sessionPresent)
{
  Serial.println("******  Connected to MQTT.");
  Serial.print("******  Session present: ");
  Serial.println(sessionPresent);
  Serial.println(" ");
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  if (WiFi.isConnected())
  {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
  Serial.println("Disconnected from MQTT.");
  Serial.println("Reconnecting to MQTT network.");
}

void onMqttPublish(uint16_t packetId)
{
  Serial.print("MQTT Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  // tree = packetId ;
}

/*  WIFI  */
void connectToWifi()
{ 
  WiFi.begin(SECRET_SSID, SECRET_PASS);             // secrets.h
  Serial.print(">>>>>>  Connecting to Wi-Fi...");
}

void onWifiConnect(const WiFiEventStationModeGotIP &event)
{
  connectToMqtt();
  Serial.println("******  Connected to Wi-Fi. ");
  Serial.println("");
  Serial.println(">>>>>>  Establishing MQTT connection");
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event)
{
  Serial.println("******  Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach();                  // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void setupMqtt()
{
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}


void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin(D2, D1);         //join i2c bus with SDA=D2 and SCL=D1 of NodeMCU


  pinMode(PIN_DUST, INPUT);
  pinMode(PIN_TURB, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_TURBID, OUTPUT);
  delay(500);
  Serial.println("");
  Serial.println("");
  Serial.println("~~~~~~  WILD HIVE UNITS");
  Serial.print("."); delay(250); Serial.print(".."); delay(250); Serial.print("..."); delay(250); Serial.println("...."); delay(250); Serial.println("*");
  Serial.println("");
  Serial.println("~~~~~~  MQTT setup");
  setupMqtt();   //  WiFi & MQTT

  Serial.println("~~~~~~  I2C Scanner setup");
 scanI2cBus();

 initDust();
 initTurb();

  initAHT();
initBMP180();

initDallas();

}

void loop()
{

  loopBlink();                //  bUILTIN lED
  runDust();
  runTurbidity();
  runLDR();

  runAHT();
  runBMP180();

//  TESTING

runDallasByIndex();


//  END TEST

//*************************** MQTT SENDER ***
  unsigned long currentMillis = millis(); // Publishes a new MQTT message (mqttPubInterval = 10 seconds)

  if (currentMillis - previousMillis >= mqttPubInterval)
  { // Save the last time a new reading was published

    previousMillis = currentMillis;
    //  %i  int
    //  %d  decimal int, 
    //  %f, %.3f float to 3 decimal places, 
    //  %lu unsigned long
    //  ref: Summit\Code Snippits>Variable Symbols printf.docx

    // Dust Occupancy.    Pub  MQTT message on topic nodemcu/wildhiveunit/dust/lpo
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_DUST_LPO, 1, true, String(lowpulseoccupancy).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ",MQTT_PUB_DUST_LPO, packetIdPub1);
    Serial.printf("Message: %lu \n", lowpulseoccupancy);

    // Dust Ratio.   Pub MQTT message on topic nodemcu/wildhiveunit/dust/ratio
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_DUST_RATIO, 1, true, String(ratio).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_DUST_RATIO, packetIdPub2);
    Serial.printf("Message: %.2f \n", ratio);

    // Dust Concentration.         Pub MQTT message on topic nodemcu/wildhiveunit/dust/consen
    uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_DUST_CONSEN, 1, true, String(concentration).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_DUST_CONSEN, packetIdPub3);
    Serial.printf("Message: %.2f \n", concentration);

   // LDR.     Pub  MQTT message on topic nodemcu/wildhiveunit/ldr
    uint16_t packetIdPub4 = mqttClient.publish(MQTT_PUB_LDR, 1, true, String(lux).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_LDR, packetIdPub4);
    Serial.printf("Message: %d \n", lux);

   // BMP 180 Temperature.     Pub  MQTT message on topic nodemcu/wildhiveunit/bmp180/temp
    uint16_t packetIdPub5 = mqttClient.publish(MQTT_PUB_BMP180_TEMP, 1, true, String(cur180Temp).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_BMP180_TEMP, packetIdPub5);
    Serial.printf("Message: %.2f \n", cur180Temp);

   // BMP 180 Pressure.     Pub  MQTT message on topic nodemcu/wildhiveunit/bmp180/pres
    uint16_t packetIdPub6 = mqttClient.publish(MQTT_PUB_BMP180_PRES, 1, true, String(calToSeaPres).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_BMP180_PRES, packetIdPub6);
    Serial.printf("Message: %.2f \n", calToSeaPres);

    // Turbidity.      Pub MQTT message on topic nodemcu/wildhiveunit/turbid
    uint16_t packetIdPub7 = mqttClient.publish(MQTT_PUB_TURBID, 1, true, String(digitalTurbidVal).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_TURBID, packetIdPub7);
    Serial.printf("Message: %i \n", digitalTurbidVal);
    
    // AHT Humidity.      Pub MQTT message on topic nodemcu/wildhiveunit/aht/humid
    uint16_t packetIdPub8 = mqttClient.publish(MQTT_PUB_AHT_HUMID, 1, true, String(ahtHumid).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_AHT_HUMID, packetIdPub8);
    Serial.printf("Message: %.2f \n", ahtHumid);
 






  } // X millis mqtt send timer



  
}