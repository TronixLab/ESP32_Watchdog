#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <esp_task_wdt.h>

int WDT_TIMEOUT = 5;                  // WDT Timeout in seconds

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void vTask4(void *pvParameters);
void vTaskIdle(void *pvParameters);

int flag[] = {0, 0, 0, 0};

void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    vTask1                    // Task function
    ,  "Task1"                // Task name
    ,  1024                   // Stack size
    ,  NULL                   // Parameters passed to the task function
    ,  1                      // Priority
    ,  NULL                   // Task handle
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    vTask2                    // Task function
    ,  "Task2"                // Task name
    ,  1024                   // Stack size
    ,  NULL                   // Parameters passed to the task function
    ,  1                      // Priority
    ,  NULL                   // Task handle
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    vTask3                    // Task function
    ,  "Task3"                // Task name
    ,  1024                   // Stack size
    ,  NULL                   // Parameters passed to the task function
    ,  2                      // Priority
    ,  NULL                   // Task handle
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    vTask4                    // Task function
    ,  "Task4"                // Task name
    ,  1024                   // Stack size
    ,  NULL                   // Parameters passed to the task function
    ,  3                      // Priority
    ,  NULL                   // Task handle
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    vCheckFlagTask            // Task function
    ,  "CheckFlags"           // Task name
    ,  1024                   // Stack size
    ,  NULL                   // Parameters passed to the task function
    ,  3                      // Priority
    ,  NULL                   // Task handle
    ,  ARDUINO_RUNNING_CORE);
}

void loop() {
  // Empty. Things are done in Tasks.
}

void vTask1(void* pvParameters) {
  for (;;)
  {
    Serial.println("Task1");
    // Mimic the MCU hang event using Serial port
    if (Serial.available() > 0) {
      if (Serial.parseInt() == 1) {
        while (true) {
          Serial.println("MCU hang event!!!");
        }
      }
    }
    flag[0] = 1;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void vTask2(void* pvParameters) {
  for ( ;; )
  {
    Serial.println("Task2");
    // Mimic the MCU hang event using Serial port
    if (Serial.available() > 0) {
      if (Serial.parseInt() == 1) {
        while (true) {
          Serial.println("MCU hang event!!!");
        }
      }
    }
    flag[1] = 1;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void vTask3(void* pvParameters) {
  for ( ;; )
  {
    Serial.println("Task3");
    // Mimic the MCU hang event using Serial port
    if (Serial.available() > 0) {
      if (Serial.parseInt() == 1) {
        while (true) {
          Serial.println("MCU hang event!!!");
        }
      }
    }
    flag[2] = 1;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void vTask4(void* pvParameters) {
  for ( ;; )
  {
    Serial.println("Task4");
    // Mimic the MCU hang event using Serial port
    if (Serial.available() > 0) {
      if (Serial.parseInt() == 1) {
        while (true) {
          Serial.println("MCU hang event!!!");
        }
      }
    }
    flag[3] = 1;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void vCheckFlagTask(void* pvParameters) {
  for ( ;; )
  {
    // Check all flags if True then kick the dog
    if (flag[0] == 1 && flag[1] == 1 && flag[2] == 1 && flag[3] == 1) {
      // Reset all flags
      flag[0] = 0;
      flag[1] = 0;
      flag[2] = 0;
      flag[3] = 0;
      // Kick the dog
      WDT_TIMEOUT = 5;
    }
    else {
      WDT_TIMEOUT --;
      if (WDT_TIMEOUT == 0) {
        ESP.restart();
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
