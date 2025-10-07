cmake_minimum_required(VERSION 3.24)
project(3sx LANGUAGES C)

# ======================================
# Options and platform setup
# ======================================

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ======================================
# Include directories
# ======================================

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/sdk
    ${PROJECT_SOURCE_DIR}/include/cri
    ${PROJECT_SOURCE_DIR}/include/cri/ee
    ${PROJECT_SOURCE_DIR}/zlib
    ${PROJECT_SOURCE_DIR}/libco
)

# ======================================
# Source collection
# ======================================

file(GLOB_RECURSE GAME_SRC src/sf33rd/*.c)
file(GLOB_RECURSE CRI_SRC src/cri/*.c)
file(GLOB_RECURSE BIN2OBJ_SRC src/bin2obj/*.c)
file(GLOB_RECURSE PORT_SRC src/port/*.c)
file(GLOB_RECURSE ZLIB_SRC zlib/*.c)

add_subdirectory(libco)

add_executable(3sx MACOSX_BUNDLE
    ${GAME_SRC} ${CRI_SRC} ${BIN2OBJ_SRC} ${PORT_SRC} ${ZLIB_SRC}
)

# ======================================
# Compiler and linker flags
# ======================================

target_compile_definitions(3sx PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
    TARGET_SDL3
    SOUND_DISABLED
    XPT_TGT_EE
    _POSIX_C_SOURCE
)

target_compile_options(3sx PRIVATE
    -Wall
    -Werror
    -Wpointer-to-int-cast
    -Wno-macro-redefined
    -Wno-deprecated-non-prototype
    -Wno-missing-braces
    -Wno-self-assign
    -Wno-format
    -Wno-tautological-constant-out-of-range-compare
    -Wno-tautological-overlap-compare
    -Wno-gcc-compat
    -Wno-unknown-pragmas
    -Wno-c2x-extensions
    -Wno-unused-value
    -Wno-unused-variable
    -Wno-unused-but-set-variable
    -Wno-unused-comparison
    -Wno-unused-function
    -Wno-int-to-pointer-cast
    -Wno-int-conversion
    -Wno-incompatible-pointer-types
    -Wno-incompatible-function-pointer-types
    -Wno-pointer-sign
    -Wno-shift-count-overflow
)

target_link_libraries(3sx PRIVATE
    libco
    m
)

set_target_properties(3sx PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/Info.plist"
)

if(APPLE)
    set_target_properties(3sx PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
    )
elseif(UNIX AND NOT APPLE)
    set_target_properties(3sx PROPERTIES
        INSTALL_RPATH "\$ORIGIN/../lib"
    )
endif()

# ======================================
# Dependencies
# ======================================

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(FFMPEG_ROOT "${THIRD_PARTY_DIR}/ffmpeg/build")
set(SDL3_ROOT "${THIRD_PARTY_DIR}/sdl3/build")

include_directories(
    ${FFMPEG_ROOT}/include
    ${SDL3_ROOT}/include
)

if(APPLE)
    target_link_libraries(3sx PRIVATE
        ${FFMPEG_ROOT}/lib/libavcodec.dylib
        ${FFMPEG_ROOT}/lib/libavformat.dylib
        ${FFMPEG_ROOT}/lib/libavutil.dylib
        ${FFMPEG_ROOT}/lib/libswresample.dylib
        ${SDL3_ROOT}/lib/libSDL3.0.dylib
    )
elseif(WIN32)
    target_link_libraries(3sx PRIVATE
        ${FFMPEG_ROOT}/lib/avcodec.lib
        ${FFMPEG_ROOT}/lib/avformat.lib
        ${FFMPEG_ROOT}/lib/avutil.lib
        ${FFMPEG_ROOT}/lib/swresample.lib
        ${SDL3_ROOT}/lib/SDL3.lib
    )
elseif(UNIX)
    target_link_libraries(3sx PRIVATE
        ${FFMPEG_ROOT}/lib/libavcodec.so
        ${FFMPEG_ROOT}/lib/libavformat.so
        ${FFMPEG_ROOT}/lib/libavutil.so
        ${FFMPEG_ROOT}/lib/libswresample.so
        ${SDL3_ROOT}/lib/libSDL3.so
    )
endif()

# ======================================
# Installation
# ======================================

if(APPLE)
    install(TARGETS 3sx
        RUNTIME DESTINATION bin
        BUNDLE DESTINATION .
    )

    file(GLOB FFMPEG_DYLIBS "${FFMPEG_ROOT}/lib/*.dylib")

    install(FILES
        ${FFMPEG_DYLIBS}
        ${SDL3_ROOT}/lib/libSDL3.0.dylib
        DESTINATION 3SX.app/Contents/Frameworks
    )
elseif(WIN32)
    install(TARGETS 3sx
        RUNTIME DESTINATION bin
    )

    file(GLOB FFMPEG_DLLS "${FFMPEG_ROOT}/bin/*.dll")

    install(FILES
        ${FFMPEG_DLLS}
        ${SDL3_ROOT}/bin/SDL3.dll
        DESTINATION bin
    )
elseif(UNIX)
    install(TARGETS 3sx
        RUNTIME DESTINATION bin
    )

    file(GLOB FFMPEG_SO "${FFMPEG_ROOT}/lib/*.so*")

    install(FILES
        ${FFMPEG_SO}
        ${SDL3_ROOT}/lib/libSDL3.so
        DESTINATION lib
    )
endif()

# ======================================
# Packaging
# ======================================

set(CPACK_PACKAGE_NAME "3SX")
set(CPACK_PACKAGE_VENDOR "CrowdedStreet")
# set(CPACK_PACKAGE_VERSION "1.0.0")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else(UNIX)
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
