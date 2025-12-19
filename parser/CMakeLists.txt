file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)

if (NOT SOURCES)
	message(WARNING "No source files found in parser/src/")
	set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp)
	file(WRITE ${SOURCES} "// Empty source file\n")
endif()

add_library(parser STATIC ${SOURCES} ${HEADERS})
target_include_directories(parser PUBLIC include)
target_link_libraries(parser PUBLIC global)