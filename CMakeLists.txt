cmake_minimum_required(VERSION 3.19)
project(kernel NONE)

enable_language(C CXX ASM)
set(CMAKE_CXX_STANDARD 20)

add_executable(kernel src/kernel/main.cpp src/boot/boot.s)

target_compile_options(kernel PRIVATE -nostdlibinc -nostdlib -ffreestanding -target x86_64-unknown-gnu -mno-red-zone -Wall -Wextra -msoft-float -mno-mmx -mno-sse -mcmodel=large -fno-pie)
target_link_options(kernel PRIVATE -T "${CMAKE_CURRENT_SOURCE_DIR}/tools/kernel.lds" -nostdlib -target x86_64-unknown-gnu -fuse-ld=lld -Wl,--no-pie -Wl,--no-dynamic-linker)
set_target_properties(kernel PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/tools/kernel.lds")
