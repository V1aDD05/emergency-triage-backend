cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(HospitalTriage VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции
option(BUILD_TESTS "Build unit tests" ON)
option(USE_SQLITE "Use SQLite for persistence" OFF) #ON на 3-ем этапе

#---------------------------------------------------------
# Подключение внешних зависимостей при помощи FetchContent
#---------------------------------------------------------
include(FetchContent)

#cpp-httplib (header-only)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.14.0
)
FetchContent_MakeAvailable(httplib)

# nlohmann/json (header-only)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Google Test (только если BUILD_TESTS)
if(BUILD_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()

#SQLite (только если USE_SQLITE)
if(USE_SQLITE)
    find_package(SQLite3 REQUIRED)
endif()

#--------------------------
# Основной исполняемый файл
#--------------------------
# Библиотека с основной логикой (без main)
add_library(hospital_lib STATIC
    src/api/handlers.cpp
    src/core/triage.cpp
    src/storage/storage.cpp
    src/utils/json_utils.cpp
)

target_include_directories(hospital_lib PUBLIC src)
target_link_libraries(hospital_lib
    PUBLIC
        httplib
        nlohmann_json::nlohmann_json
)

if(USE_SQLITE)
    target_link_libraries(hospital_lib PUBLIC SQLite::SQLite3)
    target_compile_definitions(hospital_lib PUBLIC USE_SQLITE=1)
endif()

# Исполняемый файл (точка входа)
add_executable(hospital_triage src/main.cpp)
target_link_libraries(hospital_triage PRIVATE hospital_lib)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

