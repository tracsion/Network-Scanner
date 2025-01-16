#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <chrono>

std::mutex outputMutex;

void scanPort(const std::string& ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cerr << "Socket creation failed for port " << port << "\n";
        return;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &address.sin_addr);

    auto startTime = std::chrono::high_resolution_clock::now();

    if (connect(sock, (struct sockaddr*)&address, sizeof(address)) == 0) {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;

        struct servent* service = getservbyport(htons(port), "tcp");
        const char* serviceName = (service && service->s_name) ? service->s_name : "unknown";

        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "Port " << port << " is open on " << ip
                  << " (Service: " << serviceName
                  << ", Response Time: " << elapsed.count() << "s)\n";
    }

    close(sock);
}

int main() {
    std::string targetIP;
    int startPort, endPort, threadLimit;

    std::cout << "Enter the target IP address: ";
    std::cin >> targetIP;
    std::cout << "Enter the start port: ";
    std::cin >> startPort;
    std::cout << "Enter the end port: ";
    std::cin >> endPort;
    std::cout << "Enter the maximum number of threads: ";
    std::cin >> threadLimit;

    std::vector<std::thread> threads;
    int activeThreads = 0;

    for (int port = startPort; port <= endPort; ++port) {
        while (activeThreads >= threadLimit) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        threads.emplace_back([&]() {
            scanPort(targetIP, port);
            --activeThreads;
        });

        ++activeThreads;
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "Scanning complete!\n";
    return 0;
}
