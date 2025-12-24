add_executable(${NAME}
        main.cpp
        BlinkAgent.cpp
        Agent.cpp
        LogAgentA.cpp
        LogAgentB.cpp
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(${NAME} 
	 pico_stdlib
     FreeRTOS-Kernel-Heap4
     freertos_config
	)
	
target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
     )
     
# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# enable usb output, and uart output
pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 1)
target_compile_definitions(${NAME} PRIVATE
    AGENT_A_LOG_LEVEL=LOG_DEBUG
    AGENT_B_LOG_LEVEL=LOG_INFO
)
