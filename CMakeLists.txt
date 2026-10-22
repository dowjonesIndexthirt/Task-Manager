cmake_minimum_required(VERSION 3.28)

project(TerrGen)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Enable compile_commands.json (clang-tidy needs it)

# main code
add_executable(procReader src/main.cpp src/procRead.cpp)

# testing
# add_executable(unit_test src/unit_test.cpp src/helloworld.cpp)

# enable_testing()

# add_test(
#     NAME unit_test
#     COMMAND $<TARGET_FILE:unit_test>
# )

# Compiler warnings
target_compile_options(procReader PRIVATE
  $<$<CXX_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

option(ENABLE_WERROR "Treat warnings as errors" ON)
if(ENABLE_WERROR)
  target_compile_options(procReader PRIVATE
    $<$<CXX_COMPILER_ID:Clang,GNU>:-Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
  )
endif()

