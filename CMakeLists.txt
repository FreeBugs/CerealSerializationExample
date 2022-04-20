cmake_minimum_required(VERSION 3.21)
set(project_name manual_sequence_cereal)
project(${project_name})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(ENV{CONAN_REVISIONS_ENABLED} 1)
include(conan)

set(CONAN_SYSTEM_INCLUDES On)

conan_cmake_run(
        REQUIRES
        cereal/1.3.0
        BASIC_SETUP
        CMAKE_TARGETS
        BUILD missing
)

add_executable(${project_name} main.cpp)

target_link_libraries(${project_name}
        PUBLIC CONAN_PKG::cereal
        )