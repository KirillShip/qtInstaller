# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "RelWithDebInfo")
  file(REMOVE_RECURSE
  "CMakeFiles\\installer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\installer_autogen.dir\\ParseCache.txt"
  "installer_autogen"
  )
endif()
