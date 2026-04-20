# Minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Project name and language
project(GestionTarifsVols CXX)

# Force C++17 standard (required for std::optional)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include header directory
include_directories(include)

# --- PART 1 : MAIN EXECUTABLE ---
# List application source files
set(APP_SOURCES
    src/FlightNbr.cpp
    src/GestionnaireCSV.cpp
    src/OD.cpp
    src/Tarif.cpp
)

# Create main executable
add_executable(main_app main.cpp ${APP_SOURCES})

# --- PART 2 : UNIT TESTS WITH GOOGLETEST ---
# Download GoogleTest via Git (more robust against network timeouts)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(tests_app 
    tests/test_Tarif.cpp 
    tests/test_OD.cpp
    tests/test_FlightNbr.cpp
    tests/test_GestionnaireCSV.cpp
    ${APP_SOURCES}
)

# Link GoogleTest to test executable
target_link_libraries(tests_app gtest_main)

# Enable CTest module
include(CTest)
add_test(NAME AllTests COMMAND tests_app)