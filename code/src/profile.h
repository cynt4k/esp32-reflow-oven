#include <Arduino.h>
#include <Preferences.h>
#include "defines.h"

#ifndef _PROFILE_H
#define _PROFILE_H
#define N_PROFILES 4

// enum Parameters {
//     PARAMETER_HEATUP1,
//     PARAMETER_HEATUP2,
//     PARAMETER_REFLOW_TEMP,
//     PARAMETER_REFLOW_TIME,
// }


extern const char * parameterStr[];
extern Preferences preferences;

// extern bool init_profile();

class Profile
{
public:
    Profile(uint8_t id);
    Profile() = delete;
    void save_profile();
    void read_values();
    void clear();
    bool save_value(Parameter parameter);
    String get_key(Parameter parameter);
    float_t read_value(Parameter parameter);

    uint8_t id;

    float_t vals[PARAMETER_MAX_SIZE];

private:
    bool check_key_exist(const char * key);

    float_t default_vals[PARAMETER_MAX_SIZE] = {
        3.0,
        100,
        150,
        2,
        50,
        230,
        20,
        0.025,
        100,
        250,
        0.05,
        300,
        350,
        0.05,
        300,
        100,
        40,
        230,
    };

    String key_prefix;
};

#endif
