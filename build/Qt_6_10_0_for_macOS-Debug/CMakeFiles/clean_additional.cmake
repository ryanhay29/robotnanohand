# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/RobotHand_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RobotHand_autogen.dir/ParseCache.txt"
  "RobotHand_autogen"
  )
endif()
