#include <Arduino.h>
#include <Sensors.h>







void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  pinMode(PIN_DUST, INPUT);
  pinMode(PIN_TURB, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_TURBID, OUTPUT);

 initDust();
 initTurb();
}

void loop()
{

  loopBlink();          //  bUILTIN lED
  runDust();
  runTurbidity();
  runLDR();

}