#include <Arduino.h>
#include "Sensors.h"

/*  NOTES
    Dust sensor is  5 Volts
    Dust reading every 30 seconds.
*/

unsigned long loopCounter = 10;
/*  DUST SENSOR */
unsigned long dustDuration;
unsigned long dustStartTime;             // millis timeer
unsigned long dustSampletime_ms = 30000; // sample period 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

/*  TURBIDITY SENSOR */
int digitalTurbidVal = 0;                  //  Turbidity sensor init val.
unsigned long turbidStartTime; // millis timeer
unsigned long turbidSampletime_ms = 30000; // sample period 30s ;

/*  LDR SENSOR  */
const int  PIN_LDR  = A0;            // Light sensor with 10kOhm res.

int analogPinVal; // Analog value from the sensor
int lux;          //Lux value


/*  FUNCTIONS */
void loopBlink()
{
    loopCounter--;
    if(loopCounter == 0){
        loopCounter = 10;
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    Serial.print("LOOP ");
    Serial.println(loopCounter);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    Serial.println(" ");
}

void ledFlip(int pin)        //  BLUE led for water level
{                            //  Yellow led for moisture
    digitalWrite(pin, HIGH); //
    delay(200);              //   RED water dirty
    digitalWrite(pin, LOW);
    delay(200);
}

void initDust()
{
    dustStartTime = millis(); // get the current time for Dust Sensor;
}

void initTurb()
{
    turbidStartTime = millis(); // get the current time for Dust Sensor;
}

void runDust()
{
    dustDuration = pulseIn(PIN_DUST, LOW);
    lowpulseoccupancy = lowpulseoccupancy + dustDuration;

    if ((millis() - dustStartTime) > dustSampletime_ms) // if the sample time == 30s
    {
        ratio = lowpulseoccupancy / (dustSampletime_ms * 10.0);                         // Integer percentage 0=>100
        concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
        Serial.println(" ");
        Serial.println("~~~ Dust Sensor ~~~");
        Serial.print("Low Pulse Occupancy: ");
        Serial.println(lowpulseoccupancy);
        Serial.print("              Ratio: ");
        Serial.println(ratio);
        Serial.print("      Concentration: ");
        Serial.println(concentration);
        Serial.println(" ");
        lowpulseoccupancy = 0;
        dustStartTime = millis();
    }
}

void runTurbidity()
{
    if ((millis() - turbidStartTime) > turbidSampletime_ms) // if the sample time == 30s
    {
        Serial.println("~~~ Turbidity Sensor ~~~ ");
        digitalTurbidVal = digitalRead(PIN_TURB);
        Serial.print("Raw pin: ");
        Serial.println(digitalTurbidVal);

        if (digitalTurbidVal == 0)
        {
            ledFlip(LED_TURBID);
            Serial.println("Water is too dirty.");
        }
        else
        {
            digitalWrite(LED_TURBID, LOW);
        }
        Serial.println(" ");
        turbidStartTime = millis();
    }
}

int sensorRawToPhys(int raw)    //  analog read to Lux
{
  // Conversion rule
  float Vout = float(raw) * (VIN / float(1023));// Conversion analog to voltage
  float RLDR = (R * (VIN - Vout))/Vout;         // Conversion voltage to resistance
  int phys=500/(RLDR/1000);                     // Conversion resitance to lumen
  return phys;
}

void runLDR(){
  analogPinVal = analogRead(PIN_LDR);
    if (analogPinVal == 1024)  //(analogRead(PIN_LDR) < 1024)
    {
analogPinVal = 1023;
    }
  lux=sensorRawToPhys(analogPinVal);

  Serial.print("Raw value from sensor= ");
  Serial.println(analogPinVal); // the analog reading
  Serial.print("Physical value from sensor = ");
  Serial.print(lux); // the analog reading
  Serial.println(" lumen"); // the analog reading
   delay(DELAY_LDR);
}


