cmake_minimum_required(VERSION 3.17)
project(kernel NONE)

enable_language(C CXX ASM)
set(CMAKE_CXX_STANDARD 20)

add_executable(kernel src/kernel/interrupt/idt/manager/IDTManager.cpp src/kernel/main.cpp src/boot/cpuid.s src/boot/paging.s src/boot/boot.s src/boot/main.s src/kernel/display/Display.cpp src/kernel/display/Display.h src/kernel/io/IO.cpp src/kernel/io/IO.h src/kernel/asm/asm.h src/kernel/scheduler/Timer.cpp src/kernel/scheduler/Timer.h src/kernel/interrupt/pic/PIC.cpp src/kernel/interrupt/pic/PIC.h src/kernel/interrupt/idt/handler/IDTHandler.cpp src/kernel/interrupt/idt/handler/IDTHandler.h)

target_compile_options(kernel PRIVATE -g -nostdlibinc -nostdlib -ffreestanding -target x86_64-unknown-gnu -mno-red-zone -Wall -Wextra -msoft-float -mno-mmx -mno-sse -mcmodel=large -fno-pie -masm=intel)
target_link_options(kernel PRIVATE -g -Wl,-T "${CMAKE_CURRENT_SOURCE_DIR}/tools/kernel.lds" -nostdlib -target x86_64-unknown-gnu -fuse-ld=lld -Wl,--no-pie -Wl,--no-dynamic-linker)
set_target_properties(kernel PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/tools/kernel.lds")
