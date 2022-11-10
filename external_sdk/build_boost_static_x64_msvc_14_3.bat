call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
b2.exe toolset=msvc-14.3 --clean
b2.exe install toolset=msvc-14.3 address-model=64 link=static --libdir=libraries -stdlib=libc++ runtime-link=static --without-python --without-graph --without-graph_parallel --without-math --without-test --without-wave --without-chrono --without-context
