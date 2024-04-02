cmake_minimum_required(VERSION 3.15)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name.
set(PROJECT_NAME ga_lib)
project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 17)

##########################################################
# User configurable options of the template
##########################################################

#! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
#  but sometimes it creates too much trouble, so default is OFF.
option(WARNINGS_AS_ERRORS
        "Promotes compile-time warnings to errors that prevent the successful builds" OFF)

#! Always use PVS Studio while developing.
option(ENABLE_PVS_STUDIO "PVS Studio code analyser" OFF)

#! Select appropriate sanitizers.
#  Definitely enable sanitizers while developing.
#  Disable it for the production builds and before submitting for grading.
#  Only one of Memory (MSAN), Address (ASAN), or Thread (TSan)
#  sanitizers is applicable at the time -- the first defined.

#! UndefinedBehaviorSanitizer (UBSan).
#  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
set(ENABLE_UBSan OFF)
#! AddressSanitizer -- detects use after free or after scope exit,
#  memory overflows and leaks.
#  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
set(ENABLE_ASAN OFF)
#! ThreadSanitizer -- detects data races.
#  Info: https://clang.llvm.org/docs/ThreadSanitizer.html
set(ENABLE_TSan OFF)
#! MemorySanitizer -- detects uninitialized memory reads
#  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
set(ENABLE_MSAN OFF)


#! Be default -- build release version if not specified otherwise.
option(CMAKE_BUILD_TYPE
        "Select the build optimization type Debug/RelseaseWithDebugInfo/Release" Release)

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)

##########################################################
# Project files, packages, libraries and so on
##########################################################

#! Project main executable source compilation
#add_executable(held-carp held-carp.cpp timer.h)
###### uncomment ######

add_executable(held-carp_parallel held-carp_parallel.cpp timer.h TreadSafeQ.h)

###### uncomment ######
add_executable(TSP_GA timer.h TSP_GA.cpp TreadSafeQ.h)
add_executable(TSP_GA_parallel timer.h TreadSafeQ.h TPool.h TPool.cpp TSP_GA_parallel.cpp)
add_executable(GA test_lib.cpp GeneticAlgorithm.cpp GeneticAlgorithm.hpp TPool.h TPool.cpp timer.h)

#! Put path to your project headers
#target_include_directories(${PROJECT_NAME} PRIVATE options_parser)

###### uncomment ######

find_package(TBB REQUIRED)
target_link_libraries(held-carp_parallel PRIVATE TBB::tbb)

###### uncomment ######

#! Add external packages
# options_parser requires boost::program_options library
#find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
#target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system)


##########################################################
# Fixed CMakeLists.txt part
##########################################################

###### uncomment ######

#INSTALL(PROGRAMS
#        $<TARGET_FILE:held-carp>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
#        DESTINATION bin)

###### uncomment ######

# Define ALL_TARGETS variable to use in PVS and Sanitizers

###### uncomment ######

set(ALL_TARGETS GA)

###### uncomment ######

# Include CMake setup
include(cmake/main-config.cmake)