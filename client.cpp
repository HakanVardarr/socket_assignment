#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <format>
#include <iostream>
#include <string>
#include <thread>

const int PORT = 8080;
const std::string SERVER_IP = "127.0.0.1";

int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        std::cerr << "Failed to create client socket: " << std::strerror(errno) << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP.c_str());

    if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Connection failed to the server: " << std::strerror(errno) << std::endl;
        return 1;
    }

    std::string client_name = "Client of Hakan Vardar";
    int client_number;

    std::cout << "Enter a number: ";
    if (!(std::cin >> client_number)) {
        std::cerr << "Given number is invalid: " << std::strerror(errno) << std::endl;
        return 1;
    }

    std::string message = std::format("{} {}", client_name, client_number);
    send(sock_fd, message.c_str(), message.size(), 0);

    char buffer[1024];
    ssize_t bytes_recieved = recv(sock_fd, buffer, sizeof(buffer), 0);

    if (bytes_recieved > 0) {
        buffer[bytes_recieved] = '\0';

        std::string server_message(buffer);

        size_t i = 0;
        char current_char = server_message[i];
        if (isdigit(current_char)) {
            std::cerr << "Invalid format!" << std::endl;
            close(sock_fd);
            return 1;
        }

        while (!isdigit(current_char) && i < server_message.length()) {
            current_char = server_message[++i];
        }

        if (i >= server_message.length()) {
            std::cerr << "Invalid format!" << std::endl;
            close(sock_fd);
            return 1;
        }

        std::string server_name = server_message.substr(0, i);
        try {
            int server_number = std::stoi(server_message.substr(i));
            if (server_number < 0 || client_number > 100) {
                std::cerr << "Invalid format!" << std::endl;
                close(sock_fd);
                return 1;
            }

            std::cout << "Clients name: " << client_name << "\n";
            std::cout << "Servers name: " << server_name << "\n";
            std::cout << "Clients number: " << client_number << "\n";
            std::cout << "Servers number: " << server_number << "\n";
            std::cout << "Sum of the numbers: " << server_number + client_number << std::endl;

        } catch (std::invalid_argument e) {
            std::cerr << "Invalid format!" << std::endl;
            close(sock_fd);
            return 1;
        }
    }

    close(sock_fd);

    return 0;
}