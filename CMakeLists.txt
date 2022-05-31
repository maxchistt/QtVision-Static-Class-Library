################################################################################
#                                                                              #
#  Copyright (c) 2016, C3D Labs LLC                                            #
#  All rights reserved.                                                        #
#                                                                              #
#  This file is part of C3DVision Library                                      #
#  http://c3dlabs.com                                                          #
#                                                                              #
#  Exclusive rights to C3D Vision, any copies or components (parts) thereof    #
#  and other intellectual property belong to C3D Labs, LLC (the Rightholder).  # 
#  The license rights and restrictions are regulated by the Agreement between  #
#  the User and the Rightholder.                                               #
#                                                                              #
################################################################################

PROJECT(QtVision-Static-Class-Library)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Runtime output directory
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

IF(MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG_INIT} -D_DEBUG -D_DRAWGI /Zi /W4 /D_DEBUG /D_DRAWGI /MP /bigobj") 
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -D_SECURE_SCL=0 /W4") 
ELSE()
    SET(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG_INIT} -D_DRAWGI -Wno-deprecated-declarations -fno-operator-names -fopenmp")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -D_SECURE_SCL=0 -Wno-deprecated-declarations -fno-operator-names -fopenmp")
ENDIF()

# Unicode build
OPTION(C3DVision_USING_UNICODE "Enable Unicode support" ON)

OPTION(C3DVision_GUI_QT5_APP "Build Qt5 support" ON)

# Libraries    
if( NOT C3D_MATH_DIR )
    SET(C3D_MATH_DIR   ${CMAKE_SOURCE_DIR}/C3D/Kernel)
endif()

if( NOT C3D_Vision_DIR )
    SET(C3D_Vision_DIR ${CMAKE_SOURCE_DIR}/C3D/Vision)
endif()

# Libraries    
if( NOT C3D_Math_INCLUDE_DIR )
    SET(C3D_Math_INCLUDE_DIR   ${C3D_MATH_DIR}/Include)
endif()

if( NOT C3D_Vision_INCLUDE_DIR )
    SET(C3D_Vision_INCLUDE_DIR ${C3D_Vision_DIR}/Include)
endif()

# Libraries  
SET(Math_SOURCE_DIR   ${C3D_MATH_DIR})

IF (MSVC)
  SET(C3DLIBNAME "c3d.lib")
  SET(C3DDLLNAME "c3d.dll")
ENDIF()

SET(Math_OUTPUT optimized ${Math_SOURCE_DIR}/Release/${C3DLIBNAME} debug ${Math_SOURCE_DIR}/Debug/${C3DLIBNAME})

SET(Vision_SOURCE_DIR ${C3D_Vision_DIR})

IF (MSVC)
  SET(C3DVISIONLIBNAME "c3dvision.lib")
  SET(C3DVISIONDLLNAME "c3dvision.dll")
ENDIF()

SET(Vision_OUTPUT optimized ${Vision_SOURCE_DIR}/Release/${C3DVISIONLIBNAME} debug ${Vision_SOURCE_DIR}/Debug/${C3DVISIONLIBNAME})

# Libraries all c3d
SET(C3D_ALL_LIB_LINKS ${Math_OUTPUT} ${Vision_OUTPUT})

#binary
set(EXAMPLES_BIN ${CMAKE_BINARY_DIR}/bin/)
if( CMAKE_RUNTIME_OUTPUT_DIRECTORY )
    set(EXAMPLES_BIN ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_BIN})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EXAMPLES_BIN})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_BIN})


IF(C3DVision_USING_UNICODE)
  ADD_DEFINITIONS( 
    -DUNICODE
    -D_UNICODE
    )
ENDIF(C3DVision_USING_UNICODE)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Shared
    ${CMAKE_CURRENT_SOURCE_DIR}/Common
    ${QT_USE_FILE}
    )

IF(UNIX)
  ADD_DEFINITIONS(
      -std=c++11
  )
  SET(STDLIB stdc++)
ELSE()
  SET(STDLIB "")
ENDIF()



IF( NOT CMAKE_BUILD_TYPE )
    #copy external dll, lib, pdb Math and Vision
    file(INSTALL ${Math_SOURCE_DIR}/Debug/${C3DDLLNAME}           DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
    file(INSTALL ${Math_SOURCE_DIR}/Release/${C3DDLLNAME}         DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
    IF( NOT "${C3D_Vision_DIR}" STREQUAL "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
        file(INSTALL ${Vision_SOURCE_DIR}/Debug/${C3DVISIONDLLNAME}   DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
        file(INSTALL ${Vision_SOURCE_DIR}/Release/${C3DVISIONDLLNAME} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
    ENDIF()
ELSE ()
  #copy external dll, lib, pdb Math and Vision
  IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(INSTALL ${Math_SOURCE_DIR}/Debug/${C3DDLLNAME}           DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(INSTALL ${Vision_SOURCE_DIR}/Debug/${C3DVISIONDLLNAME}   DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  ELSE ()
    file(INSTALL ${Math_SOURCE_DIR}/Release/${C3DDLLNAME}         DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(INSTALL ${Vision_SOURCE_DIR}/Release/${C3DVISIONDLLNAME} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  ENDIF ()
ENDIF()

# ExportLibDll 
################################################################################
IF (CMAKE_BUILD_TYPE STREQUAL "Release" OR NOT CMAKE_BUILD_TYPE)
  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/QtVision/Include/" cmd_path_xcopy_from_i)
  IF( NOT CMAKE_BUILD_TYPE )
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/" cmd_path_xcopy_from_r)
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/" cmd_path_xcopy_from_d)
  ELSE()
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/" cmd_path_xcopy_from_r)
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../../Debug/Bin" cmd_path_xcopy_from_d)
  ENDIF()

  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/ExportLibDll/QtVision/Include/" cmd_path_xcopy_to_i)
  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/ExportLibDll/QtVision/Release/" cmd_path_xcopy_to_r)
  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/ExportLibDll/QtVision/Debug/" cmd_path_xcopy_to_d)

  IF( NOT CMAKE_BUILD_TYPE )
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../QtVision/Release/" cmd_path_xcopy_from_r_l)
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../QtVision/Debug/" cmd_path_xcopy_from_d_l)
  ELSE()
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../QtVision/" cmd_path_xcopy_from_r_l)
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../../Debug/QtVision/" cmd_path_xcopy_from_d_l)
  ENDIF()

  ADD_CUSTOM_TARGET( ExportLib
    COMMAND xcopy /y "${cmd_path_xcopy_from_i}" "${cmd_path_xcopy_to_i}"
    COMMAND xcopy /y "${cmd_path_xcopy_from_r_l}\\QtVision.lib" "${cmd_path_xcopy_to_r}"
    COMMAND xcopy /y "${cmd_path_xcopy_from_r}\\QtVision.dll" "${cmd_path_xcopy_to_r}"
    COMMAND xcopy /y "${cmd_path_xcopy_from_d_l}\\QtVision.lib" "${cmd_path_xcopy_to_d}"
    COMMAND xcopy /y "${cmd_path_xcopy_from_d}\\QtVision.dll" "${cmd_path_xcopy_to_d}"
    COMMENT "Export to *ProjectSourceDir*/ExportLibDll/"
  VERBATIM)
ENDIF ()

###############################################################################
#  QT APP
###############################################################################
IF (C3DVision_GUI_QT5_APP)
    SET(DESIRED_QT_VERSION "5" CACHE STRING "")
    find_package( Qt5Core REQUIRED )
    find_package( Qt5Gui REQUIRED )
    find_package( Qt5OpenGL REQUIRED )
  
    ADD_DEFINITIONS(-D__USE_QT__)
    ADD_DEFINITIONS(-DQT_NO_DEPRECATED_WARNINGS)

    SET(QtVision_OUTPUT "QtVision")
    SET(QtVision_DIR QtVision)
    ADD_SUBDIRECTORY(QtVision)
    ADD_DEFINITIONS( -DNOMINMAX )
ENDIF()
