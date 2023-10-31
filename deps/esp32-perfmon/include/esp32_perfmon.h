#ifndef COMPONENTS_PERFMON_INCLUDE_PERFMON_H_
#define COMPONENTS_PERFMON_INCLUDE_PERFMON_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

uint64_t cpu0_idle_calls() ;
uint64_t cpu1_idle_calls() ;
void reset_cpu0_idle_calls() ;
void reset_cpu1_idle_calls() ;
uint64_t cpu0_idle_last_calls() ;
uint64_t cpu1_idle_last_calls() ;

uint8_t cpu0_usage() ;
uint8_t cpu1_usage() ;

uint32_t max_calls_per_second() ;

esp_err_t perfmon_start();

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_PERFMON_INCLUDE_PERFMON_H_ */
