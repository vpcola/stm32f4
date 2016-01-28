#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

// For FreeRTOS to use the heap at CCM Ram
//
#ifdef __ARMCC_VERSION
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__((at( 0x10000000 )));
#else
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__((section(".ccmram")));
#endif
