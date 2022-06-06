#ifndef MQTT_NODE_RED_H
#define MQTT_NODE_RED_H

#define MQTT_HOST "test.mosquitto.org"
#define MQTT_PORT 1883

//  MQTT Topics
#define MQTT_PUB_DUST_LPO       "nodemcu/wildhiveunit/dust/lpo"
#define MQTT_PUB_DUST_RATIO     "nodemcu/wildhiveunit/dust/ratio"
#define MQTT_PUB_DUST_CONSEN    "nodemcu/wildhiveunit/dust/consen"
#define MQTT_PUB_TURBID         "nodemcu/wildhiveunit/turbid"
#define MQTT_PUB_LDR            "nodemcu/wildhiveunit/ldr"
#define MQTT_PUB_BMP180_TEMP    "nodemcu/wildhiveunit/bmp180/temp"
#define MQTT_PUB_BMP180_PRES    "nodemcu/wildhiveunit/bmp180/pres"
#define MQTT_PUB_AHT_HUMID      "nodemcu/wildhiveunit/aht/humid"
#define MQTT_PUB_DALLAS_CORE    "nodemcu/wildhiveunit/dallas/core"

#endif