cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VERSION})

find_package(Boost REQUIRED COMPONENTS unit_test_framework program_options filesystem regex)

add_executable(bayan main.cpp)

add_executable(test_bayan test.cpp)

set_target_properties(bayan test_bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(bayan test_bayan PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(bayan
	${Boost_LIBRARIES}
)

target_link_libraries(test_bayan
	${Boost_LIBRARIES}
)
 
target_include_directories(bayan
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(bayan
    PRIVATE "${CMAKE_BINARY_DIR}/test_objects" 
)

target_include_directories(test_bayan
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(test_bayan
    PRIVATE "${CMAKE_BINARY_DIR}/test_objects" 
)

install(TARGETS bayan test_bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT alexsh93@live.ru)
include(CPack)

enable_testing()
add_test(test_bayan test_same_files_count)
add_test(test_bayan test_hash)
add_test(test_bayan copies)			
