#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <format>
#include <iostream>
#include <string>
#include <thread>

const int PORT = 8080;
const std::string server_name = "Server of Hakan Vardar";

#define CLOSE_AND_RETURN(fd) \
    close(fd);               \
    return;

void handle_client(int client_fd) {
    int server_number = rand() % 100 + 1;

    char buffer[1024];
    ssize_t bytes_recieved = read(client_fd, buffer, sizeof(buffer));
    if (bytes_recieved <= 0) {
        CLOSE_AND_RETURN(client_fd);
    }

    size_t i = 0;
    buffer[bytes_recieved] = '\0';

    std::string client_message(buffer);
    if (std::isdigit(buffer[i])) {
        std::cerr << "Invalid Format!" << std::endl;
        CLOSE_AND_RETURN(client_fd);
    }

    while (!std::isdigit(buffer[++i]) && i < client_message.length()) {
    }

    if (i >= client_message.length()) {
        std::cerr << "Invalid Format!" << std::endl;
        CLOSE_AND_RETURN(client_fd);
    }

    std::string client_name = client_message.substr(0, i - 1);
    try {
        int client_number = std::stoi(client_message.substr(i));
        if (client_number < 0 || client_number > 100) {
            std::cerr << "Invalid number!" << std::endl;
            CLOSE_AND_RETURN(client_fd);
        }

        std::cout << "Clients name: " << client_name << "\n";
        std::cout << "Servers name: " << server_name << "\n";
        std::cout << "Clients number: " << client_number << "\n";
        std::cout << "Servers number: " << server_number << "\n";
        std::cout << "Sum of the numbers: " << server_number + client_number << std::endl;

        std::string response = server_name + std::to_string(server_number);
        send(client_fd, response.c_str(), response.size(), 0);

    } catch (std::invalid_argument err) {
        std::cerr << "Invalid Format!" << std::endl;
        CLOSE_AND_RETURN(client_fd);
    }

    std::cout << "-------------------" << std::endl;
    close(client_fd);
}

int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        std::cerr << std::format("Failed to create socket: {}", std::strerror(errno)) << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << std::format("Failed to bind socket: {}", std::strerror(errno)) << std::endl;
        return 1;
    }

    listen(sock_fd, 99);
    std::cout << "Listening on port: " << PORT << std::endl;
    std::cout << "-------------------" << std::endl;

    for (;;) {
        int client_fd = accept(sock_fd, NULL, NULL);
        if (client_fd == -1) {
            std::cerr << std::format("Failed to accept client connection: {}", std::strerror(errno))
                      << std::endl;
            continue;
        }

        std::thread client_thread(handle_client, client_fd);
        client_thread.detach();
    }

    close(sock_fd);
}
