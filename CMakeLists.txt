cmake_minimum_required(VERSION 3.10)
project(Lesbix C ASM)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# All files (C sources, assembly, and headers) are common to both targets
set(ALL_FILES
        src/commands/cat.c
        src/commands/cd.c
        src/commands/clear.c
        src/commands/echo.c
        src/commands/exit.c
        src/commands/hazelnut.c
        src/commands/help.c
        src/commands/ls.c
        src/commands/mkdir.c
        src/commands/pwd.c
        src/file/drivers/example_driver.c
        src/file/consts.c
        src/file/filesystem.c
        src/hazelnut/hazelnut_out.c
        src/hazelnut/hazelnut_vm.c
        src/tools/halt.c
        src/tools/utils.c
        src/vga/graphics.c
        src/vga/initialise.c
        src/vga/vga.c
        src/vga/vgatext.c
        src/cli/cli.c
        src/handle_command/handle_command.c
        src/main.c
        src/uart/uart.c
        src/startup.s
        src/tools/utils.h
        src/uart/uart.h
        src/file/filesystem.h
        src/file/drivers/example_driver.h
        src/file/consts.h
        src/commands/cat.h
        src/commands/cd.h
        src/tools/halt.h
        src/commands/clear.h
        src/commands/echo.h
        src/commands/exit.h
        src/commands/hazelnut.h
        src/commands/help.h
        src/commands/ls.h
        src/commands/mkdir.h
        src/commands/pwd.h
        src/cli/cli.h
        src/handle_command/handle_command.h
        src/hazelnut/hazelnut_vm.h
        src/hazelnut/hazelnut_out.h
        src/vga/graphics.h
        src/vga/initialise.h
        src/vga/vga.h
        src/vga/vgatext.h
)

# Branch configuration based on whether the ARM toolchain is active
if(CMAKE_SYSTEM_NAME STREQUAL "Generic")
    # ARM Bare-Metal Target
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm926ej-s -nostdlib -ffreestanding")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld -nostdlib")

    add_executable(kernel.elf ${ALL_FILES})

    add_custom_command(TARGET kernel.elf POST_BUILD
        COMMAND arm-none-eabi-objcopy -O binary kernel.elf kernel.bin
        COMMENT "Generating kernel.bin for QEMU..."
    )
else()
    # Desktop Host Target
    # Exclude bare-metal startup.s from desktop builds since it uses ARM assembly syntax
    list(REMOVE_ITEM ALL_FILES src/startup.s)
    add_executable(Lesbix ${ALL_FILES})
endif()
