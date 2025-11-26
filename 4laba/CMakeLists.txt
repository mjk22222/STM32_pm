@@ -51,12 +51,14 @@ add_definitions(-DSTM32F10X_MD)

include_directories(cmsis)
include_directories(stm32)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})  # ← ДОБАВИТЬ ЭТУ СТРОКУ

set(SOURCE_FILES
    stm32/startup/startup_stm32f10x_md.c # Choose startup file for your MCU
    stm32/startup/startup_stm32f10x_md.c
    stm32/system_stm32f10x.c
    syscalls/syscalls.c
    main.c)
    main.c
    ssd1306.c)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/.gdbinit DESTINATION ${EXECUTABLE_OUTPUT_PATH})

@@ -84,4 +86,4 @@ add_custom_command(OUTPUT flash_target
        COMMAND ${STFLASH} write ${BIN_TARGET} 0x8000000
        DEPENDS ${BIN_TARGET})

add_custom_target(flash DEPENDS flash_target)
add_custom_target(flash DEPENDS flash_target)
