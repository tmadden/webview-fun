cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(HUNTER_PACKAGES glfw)
set(HUNTER_GLFW_CMAKE_ARGS USE_MSVC_RUNTIME_LIBRARY_DLL=ON)

FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
FetchContent_MakeAvailable(SetupHunter)

FetchContent_Declare(
  webview
  GIT_REPOSITORY https://github.com/webview/webview.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(webview)

project(webview_fun)

find_package(glfw3 CONFIG REQUIRED)

file(GLOB sources CONFIGURE_DEPENDS "*.cpp")

include_directories(${PROJECT_SOURCE_DIR} ${webview_SOURCE_DIR} ${webview_SOURCE_DIR}/script)
link_directories(${webview_SOURCE_DIR}/script/microsoft.web.webview2.0.9.488/build/native/x64)

add_executable(webview_fun WIN32 ${sources})
set_property(TARGET webview_fun PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_link_libraries(webview_fun PRIVATE glfw WebView2Loader.dll.lib)

add_custom_command(TARGET webview_fun POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${webview_SOURCE_DIR}/script/microsoft.web.webview2.0.9.488/build/native/x64/WebView2Loader.dll"
        $<TARGET_FILE_DIR:webview_fun>)

