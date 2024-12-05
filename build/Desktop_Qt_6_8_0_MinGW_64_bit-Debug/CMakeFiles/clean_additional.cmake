# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\textEditor_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\textEditor_autogen.dir\\ParseCache.txt"
  "textEditor_autogen"
  )
endif()
