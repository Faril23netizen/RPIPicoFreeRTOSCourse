/***
 * Demo program to light LEDs with FreeRTOS
 *
 * Modified by:
 * Name      : Faril Pirwanhadi
 * Student ID: M14128104
 *
 * Changes:
 * - Added Name and ID output
 * - Added Task for LED on GPIO 16
 * - Added Task for LED on GPIO 17
 */

#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <math.h>
#include "hardware/adc.h"

// Asumsi library ini ada di project Anda
#include "BlinkAgent.h"
#include "CounterAgent.h"
#include "DecoderAgent.h"
#include "IOAgent.h"


//Standard Task priority
#define TASK_PRIORITY       ( tskIDLE_PRIORITY + 1UL )

//LED PAD to use (Bawaan Program)
#define LED_PAD             0
#define LED1_PAD            2
#define LED2_PAD            3
#define LED3_PAD            4
#define LED4_PAD            5
#define LED5_PAD           15

// --- EDIT: Definisi 2 LED Tambahan ---
#define LED_EXTRA_1        16 // LED Tambahan Pertama
#define LED_EXTRA_2        17 // LED Tambahan Kedua


void runTimeStats(   ){
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;

   uxArraySize = uxTaskGetNumberOfTasks();
   // printf("Number of tasks %d\n", uxArraySize); // Commented to reduce spam

   pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

   if( pxTaskStatusArray != NULL ){
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

      // Print stats
      for( x = 0; x < uxArraySize; x++ )
      {
          printf("Task: %d \t %s \t Stack:%d\n",
                 pxTaskStatusArray[ x ].xTaskNumber ,
                 pxTaskStatusArray[ x ].pcTaskName,
                 pxTaskStatusArray[ x ].usStackHighWaterMark
                 );
      }
      vPortFree( pxTaskStatusArray );
   }
}

void saltRand(){
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
    int seed = adc_read();
    srand(seed);
}

// --- EDIT: Task untuk LED Tambahan 1 (GPIO 16) ---
// Berkedip setiap 500ms
void taskLedSatu(void *params){
    gpio_init(LED_EXTRA_1);
    gpio_set_dir(LED_EXTRA_1, GPIO_OUT);

    while(true){
        gpio_put(LED_EXTRA_1, 1);
        vTaskDelay(pdMS_TO_TICKS(500)); 
        gpio_put(LED_EXTRA_1, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// --- EDIT: Task untuk LED Tambahan 2 (GPIO 17) ---
// Berkedip lebih cepat (200ms) untuk membedakan
void taskLedDua(void *params){
    gpio_init(LED_EXTRA_2);
    gpio_set_dir(LED_EXTRA_2, GPIO_OUT);

    while(true){
        gpio_put(LED_EXTRA_2, 1);       // Nyala
        vTaskDelay(pdMS_TO_TICKS(200)); // Tunggu 200ms
        gpio_put(LED_EXTRA_2, 0);       // Mati
        vTaskDelay(pdMS_TO_TICKS(200)); // Tunggu 200ms
    }
}


/***
 * Main task to blink external LED
 */
void mainTask(void *params){
    BlinkAgent blink(LED_PAD);
    CounterAgent counter(LED1_PAD, LED2_PAD, LED3_PAD, LED4_PAD);
    DecoderAgent decoder(&counter);
    IOAgent ioAgent(&decoder, LED5_PAD);

    printf("Main task started\n");
    saltRand();

    blink.start("Blink", TASK_PRIORITY);
    counter.start("Counter", TASK_PRIORITY);
    decoder.start("Decode", TASK_PRIORITY);
    ioAgent.start("IO Agent", TASK_PRIORITY +1 );

    while (true) { 
        runTimeStats();
        vTaskDelay(5000); // Update stats tiap 5 detik
    }
}


/***
 * Launch the tasks and scheduler
 */
void vLaunch( void) {

    // 1. Task Utama
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);

    // --- EDIT: Mendaftarkan 2 Task LED Baru ---
    
    // Task untuk LED GP16 (Nama task: FarilLED1)
    xTaskCreate(taskLedSatu, "FarilLED1", 128, NULL, TASK_PRIORITY, NULL);

    // Task untuk LED GP17 (Nama task: FarilLED2)
    xTaskCreate(taskLedDua,  "FarilLED2", 128, NULL, TASK_PRIORITY, NULL);

    /* Start the tasks */
    vTaskStartScheduler();
}

/***
 * Main Program
 */
int main( void )
{
    stdio_init_all();
    sleep_ms(10000);

    // --- EDIT: Identitas ---
    printf("\n\n");
    printf("========================================\n");
    printf("Name       : Faril Pirwanhadi\n");
    printf("Student ID : M14128104\n");
    printf("========================================\n");
    printf("System GO\n");

    const char *rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);
    
    vLaunch();

    return 0;
}