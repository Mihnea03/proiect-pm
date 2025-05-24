// Content of CameraWebServer.ino
// Using a .cpp extension removes warning about C/C++ IntelliSense not supporting
// .ino files in PlatformIO

#include <Arduino.h>
#include <Wire.h>
#include "esp_camera.h"
#include <WiFi.h>
#include "config.h"
#include "camera_pins.h"
#include "peripherals.h"
#include "face_utils.h"

int current_state = 0;

void startCameraServer();

void configESP() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  sensor_t * s = esp_camera_sensor_get();
  
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_framesize(s, FRAMESIZE_CIF);       // 320x240 - faster and good for face recognition
    s->set_quality(s, 12);                     // JPEG quality: 10 (better) to 63 (worst); 12 is decent
    s->set_brightness(s, 1);                   // Adjust based on lighting conditions
    s->set_contrast(s, 0);                     // Keep neutral
    s->set_saturation(s, 0);                   // Keep neutral
    s->set_whitebal(s, 1);                     // Enable auto white balance
    s->set_gain_ctrl(s, 1);                    // Auto gain
    s->set_exposure_ctrl(s, 1);                // Auto exposure
    s->set_awb_gain(s, 1);                     // Auto white balance gain
    s->set_vflip(s, 1);                        // Flip vertically if needed
    s->set_hmirror(s, 0);                      // Flip horizontally if needed
    s->set_special_effect(s, 0);              // No special effect
  }

  // WiFi.begin(CONFIG_WIFI_SSID, CONFIG_WIFI_PWD);
  
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected");

  init_face_detection();
  Serial.println("Face detection initialized.");

  ledcSetup(CONFIG_LED_LEDC_CHANNEL, CONFIG_FLASH_PWM_FREQ, CONFIG_FLASH_PWM_BITS);
  ledcAttachPin(CONFIG_FLASH_LED, CONFIG_LED_LEDC_CHANNEL);
  Serial.println("Flash LED configured.");

  Serial.println("Camera Ready!");
  Serial.print("Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect.");
}

void setup() {
  delay(5000);
  Serial.begin(CONFIG_BAUD);
  Serial.setDebugOutput(true);
  Serial.println("Starting configuration...");

  configESP();

  // Peripherals
  init_buzzer();
  init_lcd();
  init_pir();

  
  // startCameraServer();

  current_state = STATE_READY;

  // Show IP for connecting to the web server
  String ip_str = WiFi.localIP().toString();
  lcd_print(ip_str.c_str(), 1);

  lcd_print(READY_MSG, 0);
}

void loop() {
  update_alarm();

  if (current_state == STATE_READY) {
    if (check_pir()) {
      current_state = STATE_CAMERA_ON;
      lcd_print(CAMERA_ON_MSG, 0);
    }
  } else if (current_state == STATE_CAMERA_ON) {
    if (isFaceDetected()) {
      current_state = STATE_INTRUDER_DETECTED;
      lcd_print(INTRUDER_DETECTED_MSG, 0);
    }
  } else if (current_state == STATE_INTRUDER_DETECTED) {
    lcd_print(INTRUDER_DETECTED_MSG, 0);
    start_alarm(ALARM_DURATION, ALARM_TIME_BETWEEN);
    
    // if (!isFaceDetected()) {
    //   current_state = STATE_READY;
    //   lcd_print(READY_MSG, 0);
    // }
  }

  delay(200);
}

