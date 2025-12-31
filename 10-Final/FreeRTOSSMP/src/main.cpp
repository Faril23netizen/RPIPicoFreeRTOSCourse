/***
 * Demo program to use FreeRTOS SMP on Both Cores
 * Blink on Core 0 to GPIO 2
 * Blink on Core 1 to GPIO 11 (Moved from 7)
 * Counter showing on GPIO 3 to 10 (8 LEDs), using Core 1
 * Instructions sent to Counter from Main Task on Core 0
 * Jon Durrant
 * Modified for 8 LEDs with Student Info
 */

#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <math.h>
#include "hardware/structs/sio.h"

#include "BlinkAgent.h"
#include "CounterAgent.h"


//Standard Task priority
#define TASK_PRIORITY       ( tskIDLE_PRIORITY + 1UL )

//LED PAD to use
#define LED_BLINK0_PAD      2

// 8 LEDs for Counter (GPIO 3 - 10)
#define LED1_PAD            3
#define LED2_PAD            4
#define LED3_PAD            5
#define LED4_PAD            6
#define LED5_PAD            7
#define LED6_PAD            8
#define LED7_PAD            9
#define LED8_PAD            10

// Blink 1 moved to GPIO 11 to make room for counter
#define LED_BLINK1_PAD      11 


void runTimeStats(   ){
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;


   // Get number of takss
   uxArraySize = uxTaskGetNumberOfTasks();
   // printf("Number of tasks %d\n", uxArraySize);

   //Allocate a TaskStatus_t structure for each task.
   pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

   if( pxTaskStatusArray != NULL ){
      // Generate raw status information about each task.
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                                 uxArraySize,
                                 &ulTotalRunTime );

      // Print stats
      for( x = 0; x < uxArraySize; x++ )
      {
          /*
          printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
                 pxTaskStatusArray[ x ].xTaskNumber ,
                 pxTaskStatusArray[ x ].uxCurrentPriority ,
                 pxTaskStatusArray[ x ].uxBasePriority ,
                 pxTaskStatusArray[ x ].usStackHighWaterMark ,
                 pxTaskStatusArray[ x ].pcTaskName
                 );
          */
      }


      // Free array
      vPortFree( pxTaskStatusArray );
   } else {
       printf("Failed to allocate space for stats\n");
   }

   //Get heap allocation information
   HeapStats_t heapStats;
   vPortGetHeapStats(&heapStats);
   /*
   printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
           heapStats.xAvailableHeapSpaceInBytes,
           heapStats.xNumberOfFreeBlocks,
           heapStats.xNumberOfSuccessfulAllocations,
           heapStats.xNumberOfSuccessfulFrees
           );
    */
}


/***
 * Main task to blink external LED
 * @param params - unused
 */
void mainTask(void *params){
    BlinkAgent blink(LED_BLINK0_PAD);
    BlinkAgent blink1(LED_BLINK1_PAD);
    
    // Inisialisasi Counter dengan 8 PIN
    CounterAgent counter(LED1_PAD, LED2_PAD, LED3_PAD, LED4_PAD,
                         LED5_PAD, LED6_PAD, LED7_PAD, LED8_PAD);


    printf("Main task started\n");

    blink.start("Blink 0", TASK_PRIORITY);
    blink1.start("Blink 1", TASK_PRIORITY);
    counter.start("Counter", TASK_PRIORITY);

    //Bind to CORE 1
    UBaseType_t coreMask = 0x2;
    vTaskCoreAffinitySet( blink1.getTask(), coreMask );
    vTaskCoreAffinitySet( counter.getTask(), coreMask );

    //Bind to CORE 0
    coreMask = 0x1;
    vTaskCoreAffinitySet( blink.getTask(), coreMask );


    while (true) { // Loop forever
        runTimeStats();
        
        printf("\n----------------------------------------\n");
        // --- DATA MAHASISWA (Berulang & Beda Baris) ---
        printf("Name      : Faril Pirwanhadi\n");
        printf("Student ID: M14128104\n");
        // ----------------------------------------------
        
        printf("Main executing on Core %ld\n", sio_hw->cpuid);

        // Random 0-255 (8 bit full)
        uint8_t r = rand() & 0xFF;
        
        printf("Counter Value: %d (0x%X)\n", r, r);
        printf("----------------------------------------\n");

        counter.on(r);
        vTaskDelay(3000); // Delay 3 detik sebelum ulang
    }
}




/***
 * Launch the tasks and scheduler
 */
void vLaunch( void) {

    //Start blink task
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

/***
 * Main
 * @return
 */
int main( void )
{
    //Setup serial over USB and give a few seconds to settle before we start
    stdio_init_all();
    sleep_ms(2000);
    printf("GO\n");

    //Start tasks and scheduler

    const char *rtos_name;
#if ( portSUPPORT_SMP == 1 )
    rtos_name = "FreeRTOS SMP";
#else
    rtos_name = "FreeRTOS";
#endif

#if ( portSUPPORT_SMP == 1 ) && ( configNUM_CORES == 2 )
    printf("%s on both cores:\n", rtos_name);
#else
    printf("Starting %s on core 0:\n", rtos_name);
#endif

    vLaunch();


    return 0;
}