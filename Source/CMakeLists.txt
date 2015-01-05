# Set project name
project (FlowerPixieTsumiki)

# Set minimum version
cmake_minimum_required (VERSION 2.8.6)

if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        cmake_policy (SET CMP0022 NEW) # INTERFACE_LINK_LIBRARIES defines the link interface
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        cmake_policy (SET CMP0026 OLD) # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0042 NEW) # MACOSX_RPATH is enabled by default
    endif ()
endif ()

# Set CMake modules search path
set (CMAKE_MODULE_PATH
    $ENV{URHO3D_HOME}/Source/CMake/Modules
    $ENV{CMAKE_PREFIX_PATH}/share/Urho3D/CMake/Modules
    ${CMAKE_INSTALL_PREFIX}/share/Urho3D/CMake/Modules
    CACHE PATH "Path to Urho3D-specific CMake modules")

# Include Urho3D CMake common module
include (Urho3D-CMake-common)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

# Define target name
set (TARGET_NAME Main)

# Define source files
define_source_files ()

# Setup target with resource copying
setup_main_executable ()

# Setup test cases
if (URHO3D_ANGELSCRIPT)
    add_test (NAME ExternalLibAS COMMAND ${TARGET_NAME} Data/Scripts/12_PhysicsStressTest.as -w -timeout ${URHO3D_TEST_TIME_OUT})
endif ()
if (URHO3D_LUA)
    add_test (NAME ExternalLibLua COMMAND ${TARGET_NAME} Data/LuaScripts/12_PhysicsStressTest.lua -w -timeout ${URHO3D_TEST_TIME_OUT})
endif ()
