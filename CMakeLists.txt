cmake_minimum_required(VERSION 3.10)
project(Lesbix C ASM)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Core sources shared across targets
set(CORE_C_SRCS
    src/commands/cat.c
    src/commands/cd.c
    src/commands/clear.c
    src/commands/echo.c
    src/commands/exit.c
    src/commands/hazelnut.c
    src/commands/help.c
    src/commands/ls.c
    src/commands/mkdir.c
    src/commands/basic.c
    src/commands/pwd.c
    src/file/drivers/example_driver.c
    src/file/consts.c
    src/file/error_message.c
    src/file/filesystem.c
    src/hazelnut/hazelnut_out.c
    src/hazelnut/hazelnut_vm.c
    src/tools/halt.c
    src/tools/utils.c
    src/graphics/vga/graphics_vga.c
    src/graphics/vga/initialise.c
    src/graphics/vga/vga.c
    src/graphics/vga/vgatext.c
    src/cli/cli.c
    src/handle_command/handle_command.c
    src/main.c
    src/uart/uart.c

    src/basic/basic.c
)

set(HEADERS
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
    src/commands/basic.h
    src/cli/cli.h
    src/handle_command/handle_command.h
    src/hazelnut/hazelnut_vm.h
    src/hazelnut/hazelnut_out.h
    src/graphics/vga/graphics_vga.h
    src/graphics/vga/initialise.h
    src/graphics/vga/vga.h
    src/graphics/vga/vgatext.h

    src/basic/basic.h
)

# ------------------------------------------------------------------------------
# Target Selection
# ------------------------------------------------------------------------------

if(TARGET_CAT65)
    # ==========================================================================
    # 65C02 / CC65 Build Target
    # ==========================================================================
    message(STATUS "Configuring build for 65C02 (Cat65 CC65 Toolchain)...")

    find_program(CC65_BIN cc65 REQUIRED)
    find_program(CA65_BIN ca65 REQUIRED)
    find_program(LD65_BIN ld65 REQUIRED)

    set(CAT65_CFG "${CMAKE_CURRENT_SOURCE_DIR}/build-config/cat65/config.cfg")
    set(CAT65_CRT0 "${CMAKE_CURRENT_SOURCE_DIR}/build-config/cat65/crt0.s")

    # Assemble crt0.s startup file
    set(CRT0_OBJ "${CMAKE_CURRENT_BINARY_DIR}/crt0.o")
    add_custom_command(
        OUTPUT ${CRT0_OBJ}
        COMMAND ${CA65_BIN} --cpu 65C02 ${CAT65_CRT0} -o ${CRT0_OBJ}
        DEPENDS ${CAT65_CRT0}
        COMMENT "Assembling crt0.s for 65C02..."
    )

    set(ALL_6502_OBJS ${CRT0_OBJ})

    # Rule: Process each C source through cc65 -> ca65 -> .o
    foreach(c_file ${CORE_C_SRCS})
        get_filename_component(fname ${c_file} NAME_WE)
        set(asm_out "${CMAKE_CURRENT_BINARY_DIR}/${fname}.s")
        set(obj_out "${CMAKE_CURRENT_BINARY_DIR}/${fname}.o")

        add_custom_command(
            OUTPUT ${obj_out}
            COMMAND ${CC65_BIN} --cpu 65C02 -O -I ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/${c_file} -o ${asm_out}
            COMMAND ${CA65_BIN} --cpu 65C02 ${asm_out} -o ${obj_out}
            DEPENDS ${c_file} ${HEADERS}
            COMMENT "Compiling & Assembling (65C02): ${c_file}"
        )
        list(APPEND ALL_6502_OBJS ${obj_out})
    endforeach()

    # Link step via ld65
    add_custom_target(cat65 ALL
        COMMAND ${LD65_BIN} -C ${CAT65_CFG} ${ALL_6502_OBJS} none.lib -o ${CMAKE_CURRENT_BINARY_DIR}/program.c65
        DEPENDS ${ALL_6502_OBJS}
        COMMENT "Linking 65C02 binary with ld65 -> program.c65"
    )

elseif(CMAKE_SYSTEM_NAME STREQUAL "Generic")
    # ==========================================================================
    # ARM Bare-Metal Target
    # ==========================================================================
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm926ej-s -nostdlib -ffreestanding")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld -nostdlib")

    add_executable(kernel.elf
        ${CORE_C_SRCS}
        ${HEADERS}
        src/startup.s
        src/graphics/graphics.c
        src/graphics/xlib_driver/xlib_driver.c
        src/graphics/xlib_driver/xlib_driver.h
    )

    add_custom_command(TARGET kernel.elf POST_BUILD
        COMMAND arm-none-eabi-objcopy -O binary kernel.elf kernel.bin
        COMMENT "Generating kernel.bin for QEMU..."
    )

else()
    # ==========================================================================
    # Desktop Host Target
    # ==========================================================================
    add_executable(Lesbix
        ${CORE_C_SRCS}
        ${HEADERS}
        src/graphics/graphics.c
        src/file/drivers/linux_file_driver.c
        src/file/drivers/linux_file_driver.h

    )
endif()
