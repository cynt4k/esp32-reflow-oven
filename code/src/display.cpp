#include "display.h"

// const char* BUTTON_START = "bStart";
// const char* BUTTON_STOP = "bStop";

Profile profile_0 = Profile(0);
Profile profile_1 = Profile(1);
Profile profile_2 = Profile(2);
Profile profile_3 = Profile(3);

Profile *profiles[N_PROFILES] = {
    &profile_0,
    &profile_1,
    &profile_2,
    &profile_3,
};

Profile *current_profile = &profile_0;

// Status Page
NexButton b_start(0, 1, "b0");
NexButton b_stop(0, 2, "b1");
NexButton b_configure(0, 3, "b2");
NexButton b_profile_switch(0, 15, "b3");

NexText t_status_heatup1(0, 4, "t1");
NexText t_status_soak_celcius(0, 5, "t2");
NexText t_status_soak_seconds(0, 6, "t3");
NexText t_status_heatup2(0, 7, "t4");
NexText t_status_reflow_celcius(0, 8, "t5");
NexText t_status_reflow_seconds(0, 9, "t6");
NexText t_status_message(0, 10, "g1");

NexWaveform w_status(0, 11, "s0");
NexTimer tm_status_update_graph(0, 12, "tm0");
NexNumber n_status_temperature(0, 13, "n0");
NexNumber n_status_calc_temperature(0, 15, "n1");
NexVariable v_status_reflow_running(0, 14, "reflowRunning");

// Parameter Page
NexButton b_parameter_heatup1_time(3, 1, "b0");
NexButton b_parameter_soak_time(3, 2, "b1");
NexButton b_parameter_soak_temp(3, 3, "b2");
NexButton b_parameter_heatup2_time(3, 4, "b3");
NexButton b_parameter_reflow_time(3, 5, "b4");
NexButton b_parameter_reflow_temp(3, 6, "b5");
NexButton b_parameter_advanced(3, 7, "b6");
NexButton b_parameter_back(3, 8, "b7");

NexText t_parameter_heatup1_time(3, 9, "t0");
NexText t_parameter_soak_time(3, 10, "t1");
NexText t_parameter_soak_temp(3, 11, "t2");
NexText t_parameter_heatup2_time(3, 12, "t3");
NexText t_parameter_reflow_time(3, 13, "t4");
NexText t_parameter_reflow_temp(3, 14, "t5");

// Advanced Page
NexButton b_advanced_P_term(2, 1, "b0");
NexButton b_advanced_I_term(2, 2, "b1");
NexButton b_advanced_D_term(2, 3, "b2");
NexButton b_advanced_cooldown_temp(2, 8, "b4");
NexButton b_advanced_temp_limit_min(2, 9, "b5");
NexButton b_advanced_temp_limit_max(2, 10, "b6");
NexButton b_advanced_back(2, 4, "b3");

NexText t_advanced_P_term(2, 1, "t0");
NexText t_advanced_I_term(2, 2, "t1");
NexText t_advanced_D_term(2, 3, "t2");
NexText t_advanced_cooldown_temp(2, 11, "t3");
NexText t_advanced_temp_limit_min(2, 12, "t4");
NexText t_advanced_temp_limit_max(2, 13, "t5");

// Profile Page
NexButton b_profile_1(4, 1, "b0");
NexButton b_profile_2(4, 2, "b1");
NexButton b_profile_3(4, 3, "b2");
NexButton b_profile_4(4, 4, "b3");
NexButton b_profile_back(4, 5, "b4");

// Numpad
NexButton b_numpad_0(1, 1, "b0");
NexButton b_numpad_1(1, 2, "b1");
NexButton b_numpad_2(1, 3, "b2");
NexButton b_numpad_3(1, 4, "b3");
NexButton b_numpad_4(1, 5, "b4");
NexButton b_numpad_5(1, 6, "b5");
NexButton b_numpad_6(1, 7, "b6");
NexButton b_numpad_7(1, 8, "b7");
NexButton b_numpad_8(1, 9, "b8");
NexButton b_numpad_9(1, 10, "b9");
NexButton b_numpad_dot(1, 11, "b10");
NexButton b_numpad_enter(1, 12, "b11");
NexButton b_numpad_cancel(1, 13, "b12");
NexButton b_numpad_back(1, 14, "b13");

NexText t_numpad_input(1, 15, "t0");

NexTouch *nex_listen_list[] = {
  // Status
  &b_start,
  &b_stop,
  &b_configure,
  &b_profile_switch,
  // Parameter
  &b_parameter_heatup1_time,
  &b_parameter_heatup2_time,
  &b_parameter_advanced,
  &b_parameter_back,
  &b_parameter_reflow_temp,
  &b_parameter_reflow_time,
  &b_parameter_soak_temp,
  &b_parameter_soak_time,
  // Advanced
  &b_advanced_back,
  &b_advanced_D_term,
  &b_advanced_I_term,
  &b_advanced_P_term,
  &b_advanced_cooldown_temp,
  &b_advanced_temp_limit_min,
  &b_advanced_temp_limit_max,
  // Profile
  &b_profile_1,
  &b_profile_2,
  &b_profile_3,
  &b_profile_4,
  &b_profile_back,
  // Numpad
  &b_numpad_0,
  &b_numpad_1,
  &b_numpad_2,
  &b_numpad_3,
  &b_numpad_4,
  &b_numpad_5,
  &b_numpad_6,
  &b_numpad_7,
  &b_numpad_8,
  &b_numpad_9,
  &b_numpad_dot,
  &b_numpad_enter,
  &b_numpad_back,
  &b_numpad_cancel,
  NULL
};

const char* statusStr[] = { "Ready", "Heatup 1", "Soak", "Heatup 2", "Reflow", "Cooldown", "Finished", "Aborted", "Too hot", "ERROR" };

Page current_page = PAGE_STATUS;
Page previous_page = current_page;
Status current_status = STATUS_READY;
Status previous_status;

char numpad_value_char[5];
float_t numpad_value = 0.0;
uint8_t numpad_pos = -1;

// Temperature control values
uint32_t last_temp = 0;
uint32_t last_calc_temp = 0;

bool trigger_start = false;

Parameter current_parameter = PARAMETER_NONE;

void update_status_message(bool force) {
    if (current_status == previous_status && !force) return;
    t_status_message.setText(statusStr[current_status]);
    previous_status = current_status;
}

void bStartStopPopCallback(void *ptr) {
  NexButton *btn = (NexButton *)ptr;
  if (b_start.compare((NexObject *)btn)) {
    switch (current_status) {
        case STATUS_READY:
        case STATUS_ERROR:
        case STATUS_FINISHED:
        case STATUS_ABORTED:
            current_status = STATUS_READY;
            break;
        default: return;
    }
    trigger_start = true;
    w_status.clearChannel(1);
    v_status_reflow_running.setValue(1);
    tm_status_update_graph.enable();
  }
  if (b_stop.compare((NexObject *)btn)) {
    if (current_status == STATUS_READY) {
        return;
    }
    if (current_status != STATUS_FINISHED) {
        current_status = STATUS_ABORTED;
    } else {
        current_status = STATUS_READY;
    }
    v_status_reflow_running.setValue(0);
    w_status.clearChannel(1);
    tm_status_update_graph.disable();
  }
}

void clear_numpad() {
    numpad_pos = -1;
    numpad_value = 0.0;
    t_numpad_input.setText("");
    memset(numpad_value_char, 0, sizeof(numpad_value_char));
}

float_t calculate_numpad() {
    return atof(numpad_value_char);
}

void add_numpad_val(uint8_t val) {
    numpad_value_char[numpad_pos] = val + 0x30;
    // numpad_cache[numpad_pos] = val;
}

void save_parameter_settings(float_t new_value) {
    current_profile->vals[current_parameter] = new_value;
    if (!current_profile->save_value(current_parameter)) {
        Serial.print("could not save parameter: ");
        Serial.println(current_profile->get_key(current_parameter));
    }
}

void bNumpadCallback(void *ptr) {
    NexButton *btn = (NexButton *)ptr;
    NexObject *btnObj = (NexButton *)ptr;

    if (btn->compare(&b_numpad_cancel)) {
        clear_numpad();
        return;
    }
    if (btn->compare(&b_numpad_enter)) {
        numpad_value = calculate_numpad();
        save_parameter_settings(numpad_value);
        return;
    }

    if (numpad_pos == 4) {
        return;
    }
    numpad_pos++;

    if (btn->compare(&b_numpad_0)) {
        add_numpad_val(0);
    }
    if (btn->compare(&b_numpad_1)) {
        add_numpad_val(1);
    }
    if (btn->compare(&b_numpad_2)) {
        add_numpad_val(2);
    }
    if (btn->compare(&b_numpad_3)) {
        add_numpad_val(3);
    }
    if (btn->compare(&b_numpad_4)) {
        add_numpad_val(4);
    }
    if (btn->compare(&b_numpad_5)) {
        add_numpad_val(5);
    }
    if (btn->compare(&b_numpad_6)) {
        add_numpad_val(6);
    }
    if (btn->compare(&b_numpad_7)) {
        add_numpad_val(7);
    }
    if (btn->compare(&b_numpad_8)) {
        add_numpad_val(8);
    }
    if (btn->compare(&b_numpad_9)) {
        add_numpad_val(9);
    }
    if (btn->compare(&b_numpad_dot)) {
        for (uint8_t i = 0; i < numpad_pos; i++) {
            if (numpad_value_char[i] == 0x2E) {
                return;
            }
        }
        
        numpad_value_char[numpad_pos] = 0x2E;

        return;
    }
    Serial.println(numpad_value_char);
    t_numpad_input.setText(numpad_value_char);
}

void format_value(float_t value, char val_buff[10]) {
    float_t decimal_places = value - static_cast<int16_t>(value);
    #ifdef DEBUG
    Serial.print("Decimal value after comma: ");
    Serial.println(decimal_places);
    #endif
    if (decimal_places <= 0) {
        sprintf(val_buff, "%.0f", value);
    } else if (value < 0.1) {
        sprintf(val_buff, "%.3f", value);
    } else if (value < 1) {
        sprintf(val_buff, "%.2f", value);
    } else {
        sprintf(val_buff, "%.1f", value);
    }
}

void set_values_advanced_page() {
    delay(100);
    char val_buff[10];
    // P-Term
    format_value(current_profile->vals[PARAMETER_ADVANCED_P_TERM], val_buff);
    t_advanced_P_term.setText(val_buff);
    // I-Term
    format_value(current_profile->vals[PARAMETER_ADVANCED_I_TERM], val_buff);
    t_advanced_I_term.setText(val_buff);
    // D-Term
    format_value(current_profile->vals[PARAMETER_ADVANCED_D_TERM], val_buff);
    t_advanced_D_term.setText(val_buff);
    // Cooldown Temp
    format_value(current_profile->vals[PARAMETER_ADVANCED_COOLDOWN_TEMP], val_buff);
    t_advanced_cooldown_temp.setText(val_buff);
    // Temp limit min
    format_value(current_profile->vals[PARAMETER_ADVANCED_LIMIT_TEMP_MIN], val_buff);
    t_advanced_temp_limit_min.setText(val_buff);
    // Temp limit max
    format_value(current_profile->vals[PARAMETER_ADVANCED_LIMIT_TEMP_MAX], val_buff);
    t_advanced_temp_limit_max.setText(val_buff);
}

void set_values_parameter_page() {
    delay(100);
    //Display values
    char val_buff[10];
    // Heatup_1 
    sprintf(val_buff, "%.2f", current_profile->vals[PARAMETER_HEATUP1]);
    t_parameter_heatup1_time.setText(val_buff);
    // Heatup_2 
    sprintf(val_buff, "%.2f", current_profile->vals[PARAMETER_HEATUP2]);
    t_parameter_heatup2_time.setText(val_buff);
    // Reflow temp
    sprintf(val_buff, "%.1f", current_profile->vals[PARAMETER_REFLOW_TEMP]);
    t_parameter_reflow_temp.setText(val_buff);
    // Reflow time
    sprintf(val_buff, "%.0f", current_profile->vals[PARAMETER_REFLOW_TIME]);
    t_parameter_reflow_time.setText(val_buff);
    // Soak temp
    sprintf(val_buff, "%.1f", current_profile->vals[PARAMETER_SOAK_TEMP]);
    t_parameter_soak_temp.setText(val_buff);
    // Soak time
    sprintf(val_buff, "%.0f", current_profile->vals[PARAMETER_SOAK_TIME]);
    t_parameter_soak_time.setText(val_buff);
}

void draw_reflow_graph() {
    float_t heatup_1 = current_profile->vals[PARAMETER_HEATUP1];
    float_t heatup_2 = current_profile->vals[PARAMETER_HEATUP2];
    float_t soak_time = current_profile->vals[PARAMETER_SOAK_TIME];
    float_t soak_temp = current_profile->vals[PARAMETER_SOAK_TEMP];
    float_t reflow_time = current_profile->vals[PARAMETER_REFLOW_TIME];
    float_t reflow_temp = current_profile->vals[PARAMETER_REFLOW_TEMP];
    // float_t cooldown_temp = current_profile->vals[PARAMETER_ADVANCED_COOLDOWN_TEMP];

    int16_t heatup1_steps = (soak_temp - last_temp) / heatup_1;
    int16_t heatup2_steps = (reflow_temp - soak_temp) / heatup_2;

    float_t update_val = last_temp;
    for (int16_t i = 0; i < heatup1_steps / GRAPH_X_RESCALE; i++) {
        update_val += (heatup_1 * GRAPH_X_RESCALE);
        delay(10);
        w_status.addValue(0, (uint8_t)update_val);
    }
    for (int16_t i = 0; i < soak_time / GRAPH_X_RESCALE; i++) {
        update_val = soak_temp;
        delay(10);
        w_status.addValue(0, (uint8_t)update_val);
    }
    for (int16_t i = 0; i < heatup2_steps / GRAPH_X_RESCALE; i++) {
        update_val += (heatup_2 * GRAPH_X_RESCALE);
        delay(10);
        w_status.addValue(0, (uint8_t)update_val);
    }
    for (int16_t i = 0; i < reflow_time / GRAPH_X_RESCALE; i++) {
        update_val = reflow_temp;
        delay(10);
        w_status.addValue(0, (uint8_t)update_val);
    }
}

void set_values_status_page() {
    delay(100);
    //Display values
    char val_buff[10];
    // Heatup_1 
    sprintf(val_buff, "%.2f", current_profile->vals[PARAMETER_HEATUP1]);
    t_status_heatup1.setText(val_buff);
    // Heatup_2 
    sprintf(val_buff, "%.2f", current_profile->vals[PARAMETER_HEATUP2]);
    t_status_heatup2.setText(val_buff);
    // Reflow temp
    sprintf(val_buff, "%.1f", current_profile->vals[PARAMETER_REFLOW_TEMP]);
    t_status_reflow_celcius.setText(val_buff);
    // Reflow time
    sprintf(val_buff, "%.0f", current_profile->vals[PARAMETER_REFLOW_TIME]);
    t_status_reflow_seconds.setText(val_buff);
    // Soak temp
    sprintf(val_buff, "%.1f", current_profile->vals[PARAMETER_SOAK_TEMP]);
    t_status_soak_celcius.setText(val_buff);
    // Soak time
    sprintf(val_buff, "%.0f", current_profile->vals[PARAMETER_SOAK_TIME]);
    t_status_soak_seconds.setText(val_buff);
    // Profile
    sprintf(val_buff, "%d", current_profile->id + 1);
    b_profile_switch.setText(val_buff);
    
    draw_reflow_graph();
}

void bProfileSwitchCallback(void *ptr) {
    NexButton *btn = (NexButton *)ptr;
    NexObject *btnObj = (NexButton *)ptr;

    if (btn->compare(&b_profile_1)) {
        current_profile = profiles[0];
    }
    if (btn->compare(&b_profile_2)) {
        current_profile = profiles[1];
    }
    if (btn->compare(&b_profile_3)) {
        current_profile = profiles[2];
    }
    if (btn->compare(&b_profile_4)) {
        current_profile = profiles[3];
    }
    previous_page = PAGE_PROFILE;
    current_page = PAGE_STATUS;
    n_status_temperature.setValue(last_temp);
    set_values_status_page();
    update_status_message(true);
}

void change_profile(uint8_t number) {
    if (number >= N_PROFILES) {
        Serial.print("max size of profiles is ");
        Serial.println(N_PROFILES);
        return;
    }
    current_profile = profiles[number];
}

void bPageSwitchCallback(void *ptr) {
    NexButton *btn = (NexButton *)ptr;
    Page currentPageTmp = current_page;
    if (btn->compare(&b_configure)) {
        current_page = PAGE_PARAMETER;
        set_values_parameter_page();
    }
    if (btn->compare(&b_profile_switch)) {
        current_page = PAGE_PROFILE;
    }
    if (btn->compare(&b_parameter_advanced)) {
        current_page = PAGE_ADVANCED;
        set_values_advanced_page();
    }
    if (btn->compare(&b_numpad_back)) {
        current_page = previous_page;
        current_parameter = PARAMETER_NONE;
        clear_numpad();
        switch (current_page) {
            case PAGE_ADVANCED: set_values_advanced_page(); break;
            case PAGE_PARAMETER: set_values_parameter_page(); break;
        }
    }
    if (btn->compare(&b_advanced_back)) {
        current_page = PAGE_PARAMETER;
        set_values_parameter_page();
    }
    if (btn->compare(&b_parameter_back) || btn->compare(&b_profile_back)) {
        current_page = PAGE_STATUS;
        n_status_temperature.setValue(last_temp);
        set_values_status_page();
        update_status_message(true);
    }
    previous_page = currentPageTmp;
}

void bParameterChangeCallback(void *ptr) {
    NexButton *btn = (NexButton *)ptr;
    previous_page = current_page;
    current_page = PAGE_NUMPAD;
    if (btn->compare(&b_parameter_heatup1_time)) {
        current_parameter = PARAMETER_HEATUP1;
    }
    if (btn->compare(&b_parameter_heatup2_time)) {
        current_parameter = PARAMETER_HEATUP2;
    }
    if (btn->compare(&b_parameter_reflow_temp)) {
        current_parameter = PARAMETER_REFLOW_TEMP;
    }
    if (btn->compare(&b_parameter_reflow_time)) {
        current_parameter = PARAMETER_REFLOW_TIME;
    }
    if (btn->compare(&b_parameter_soak_temp)) {
        current_parameter = PARAMETER_SOAK_TEMP;
    }
    if (btn->compare(&b_parameter_soak_time)) {
        current_parameter = PARAMETER_SOAK_TIME;
    }
    if (btn->compare(&b_advanced_D_term)) {
        current_parameter = PARAMETER_ADVANCED_D_TERM;
    }
    if (btn->compare(&b_advanced_I_term)) {
        current_parameter = PARAMETER_ADVANCED_I_TERM;
    }
    if (btn->compare(&b_advanced_P_term)) {
        current_parameter = PARAMETER_ADVANCED_P_TERM;
    }
    if (btn->compare(&b_advanced_cooldown_temp)) {
        current_parameter = PARAMETER_ADVANCED_COOLDOWN_TEMP;
    }
    if (btn->compare(&b_advanced_temp_limit_min)) {
        current_parameter = PARAMETER_ADVANCED_LIMIT_TEMP_MIN;
    }
    if (btn->compare(&b_advanced_temp_limit_max)) {
        current_parameter = PARAMETER_ADVANCED_LIMIT_TEMP_MAX;
    }
}

void Display::setup_display() {
    while (!nexInit()) {
        Serial.println("initialization display failed");
        delay(500);
    }

    for(uint8_t i = 0; i < N_PROFILES; i++) {
        (*profiles[i]).read_values();
        (*profiles[i]).save_profile();
    }
    // current_profile.read_values();
    // current_profile.save_profile();

    // Callbacks Start/Stop
    b_start.attachPop(bStartStopPopCallback, &b_start);
    b_stop.attachPop(bStartStopPopCallback, &b_stop);

    // Callbacks Page Switch
    b_configure.attachPop(bPageSwitchCallback, &b_configure);
    b_profile_switch.attachPop(bPageSwitchCallback, &b_profile_switch);
    b_advanced_back.attachPop(bPageSwitchCallback, &b_advanced_back);
    b_parameter_advanced.attachPop(bPageSwitchCallback, &b_parameter_advanced);
    b_numpad_back.attachPop(bPageSwitchCallback, &b_numpad_back);
    b_parameter_back.attachPop(bPageSwitchCallback, &b_parameter_back);
    b_profile_back.attachPop(bPageSwitchCallback, &b_profile_back);

    // Callback Profile Switch
    b_profile_1.attachPop(bProfileSwitchCallback, &b_profile_1);
    b_profile_2.attachPop(bProfileSwitchCallback, &b_profile_2);
    b_profile_3.attachPop(bProfileSwitchCallback, &b_profile_3);
    b_profile_4.attachPop(bProfileSwitchCallback, &b_profile_4);

    // Callback Parameter Change
    b_parameter_heatup1_time.attachPop(bParameterChangeCallback, &b_parameter_heatup1_time);
    b_parameter_heatup2_time.attachPop(bParameterChangeCallback, &b_parameter_heatup2_time);
    b_parameter_reflow_temp.attachPop(bParameterChangeCallback, &b_parameter_reflow_temp);
    b_parameter_reflow_time.attachPop(bParameterChangeCallback, &b_parameter_reflow_time);
    b_parameter_soak_temp.attachPop(bParameterChangeCallback, &b_parameter_soak_temp);
    b_parameter_soak_time.attachPop(bParameterChangeCallback, &b_parameter_soak_time);

    // Callback Advanced Parameter Change
    b_advanced_D_term.attachPop(bParameterChangeCallback, &b_advanced_D_term);
    b_advanced_I_term.attachPop(bParameterChangeCallback, &b_advanced_I_term);
    b_advanced_P_term.attachPop(bParameterChangeCallback, &b_advanced_P_term);
    b_advanced_cooldown_temp.attachPop(bParameterChangeCallback, &b_advanced_cooldown_temp);
    b_advanced_temp_limit_min.attachPop(bParameterChangeCallback, &b_advanced_temp_limit_min);
    b_advanced_temp_limit_max.attachPop(bParameterChangeCallback, &b_advanced_temp_limit_max);

    //Callback Numpad Handling
    b_numpad_0.attachPop(bNumpadCallback, &b_numpad_0);
    b_numpad_1.attachPop(bNumpadCallback, &b_numpad_1);
    b_numpad_2.attachPop(bNumpadCallback, &b_numpad_2);
    b_numpad_3.attachPop(bNumpadCallback, &b_numpad_3);
    b_numpad_4.attachPop(bNumpadCallback, &b_numpad_4);
    b_numpad_5.attachPop(bNumpadCallback, &b_numpad_5);
    b_numpad_6.attachPop(bNumpadCallback, &b_numpad_6);
    b_numpad_7.attachPop(bNumpadCallback, &b_numpad_7);
    b_numpad_8.attachPop(bNumpadCallback, &b_numpad_8);
    b_numpad_9.attachPop(bNumpadCallback, &b_numpad_9);
    b_numpad_dot.attachPop(bNumpadCallback, &b_numpad_dot);
    b_numpad_enter.attachPop(bNumpadCallback, &b_numpad_enter);
    b_numpad_cancel.attachPop(bNumpadCallback, &b_numpad_cancel);

    // set the default values
    set_values_status_page();

    // set status message
    update_status_message(true);
}

void Display::display_loop() {
    switch (current_page) {
        case PAGE_STATUS:
            update_status_message(false);
            break;
    }
    switch (current_status) {
        case STATUS_FINISHED:
            uint32_t status;
            v_status_reflow_running.getValue(&status);
            if (status != 0) {
                v_status_reflow_running.setValue(0);
                tm_status_update_graph.disable();
            }
    }
    nexLoop(nex_listen_list);
}

void Display::set_temperature(float_t temperature) {
    if (last_temp == temperature) {
        return;
    }
    if (current_page == PAGE_STATUS) {
        n_status_temperature.setValue(temperature);
    }
    last_temp = temperature;
}

void Display::set_calc_temperature(float_t temperature) {
    if (last_calc_temp == temperature) {
        return;
    }
    if (current_page == PAGE_STATUS) {
        n_status_calc_temperature.setValue(temperature);
    }
    last_calc_temp = temperature;
}

float_t Display::get_numpad_value() {
    return numpad_value;
}