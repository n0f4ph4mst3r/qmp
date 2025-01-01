set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)

if(PORT MATCHES "qtbase|qtmultimedia|qtshadertools")
    set (VCPKG_LIBRARY_LINKAGE dynamic)
else()
    set (VCPKG_LIBRARY_LINKAGE static)
endif()

set(VCPKG_FIXUP_ELF_RPATH ON)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)