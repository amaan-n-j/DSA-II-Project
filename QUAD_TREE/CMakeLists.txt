cmake_minimum_required(VERSION 3.22)

list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu")

project(N-particle_simulation_using_Quadtree_for_collision_detection)

#option(USE_USER_VALUES "Use user-provided values for macros" OFF)

if(USE_USER_VALUES)
    set(MAX_VELOCITY_COMPONENT ${MAX_VELOCITY_COMPONENT} CACHE STRING "Maximum velocity component")
    set(MAX_MASS ${MAX_MASS} CACHE STRING "Maximum mass")
    set(MIN_MASS ${MIN_MASS} CACHE STRING "Minimum mass")
    set(MAX_RADIUS ${MAX_RADIUS} CACHE STRING "Maximum radius")
    set(MIN_RADIUS ${MIN_RADIUS} CACHE STRING "Minimum radius")
    set(PARTICLE_COUNT ${PARTICLE_COUNT} CACHE STRING "Particle count")
else()
# default values for the macros
    set(MAX_VELOCITY_COMPONENT 0.2)
    set(MAX_MASS 0.3)
    set(MIN_MASS 0.1)
    set(MAX_RADIUS 2.0)
    set(MIN_RADIUS 2.0)
    set(PARTICLE_COUNT 200)

endif()

# Configure the header file
configure_file(${CMAKE_SOURCE_DIR}/include/randomParticleGenerator.h.in ${CMAKE_SOURCE_DIR}/include/randomParticleGenerator.h)

# Setting a variale with path of the source files
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

#Getting all the source files from the source folder
file(GLOB SOURCES ${SOURCE_DIR}/*.c)

# Creaating an excetuable target
add_executable(sim ${SOURCES})

# Check for the OpenGL Libraries in the system
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

#setting compiler flags
set(COM_FLAGS -Ofast -Wall )

# Applying all the compiling flags 
foreach(flag ${COM_FLAGS})
	target_compile_options(sim PUBLIC ${flag})
endforeach()

#linking the flags
target_link_libraries(sim PUBLIC m)
target_link_libraries(sim PUBLIC GL GLU glut)

#Adding the include files to the target
target_include_directories(sim PUBLIC ${INCLUDE_DIR})

