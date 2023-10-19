cmake_minimum_required(VERSION 3.5)
project(ssb-ccl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(Eigen3 REQUIRED)

add_library(${PROJECT_NAME} SHARED src/ssb_ccl.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    Eigen3::Eigen
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_export
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

install(
    EXPORT  ${PROJECT_NAME}_export
    FILE    ${PROJECT_NAME}-config.cmake
    DESTINATION share/cmake/${PROJECT_NAME}
    EXPORT_LINK_INTERFACE_LIBRARIES
)

install(
  DIRECTORY include/
  DESTINATION include
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
add_ccl_test(backward_scan)