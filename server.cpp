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
        std::vector<std::thread> threads;
        for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            threads.emplace_back([this]() { io_context_.run(); });
        }

        for (auto& thread : threads) {
            thread.join();
        }
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

        // Parse headers
        std::unordered_map<std::string, std::string> headers;
        std::string header_line;
        while (std::getline(request_stream, header_line) && header_line != "\r") {
            size_t colon_pos = header_line.find(':');
            if (colon_pos != std::string::npos) {
                std::string header_name = header_line.substr(0, colon_pos);
                std::string header_value = header_line.substr(colon_pos + 2); // Skip ": "
                headers[header_name] = header_value;
            }
        }

        // Parse body if needed
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
    router.add_route("GET", "/hello", [](const std::string& method, const std::string& req, std::string& resp) {
        resp = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    });

    router.add_route("POST", "/data", [](const std::string& method, const std::string& req, std::string& resp) {
        resp = "HTTP/1.1 200 OK\r\nContent-Length: 7\r\n\r\nReceived";
    });

    try {
        HttpServer server(8080, router);
        server.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
