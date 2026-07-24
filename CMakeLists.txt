cmake_minimum_required(VERSION 3.10) # or keep 4.3 if your setup supports it
project(Lesbix C)

set(CMAKE_C_STANDARD 11)

# Add the src directory to the include search path so #include "uart.h" or similar works cleanly
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(Lesbix
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
        src/vga/vgatext.h)