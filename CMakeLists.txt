cmake_minimum_required(VERSION 3.13)

if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()


project(MBook VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src)


message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")

#set(PODOFO_DIR /root/podofo)



#find_library(PODOFO_LIBRARY PoDoFo HINTS "${PODOFO_DIR}/lib")
#if( ${PODOFO_LIBRARY} STREQUAL "PODOFO_LIBRARY-NOTFOUND")
#    message("Please set PODOFO_LIBRARY variable to location of libpodofo")
#endif()



#target_link_libraries(MBook PRIVATE podofo)        check link libraries








