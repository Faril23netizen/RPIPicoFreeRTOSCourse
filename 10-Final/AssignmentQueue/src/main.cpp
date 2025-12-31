/***
 * Demo program to light 8 LEDs as binary random value
 * Uses FreeRTOS Task
 * Jon Durrant
 * Modified for 8 LEDs with Student Info
 */

#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <math.h>

#include "BlinkAgent.h"
#include "CounterAgent.h"


//Standard Task priority
#define TASK_PRIORITY       ( tskIDLE_PRIORITY + 1UL )

//LED PAD to use
#define LED_PAD             2

// 4 LED Pertama
#define LED1_PAD            3
#define LED2_PAD            4
#define LED3_PAD            5
#define LED4_PAD            6

// 4 LED Tambahan (Pastikan wiring sesuai GPIO ini)
#define LED5_PAD            7
#define LED6_PAD            8
#define LED7_PAD            9
#define LED8_PAD            10


void runTimeStats(   ){
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;


   // Get number of takss
   uxArraySize = uxTaskGetNumberOfTasks();
   printf("Number of tasks %d\n", uxArraySize);

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
          printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
                 pxTaskStatusArray[ x ].xTaskNumber ,
                 pxTaskStatusArray[ x ].uxCurrentPriority ,
                 pxTaskStatusArray[ x ].uxBasePriority ,
                 pxTaskStatusArray[ x ].usStackHighWaterMark ,
                 pxTaskStatusArray[ x ].pcTaskName
                 );
      }


      // Free array
      vPortFree( pxTaskStatusArray );
   } else {
       printf("Failed to allocate space for stats\n");
   }

   //Get heap allocation information
   HeapStats_t heapStats;
   vPortGetHeapStats(&heapStats);
   
   // --- BAGIAN INI SAYA AKTIFKAN KEMBALI ---
   printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
           heapStats.xAvailableHeapSpaceInBytes,
           heapStats.xNumberOfFreeBlocks,
           heapStats.xNumberOfSuccessfulAllocations,
           heapStats.xNumberOfSuccessfulFrees
           );
}


/***
 * Main task to blink external LED
 * @param params - unused
 */
void mainTask(void *params){
    BlinkAgent blink(LED_PAD);
    
    // Inisialisasi CounterAgent dengan 8 LED
    CounterAgent counter(LED1_PAD, LED2_PAD, LED3_PAD, LED4_PAD, 
                         LED5_PAD, LED6_PAD, LED7_PAD, LED8_PAD);

    printf("Main task started\n");

    blink.start("Blink", TASK_PRIORITY);
    counter.start("Counter", TASK_PRIORITY);

    while (true) { // Loop forever
        runTimeStats();
        
        printf("Name: Faril Pirwanhadi\n");
        printf("Student ID: M14128104\n");
        // ----------------------------------------

        // Random 8 bit (0-255)
        uint8_t r = rand() & 0xFF; 
        
        counter.blink(r);
        printf("Blinking Random Value: R=0x%X\n", r);
        
        // Delay 3000ms (3 detik)
        vTaskDelay(3000);
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
    const char *rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();


    return 0;
}