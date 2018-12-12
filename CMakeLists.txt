cmake_minimum_required (VERSION 3.1)											#	set cmake minimum version

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/submodule/sweet")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/utility.cmake")

# Q. can 'project(projname)' command used before 'include("cmake/HunterGate")' ??
project(2018_fall_image_processing_final)
show_variables(CMAKE_SYSTEM_PREFIX_PATH)
show_variables(CMAKE_PREFIX_PATH)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/export/bin")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/export/lib")
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/export/lib")

set(CMAKE_C_STANDARD              90)
set(CMAKE_C_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_STANDARD            14)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_COMPONENTS
  # atomic
  # chrono
  # container
  # context
  # coroutine
  # date_time
  # exception
  # fiber
  filesystem
  # iostreams
  # locale
  # random
  # regex
  # serialization
  system
  # thread
  )

find_package(Boost REQUIRED COMPONENTS ${Boost_COMPONENTS})
find_package(fmt CONFIG)
find_package(date CONFIG)
find_package(OpenCV REQUIRED)

set(packages
  Boost
  fmt
  OpenCV
  date
  )
foreach(component ${Boost_COMPONENTS} boost)
  if(TARGET Boost::${component})
    sweet_config_add(Boost LIBRARIES PRIVATE Boost::${component})
  endif()
endforeach()

if(WIN32)
  # boost.asio dependency
  sweet_config_add(Boost LIBRARIES PRIVATE wsock32 ws2_32)
endif()

sweet_config_add(fmt LIBRARIES PRIVATE fmt)

foreach(package ${packages})
  config_from_generic_package("${package}" "${package}")
  message(STATUS " config: ${package}=${${package}_CONFIG_SETTINGS}")
endforeach()

add_executable(spike_cv spike/cv.cpp)
sweet_target_add(spike_cv CONFIGS OpenCV)

# add_executable(a spike/cv.cpp)
# sweet_target_add(a CONFIGS OpenCV)
# sweet_target_add(a CONFIGS date)
