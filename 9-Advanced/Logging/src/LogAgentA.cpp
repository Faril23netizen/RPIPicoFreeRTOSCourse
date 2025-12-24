/*
 * LogAgentA.cpp
 *
 *  Created on: 11 Feb 2025
 *      Author: jondurrant
 */

#include "LogAgentA.h"

#define LIBRARY_LOG_NAME "Agent A"
#define LIBRARY_LOG_LEVEL AGENT_A_LOG_LEVEL
#include <logging_stack.h>


LogAgentA::LogAgentA() {
	LogDebug(("Constructor"));

}

LogAgentA::~LogAgentA() {
	// NOP
}

void LogAgentA::run(){
	LogInfo(("Starting"));
	for (;;){
		LogDebug(("In loop"));
		LogInfo(("Loop"));
		vTaskDelay(2000);
	}
}


configSTACK_DEPTH_TYPE LogAgentA::getMaxStackSize(){
	return 256;
}
