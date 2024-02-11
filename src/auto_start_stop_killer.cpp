#include <Arduino.h>
#include <EEPROM.h>

// CONFIG
const int HOLD_BEFORE_DISABLE = 15000; // ms > 10s recommended
const int HOLD_DURATION = 500;        // ms
boolean TOGGLE_ENABLED = false;       // OEM wiring modification required if toggle is enabled. Only splicing is needed if disabled.

// PINS
const int ENABLED_INDICATOR = 8; // OEM built-in Auto Start/Stop LED (optional)
const int OEM_BUTTON = 2;        // OEM Auto Start/Stop button
const int RELAY = 14;            // Relay to disable Auto Start/Stop

// EEPROM
const int ENABLED_EEPROM_ADDRESS = 0;

boolean auto_start_stop_disabled = false;
boolean button_is_pressed = false;

void setup()
{
  pinMode(RELAY, OUTPUT);
  pinMode(OEM_BUTTON, INPUT); // Set HIGH to toggle
  pinMode(ENABLED_INDICATOR, OUTPUT);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop()
{

  if (TOGGLE_ENABLED)
  {
    int state = digitalRead(OEM_BUTTON);

    if (state == HIGH)
    {
      if (!button_is_pressed)
      {
        button_is_pressed = true;
        EEPROM.read(ENABLED_EEPROM_ADDRESS) ? EEPROM.write(ENABLED_EEPROM_ADDRESS, false) : EEPROM.write(ENABLED_EEPROM_ADDRESS, true);
        auto_start_stop_disabled = true; // pressing the OEM button disables the auto start/stop
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
      button_is_pressed = false;
    }

    delay(10);
  }
  else
  {
    EEPROM.write(ENABLED_EEPROM_ADDRESS, true);
  }

  EEPROM.read(ENABLED_EEPROM_ADDRESS) ? digitalWrite(ENABLED_INDICATOR, HIGH) : digitalWrite(ENABLED_INDICATOR, LOW);

  if (!auto_start_stop_disabled && EEPROM.read(ENABLED_EEPROM_ADDRESS))
  {
    delay(HOLD_BEFORE_DISABLE);
    digitalWrite(RELAY, HIGH);
    delay(HOLD_DURATION);
    digitalWrite(RELAY, LOW);
    auto_start_stop_disabled = true;
  }
}