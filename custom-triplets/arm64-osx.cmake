set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
# This changed from static to dynamic
set(VCPKG_LIBRARY_LINKAGE dynamic)

set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
set(VCPKG_OSX_ARCHITECTURES arm64)

# this is to support qt build on macos, which does not support macos 15 yet.
set(VCPKG_OSX_DEPLOYMENT_TARGET 14.0)
