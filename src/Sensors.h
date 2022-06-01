#ifndef SENSORS_WILD_HIVE_UNIT_H
#define SENSORS_WILD_HIVE_UNIT_H

#define PIN_DUST D8             // Dust wire is plugged into D8 on the NodeMCU


        /*     FCN protos   */
void initDust();
void runDust();             //  Dust Sensor
void loopBlink();

#endif