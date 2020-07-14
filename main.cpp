// main.c
#include "webview.h"
#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInt, HINSTANCE hPrevInst, LPSTR lpCmdLine,
                   int nCmdShow)
{
#else
int main()
{
#endif
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    webview::webview w(false, nullptr);
    w.set_title("Minimal example");
    w.set_size(1200, 800, WEBVIEW_HINT_MIN);
    w.navigate("http://127.0.0.1:8000/");
    w.run();
    return 0;
}
