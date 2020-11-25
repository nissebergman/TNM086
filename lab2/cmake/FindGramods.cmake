# Try to find Gramods
#
# Once done this will define
#
#  - Gramods_FOUND
#  - Gramods_INCLUDE_DIRS
#  - Gramods_LIBS

find_path(Gramods_INCLUDE_DIR gmCore/config.hh)

find_library(gmCore_LIBRARY NAMES gmCore libgmCore)
find_library(gmGraphics_LIBRARY NAMES gmGraphics libgmGraphics)
find_library(gmNetwork_LIBRARY NAMES gmNetwork libgmNetwork)
find_library(gmTrack_LIBRARY NAMES gmTrack libgmTrack)

find_package(Eigen3 REQUIRED)
find_package(ASIO REQUIRED)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Gramods DEFAULT_MSG
  Gramods_INCLUDE_DIR
  gmCore_LIBRARY
  gmGraphics_LIBRARY
  gmNetwork_LIBRARY
  gmTrack_LIBRARY
  )

mark_as_advanced(
  Gramods_INCLUDE_DIR
  gmCore_LIBRARY
  gmGraphics_LIBRARY
  gmNetwork_LIBRARY
  gmTrack_LIBRARY
  )

set(Gramods_LIBS
  ${gmCore_LIBRARY}
  ${gmGraphics_LIBRARY}
  ${gmNetwork_LIBRARY}
  ${gmTrack_LIBRARY}
  Eigen3::Eigen
  )

set(Gramods_INCLUDE_DIRS
  ${Gramods_INCLUDE_DIR}
  ${ASIO_INCLUDE_DIRS}
  )
