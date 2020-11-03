#include <restinio/all.hpp>

#include <thread>
#include <chrono>

void wrap_with_server(std::function<void()> main)
{
    // Create server instance on the main thread.
    using my_server_t = restinio::http_server_t<restinio::default_single_thread_traits_t>;
    my_server_t server{
        restinio::own_io_context(),
        [](auto &settings) {
            settings.port(8080);
            settings.address("localhost");
            settings.request_handler([](auto req) {
                return req->create_response().set_body("<html><head></head><body><p>Hello World</p></body></html>").done();
            });
        }};
    // Create a separate thread to control execution of RESTinio's server.
    std::thread restinio_control_thread{[&server] {
        // Use restinio::run to launch RESTinio's server.
        // This run() will return only if server stopped from
        // some other thread.
        restinio::run(restinio::on_thread_pool(
            4,                                      // Count of worker threads for RESTinio.
            restinio::skip_break_signal_handling(), // Don't react to Ctrl+C.
            server)                                 // Server to be run.
        );
    }};

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    main();

    // Now RESTinio can be stopped.
    restinio::initiate_shutdown(server);
    // Wait for completeness of RESTinio's shutdown.
    restinio_control_thread.join();
}