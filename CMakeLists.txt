cmake_minimum_required(VERSION 3.5)


set(PROJECT_NAME Checkers)
set(EXECUTABLE out)
set(CXX_STANDARD 17)

set(LIBRARIES GL GLEW glfw)
set(INCLUDE_DIRECTORY 	${CMAKE_SOURCE_DIR}/include resource/libs/stb_image/)
file(GLOB SOURCES "src/*.cpp")
file(GLOB LIB_SOURCES "resource/libs/*/*.cpp")


include_directories(${INCLUDE_DIRECTORY})

project(${PROJECT_NAME})

add_executable(${EXECUTABLE} ${SOURCES} ${LIB_SOURCES})
set_target_properties(${EXECUTABLE} PROPERTIES 
	CXX_STANDARD ${CXX_STANDARD})

target_link_libraries(${EXECUTABLE} ${LIBRARIES})