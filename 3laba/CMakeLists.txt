@@ -1,10 +1,15 @@
cmake_minimum_required(VERSION 3.0)
cmake_minimum_required(VERSION 3.10)

# Disable compiler checking
# Otherwise it will fall with `reference to _exit() not found` error
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(stm32_template)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
