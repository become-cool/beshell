#include "thread.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

// 函数执行的任务
static void run_lambda_on_core(void *params) {
    // 解包传入的参数（包含信号量和函数）
    auto data = (std::pair<std::function<void()>, SemaphoreHandle_t>*)params;
    auto func = data->first;                   // lambda 函数
    SemaphoreHandle_t doneSemaphore = data->second; // 信号量

    // 执行 lambda 函数
    func();

    // 释放信号量，表示任务已完成
    xSemaphoreGive(doneSemaphore);

    // 删除任务
    vTaskDelete(NULL);
}

void run_wait_on_core (std::function<void()> func, uint8_t core_id) {
    // 创建信号量
    SemaphoreHandle_t doneSemaphore = xSemaphoreCreateBinary();

    // 将 lambda 和信号量打包到一起
    auto params = new std::pair<std::function<void()>, SemaphoreHandle_t>(func, doneSemaphore);

    // 创建绑定到指定 CPU 核心的任务
    xTaskCreatePinnedToCore(run_lambda_on_core, "lambdaTask", 2048, params, 5, NULL, (BaseType_t)core_id);

    // 等待信号量释放（等待任务完成）
    xSemaphoreTake(doneSemaphore, portMAX_DELAY);

    // 删除信号量
    vSemaphoreDelete(doneSemaphore);

    // 释放参数内存
    delete params;
}