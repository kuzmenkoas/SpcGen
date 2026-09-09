cmake_minimum_required(VERSION 3.28)

message(STATUS "========================================")
message(STATUS "  Running clang-format")
message(STATUS "========================================")

find_program(CLANG_FORMAT clang-format)
if(NOT CLANG_FORMAT)
    message(FATAL_ERROR "❌ clang-format not found in PATH")
endif()
message(STATUS "✅ Found: ${CLANG_FORMAT}")

message(STATUS "📁 Formatting .cpp and .h files (excluding build/)...")

execute_process(
    COMMAND sh -c "find . -path ./build -prune -o -type f \\( -name '*.cpp' -o -name '*.h' \\) -print0 | xargs -0 ${CLANG_FORMAT} -i --style=file"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
    RESULT_VARIABLE RESULT
)

if(RESULT EQUAL 0)
    message(STATUS "✅ Formatting completed successfully")
else()
    message(FATAL_ERROR "❌ Formatting failed with code: ${RESULT}")
endif()

message(STATUS "========================================")  