cmake_minimum_required(VERSION 3.14)
project(webview_fun)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Download/include CMake/Conan integration.
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake"
                  TLS_VERIFY ON)
endif()
include(${CMAKE_CURRENT_BINARY_DIR}/conan.cmake)

# Invoke Conan.
conan_add_remote(
    NAME bincrafters
    INDEX 1
    URL https://api.bintray.com/conan/bincrafters/public-conan
    VERIFY_SSL True)
conan_cmake_run(
    REQUIRES
        glfw/3.3.2
    BASIC_SETUP
    BUILD missing)

FetchContent_Declare(
  webview
  GIT_REPOSITORY https://github.com/webview/webview.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(webview)

file(GLOB sources CONFIGURE_DEPENDS "*.cpp")

include_directories(${PROJECT_SOURCE_DIR} ${webview_SOURCE_DIR} ${webview_SOURCE_DIR}/script)
link_directories(${webview_SOURCE_DIR}/script/microsoft.web.webview2.0.9.488/build/native/x64)

add_executable(webview_fun WIN32 ${sources})
set_property(TARGET webview_fun PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(webview_fun PUBLIC ${CONAN_INCLUDE_DIRS})
target_compile_definitions(webview_fun PUBLIC
  ${CONAN_DEFINES} ${CONAN_COMPILE_DEFINITIONS})
target_link_directories(webview_fun PUBLIC ${CONAN_LIB_DIRS})
target_link_libraries(webview_fun PUBLIC ${CONAN_LIBS} WebView2Loader.dll.lib)

add_custom_command(TARGET webview_fun POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${webview_SOURCE_DIR}/script/microsoft.web.webview2.0.9.488/build/native/x64/WebView2Loader.dll"
        $<TARGET_FILE_DIR:webview_fun>)
