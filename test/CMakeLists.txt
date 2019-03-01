add_subdirectory(libgraphc)
add_subdirectory(Billard)
add_subdirectory(utility)
add_subdirectory(mom_explorer)
add_subdirectory(ModernBillard)
add_subdirectory(TimeMaster)
add_subdirectory(testFederate)

# bug #55104 Test if rpc include directory is available
# reason is Fedora removed (in v28) the rpc header from the standard glibc
FIND_PATH(RPC_INCLUDE_DIR
	NAMES rpc/rpc.h
	HINTS /usr/include/tirpc
	NO_DEFAULT_PATH
	)
FIND_PATH(RPC_INCLUDE_DIR NAMES rpc/rpc.h)
if(NOT RPC_INCLUDE_DIR)
    MESSAGE("Could not find RPC Headers, certiCheckHostAndIP test won't be compiled!")
endif()
# Do not compile this on Win32 (not very useful)
if ((NOT WIN32) AND RPC_INCLUDE_DIR)
   if (CMAKE_SYSTEM_NAME MATCHES "Linux")
      set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/certiCheckHostAndIP.c PROPERTIES COMPILE_DEFINITIONS "LINUX")
   endif()
   add_executable(CertiCheckHostAndIP certiCheckHostAndIP.c)
   install(TARGETS CertiCheckHostAndIP
           RUNTIME DESTINATION bin
           LIBRARY DESTINATION lib
           ARCHIVE DESTINATION lib)
endif((NOT WIN32) AND RPC_INCLUDE_DIR)
