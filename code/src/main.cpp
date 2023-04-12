#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <max6675.h>
// #include <Preferences.h>
#include "display.h"
#include "profile.h"
#include "defines.h"
#include <PID_v1.h>

MAX6675 thermocouple(MAX_CLK, MAX_CS, MAX_DO);
Display display(&thermocouple);

double setpoint, input, output;
double kp, ki, kd;
int window_size;
unsigned long windows_start_time;
unsigned long next_read_temp;
unsigned long next_heatup_interval;
unsigned long timer_soak;
unsigned long timer_reflow;
reflow_status_t reflow_status;

PID pid(&input, &output, &setpoint, kp, ki, kd, DIRECT);

void setup() {
  Serial.begin(115200);
  Serial.println("startup");
  while (!preferences.begin("profile", false)) {
    Serial.println("failed to init profile storage");
    delay(100);
  }

  pinMode(SSR_PIN, OUTPUT);

  delay(100);
  display.setup_display();

  next_read_temp = millis();
  next_heatup_interval = millis();
  window_size = 2000;

  kp = current_profile->vals[PARAMETER_ADVANCED_P_TERM];
  ki = current_profile->vals[PARAMETER_ADVANCED_I_TERM];
  kd = current_profile->vals[PARAMETER_ADVANCED_D_TERM];
  pid.SetTunings(kp, ki, kd);

  input = 18; // DEFAULT Temperature

  Serial.println("setup completed");
}

Page last_page = current_page;

double fake_temperature() {
  switch (current_status) {
    case STATUS_HEATUP1:
      return input + 0.75;
    case STATUS_SOAK:
      return input;
    case STATUS_HEATUP2:
      return input + 0.75;
    case STATUS_REFLOW:
      return input;
    case STATUS_COOLDOWN:
      return input - 0.4;
    case STATUS_FINISHED:
    case STATUS_ABORTED:
    case STATUS_READY:
      return 18;  
  }
  return 18;
}
void loop() {
  display.display_loop();
  unsigned long now;

  if (millis() > next_read_temp) {
    next_read_temp += SENSOR_SAMPLING_TIME;
    #ifdef FAKE_TEMPERATURE
    input = fake_temperature();
    #else
    input = thermocouple.readCelsius();
    #endif
    #ifdef DEBUG
    Serial.print("temperature: ");
    Serial.println(input);
    #endif
    display.set_temperature(input);
  }

  switch(current_status) {
    case STATUS_READY:
      if (input >= current_profile->vals[PARAMETER_ADVANCED_LIMIT_TEMP_MIN]) {
        current_status = STATUS_TOO_HOT;
        break;
      }
      if (trigger_start) {
        trigger_start = false;
        Serial.println("starting reflow routine");
        windows_start_time = millis();
        setpoint = input + current_profile->vals[PARAMETER_HEATUP1];
        pid.SetOutputLimits(0, window_size);
        pid.SetSampleTime(PID_SAMPLE_TIME);
        pid.SetMode(AUTOMATIC);
        current_status = STATUS_HEATUP1;
        next_heatup_interval += HEATUP_CHECK_INTERVAL;
      }
      break;
    case STATUS_HEATUP1:
      reflow_status = REFLOW_STATUS_ON;
      if (millis() >= next_heatup_interval) {
        next_heatup_interval += HEATUP_CHECK_INTERVAL;
        setpoint += current_profile->vals[PARAMETER_HEATUP1];
      }
      if (input >= current_profile->vals[PARAMETER_SOAK_TEMP]) {
        timer_soak = millis() + current_profile->vals[PARAMETER_SOAK_TIME] * 1000; // Soak time in seconds
        setpoint = current_profile->vals[PARAMETER_SOAK_TEMP];
        current_status = STATUS_SOAK;
      }
      break;
    case STATUS_SOAK:
      if (millis() > timer_soak) {
        // setpoint = current_profile->vals[PARAMETER_HEATUP2];
        setpoint = input + current_profile->vals[PARAMETER_HEATUP2];
        next_heatup_interval = millis();
        current_status = STATUS_HEATUP2;
      }
      break;
    case STATUS_HEATUP2:
      if (millis() >= next_heatup_interval) {
        next_heatup_interval += HEATUP_CHECK_INTERVAL;
        setpoint += current_profile->vals[PARAMETER_HEATUP2];
      }
      if (input >= current_profile->vals[PARAMETER_REFLOW_TEMP]) {
        timer_reflow = millis() + current_profile->vals[PARAMETER_REFLOW_TIME] * 1000; // Reflow time in seconds
        setpoint = current_profile->vals[PARAMETER_REFLOW_TEMP];
        current_status = STATUS_REFLOW;
      }
      break;
    case STATUS_REFLOW:
      if (millis() > timer_reflow) {
        setpoint = current_profile->vals[PARAMETER_ADVANCED_COOLDOWN_TEMP];
        current_status = STATUS_COOLDOWN;
      }
      break;
    case STATUS_COOLDOWN:
      if (input < current_profile->vals[PARAMETER_ADVANCED_COOLDOWN_TEMP]) {
        reflow_status = REFLOW_STATUS_OFF;
        current_status = STATUS_FINISHED;
      }
      break;
    case STATUS_ABORTED:
      reflow_status = REFLOW_STATUS_OFF;
    case STATUS_TOO_HOT:
      if (input < current_profile->vals[PARAMETER_ADVANCED_LIMIT_TEMP_MIN]) {
        current_status = STATUS_READY;
      }
      break;
  }

  // PID computation and SSR control
  if (reflow_status == REFLOW_STATUS_ON) {
    now = millis();
    pid.Compute();

    if ((now - windows_start_time) > window_size) {
      // Time to shift the relay window
      windows_start_time += window_size;
    }
    if (output > (now - windows_start_time)) digitalWrite(SSR_PIN, HIGH);
    else digitalWrite(SSR_PIN, LOW);
  } else {
    digitalWrite(SSR_PIN, LOW);
  }
}