/*
 * LogAgentA.h
 *
 * Agent to Log messages every 2 seconds
 *
 *  Created on: 11 Feb 2025
 *      Author: jondurrant
 */

#ifndef LOGAGENTA_H_
#define LOGAGENTA_H_

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Agent.h"

#ifndef AGENT_A_LOG_LEVEL
#define AGENT_A_LOG_LEVEL LOG_ERROR
#endif


class LogAgentA: public Agent {
public:
	LogAgentA();
	virtual ~LogAgentA();

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
};


#endif /* LOGAGENTA_H_ */
