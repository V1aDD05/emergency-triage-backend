set(SRC_DIR ${PROJECT_SOURCE_DIR}/backend/src)

add_executable(unit_tests test_triage.cpp test_storage.cpp)
target_link_libraries(unit_tests PRIVATE hospital_backend gtest_main)
target_include_directories(unit_tests PRIVATE ${SRC_DIR})

include(GoogleTest)
gtest_discover_tests(unit_tests)
