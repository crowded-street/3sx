cmake_minimum_required(VERSION 3.24)
project(3sx LANGUAGES C CXX)

include(${PROJECT_SOURCE_DIR}/cmake/imgui_sources.cmake)

set(THREESX_CONFIGURATIONS Debug Release RelWithDebInfo MinSizeRel)
if(NOT PSP)
    list(APPEND THREESX_CONFIGURATIONS Statcheck)
endif()

get_property(THREESX_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(THREESX_IS_MULTI_CONFIG)
    set(CMAKE_CONFIGURATION_TYPES "${THREESX_CONFIGURATIONS}" CACHE STRING "Available build types" FORCE)
else()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${THREESX_CONFIGURATIONS}")
endif()

set(CMAKE_C_FLAGS_STATCHECK "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_STATCHECK "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_STATCHECK "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_STATCHECK "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
set(CMAKE_MODULE_LINKER_FLAGS_STATCHECK "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")

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
    ${PROJECT_SOURCE_DIR}/src/imgui/imgui
    ${PROJECT_SOURCE_DIR}/src/imgui/dcimgui
    ${PROJECT_SOURCE_DIR}/src/sdk
    ${PROJECT_SOURCE_DIR}/src/platform/video/opengl/glad
)

# ======================================
# Source collection
# ======================================

file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS src/*.c)

add_executable(3sx MACOSX_BUNDLE
    ${GAME_SRC}
)

if(NOT PSP)
    target_sources(3sx PRIVATE
        $<$<CONFIG:Debug>:${IMGUI_SOURCES}>
    )
endif()

# ======================================
# Compiler and linker flags
# ======================================

if(PSP)
    target_compile_definitions(3sx PRIVATE
        $<$<CONFIG:Debug>:DEBUG>

        $<$<CONFIG:Release>:RELEASE>

        CRS_APP_DRIVER_PSP
        CRS_VIDEO_DRIVER_PSP
        CRS_INPUT_DRIVER_SDL
    )
else()
    target_compile_definitions(3sx PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Debug>:NETPLAY_ENABLED>

        $<$<CONFIG:Release>:RELEASE>
        $<$<CONFIG:Release>:CHECKSUM>
        
        $<$<CONFIG:Statcheck>:STATCHECK>

        # GekkoNet is a pre-built static lib; these were previously propagated by its cmake target
        GEKKONET_STATIC
        GEKKONET_NO_ASIO

        CRS_APP_DRIVER_SDL
        CRS_VIDEO_DRIVER_SDL_GENERIC
        CRS_VIDEO_DRIVER_SDL_GPU
        CRS_VIDEO_DRIVER_OPENGL
        $<$<NOT:$<CONFIG:Statcheck>>:CRS_INPUT_DRIVER_SDL>
        $<$<CONFIG:Statcheck>:CRS_INPUT_DRIVER_STATCHECK>

        ARCADE_ROM
        SOUND_ENABLED
        $<$<CONFIG:Debug>:IMGUI>

        GL_SILENCE_DEPRECATION
    )
endif()


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

    $<$<OR:$<CONFIG:Debug>,$<CONFIG:Statcheck>>:-Wno-unused-function>
    # $<$<CONFIG:Debug>:-Wno-unused-variable>
    -Wno-unused-variable
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:Statcheck>>:-Wno-unused-but-set-variable>
)

if(PSP)
    target_compile_options(3sx PRIVATE -G0)
    target_link_options(3sx PRIVATE -G0)
endif()

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

find_package(ZLIB REQUIRED)
find_package(OpenGL REQUIRED)

set(FFMPEG_ROOT        "${THIRD_PARTY_DIR}/ffmpeg/build")
set(SDL3_ROOT          "${THIRD_PARTY_DIR}/sdl3/build")
set(LIBCDIO_ROOT       "${THIRD_PARTY_DIR}/libcdio/build")
set(GEKKONET_ROOT      "${THIRD_PARTY_DIR}/GekkoNet/build")
set(SDL3_NET_ROOT      "${THIRD_PARTY_DIR}/SDL_net/build")
set(MINIZIP_NG_ROOT    "${THIRD_PARTY_DIR}/minizip-ng/build")
set(TF_PSA_CRYPTO_ROOT "${THIRD_PARTY_DIR}/tf-psa-crypto/build")

include_directories(
    ${FFMPEG_ROOT}/include
    ${SDL3_ROOT}/include
    ${LIBCDIO_ROOT}/include
    ${GEKKONET_ROOT}/include
    ${SDL3_NET_ROOT}/include
    ${MINIZIP_NG_ROOT}/include
    ${TF_PSA_CRYPTO_ROOT}/include
)

if(PSP)
    target_link_libraries(3sx PRIVATE
        m
        "${SDL3_ROOT}/lib/libSDL3.a"
        GL
        ZLIB::ZLIB
        pspdebug
        pspvram
        pspaudio
        pspvfpu
        pspgu
        pspge
        pspdisplay
        psphprm
        pspctrl
        psppower
    )
else()
    target_link_libraries(3sx PRIVATE
        m
        "${LIBCDIO_ROOT}/lib/libiso9660.a"
        "${LIBCDIO_ROOT}/lib/libcdio.a"
        "${GEKKONET_ROOT}/lib/libGekkoNet.a"
        "${SDL3_NET_ROOT}/lib/libSDL3_net.a"
        "${MINIZIP_NG_ROOT}/lib/libminizip-ng.a"
        "${TF_PSA_CRYPTO_ROOT}/lib/libtfpsacrypto.a"
        ZLIB::ZLIB
        OpenGL::GL
        stdc++ # For GekkoNet
    )
endif()

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
        bcrypt
		dbghelp
        ws2_32
        iphlpapi
    )
    target_link_options(3sx PRIVATE
        "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>,$<CONFIG:Statcheck>>:--for-linker>"
        "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>,$<CONFIG:Statcheck>>:--pdb=3sx.pdb>"
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

set(SDL_GPU_SHADER_DIR "${CMAKE_SOURCE_DIR}/src/platform/video/sdl_gpu/shaders/compiled")
set(OPENGL_SHADER_DIR "${CMAKE_SOURCE_DIR}/src/platform/video/opengl/shaders")

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

    install(CODE "file(REMOVE_RECURSE \"\${CMAKE_INSTALL_PREFIX}/3SX.app/Contents/Resources/shaders/sdlgpu/compiled\")")

    install(DIRECTORY "${SDL_GPU_SHADER_DIR}/"
        DESTINATION 3SX.app/Contents/Resources/shaders/sdlgpu
    )

    install(DIRECTORY "${OPENGL_SHADER_DIR}/"
        DESTINATION 3SX.app/Contents/Resources/shaders/opengl
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
        "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>,$<CONFIG:Statcheck>>:${CMAKE_CURRENT_BINARY_DIR}/3sx.pdb>"
        DESTINATION bin
    )

    install(FILES
        ${PROJECT_LICENSE_FILES}
        DESTINATION bin/licenses
    )

    install(CODE "file(REMOVE_RECURSE \"\${CMAKE_INSTALL_PREFIX}/bin/shaders/sdlgpu/compiled\")")

    install(DIRECTORY "${SDL_GPU_SHADER_DIR}/"
        DESTINATION bin/shaders/sdlgpu
    )

    install(DIRECTORY "${OPENGL_SHADER_DIR}/"
        DESTINATION bin/shaders/opengl
    )
elseif(UNIX)
    install(TARGETS 3sx
        RUNTIME DESTINATION bin
    )

    file(GLOB FFMPEG_SO "${FFMPEG_ROOT}/lib/*.so*")
    file(GLOB SDL3_SO "${SDL3_ROOT}/lib/libSDL3.so*")

    install(FILES
        ${FFMPEG_SO}
        ${SDL3_SO}
        DESTINATION lib
    )

    install(FILES
        ${PROJECT_LICENSE_FILES}
        DESTINATION share/3sx/licenses
    )

    install(CODE "file(REMOVE_RECURSE \"\${CMAKE_INSTALL_PREFIX}/bin/shaders/sdlgpu/compiled\")")

    install(DIRECTORY "${SDL_GPU_SHADER_DIR}/"
        DESTINATION bin/shaders/sdlgpu
    )

    install(DIRECTORY "${OPENGL_SHADER_DIR}/"
        DESTINATION bin/shaders/opengl
    )
endif()

# ======================================
# Packaging
# ======================================

if(PSP)
    create_pbp_file(
        TARGET 3sx
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        MUSIC_PATH NULL
        PREVIEW_PATH NULL
        TITLE "3SX"
        VERSION 0.1
    )
else()
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
endif()
