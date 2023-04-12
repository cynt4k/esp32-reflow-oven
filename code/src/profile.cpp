#include "profile.h"

const char * parameterStr[] = {
    "heatup_1",
    "soak_time",
    "soak_temp",
    "heatup_2",
    "reflow_time",
    "reflow_temp",
    "d_term",
    "i_term",
    "p_term",
    "cool_temp",
    "temp_min",
    "temp_max",
};

Preferences preferences;

Profile::Profile(uint8_t id) {
    this->id = id;
    this->key_prefix = "p" + String(id) + "_";
}

void Profile::save_profile() {
    for (int i = 0; i < PARAMETER_MAX_SIZE; i++) {
        save_value(Parameter(i));
    }
}

bool Profile::save_value(Parameter parameter) {
    String key = get_key(parameter).c_str();
    #ifdef DEBUG
    Serial.print("saving parameter: ");
    Serial.print(key);
    Serial.print(" value: ");
    Serial.println(vals[parameter]);
    #endif
    size_t size = preferences.putFloat(key.c_str(), vals[parameter]);
    #ifdef DEBUG
    float_t new_value = read_value(parameter);
    Serial.print("saved parameter value: ");
    Serial.println(new_value);
    #endif
    if (size == 0) {
        return false;
    }
    return true;
}

float_t Profile::read_value(Parameter parameter) {
    return preferences.getFloat(get_key(parameter).c_str());
}

bool Profile::check_key_exist(const char * key) {
    return preferences.isKey(key);
}

String Profile::get_key(Parameter parameter) {
    #ifdef DEBUG
    Serial.print("read key prefix: ");
    Serial.print(this->key_prefix);
    Serial.print(" key suffix: ");
    Serial.println(parameterStr[parameter]);
    #endif
    return this->key_prefix + parameterStr[parameter];
}

void Profile::read_values() {
    for (int i = 0; i < PARAMETER_MAX_SIZE; i++) {
        String key = get_key(Parameter(i));
        float_t read_val = preferences.getFloat(key.c_str(), default_vals[i]);
        #ifdef DEBUG
        Serial.print("read val for parameter: ");
        Serial.print(key);
        Serial.print(" value: ");
        Serial.println(read_val);
        #endif
        vals[i] = read_val;
    }
}

void Profile::clear() {
    preferences.clear();
}