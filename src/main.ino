
/*
Blink LED - Sonoff Basic
*/


// Pin config
int LED_SONOFF = 13;


// Blink duration
int BLINK_DURATION = 2000;


void setup() {
  // initialise digital pin LED_SONOFF as an output.
  pinMode(LED_SONOFF, OUTPUT);
}


void loop() {
  digitalWrite(LED_SONOFF, LOW); // LOW will turn on the LED
  delay(BLINK_DURATION);
  digitalWrite(LED_SONOFF, HIGH); // HIGH will turn off the LED
  delay(BLINK_DURATION);
}
