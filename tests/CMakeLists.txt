# Тесты для логики сортировки
add_executable(triage_tests test_triage.cpp)
target_link_libraries(triage_tests PRIVATE hospital_lib gtest_main)

# Тесты для работы с данными
add_executable(storage_tests test_storage.cpp)
target_link_libraries(storage_tests PRIVATE hospital_lib gtest_main)

# Регистрация тестов
include(GoogleTest)
gtest_discover_tests(triage_tests)
gtest_discover_tests(storage_tests)