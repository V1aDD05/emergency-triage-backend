add_library(hospital_backend STATIC
    api/handlers.cpp
    core/triage.cpp
    storage/storage.cpp
    storage/patient.cpp
    utils/json_serialisation.cpp
)

target_include_directories(hospital_backend 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(hospital_backend
    PUBLIC
        httplib
        nlohmann_json::nlohmann_json
)

if(USE_SQLITE)
    target_link_libraries(hospital_backend PUBLIC SQLite::SQLite3)
    target_compile_definitions(hospital_backend PUBLIC USE_SQLITE=1)
endif()
