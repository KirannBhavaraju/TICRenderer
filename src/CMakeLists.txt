# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (NanoVGSecond)

find_package(OpenGL REQUIRED)
find_package(VTK COMPONENTS 
  vtkCommonCore
  vtkIOImage
  vtkInteractionImage
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2 QUIET)
if (NOT VTK_FOUND)
  message("Skipping NanoVGSecond: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")


add_subdirectory (external)
add_subdirectory (nanovg)

include_directories(
	nanovg/src/
	external/glfw-3.1/include/
	external/glm-stable/
	external/glew-2.1.0/include/
	external/
	.
)

find_library(NANOVG SHARED nanovg)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	${VTK_LIBRARIES}
	glfw
	GLEW_2100_x2
	nanovg
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(NanoVGSecond
	common/shader.cpp
	common/shader.hpp
	common/texture.hpp
	common/texture.cpp
	common/TICPP.h
	common/TICPP.cpp
	common/Spline.h
	demo.h
	demo.c
	perf.h
	perf.c
	stb_image_write.h
	basicShader.ver
	basicShader.frag
	NanoVGSecond.cpp
)

target_link_libraries(NanoVGSecond ${ALL_LIBS})