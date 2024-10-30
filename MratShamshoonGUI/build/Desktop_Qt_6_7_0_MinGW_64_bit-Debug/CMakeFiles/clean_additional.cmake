# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MratShamshoonGUI_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MratShamshoonGUI_autogen.dir\\ParseCache.txt"
  "MratShamshoonGUI_autogen"
  )
endif()
