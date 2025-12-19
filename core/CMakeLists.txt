file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)

if (NOT SOURCES)
	message(WARNING "No source files found in core/src/")
	set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp)
	file(WRITE ${SOURCES} "// Empty source file\n")
endif()

add_library(core STATIC ${SOURCES} ${HEADERS})
target_include_directories(core PUBLIC include)
target_link_libraries(core PUBLIC global device)