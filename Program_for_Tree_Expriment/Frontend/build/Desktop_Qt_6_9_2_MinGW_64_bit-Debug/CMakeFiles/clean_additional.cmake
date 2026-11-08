# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Frontend_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Frontend_autogen.dir\\ParseCache.txt"
  "Frontend_autogen"
  )
endif()
