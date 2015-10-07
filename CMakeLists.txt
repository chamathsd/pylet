if(WIN32)
    cmake_minimum_required(VERSION 3.1.0)
    set(CMAKE_CXX_STANDARD 11)
else()
    cmake_minimum_required(VERSION 2.8.12)
    add_compile_options(-std=c++11)
endif()

# --- BUILD CONFIGURATION ---

project(pylet)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(SOURCE
    src/pylet.cpp
    src/ui/pylet_window.cpp
    src/ui/pylet_window.h
    src/ui/code_editor_interface.cpp
    src/ui/code_editor_interface.h
    src/ui/code_editor_numbers.h 
    src/ui/code_editor_highlighter.cpp
    src/ui/code_editor_highlighter.h
)

# set(RESOURCES)

set(GUI_TYPE "")

if(WIN32)
    set(GUI_TYPE WIN32)
elseif(UNIX)
    if(APPLE)
        set(GUI_TYPE MACOSX_BUNDLE)
    endif(APPLE)
else()
    # Suspend the configuration process if the target OS isn't recognized. 
    MESSAGE(FATAL_ERROR "Target OS not recognized or supported, aborting CMake process.")
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# qt5_add_resources(UI_RESOURCES ${RESOURCES})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
)
add_executable(pylet
    ${GUI_TYPE}
    ${SOURCE}
    # ${UI_RESOURCES}
)
target_link_libraries(pylet
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)
if(MSVC)
    source_group("src" REGULAR_EXPRESSION "src/[^/.]*\.")
    source_group("src\\ui" REGULAR_EXPRESSION "src/ui.*\.")
endif()

# --- RUNTIME DEPENDENCIES ---
# Only necessary on Windows.

if(WIN32)
    cmake_policy(SET CMP0026 OLD)

    get_target_property(BuildLocation pylet LOCATION)
    get_filename_component(TargetDirectory ${BuildLocation} PATH)

    if (EXISTS ${Qt5Core_DIR}/../../../bin/icuin53.dll)
        set(ICU_VER 53)
    else()
        set(ICU_VER 54)
    endif()

    foreach(DEPENDENCY
        Qt5Widgetsd.dll
        Qt5Guid.dll
        Qt5Cored.dll
    )
        add_custom_command(TARGET pylet POST_BUILD
            # Copy the given dependency into the executable directory.
            COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/${DEPENDENCY} ${TargetDirectory}
            COMMENT "Copying ${DEPENDENCY} into '${TargetDirectory}/'" VERBATIM
        )
    endforeach()
endif()