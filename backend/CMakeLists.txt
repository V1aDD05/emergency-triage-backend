add_library(hospital_backend STATIC
    src/api/handlers.cpp
    src/core/triage.cpp
    src/storage/storage.cpp
    src/storage/patient.cpp
    src/utils/json_serialisation.cpp
)

target_include_directories(hospital_backend 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(hospital_backend
    PUBLIC
        httplib
        nlohmann_json::nlohmann_json
        spdlog
)

if(USE_SQLITE)
    target_link_libraries(hospital_backend PUBLIC SQLite::SQLite3)
    target_compile_definitions(hospital_backend PUBLIC USE_SQLITE=1)
endif()
