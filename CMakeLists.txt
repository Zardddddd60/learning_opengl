cmake_minimum_required(VERSION 3.25)

project(load_model)

set(CMAKE_CXX_STANDARD 17)

file(GLOB VENDOR_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendors/**/*.c" "${PROJECT_SOURCE_DIR}/vendors/**/*.cpp")
file(GLOB SRC_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*.cpp")
link_directories(${PROJECT_SOURCE_DIR}/dependencies/library)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include)
add_executable(${PROJECT_NAME} ${VENDOR_SOURCES} ${SRC_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw)

# file(GLOB VENDOR_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendors/**/*.c" "${PROJECT_SOURCE_DIR}/vendors/**/*.cpp")
# # file(GLOB VENDOR_HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendors/**/*.h" "${PROJECT_SOURCE_DIR}/vendors/**/*.hpp")
# add_library( vendorLib ${VENDOR_HEADERS} ${VENDOR_SOURCES} )
# foreach( testsourcefile ${VENDOR_SOURCES} )
#     message(STATUS ${testsourcefile})
# endforeach( testsourcefile ${VENDOR_SOURCES} )