if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

### GLFW ###

add_subdirectory (glfw-3.1)

include_directories(
	glfw-3.1/include/GLFW/
	glew-2.1.0/include/
	glm-stable/
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

### GLEW ###

set(GLEW_HEADERS
)

set(GLEW_SOURCE
	glew-2.1.0/src/glew.c
)

add_library(GLEW_2100_x2 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)


### NANOVG ###
# will be added from the subfolder

target_link_libraries(GLEW_2100_x2
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)
