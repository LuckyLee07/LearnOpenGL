cmake_minimum_required(VERSION 3.0.0)
project(LearnOpenGL)
#set(PROJECT_NAME LearnOpenGL)

# 使用 C++ 11 标准
set(CMAKE_CXX_STANDARD 11)

# GLFW：添加头文件及目标链接
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW)
include_directories(${GLFW_DIR}/include)
link_libraries(${GLFW_DIR}/lib/libglfw.3.dylib)

# GLEW：添加头文件及目标链接
set(GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW)
include_directories(${GLEW_DIR}/include)
link_libraries(${GLEW_DIR}/lib/libGLEW.2.2.0.dylib)

#SET(CMAKE_BUILD_TYPE "Debug")  
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 执行编译命令
include_directories(src)
include_directories(src/vendor)
# 添加指定目录代码
aux_source_directory(src DIR_SRCS)
aux_source_directory(src/vendor/stb_image DIR_SRCS)
# 添加一个可编译的目标到工程
add_executable(${PROJECT_NAME} ${DIR_SRCS})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
