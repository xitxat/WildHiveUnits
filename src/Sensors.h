#ifndef SENSORS_WILD_HIVE_UNIT_H
#define SENSORS_WILD_HIVE_UNIT_H

#define PIN_DUST D8     // 5V. Dust wire is plugged into D8 on the NodeMCU
#define PIN_TURB D5     // 5V. 

#define LED_TURBID D6
                        // LDR: 1 pin to +5V. 2nd pin to A0 AND parallel to resistor to ground.
#define DELAY_LDR 500   // Delay between two measurements in ms
#define VIN 5           // V power voltage
#define R 1000          // ohm resistance value. 10K for indoor readings / 1K for outdoor

#define ONE_WIRE_BUS D7 // Dallas hive temp

        /* MQTT DATA    */      // Pure decleration. Solves OOScope  error in mqtt PUB statements.   
                                // Called in main.cpp         
extern unsigned long lowpulseoccupancy;
extern float ratio;
extern float concentration;
extern int lux;
extern float cur180Temp;
extern float calToSeaPres;
extern int digitalTurbidVal;
extern float ahtHumid;




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

void initDallas();
void runDallasByIndex();
void oneWireScanner();


#endif