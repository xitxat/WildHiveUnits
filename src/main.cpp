#include <Arduino.h>

  /*  NOTES 
  Dust sensor is  Volts
  */
//int LED_BUILTIN = 13;

#define PIN_DUST D8             // Dust wire is plugged into D8 on the NodeMCU

  /*  DUST SENSOR */
unsigned long dustDuration;
unsigned long dustStartTime;
unsigned long dustSampletime_ms = 30000;//sample 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;


  /*  FUNCTIONS */
void runDust(){
    dustDuration = pulseIn(PIN_DUST, LOW);
    lowpulseoccupancy = lowpulseoccupancy+dustDuration;

    if ((millis()-dustStartTime) > dustSampletime_ms)//if the sample time == 30s
    {
        ratio = lowpulseoccupancy/(dustSampletime_ms*10.0);  // Integer percentage 0=>100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
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

void setup() {
    Serial.begin(9600);
      while (!Serial)
    ;
    pinMode(PIN_DUST,INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    dustStartTime = millis();             //get the current time for Dust Sensor;



}

void loop() {
           digitalWrite(LED_BUILTIN, LOW);
delay(1000);
        Serial.println("LOOP ");
      digitalWrite(LED_BUILTIN, HIGH);
delay(1000);

runDust();

}