file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)

if (NOT SOURCES)
	message(WARNING "No source files found in plotter/src/")
	set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp)
	file(WRITE ${SOURCES} "// Empty source file\n")
endif()

add_library(plotter STATIC ${SOURCES} ${HEADERS})
target_include_directories(plotter PUBLIC include)
target_link_libraries(plotter PUBLIC global)