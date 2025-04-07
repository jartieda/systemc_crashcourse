project(simple-cpu-model)
cmake_minimum_required(VERSION 3.10)



# Set the SystemC include and library directories
set(SYSTEMC_INCLUDE_DIR $ENV{SYSTEMC_HOME}/include)
set(SYSTEMC_LIBRARY_DIR $ENV{SYSTEMC_HOME}/lib)

include_directories(src/cpu)

file(GLOB CPU_SOURCES
    src/cpu/*.cpp
)

add_executable(simple_cpu_model ${CPU_SOURCES} src/main.cpp)

# Link against SystemC
target_include_directories(simple_cpu_model PRIVATE ${SYSTEMC_INCLUDE_DIR})
target_link_libraries(simple_cpu_model ${SYSTEMC_LIBRARY_DIR}/libsystemc.so)
