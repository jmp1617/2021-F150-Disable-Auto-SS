#include <Arduino.h>
#include <EEPROM.h>

// CONFIG
const int HOLD_BEFORE_DISABLE = 3000; // ms
const int HOLD_DURATION = 500; // ms

// PINS
const int ENABLED_INDICATOR = 8; // OEM built-in Auto Start/Stop LED (optional)
const int OEM_BUTTON = 2; // OEM Auto Start/Stop button
const int RELAY = 14; // Relay to disable Auto Start/Stop

// EEPROM
const int ENABLED_EEPROM_ADDRESS = 0;

boolean auto_start_stop_disabled = false;
boolean button_is_pressed = false;

void setup() {
  pinMode(RELAY, OUTPUT);
  pinMode(OEM_BUTTON, INPUT);
  pinMode(ENABLED_INDICATOR, OUTPUT);
}

void loop() {

  int state = digitalRead(OEM_BUTTON);

  if(state == HIGH) {
    if(!button_is_pressed) {
      EEPROM.read(ENABLED_EEPROM_ADDRESS) ? EEPROM.write(ENABLED_EEPROM_ADDRESS, false) : EEPROM.write(ENABLED_EEPROM_ADDRESS, true);
      auto_start_stop_disabled = true; // pressing the OEM button disables the auto start/stop
      digitalWrite(LED_BUILTIN, HIGH);
      button_is_pressed = true;
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    button_is_pressed = false;
  }

  EEPROM.read(ENABLED_EEPROM_ADDRESS) ? digitalWrite(ENABLED_INDICATOR, HIGH) : digitalWrite(ENABLED_INDICATOR, LOW);

  if(!auto_start_stop_disabled && EEPROM.read(ENABLED_EEPROM_ADDRESS)) {
    delay(HOLD_BEFORE_DISABLE);
    digitalWrite(RELAY, HIGH);  
    delay(HOLD_DURATION);                      
    digitalWrite(RELAY, LOW);
    auto_start_stop_disabled = true;
  }
}