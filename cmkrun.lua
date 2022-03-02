Command {
    name = 'genbuild',
    description = 'generate the cmake build',
    fn = function(...)
        info('vcpkg based cmake build on windows.')
        local vcpkg_root = os.getenv('VCPKG_ROOT')
        local triplet = "x64-linux"
        if isWindows then
            triplet = "x64-windows"
        end
	local extra_args = ''
	if not isWindows then
		extra_args = extra_args .. ' -DSKIA_DIR=$HOME/lib/skia'
		extra_args = extra_args .. ' -DSKIA_LIBRARY_DIR=$HOME/lib/skia/out/Release-x64'
		extra_args = extra_args .. ' -DSKIA_LIBRARY=$HOME/lib/skia/out/Release-x64/libskia.a'
		extra_args = extra_args .. ' -DSKIA_OPENGL_LIBRARY=""'
	end
        if vcpkg_root ~= nil then
            run_cmake(
                ".", "-B", CMAKE_BUILD_DIR, "-DCMAKE_TOOLCHAIN_FILE=" ..
                    vcpkg_root .. "/scripts/buildsystems/vcpkg.cmake",
                "-DVCPKG_TARGET_TRIPLET=" .. triplet .. extra_args, ...)
        end
    end
}
