
# generated by Buildyard, do not edit. Sets FOUND_REQUIRED if all required
# dependencies are found. Used by Buildyard.cmake
set(FIND_REQUIRED_FAILED)
find_package(vmmlib 1.7.0 QUIET)
if(NOT vmmlib_FOUND AND NOT VMMLIB_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} vmmlib")
endif()
find_package(Lunchbox 1.9.0 QUIET)
if(NOT Lunchbox_FOUND AND NOT LUNCHBOX_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} Lunchbox")
endif()
find_package(Collage 1.1.0 QUIET)
if(NOT Collage_FOUND AND NOT COLLAGE_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} Collage")
endif()
find_package(OpenGL  QUIET)
if(NOT OpenGL_FOUND AND NOT OPENGL_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} OpenGL")
endif()
find_package(Boost 1.41.0 COMPONENTS program_options QUIET)
if(NOT Boost_FOUND AND NOT BOOST_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} Boost")
endif()
if(FIND_REQUIRED_FAILED)
  set(FOUND_REQUIRED FALSE)
else()
  set(FOUND_REQUIRED TRUE)
endif()
