#include "esp32_perfmon.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "sdkconfig.h"

#include "esp_log.h"
static const char *TAG = "perfmon";

static uint64_t idle0Calls = 0;
static uint64_t idle1Calls = 0;

static uint64_t lastIdle0Calls = 0;
static uint64_t lastIdle1Calls = 0;

static uint64_t _ticks_per_sec = 0 ;

// 实际统计值，见 js_process_stat_max_calls() 函数
static const uint64_t MaxIdleCalls = 400100;


uint8_t cpu0_usage() {
	return ((MaxIdleCalls-lastIdle0Calls) * 1000 / MaxIdleCalls + 5)/10 ;
}
uint8_t cpu1_usage() {
	return ((MaxIdleCalls-lastIdle1Calls) * 1000 / MaxIdleCalls + 5)/10 ;
}
uint64_t cpu0_idle_last_calls() {
	return lastIdle0Calls ;
}
uint64_t cpu1_idle_last_calls() {
	return lastIdle1Calls ;
}

uint64_t cpu0_idle_calls() {
	return idle0Calls ;
}
uint64_t cpu1_idle_calls() {
	return idle1Calls ;
}

void reset_cpu0_idle_calls() {
	idle0Calls = 0;
}
void reset_cpu1_idle_calls() {
	idle1Calls = 0;
}

uint32_t max_calls_per_second() {
	return MaxIdleCalls ;
}

IRAM_ATTR static bool idle_task_0() {
	idle0Calls ++ ;
	return false;
}

IRAM_ATTR static bool idle_task_1() {
	idle1Calls ++ ;
	return false;
}


static void perfmon_task(void *args) {
	while (1)
	{
		lastIdle0Calls = idle0Calls;
		lastIdle1Calls = idle1Calls;

		idle0Calls = 0;
		idle1Calls = 0;
		
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

esp_err_t perfmon_start()
{
	// return ESP_OK ;
	ESP_ERROR_CHECK(esp_register_freertos_idle_hook_for_cpu(idle_task_0, 0));
	ESP_ERROR_CHECK(esp_register_freertos_idle_hook_for_cpu(idle_task_1, 1));
	// TODO calculate optimal stack size
	xTaskCreate(perfmon_task, "perfmon", 1024, NULL, 1, NULL);
	return ESP_OK;
}
