cmake_minimum_required(VERSION 3.15)
project(webview_fun)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  webview
  GIT_REPOSITORY https://github.com/webview/webview.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(webview)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")

include_directories(${PROJECT_SOURCE_DIR} ${webview_SOURCE_DIR} ${webview_SOURCE_DIR}/script)
link_directories(${webview_SOURCE_DIR}/script/microsoft.web.webview2.0.9.488/build/native/x64)

add_executable(webview_fun WIN32 ${sources})
target_link_libraries(webview_fun WebView2Loader.dll.lib)

add_custom_command(TARGET webview_fun POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${webview_SOURCE_DIR}/script/microsoft.web.webview2.0.9.488/build/native/x64/WebView2Loader.dll"
        $<TARGET_FILE_DIR:webview_fun>)

