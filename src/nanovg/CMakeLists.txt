cmake_minimum_required (VERSION 2.8.12)

project (nanovg C)

if (APPLE)
	cmake_policy(SET CMP0042 NEW)
endif()
if (MSVC)
	cmake_policy(SET CMP0054 NEW)
endif()

set(CMAKE_DEBUG_POSTFIX d)

#-------------------------------------------
#		 Nanovg   Build
#-------------------------------------------

if(NOT NVG_ROOT_DIR)
	set(NVG_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "The root directory of nanovg")
  message(STATUS "Nvg root directory is :: ${NVG_ROOT_DIR}")
endif()

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (WIN32)
  set (NVG_LIB_NAME nanovg)
  message(STATUS "Nvg library name is ${NVG_LIB_NAME}")
else ()
  set (NVG_LIB_NAME NANOVG)
  message(STATUS "Nvg library name is ${NVG_LIB_NAME}")
  set (DLL_PREFIX lib)
endif ()

find_package (OpenGL REQUIRED)

set (NVG_LIBRARIES ${OPENGL_LIBRARIES})

set(NVG_INCLUDE_DIR "${NVG_ROOT_DIR}/src" PARENT_SCOPE)
include_directories (${NVG_ROOT_DIR}/src)
message(STATUS "Nvg include dir is ${NVG_INCLUDE_DIR}")

set (NVG_PUBLIC_HEADERS_FILES ${NVG_ROOT_DIR}/src/nanovg.h ${NVG_ROOT_DIR}/src/nanovg_gl.h ${NVG_ROOT_DIR}/src/nanovg_gl_utils.h ${NVG_ROOT_DIR}/src/stb_image.h ${NVG_ROOT_DIR}/src/stb_truetype.h ${NVG_ROOT_DIR}/src/fontstash.h)
set (NVG_SRC_FILES ${NVG_ROOT_DIR}/src/nanovg.c)


add_library (nanovg SHARED ${NVG_PUBLIC_HEADERS} ${NVG_SRC_FILES})
set_target_properties (nanovg PROPERTIES COMPILE_DEFINITIONS "NVG_BUILD" OUTPUT_NAME "${NVG_LIB_NAME}" PREFIX "${DLL_PREFIX}")
add_library (nanovg_s STATIC ${NVG_PUBLIC_HEADERS} ${NVG_SRC_FILES})
set_target_properties (nanovg_s PROPERTIES COMPILE_DEFINITIONS "NVG_STATIC" OUTPUT_NAME "${NVG_LIB_NAME}")

if (MSVC)
  # add options from visual studio project
  target_compile_definitions (nanovg PRIVATE "NVG_BUILD;VC_EXTRALEAN")
  target_compile_definitions (nanovg_s PRIVATE "NVG_STATIC;VC_EXTRALEAN")

  # kill security checks which are dependent on stdlib
  target_compile_options (nanovg PRIVATE -GS-)
  target_compile_options (nanovg_s PRIVATE -GS-)

elseif (WIN32 AND ((CMAKE_C_COMPILER_ID MATCHES "GNU") OR (CMAKE_C_COMPILER_ID MATCHES "Clang")))
  # remove stdlib dependency on windows with GCC and Clang (for similar reasons
  # as to MSVC - to allow it to be used with any Windows compiler)
  target_compile_options (nanovg PRIVATE -fno-builtin -fno-stack-protector)
  target_compile_options (nanovg_s PRIVATE -fno-builtin -fno-stack-protector)
  target_link_libraries (nanovg LINK_PRIVATE -nostdlib)
endif ()

target_link_libraries (nanovg LINK_PUBLIC ${NVG_LIBRARIES})
target_link_libraries (nanovg_s ${NVG_LIBRARIES})

if(CMAKE_VERSION VERSION_LESS 2.8.12)
  set(MAYBE_EXPORT "")
else()
  target_compile_definitions(nanovg_s INTERFACE "NVG_STATIC")
  foreach(t nanovg nanovg_s)
    target_include_directories(${t} PUBLIC $<INSTALL_INTERFACE:src>)
  endforeach()
  set(MAYBE_EXPORT EXPORT nanovg-targets)
endif()

set (prefix ${CMAKE_INSTALL_PREFIX})
set (exec_prefix ${CMAKE_INSTALL_PREFIX})
set (libdir ${CMAKE_INSTALL_FULL_LIBDIR})
set (includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set (libname ${NVG_LIB_NAME})
set (cflags)
