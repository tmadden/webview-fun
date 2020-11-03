
#include "webview.h"

#include "winuser.h"

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <stdlib.h>
#include <stdio.h>

#include "server.hpp"

std::unique_ptr<webview::browser> the_browser;

static void error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void window_size_callback(GLFWwindow *window, int width, int height)
{
    the_browser->resize(glfwGetWin32Window(window));
    the_browser->eval("document.body.innerHTML = \"<p>Hello, World!!!</p>\";");
    // the_browser->eval("location.reload(true);");
}

static void on_message(const std::string msg)
{
}

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInt, HINSTANCE hPrevInst, LPSTR lpCmdLine,
                   int nCmdShow)
{
#else
int main()
{
#endif
    wrap_with_server([&] {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        // webview::webview w(true, nullptr);
        // w.set_size(960, 1280, WEBVIEW_HINT_NONE);
        // w.set_title("Minimal example");
        // w.navigate("http://localhost:8080");
        // w.run();

        GLFWwindow *window;

        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
            exit(EXIT_FAILURE);

        window = glfwCreateWindow(1200, 1600, "Simple Example", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        glfwSetKeyCallback(window, key_callback);

        glfwSetWindowSizeCallback(window, window_size_callback);

        HWND hwnd = glfwGetWin32Window(window);
        the_browser = std::make_unique<webview::edge_chromium>();
        if (!the_browser->embed(hwnd, true, on_message))
        {
            the_browser = std::make_unique<webview::edge_html>();
            the_browser->embed(hwnd, true, on_message);
        }
        the_browser->resize(hwnd);
        the_browser->navigate("http://127.0.0.1:8080/");
        //the_browser->navigate("https://google.com/");
        //the_browser->init("document.body.innerHTML = \"<p>Hello, World!!!</p>\";");
        //the_browser->eval(
        //"alert(location.hostname);");

        while (!glfwWindowShouldClose(window))
        {
            float ratio;
            int width, height;

            glfwGetFramebufferSize(window, &width, &height);
            ratio = width / (float)height;

            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
            glMatrixMode(GL_MODELVIEW);

            glLoadIdentity();
            glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

            glBegin(GL_TRIANGLES);
            glColor3f(1.f, 0.f, 0.f);
            glVertex3f(-0.6f, -0.4f, 0.f);
            glColor3f(0.f, 1.f, 0.f);
            glVertex3f(0.6f, -0.4f, 0.f);
            glColor3f(0.f, 0.f, 1.f);
            glVertex3f(0.f, 0.6f, 0.f);
            glEnd();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwDestroyWindow(window);

        glfwTerminate();
        exit(EXIT_SUCCESS);
    });

    return 0;
}
