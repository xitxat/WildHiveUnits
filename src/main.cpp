#include <Arduino.h>
#include <Sensors.h>
#include <Wire.h>

//#include "I2CScanner.h"

// I2CScanner scanner;





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

  Serial.println("~~~~~~  I2C Scanner");
 scanI2cBus();
 initDust();
 initTurb();

  initAHT();
initBMP180();


}

void loop()
{

  loopBlink();                //  bUILTIN lED
  runDust();
  runTurbidity();
  runLDR();

  runAHT();
  runBMP180();

//***************************************************



  
}