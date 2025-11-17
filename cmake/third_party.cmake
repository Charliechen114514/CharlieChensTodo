# cmake/third_party.cmake
include(FetchContent)

# ------------------------------
# QMaterialCraft
# ------------------------------

set(QMATERIALCRAFT_DIR ${CMAKE_SOURCE_DIR}/third_party/QMaterialCraft)

message("Installing QMaterialCraft Source to the ${QMATERIALCRAFT_DIR}")

FetchContent_Declare(
    QMaterialCraft
    GIT_REPOSITORY https://github.com/Charliechen114514/QMaterialCraft.git
    GIT_TAG        main
    SOURCE_DIR     ${QMATERIALCRAFT_DIR}
)

FetchContent_MakeAvailable(QMaterialCraft)

set(PLAN_PARSER ${CMAKE_SOURCE_DIR}/third_party/plan_parser)
message("Installing plan-parser Source to the ${PLAN_PARSER}")

FetchContent_Declare(
    PlanParser
    GIT_REPOSITORY https://github.com/Charliechen114514/plan-parser.git
    GIT_TAG        main
    SOURCE_DIR     ${PLAN_PARSER}
)

FetchContent_MakeAvailable(PlanParser)

set(CCLoggerPlace ${CMAKE_SOURCE_DIR}/third_party/cclogger)
message("Installing CCLogger Source to the ${CCLoggerPlace}")

FetchContent_Declare(
    CCLoggerLibrary
    GIT_REPOSITORY https://github.com/CCUtilCommons/CCLogger.git
    GIT_TAG        main
    SOURCE_DIR     ${CCLoggerPlace}
)

FetchContent_MakeAvailable(CCLoggerLibrary)

