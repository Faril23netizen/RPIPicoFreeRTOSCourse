/*
 * CounterAgent.h
 *
 * Show binary view of count on LEDs
 *
 * Created on: 22 Aug 2022
 * Author: jondurrant
 */

#ifndef COUNTERAGENT_H_
#define COUNTERAGENT_H_

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Agent.h"

// Ubah menjadi 8 LED
#define COUNT_LEDS          8 

#define COUNT_QUEUE_LEN     10
#define COUNT_BLINK_DELAY   500

class CounterAgent : public Agent {
public:

    /***
     * Contructor
     * @param gp1 GPIO PAD for 1st LED - units (Bit 0)
     * @param gp2 GPIO PAD for 2nd LED - 2 (Bit 1)
     * @param gp3 GPIO PAD for 3rd LED - 4 (Bit 2)
     * @param gp4 GPIO PAD for 4th LED - 8 (Bit 3)
     * @param gp5 GPIO PAD for 5th LED - 16 (Bit 4)
     * @param gp6 GPIO PAD for 6th LED - 32 (Bit 5)
     * @param gp7 GPIO PAD for 7th LED - 64 (Bit 6)
     * @param gp8 GPIO PAD for 8th LED - 128 (Bit 7)
     */
    CounterAgent(uint8_t gp1=0, uint8_t gp2=0, uint8_t gp3=0, uint8_t gp4=0,
                 uint8_t gp5=0, uint8_t gp6=0, uint8_t gp7=0, uint8_t gp8=0);

    /***
     * Destructor
     */
    virtual ~CounterAgent();

    /***
     * Turn LEDs on and display count
     * @param count - between 0 and 0xFF
     */
    virtual void on(uint8_t count);

    /***
     * Turn LEDs off
     */
    virtual void off();

    /***
     * Blink LEDs with displayed count
     * @param count - between 0 and 0xFF
     */
    virtual void blink(uint8_t count);

protected:

    /***
     * Run loop for the agent.
     */
    virtual void run();


    /***
     * Get the static depth required in words
     * @return - words
     */
    virtual configSTACK_DEPTH_TYPE getMaxStackSize();

    /***
     * Initialise the LEDs
     */
    virtual void init();


    /***
     * Set the LEDs to the mask in count
     * @param count
     */
    virtual void setLeds(uint8_t count);

    //GPIO PAD for LED
    uint8_t pLedPads[COUNT_LEDS];

    //Queue of commands
    QueueHandle_t xCmdQ;

};

#endif /* COUNTERAGENT_H_ */