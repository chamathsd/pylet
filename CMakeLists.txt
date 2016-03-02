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

set(CORE_SOURCE
    src/pylet.cpp
)

set(GUI_SOURCE
    src/gui/pylet_window.cpp
    src/gui/pylet_window.h
    src/gui/code_editor_interface.cpp
    src/gui/code_editor_interface.h
    src/gui/code_editor_numbers.h 
    src/gui/code_editor_highlighter.cpp
    src/gui/code_editor_highlighter.h
    src/gui/editor_stack.cpp
    src/gui/editor_stack.h
    src/gui/console.cpp
    src/gui/console.h
)

set(PYTHON_SOURCE
    src/python/buffer.cpp
    src/python/buffer.h
)

set(SOURCE
    ${CORE_SOURCE}
    ${GUI_SOURCE}
    ${PYTHON_SOURCE}
)

set(RESOURCES
    res/icons.qrc
    res/fonts.qrc
)

set(GUI_TYPE "")

if(WIN32)
    # set(GUI_TYPE WIN32)
elseif(UNIX)
    if(APPLE)
        set(GUI_TYPE MACOSX_BUNDLE)
    endif(APPLE)
else()
    # Suspend the configuration process if the target OS isn't recognized. 
    MESSAGE(FATAL_ERROR "Target OS not recognized or supported, aborting CMake process.")
endif()

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(PythonLibs 3 REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)

qt5_add_resources(UI_RESOURCES ${RESOURCES})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)
link_directories(
    ${Boost_LIBRARY_DIRS}
)
add_executable(pylet
    ${GUI_TYPE}
    ${SOURCE}
    ${UI_RESOURCES}
)
target_link_libraries(pylet
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    ${PYTHON_LIBRARIES}
    ${Boost_LIBRARIES}
)
if(MSVC)
    source_group("src\\" FILES ${CORE_SOURCE})
    source_group("src\\gui" FILES ${GUI_SOURCE})
    source_group("src\\python" FILES ${PYTHON_SOURCE})
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
