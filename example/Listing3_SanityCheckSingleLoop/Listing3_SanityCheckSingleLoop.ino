#include <esp_task_wdt.h>

#define WDT_TIMEOUT 4                   // WDT Timeout in seconds
int flag[] = {0, 0, 0};
void setup() {
  Serial.begin(115200);

  esp_task_wdt_init(WDT_TIMEOUT, true); // Initialize ESP32 Task WDT
  esp_task_wdt_add(NULL);               // Subscribe to the Task WDT
}

void loop() {
  Serial.println("Task 1");
  flag[0] = 1;
  delay(100);

  Serial.println("Task 2");
  flag[1] = 1;
  delay(100);

  Serial.println("Task 3");
  flag[2] = 1;
  delay(100);

  // Mimic the MCU hang event using Serial port
  if (Serial.available() > 0) {
    if (Serial.parseInt() == 1) {
      while (true) {
        Serial.println("MCU hang event!!!");
      }
    }
  }
  else {
    // Check all flags if True then kick the dog
    if (flag[0] == 1 && flag[1] == 1 && flag[2] == 1) {
      // Reset all flags
      flag[0] = 0;
      flag[1] = 0;
      flag[2] = 0;
      // Kick the dog
      esp_task_wdt_reset();
    }
  }
}
