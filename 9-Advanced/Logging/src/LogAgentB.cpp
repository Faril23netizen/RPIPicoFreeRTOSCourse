/*
 * LogAgentB.cpp
 *
 *  Created on: 11 Feb 2025
 *      Author: jondurrant
 */

#include "LogAgentB.h"

#define LIBRARY_LOG_NAME "Agent B"
#define LIBRARY_LOG_LEVEL AGENT_B_LOG_LEVEL
#include <logging_stack.h>


LogAgentB::LogAgentB() {
	LogDebug(("Constructor"));

}

LogAgentB::~LogAgentB() {
	// NOP
}

void LogAgentB::run(){
	LogInfo(("Starting"));
	for (;;){
		LogDebug(("In loop"));
		LogInfo(("Loop"));
		vTaskDelay(2000);
	}
}


configSTACK_DEPTH_TYPE LogAgentB::getMaxStackSize(){
	return 256;
}
