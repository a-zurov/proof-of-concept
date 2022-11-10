////////////////////////////////////////////
Boost: getting started with Windows project
////////////////////////////////////////////
0.0 Download http://www.boost.org/users/download/
1.0 Extract \proof-of-concept\external_sdk\boost\boost_1_XX_0
1.1 Add BOOST_ROOT = <full_path_to>\external_sdk\boost\boost_1_XX_0 to your environment variable
2.0 Go to the directory \external_sdk\boost\boost_1_XX_0\tools\build\ and run bootstrap.bat to build the engine b2.exe
2.1 Run b2 install --prefix=PREFIX where PREFIX is the directory where you want Boost.Build to be installed,
    for example <full_path_to>\external_sdk\boost\b2build
2.3 Add PREFIX\bin to your PATH environment variable, i.e. <full_path_to>\external_sdk\boost\b2build\bin,
    if you have found b2.exe in this directory
3.0 Copy build_boost_static_x64_msvc_14_3.bat to \external_sdk\boost\boost_1_XX_0
    if you have default installation paths for Microsoft Visual Studio 2022 (or edit *.bat)
3.1 Run build_boost_##_x86_##.bat to create \boost_1_XX_0\libraries_x86 or
    run build_boost_##_x64_##.bat to create \boost_1_XX_0\libraries

P.S.File \proof-of-concept\pseudo-Com\_inherited_property\boost.vsprops should be properly connected
    with BOOST_ROOT environment variable and libraries directory (see above-mentioned 1.1 and 3.1 and *.vsprops)
    <UserMacro
        Name="BoostPath"
        Value="$(BOOST_ROOT)"
    />
https://learn.microsoft.com/en-us/cpp/build/project-property-inheritance?view=msvc-170
