
# General  ---------------------------------------------------------------------

# Minimum Version of CMake Required
cmake_minimum_required(VERSION 2.8)

# Global variables
set( global_c_flags				"-Wall"				)
set( global_c_flags_debug		"-g -DDEBUG"		)
set( global_c_flags_release		"-s -O2 -DRELEASE"	)
set( global_cxx_flags			"-std=c++11 -Wall"	)
set( global_cxx_flags_debug		"-g -DDEBUG"		)
set( global_cxx_flags_release	"-s -O2 -DRELEASE"	)

set( root_dir		${CMAKE_CURRENT_SOURCE_DIR}/..	)
set( src_in_dir		${root_dir}/src					)
set( test_in_dir	${root_dir}/test				)
set( samples_in_dir	${root_dir}/samples				)
set( doc_in_dir		${root_dir}/doc					)
set( assets_in_dir	${root_dir}/assets				)

set( bin_out_dir	${root_dir}/build/bin			)
set( lib_out_dir	${root_dir}/build/lib			)
set( doc_out_dir	${root_dir}/build/doc			)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY	${bin_out_dir}	)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${lib_out_dir}	)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}" )

# Glew-OGL ----------------------------------------------------------------------

# Generate Documentation
if(UNIX)
	set(Use_Doxygen OPTIONAL)
	include(UseDoxygen)
endif(UNIX)

# Glew-OGL sample1 -------------------------------------------------------------

	# Project properties
	project(opengl)

	# G++ Flags
	set( CMAKE_C_FLAGS				${global_c_flags}			)
	set( CMAKE_C_FLAGS_DEBUG		${global_c_flags_debug}		)
	set( CMAKE_C_FLAGS_RELEASE		${global_c_flags_release}	)
	set( CMAKE_CXX_FLAGS			${global_cxx_flags}			)
	set( CMAKE_CXX_FLAGS_DEBUG		${global_cxx_flags_debug}	)
	set( CMAKE_CXX_FLAGS_RELEASE	${global_cxx_flags_release}	)

	# Sources files list
	file( GLOB_RECURSE ${PROJECT_NAME}_src		${src_in_dir}/*.cpp		)
	file( GLOB_RECURSE ${PROJECT_NAME}_head		${src_in_dir}/*.hpp		)
	file( GLOB_RECURSE ${PROJECT_NAME}_inl		${src_in_dir}/*.inl		)
	file( GLOB_RECURSE ${PROJECT_NAME}_glsl		${src_in_dir}/*.glsl	)

	# Find dependencies
	find_package( OpenGL	3	REQUIRED						)
	find_package( GLEW			REQUIRED						)
	find_package( GLM			REQUIRED						)
	find_package( SFML		2	REQUIRED system window graphics	)

	# Compile as an executable and link lib
	add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_src} ${${PROJECT_NAME}_glsl})
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SFML_LIBRARIES})

# ------------------------------------------------------------------------------

