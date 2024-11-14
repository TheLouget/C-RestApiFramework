#include <boost/asio.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Rezolvă adresa serverului
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("localhost", "8080");

        // Creează și conectează socket-ul
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // Formatează cererea HTTP
        std::string request = "GET /hello HTTP/1.1\r\n";
        request += "Host: localhost\r\n";
        request += "Connection: close\r\n\r\n";

        // Trimite cererea
        boost::asio::write(socket, boost::asio::buffer(request));

        // Citește răspunsul
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        // Verifică răspunsul HTTP
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);

        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            std::cout << "Răspuns invalid\n";
            return 1;
        }
        if (status_code != 200) {
            std::cout << "Răspuns a eșuat: " << status_code << "\n";
            return 1;
        }

        // Citește antetele răspunsului
        boost::asio::read_until(socket, response, "\r\n\r\n");

        // Afișează antetele răspunsului
        std::string header;
        while (std::getline(response_stream, header) && header != "\r") {
            std::cout << header << "\n";
        }
        std::cout << "\n";

        // Citește corpul răspunsului
        std::ostringstream ss;
        ss << &response;
        std::cout << ss.str();
    } catch (std::exception& e) {
        std::cerr << "Excepție: " << e.what() << "\n";
    }

    return 0;
}
