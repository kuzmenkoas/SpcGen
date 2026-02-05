file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)
file(GLOB REALIZATION include/*.icpp)

if (NOT SOURCES)
	message(WARNING "No source files found in device/src/")
	set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp)
	file(WRITE ${SOURCES} "// Empty source file\n")
endif()

add_library(device STATIC ${SOURCES} ${HEADERS} ${REALIZATION})
target_include_directories(device PUBLIC include)
target_link_libraries(device PUBLIC parser)