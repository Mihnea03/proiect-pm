#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "peripherals.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buzzer
void init_buzzer() {
	pinMode(BUZZER, OUTPUT);
}

// Buzzer timing state
static bool alarm_active = false;
static unsigned long alarm_end_time = 0;
static unsigned long last_toggle_time = 0;
static int toggle_interval = 0;
static bool buzzer_state = false;

void start_alarm(int duration, int time_between) {
    alarm_active = true;
    alarm_end_time = millis() + duration;
    last_toggle_time = millis();
    toggle_interval = time_between;
    buzzer_state = false;
    digitalWrite(BUZZER, LOW);
}

void update_alarm() {
    if (!alarm_active) return;

    unsigned long now = millis();

    if (now >= alarm_end_time) {
        alarm_active = false;
        digitalWrite(BUZZER, LOW);
        return;
    }

    if (now - last_toggle_time >= toggle_interval) {
        buzzer_state = !buzzer_state;
        digitalWrite(BUZZER, buzzer_state ? HIGH : LOW);
        last_toggle_time = now;
    }
}


// PIR
void init_pir() {
	pinMode(PIR_OUT, INPUT_PULLUP);
}

bool check_pir() {
	int detected = digitalRead(PIR_OUT);	

	return (detected == HIGH);
}

// LCD
void init_lcd() {
    Wire.begin(LCD_SDA, LCD_SCL);
	lcd.init();
	lcd.backlight();
}

void lcd_print(const char *msg, int line) {
	lcd.clear();
	lcd.setCursor(0, line);
	lcd.print(msg);
}