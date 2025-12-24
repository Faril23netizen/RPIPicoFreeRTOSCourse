/*
 * LogAgentB.h
 *
 * Agent to Log messages every 2 seconds
 *
 *  Created on: 11 Feb 2025
 *      Author: jondurrant
 */

#ifndef LOGAGENTB_H_
#define LOGAGENTB_H_

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Agent.h"

#ifndef AGENT_B_LOG_LEVEL
#define AGENT_B_LOG_LEVEL LOG_ERROR
#endif


class LogAgentB: public Agent {
public:
	LogAgentB();
	virtual ~LogAgentB();

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


#endif /* LOGAGENTB_H_ */
