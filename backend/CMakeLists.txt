add_library(hospital_backend STATIC
    src/api/router.cpp
    src/storage/storage.cpp
    src/storage/patient.cpp
    src/utils/json_serialisation.cpp
    src/core/priority_calculator_factory.cpp
    src/services/patient_service.cpp
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
