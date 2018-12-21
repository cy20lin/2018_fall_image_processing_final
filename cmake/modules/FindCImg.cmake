##
## Copyright (c) 2017 ChienYu Lin
##
## Author: ChienYu Lin <cy20lin@gmail.com>
##

# - Try to find CImg
# Once done this will define
#  CImg_FOUND - System has CImg
#  CImg_INCLUDE_DIRS - The CImg include directories
#  CImg_LIBRARIES - The libraries needed to use CImg
#  CImg_DEFINITIONS - Compiler switches required for using CImg


find_path(CImg_INCLUDES CImg.h)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CImg_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(CImg DEFAULT_MSG
  CImg_LIBRARIES CImg_INCLUDES)

set(CImg_LIBRARY      ${CImg_LIBRARIES})
set(CImg_LIBS         ${CImg_LIBRARIES})
set(CImg_INCLUDE_DIRS ${CImg_INCLUDES})
set(CImg_INCLUDE_DIR  ${CImg_INCLUDES})
