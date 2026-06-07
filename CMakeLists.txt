cmake_minimum_required(VERSION 3.14)
project(NameSearch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    malena
    GIT_REPOSITORY https://github.com/daversmith/Malena.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(malena)

add_executable(${PROJECT_NAME}
    main.cpp
    App.cpp
    Scoring.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE Malena::Malena)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/names.txt"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/names.txt"
    COMMENT "Copying names.txt to output directory"
)
