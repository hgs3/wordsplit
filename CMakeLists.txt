cmake_minimum_required(VERSION 3.22)
project(wordsplit VERSION 1.0 LANGUAGES C)

option(UNDEFINED_BEHAVIOR_SANITIZER "Toggle undefined behavior sanitizer (requires Clang)" OFF)
option(ADDRESS_SANITIZER "Toggle address sanitizer (requires Clang)" OFF)
option(MEMORY_SANITIZER "Toggle memory sanitizer (requires Clang)" OFF)

enable_testing(1)

if (UNDEFINED_BEHAVIOR_SANITIZER)
    add_compile_options(-g -fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
elseif (ADDRESS_SANITIZER)
    add_compile_options(-g -fsanitize=address)
    add_link_options(-fsanitize=address)
elseif (MEMORY_SANITIZER)
    add_compile_options(-g -fsanitize=memory)
    add_link_options(-fsanitize=memory)
endif ()

include_directories(${PROJECT_SOURCE_DIR})

add_executable(test_wordsplit wordsplit.c wordsplit.h tests/wordsplit_test.c)
add_test(NAME UnitTests COMMAND test_wordsplit)

# Add fuzz tests if Clang or AppleClang is available.
if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-g -O0 -fsanitize=fuzzer)
    add_link_options(-fsanitize=fuzzer)

    add_executable(fuzz_wordsplit wordsplit.c wordsplit.h tests/wordsplit_fuzz.c)
    add_test(NAME Fuzz COMMAND fuzz_wordsplit "${PROJECT_SOURCE_DIR}/tests/corpus" -runs=10000)
    set_tests_properties(Fuzz PROPERTIES ENVIRONMENT LLVM_PROFILE_FILE=fuzz_wordsplit.profraw)
endif ()
