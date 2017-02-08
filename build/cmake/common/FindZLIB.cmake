# Utilities to use with cmake.
# (c) Willyan A. Machado, 2011
# ----------------------------------------------------------------------------
# Used to automatically find ZLib dependencies

cmake_minimum_required(VERSION 2.8)

include ( ${CMAKE_MODULE_PATH}/CMakeUtils.cmake )
getTePath( T4_DP_INC T4_DP_LIB )

FIND_PATH(ZLIB_INCLUDE_DIR 
   NAMES zlib.h
   PATHS ${T4_DP_INC}/zlib
)

IF ( WIN32 )
  IF ( MINGW )
    SET ( ZLIB_LIBRARY_NAMES libz.a )
  ELSE ( MINGW )
    SET ( ZLIB_LIBRARY_NAMES zlibstat.lib )
  ENDIF ( MINGW )
ELSE ( WIN32 )
  SET( ZLIB_LIBRARY_NAMES zlibclient zlibclient_r )
ENDIF ( WIN32 )

FIND_LIBRARY(ZLIB_LIBRARIES
   NAMES ${ZLIB_LIBRARY_NAMES}
   PATHS ${T4_DP_LIB}
   PATH_SUFIXES zlib
)

IF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARIES)
	SET(ZLIB_FOUND TRUE)
	
ELSE (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARIES)
	SET(ZLIB_FOUND FALSE)
	MESSAGE("Looked for ZLib libraries named ${ZLib_NAMES}.")
	MESSAGE("Could NOT find ZLib library")
ENDIF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARIES)

MARK_AS_ADVANCED( ZLIB_LIBRARIES ZLIB_INCLUDE_DIR )
