cmake_minimum_required(VERSION 3.25)

project(load_model1)

set(CMAKE_CXX_STANDARD 17)

# get_filename_component(LIB_HEADERS lib REALPATH)

include_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/dependencies/include
    ${PROJECT_SOURCE_DIR}/vendors
)

file(GLOB LIB_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/lib/*.cpp")
add_library(lib STATIC ${LIB_SOURCES})

file(GLOB VENDOR_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendors/**/*.c" "${PROJECT_SOURCE_DIR}/vendors/**/*.cpp")
add_library(vendor STATIC ${VENDOR_SOURCES})


link_directories(${PROJECT_SOURCE_DIR}/dependencies/library)
# add_executable(${PROJECT_NAME} main.cpp)
# assmip和glfw是在link_directories自己找到的，lib和vendor是上边add_library指定的
# target_link_libraries(${PROJECT_NAME} assimp glfw lib vendor)
# add_dependencies(${PROJECT_NAME} lib vendor)

# file(GLOB VENDOR_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendors/**/*.c" "${PROJECT_SOURCE_DIR}/vendors/**/*.cpp")
# # file(GLOB VENDOR_HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendors/**/*.h" "${PROJECT_SOURCE_DIR}/vendors/**/*.hpp")
# add_library( vendorLib ${VENDOR_HEADERS} ${VENDOR_SOURCES} )
# foreach( testsourcefile ${VENDOR_SOURCES} )
#     message(STATUS ${testsourcefile})
# endforeach( testsourcefile ${VENDOR_SOURCES} )
file(GLOB SRC_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*.cpp")

foreach(EXECUTABLE_FILE ${SRC_SOURCES})
    SET(replaced "")
    string(REPLACE ".cpp" "" replaced ${EXECUTABLE_FILE})
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" replaced ${replaced})
    set(NAME ${replaced})
    add_executable(${NAME} ${EXECUTABLE_FILE})
    target_link_libraries(${NAME} assimp glfw lib vendor)
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${NAME}")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${NAME}")
endforeach(EXECUTABLE_FILE)