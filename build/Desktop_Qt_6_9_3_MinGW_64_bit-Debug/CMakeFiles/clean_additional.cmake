# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RedeDePacotes_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RedeDePacotes_autogen.dir\\ParseCache.txt"
  "RedeDePacotes_autogen"
  "backend\\CMakeFiles\\backend_lib_autogen.dir\\AutogenUsed.txt"
  "backend\\CMakeFiles\\backend_lib_autogen.dir\\ParseCache.txt"
  "backend\\backend_lib_autogen"
  )
endif()
