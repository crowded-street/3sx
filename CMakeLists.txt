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
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/sdk
    ${PROJECT_SOURCE_DIR}/zlib
)

# ======================================
# Source collection
# ======================================

file(GLOB_RECURSE GAME_SRC src/*.c)
file(GLOB_RECURSE ZLIB_SRC zlib/*.c)

add_executable(3sx MACOSX_BUNDLE
    ${GAME_SRC} ${ZLIB_SRC}
)

# ======================================
# Compiler and linker flags
# ======================================

target_compile_definitions(3sx PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:NETPLAY_ENABLED>
    $<$<CONFIG:Release>:RELEASE>
    TARGET_SDL3
    _POSIX_C_SOURCE
    # GekkoNet is a pre-built static lib; these were previously propagated by its cmake target
    GEKKONET_STATIC
    GEKKONET_NO_ASIO
)

# Feature toggles
target_compile_definitions(3sx PRIVATE
    MEMCARD_DISABLED
    # CPS3
)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    set(DISABLED_WARNINGS
        -Wpointer-to-int-cast
        -Wno-deprecated-non-prototype
        -Wno-tautological-overlap-compare
        -Wno-c2x-extensions
        -Wno-incompatible-pointer-types
        -Wno-pointer-sign
    )
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(DISABLED_WARNINGS
        -Wno-unused-but-set-variable
        -Wno-pointer-sign
        -Wno-parentheses
        -Wno-incompatible-pointer-types
        -Wno-discarded-qualifiers
        -Wno-format-overflow
        -Wno-comment
        -Wno-maybe-uninitialized
        -Wno-array-bounds
        -Wno-aggressive-loop-optimizations
        -Wno-stringop-overflow
        -Wno-restrict
        -Wno-stringop-truncation
    )
endif()

target_compile_options(3sx PRIVATE
    -Wall
    -Werror

    -fno-strict-aliasing

    ${DISABLED_WARNINGS}
)

set_target_properties(3sx PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/Info.plist"
)

if(APPLE)
    set_target_properties(3sx PROPERTIES
        OUTPUT_NAME "3SX"
        MACOSX_BUNDLE_BUNDLE_NAME "3SX"
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

set(FFMPEG_ROOT    "${THIRD_PARTY_DIR}/ffmpeg/build")
set(SDL3_ROOT      "${THIRD_PARTY_DIR}/sdl3/build")
set(LIBCDIO_ROOT   "${THIRD_PARTY_DIR}/libcdio/build")
set(GEKKONET_ROOT  "${THIRD_PARTY_DIR}/GekkoNet/build")
set(SDL3_NET_ROOT  "${THIRD_PARTY_DIR}/SDL_net/build")

include_directories(
    ${FFMPEG_ROOT}/include
    ${SDL3_ROOT}/include
    ${LIBCDIO_ROOT}/include
    ${GEKKONET_ROOT}/include
    ${SDL3_NET_ROOT}/include
)

target_link_libraries(3sx PRIVATE
    m
    "${LIBCDIO_ROOT}/lib/libiso9660.a"
    "${LIBCDIO_ROOT}/lib/libcdio.a"
    "${GEKKONET_ROOT}/lib/libGekkoNet.a"
    "${SDL3_NET_ROOT}/lib/libSDL3_net.a"
    stdc++
)

if(APPLE)
    target_link_libraries(3sx PRIVATE
        ${FFMPEG_ROOT}/lib/libavcodec.dylib
        ${FFMPEG_ROOT}/lib/libavformat.dylib
        ${FFMPEG_ROOT}/lib/libavutil.dylib
        ${FFMPEG_ROOT}/lib/libswresample.dylib
        ${SDL3_ROOT}/lib/libSDL3.0.dylib
        iconv
    )
elseif(WIN32)
    target_link_libraries(3sx PRIVATE
        ${FFMPEG_ROOT}/lib/libavcodec.dll.a
        ${FFMPEG_ROOT}/lib/libavformat.dll.a
        ${FFMPEG_ROOT}/lib/libavutil.dll.a
        ${FFMPEG_ROOT}/lib/libswresample.dll.a
        ${SDL3_ROOT}/lib/libSDL3.dll.a
        iconv
		dbghelp
        ws2_32
        iphlpapi
    )
    target_link_options(3sx PRIVATE
        --for-linker
        --pdb=3sx.pdb
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

set(PROJECT_LICENSE_FILES
    "${CMAKE_SOURCE_DIR}/LICENSE"
    "${CMAKE_SOURCE_DIR}/THIRD_PARTY_NOTICES.txt"
)

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

    install(FILES
        ${PROJECT_LICENSE_FILES}
        DESTINATION 3SX.app/Contents/Resources/licenses
    )
elseif(WIN32)
    install(TARGETS 3sx
		RUNTIME_DEPENDENCY_SET deps
        RUNTIME DESTINATION bin
    )

    execute_process(COMMAND cygpath -m /mingw64/bin
		OUTPUT_VARIABLE binPath
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# automatically copies all the dependent DLLS, and ignores the system ones because they aren't necessary
	install(RUNTIME_DEPENDENCY_SET deps
		DIRECTORIES ${binPath} "${FFMPEG_ROOT}/bin" "${SDL3_ROOT}/bin"
		PRE_EXCLUDE_REGEXES "^api"
		POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
		DESTINATION bin
	)

    install(FILES
        "build/3sx.pdb"
        DESTINATION bin
    )

    install(FILES
        ${PROJECT_LICENSE_FILES}
        DESTINATION bin/licenses
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

    install(FILES
        ${PROJECT_LICENSE_FILES}
        DESTINATION share/3sx/licenses
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
