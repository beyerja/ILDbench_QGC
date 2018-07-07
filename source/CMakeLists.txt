#######################################################
# cmake file for building ILDbench_QGC analysis package
# @author Ete Remi, DESY
#######################################################

# -------------------------------------------------
# include directories
include_directories( BEFORE include )

# -------------------------------------------------
# build the project library
# This compiles everything in the src directory and build a library
# called libILDbench_QGC.so, that with be install in the lib directory
# in the top level directory of your project 
aux_source_directory( src SRC_FILES )
SET_SOURCE_FILES_PROPERTIES( ${SRC_FILES} PROPERTIES COMPILE_FLAGS "-pedantic" )
add_shared_library( ${PROJECT_NAME} ${SRC_FILES} )
install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib )
