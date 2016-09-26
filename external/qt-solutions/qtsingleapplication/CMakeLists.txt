cmake_minimum_required (VERSION 2.8.11)
cmake_policy(SET CMP0020 OLD)
if ( CMAKE_VERSION VERSION_GREATER 3.0 )
cmake_policy(SET CMP0043 OLD)
endif()

set(QTSINGLEAPPLICATION "qtsingleapplication")
set(PROJECT ${QTSINGLEAPPLICATION})
project(${PROJECT})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
if(WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
endif(WIN32)
set(CMAKE_AUTOMOC ON)

#find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
find_package(Qt5Core)
#find_package(Qt5Network)
#include_directories( ${QT_INCLUDE_DIR} )
#include_directories( ${QT_INCLUDE_DIR}/Qt )

set( MAIN_SRC 
		"qtlocalpeer.cpp"
		"qtsingleapplication.cpp"
		"qtsinglecoreapplication.cpp"
)

set(MAIN_QT_HDRS
		"qtlocalpeer.h"
		"qtsingleapplication.h"
		"qtsinglecoreapplication.h"
)

set(MAIN_HDRS
		"qtlocalpeer.h"
		"qtlockedfile.h"
		"qtsingleapplication.h"
		"qtsinglecoreapplication.h"
		"QtLockedFile"
		"QtSingleApplication"
)

#qt4_wrap_cpp(MOC_SOURCES ${MAIN_QT_HDRS} )

#add comment
add_library(${PROJECT} STATIC ${MAIN_SRC} ${MOC_SOURCES})
#target_link_libraries(${PROJECT} ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY})
qt5_use_modules(${PROJECT} Widgets Network)
set(QTSINGLEAPPLICATION_LIBS ${QTSINGLEAPPLICATION} PARENT_SCOPE)
