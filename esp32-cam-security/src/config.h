#include "core_version.h" // to get the installed ESP32 arduino core version
#include "sensor.h"       // for framesize_t enumerated type to set the default camera resolution

#define CAMERA_MODEL_AI_THINKER           //has PSRAM

#if defined(CAMERA_MODEL_AI_THINKER)
  #define LED_BUILTIN     33
  #define LED_BUILTIN_ON  LOW
  // Flash LED configuration
  #define CONFIG_FLASH_LED 4
  #define CONFIG_LED_LEDC_CHANNEL  LEDC_CHANNEL_7   // Channel 0 is used by camera

  // Camera settings at startup
  #define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_CIF  // 800x600
  #define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif

// common configuration

#define ALARM_DURATION 2000
#define ALARM_TIME_BETWEEN 200

// System states and messages
#define STATE_READY 0
#define STATE_CAMERA_ON 1
#define STATE_INTRUDER_DETECTED 2

#define READY_MSG "Device armed"
#define CAMERA_ON_MSG "Motion detected"
#define INTRUDER_DETECTED_MSG "Intruder detected"

// WiFi
#define CONFIG_WIFI_SSID "Mihnea"              // Mandatory
#define CONFIG_WIFI_PWD  "dyvy8873"          // Mandatory

#define CONFIG_BAUD 115200          // Optional. If mDNS is enabled, the default hostname is "esp32-cam.local"
#define CONFIG_ESP_FACE_DETECT_ENABLED          // Optional. Works at low resolution <= 320x240
#define CONFIG_ESP_FACE_RECOGNITION_ENABLED     // Optional. Works at low resolution <= 320x240

#if defined(CONFIG_FLASH_LED)
  #define CONFIG_LED_ILLUMINATOR_ENABLED
  #define CONFIG_FLASH_PWM_FREQ    50000            // Flash LED PWM frequency
  #define CONFIG_FLASH_PWM_BITS    9                // Resolution of duty cycle counter
  #define CONFIG_LED_MAX_INTENSITY 100              // A percentage (0..100) of full intensity
#endif

// Nothing below should need change

#if defined(ARDUINO_ESP32_RELEASE_1_0_6)     // Assuming version 1.0.6 or newer ESP32-Arduino core is used
  #define NO_GRAB_MODE
#endif