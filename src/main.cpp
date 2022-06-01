#include <Arduino.h>
#include <Sensors.h>
/*  NOTES
Dust sensor is  5 Volts
*/



void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  pinMode(PIN_DUST, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  initDust();

}

void loop()
{
  loopBlink();
  runDust();


}