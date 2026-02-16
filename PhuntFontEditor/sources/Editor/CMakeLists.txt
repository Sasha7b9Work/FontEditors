cmake_minimum_required(VERSION 3.15)

project (Editor)

include_directories(src
                    ../ThirdParty/wxWidgets/include
                    ../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)
                    
add_definitions(-DWXUSINGDLL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")


add_compile_options(/wd4355) # 'this': used in base member initializer list
add_compile_options(/wd4514) # 'xxx': unreferenced inline function has been removed
add_compile_options(/wd4623) # 'xxx': default constructor was implicitly defined as deleted
add_compile_options(/wd4625) # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626) # assignment operator was implicitly defined as deleted
add_compile_options(/wd4820) # 'bytes' bytes padding added after construct 'member_name'
add_compile_options(/wd5026) # move constructor was implicitly defined as deleted
add_compile_options(/wd5027) # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045) # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd26812)


set_source_files_properties(src/_files/Font.cpp
    PROPERTIES HEADER_FILE_ONLY TRUE)


file(GLOB SRC              src/*.h
                           src/*.cpp)
file(GLOB _FILES           src/_files/*)
file(GLOB DIALOGS          src/Dialogs/*.h
                           src/Dialogs/*.cpp)
file(GLOB DIALOGS_CONTROLS src/Dialogs/Controls/*)
file(GLOB FONT             src/Font/*)

source_group(src               FILES ${SRC})
source_group(_files            FILES ${_FILES})
source_group(Dialogs           FILES ${DIALOGS})
source_group(Dialogs\\Controls FILES ${DIALOGS_CONTROLS})
source_group(Font              FILES ${FONT})

add_executable(Editor ${SRC} ${_FILES} ${DIALOGS} ${DIALOGS_CONTROLS} ${FONT})

target_link_libraries(Editor ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(Editor ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)
target_link_libraries(Editor ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud_xml)

target_link_libraries(Editor WinMM.lib)
target_link_libraries(Editor version.lib)
target_link_libraries(Editor Imm32.Lib)
target_link_libraries(Editor Setupapi.lib)

