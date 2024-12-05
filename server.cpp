#include <boost/asio.hpp>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <thread>
#include <string>

using boost::asio::ip::tcp;

class Router {
public:
    using Handler = std::function<void(const std::string&, const std::string&, std::string&)>;

    void add_route(const std::string& method, const std::string& path, Handler handler) {
        routes_[method + " " + path] = handler;
    }

    bool route(const std::string& method, const std::string& path, const std::string& request, std::string& response) {
        auto it = routes_.find(method + " " + path);
        if (it != routes_.end()) {
            it->second(method, request, response);
            return true;
        }
        return false;
    }

private:
    std::unordered_map<std::string, Handler> routes_;
};

class HttpServer {
public:
    HttpServer(short port, Router& router) 
        : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)), router_(router) {
        start_accept();
    }

    void run() {
        // Obține numărul de fire pe baza numărului de nuclee hardware
        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 2;

        MyThreads thread_pool(num_threads);

        // Rulează `io_context_` în mai multe fire
        thread_pool.run([this]() { io_context_.run(); });

        // Așteaptă finalizarea tuturor firelor
        thread_pool.join();
    }

private:
    void start_accept() {
        auto socket = std::make_shared<tcp::socket>(io_context_);
        acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                std::thread(&HttpServer::handle_request, this, socket).detach();
            }
            start_accept();
        });
    }

    void handle_request(std::shared_ptr<tcp::socket> socket) {
        try {
            boost::asio::streambuf buffer;
            boost::asio::read_until(*socket, buffer, "\r\n\r\n");

            std::istream request_stream(&buffer);
            std::string request_line;
            std::getline(request_stream, request_line);

            std::string method, path, version;
            std::istringstream request_line_stream(request_line);
            request_line_stream >> method >> path >> version;

            std::unordered_map<std::string, std::string> headers;
            std::string header_line;
            while (std::getline(request_stream, header_line) && header_line != "\r") {
                size_t colon_pos = header_line.find(':');
                if (colon_pos != std::string::npos) {
                    std::string header_name = header_line.substr(0, colon_pos);
                    std::string header_value = header_line.substr(colon_pos + 2);
                    headers[header_name] = header_value;
                }
            }

            std::string body;
            if (headers.find("Content-Length") != headers.end()) {
                size_t content_length = std::stoul(headers["Content-Length"]);
                std::vector<char> body_buffer(content_length);
                request_stream.read(body_buffer.data(), content_length);
                body.assign(body_buffer.begin(), body_buffer.end());
            }

            std::string response;
            if (!router_.route(method, path, body, response)) {
                response = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
            }

            boost::asio::write(*socket, boost::asio::buffer(response));
        } catch (std::exception& e) {
            std::cerr << "Exception in thread: " << e.what() << "\n";
        }
    }

    boost::asio::io_context io_context_;
    tcp::acceptor acceptor_;
    Router& router_;
};


int main() 
{
    Router router;
    router.add_route("GET", "/interactive", [](const std::string& method, const std::string& req, std::string& resp) {
        // HTML, CSS, și JavaScript pentru a crea un efect interactiv
        std::string html_response = 
            "<!DOCTYPE html>"
            "<html lang=\"en\">"
            "<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Pagini Interactive</title>"
            "<style>"
            "body { text-align: center; font-family: Arial, sans-serif; padding: 50px; }"
            ".color-box { width: 100px; height: 100px; background-color: red; margin: 20px auto; }"
            "</style></head>"
            "<body>"
            "<h1>Schimbă Culoarea Cu JavaScript</h1>"
            "<p>Fă clic pe butonul de mai jos pentru a schimba culoarea căsuței.</p>"
            "<div class=\"color-box\" id=\"box\"></div>"
            "<button onclick=\"changeColor()\">Schimbă Culoarea</button>"
            "<script>"
            "function changeColor() {"
            "    var box = document.getElementById('box');"
            "    var colors = ['red', 'blue', 'green', 'yellow', 'orange'];"
            "    var currentColor = box.style.backgroundColor;"
            "    var newColor = colors[Math.floor(Math.random() * colors.length)];"
            "    while (newColor === currentColor) {"
            "        newColor = colors[Math.floor(Math.random() * colors.length)];"
            "    }"
            "    box.style.backgroundColor = newColor;"
            "}"
            "</script>"
            "</body>"
            "</html>";

        // Calculăm Content-Length din dimensiunea răspunsului HTML
        std::string content_length = "Content-Length: " + std::to_string(html_response.size()) + "\r\n";

        resp = "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html; charset=UTF-8\r\n" +
               content_length + "\r\n" +
               html_response;
    });

    try {
        HttpServer server(8080, router);
        server.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
