#ifndef SENSORS_WILD_HIVE_UNIT_H
#define SENSORS_WILD_HIVE_UNIT_H

#define PIN_DUST   D8             // Dust wire is plugged into D8 on the NodeMCU
#define PIN_TURB   D1
#define LED_TURBID D2 

        /*     FCN protos   */
void initDust();
void runDust();             //  Dust Sensor

void loopBlink();
void ledFlip(int pin);

#endif