if (NOT ARCCON_ROOT)
   message(FATAL_ERROR "Variable 'ARCCON_ROOT' is not set")
endif()
if (NOT ARCCON_EXPORT_TARGET)
  set(ARCCON_EXPORT_TARGET ${ARCCORE_EXPORT_TARGET})
endif()
SET(ARCCON_CMAKE_COMMANDS ${ARCCON_ROOT}/Arccon.cmake)
set(ARCCON_MODULE_PATH ${ARCCON_ROOT}/Modules)

list(APPEND CMAKE_MODULE_PATH ${ARCCON_MODULE_PATH})
include(${ARCCON_CMAKE_COMMANDS})