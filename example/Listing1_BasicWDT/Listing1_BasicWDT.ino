#include <esp_task_wdt.h>

#define WDT_TIMEOUT 3                   // WDT Timeout in seconds

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started.");
  delay(2000);
  esp_task_wdt_init(WDT_TIMEOUT, true); // Initialize ESP32 Task WDT
  esp_task_wdt_add(NULL);               // Subscribe to the Task WDT
}

void loop() {
  Serial.println("LOOP started ! ");
  for (int i = 0; i <= 10; i++) {
    Serial.print("Task: ");
    Serial.println(i);
    delay(1000);
    // Kick the dog
    esp_task_wdt_reset();
  }
  while (1) {
    Serial.println("MCU hang event!!!");
  }
}
