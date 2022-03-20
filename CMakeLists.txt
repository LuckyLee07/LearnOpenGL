cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME LearnOpenGL)

# 使用 C++ 11 标准
set(CMAKE_CXX_STANDARD 11)

# GLFW第三方库目录
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW)

# 添加头文件及目标链接
include_directories(${GLFW_DIR}/include)
link_libraries(${GLFW_DIR}/lib/libglfw.3.dylib)

#SET(CMAKE_BUILD_TYPE "Debug")  
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 执行编译命令
aux_source_directory(src DIR_SRCS)
# 添加一个可编译的目标到工程
add_executable (${PROJECT_NAME} ${DIR_SRCS})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
