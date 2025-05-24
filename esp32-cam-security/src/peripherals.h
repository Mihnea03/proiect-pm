#include <string.h>

#define BUZZER 13
#define PIR_OUT 16
#define LCD_SDA 15
#define LCD_SCL 14

void init_buzzer();
void start_alarm(int duration, int time_between);
void update_alarm();

void init_lcd();
void lcd_print(const char *msg, int line);

void init_pir();
bool check_pir();