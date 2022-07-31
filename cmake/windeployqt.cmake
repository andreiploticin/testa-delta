set(CMAKE_PREFIX_PATH "C:/Qt/6.3.1/mingw_64/lib/cmake/")

find_package(Qt6 REQUIRED COMPONENTS Core)

# get absolute path to qmake, then use it to find windeployqt executable

get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

function(windeployqt target)

    # POST_BUILD step
    # - after build, we have a bin/lib for analyzing qt dependencies
    # - we run windeployqt on target and deploy Qt libs

    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${_qt_bin_dir}/windeployqt.exe"
            # --debug
            --verbose 1
            --no-svg
            --no-opengl
            --no-opengl-sw
            --no-compiler-runtime
            --no-system-d3d-compiler
            $<TARGET_FILE:${target}>
            COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
            )

endfunction()
