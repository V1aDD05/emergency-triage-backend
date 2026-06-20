set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend)

add_executable(unit_tests test_triage.cpp test_storage.cpp)
target_link_libraries(unit_tests PRIVATE hospital_backend gtest_main)
target_include_directories(unit_tests PRIVATE ${SRC_DIR})

include(GoogleTest)
gtest_discover_tests(unit_tests)
