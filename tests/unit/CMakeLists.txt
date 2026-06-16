add_executable(unit_tests test_triage.cpp test_storage.cpp)
target_link_libraries(unit_tests PRIVATE hospital_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
