#include <Arduino.h>
#include <PID_v1.h>
// #include "profile.h"

#ifndef _DEFINES

#define DEBUG

// #define FAKE_TEMPERATURE

#define SDA_PIN         33
#define SCL_PIN         35
#define MAX_CLK         5
#define MAX_CS          23
#define MAX_DO          19
#define TXD2            17
#define RXD2            16
#define SSR_PIN         26
#define BUZZER_PIN      21

#define SENSOR_SAMPLING_TIME 1500
#define HEATUP_CHECK_INTERVAL 1000
#define BUZZER_ON_PERIOD 2000

enum Parameter {
    PARAMETER_NONE = -1,
    PARAMETER_HEATUP1,
    PARAMETER_SOAK_TIME,
    PARAMETER_SOAK_TEMP,
    PARAMETER_HEATUP2,
    PARAMETER_REFLOW_TIME,
    PARAMETER_REFLOW_TEMP,
    PARAMETER_ADVANCED_D_TERM,
    PARAMETER_ADVANCED_I_TERM,
    PARAMETER_ADVANCED_P_TERM,
    PARAMETER_ADVANCED_COOLDOWN_TEMP,
    PARAMETER_ADVANCED_LIMIT_TEMP_MIN,
    PARAMETER_ADVANCED_LIMIT_TEMP_MAX,
    PARAMETER_MAX_SIZE,
};

typedef enum REFLOW_STATUS {
    REFLOW_STATUS_OFF,
    REFLOW_STATUS_ON,
} reflow_status_t;

#define _DEFINES
#endif