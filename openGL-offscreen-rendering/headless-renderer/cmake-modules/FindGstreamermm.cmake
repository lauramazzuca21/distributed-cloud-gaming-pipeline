# Try to find GStreamermm
# Released under MIT License by Kota Weaver
# Once done this will define
#  GSTREAMERMM_FOUND - System has GStreamermm
#  GSTREAMERMM_INCLUDE_DIRS - The GStreamermm include directories
#  GSTREAMERMM_LIBRARIES - The libraries needed to use GStreamermm

include(FindPkgConfig)
pkg_search_module(GSTREAMERMM gstreamermm-1.0 gstreamermm)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gstreamermm DEFAULT_MSG GSTREAMERMM_LIBRARIES GSTREAMERMM_INCLUDE_DIRS)

mark_as_advanced(GSTREAMERMM_INCLUDE_DIRS GSTREAMERMM_LIBRARIES)
