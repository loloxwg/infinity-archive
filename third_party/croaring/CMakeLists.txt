set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

option(ROARING_DISABLE_X64 "Forcefully disable x64 optimizations even if hardware supports it (this disables AVX)" OFF)
option(ROARING_DISABLE_AVX "Forcefully disable AVX even if hardware supports it " OFF)
option(ROARING_DISABLE_NEON "Forcefully disable NEON even if hardware supports it" OFF)
option(ROARING_DISABLE_AVX512 "Forcefully disable AVX512 even if compiler supports it" OFF)

set(SRCS
    "${LIBRARY_DIR}/src/isadetection.c"
    "${LIBRARY_DIR}/src/array_util.c"
    "${LIBRARY_DIR}/src/bitset_util.c"
    "${LIBRARY_DIR}/src/bitset.c"
    "${LIBRARY_DIR}/src/containers/array.c"
    "${LIBRARY_DIR}/src/containers/bitset.c"
    "${LIBRARY_DIR}/src/containers/containers.c"
    "${LIBRARY_DIR}/src/containers/convert.c"
    "${LIBRARY_DIR}/src/containers/mixed_intersection.c"
    "${LIBRARY_DIR}/src/containers/mixed_union.c"
    "${LIBRARY_DIR}/src/containers/mixed_equal.c"
    "${LIBRARY_DIR}/src/containers/mixed_subset.c"
    "${LIBRARY_DIR}/src/containers/mixed_negation.c"
    "${LIBRARY_DIR}/src/containers/mixed_xor.c"
    "${LIBRARY_DIR}/src/containers/mixed_andnot.c"
    "${LIBRARY_DIR}/src/containers/run.c"
    "${LIBRARY_DIR}/src/memory.c"
    "${LIBRARY_DIR}/src/roaring.c"
    "${LIBRARY_DIR}/src/roaring_priority_queue.c"
    "${LIBRARY_DIR}/src/roaring_array.c")

add_library(roaring ${SRCS})

target_include_directories(roaring PRIVATE "${LIBRARY_DIR}/include/roaring")
target_include_directories(roaring SYSTEM BEFORE PUBLIC "${LIBRARY_DIR}/include")
target_include_directories(roaring SYSTEM BEFORE PUBLIC "${LIBRARY_DIR}/cpp")
