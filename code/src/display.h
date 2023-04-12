#include <Arduino.h>
#include <Nextion.h>
#include <max6675.h>
#include "defines.h"
#include "profile.h"

#ifndef _DISPLAY_H
#define _DISPLAY_H
#define UPDATE_VALUES_THRESHOLD 2000
#define PID_SAMPLE_TIME 1000
#define GRAPH_X_RESCALE 2.987

enum Page {
    PAGE_STATUS,
    PAGE_PARAMETER,
    PAGE_NUMPAD,
    PAGE_ADVANCED,
    PAGE_PROFILE,
};

enum Status {
    STATUS_READY,
    STATUS_HEATUP1,
    STATUS_SOAK,
    STATUS_HEATUP2,
    STATUS_REFLOW,
    STATUS_COOLDOWN,
    STATUS_FINISHED,
    STATUS_ABORTED,
    STATUS_TOO_HOT,
    STATUS_ERROR,
};

extern Page current_page;
extern Parameter current_parameter;
extern Status current_status;
extern Profile *current_profile;
extern bool trigger_start;

class Display {
public:
    Display(MAX6675 * thermocouple);
    void setup_display();
    void display_loop();
    float_t get_numpad_value();
    void set_temperature(float_t temperature);
private:
    MAX6675 * thermocouple;
    unsigned long last_update_status_parameter = millis();
};
#endif