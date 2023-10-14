cmake_minimum_required(VERSION 2.8.12)
project(ccl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(Eigen3 REQUIRED)

add_library(${PROJECT_NAME} SHARED src/ccl.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    Eigen3::Eigen
)


function(add_ccl_test base_name)
	add_executable(${base_name} test/${base_name}.cpp)
	target_link_libraries(${base_name} ${PROJECT_NAME})
    target_include_directories(${base_name}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
    # install(
    #     TARGETS ${base_name}
    #     DESTINATION ${PROJECT_NAME}
    # )
	# add_test(NAME ${base_name} COMMAND ${base_name})
endfunction()

add_ccl_test(first_scan)