#ifndef SENSORS_WILD_HIVE_UNIT_H
#define SENSORS_WILD_HIVE_UNIT_H

#define PIN_DUST D8 // Dust wire is plugged into D8 on the NodeMCU
#define PIN_TURB D5

#define LED_TURBID D6

#define DELAY_LDR 500 // Delay between two measurements in ms
#define VIN 5         // V power voltage
#define R 1000        // ohm resistance value. 10K for indoor readings / 1K for outdoor

/*     FCN protos   */
void scanI2cBus();

void initDust();
void runDust(); //  Dust Sensor

void loopBlink();
void ledFlip(int pin);

void initTurb();
void runTurbidity();

int sensorRawToPhys(int raw); // Analog to Lux
void runLDR();

void initAHT();
void runAHT();

void initBMP180();
void runBMP180();

#endif