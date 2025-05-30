#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "peripherals.h"
#include "driver/ledc.h"

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
    if (alarm_active)
        return;

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
        face_detected_flag = false;
        return;
    } else {
        buzzer_state = !buzzer_state;
        digitalWrite(BUZZER, buzzer_state ? HIGH : LOW);
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
    lcd.setCursor(0, line);
    for (int i = 0; i < 16; i++) lcd.print(' ');
    lcd.setCursor(0, line);
    lcd.print(msg);
}

// LED
void enable_flash(bool en)
{
    int duty = en ? 255 : 0;
    ledcWrite(CONFIG_LED_LEDC_CHANNEL, duty);
}