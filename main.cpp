#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <chrono>

std::mutex consoleLock;

void performPortScan(const std::string& ipAddress, int portNumber) {
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        std::lock_guard<std::mutex> lock(consoleLock);
        std::cerr << "Failed to create socket for port " << portNumber << "\n";
        return;
    }

    struct sockaddr_in connectionDetails;
    connectionDetails.sin_family = AF_INET;
    connectionDetails.sin_port = htons(portNumber);
    inet_pton(AF_INET, ipAddress.c_str(), &connectionDetails.sin_addr);

    auto startTime = std::chrono::steady_clock::now();

    if (connect(socketFD, (struct sockaddr*)&connectionDetails, sizeof(connectionDetails)) == 0) {
        auto endTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = endTime - startTime;

        struct servent* serviceDetails = getservbyport(htons(portNumber), "tcp");
        const char* serviceName = (serviceDetails && serviceDetails->s_name) ? serviceDetails->s_name : "N/A";

        std::lock_guard<std::mutex> lock(consoleLock);
        std::cout << "Port " << portNumber << " is accessible on " << ipAddress
                  << " (Service: " << serviceName
                  << ", Time: " << elapsedTime.count() << " seconds)\n";
    }

    close(socketFD);
}

int main() {
    std::string hostIP;
    int portStart, portEnd, maxThreads;

    std::cout << "Input target IP address: ";
    std::cin >> hostIP;
    std::cout << "Input starting port number: ";
    std::cin >> portStart;
    std::cout << "Input ending port number: ";
    std::cin >> portEnd;
    std::cout << "Set the maximum number of threads: ";
    std::cin >> maxThreads;

    std::vector<std::thread> workerThreads;
    int ongoingThreads = 0;

    for (int port = portStart; port <= portEnd; ++port) {
        while (ongoingThreads >= maxThreads) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        workerThreads.emplace_back([&]() {
            performPortScan(hostIP, port);
            --ongoingThreads;
        });

        ++ongoingThreads;
    }

    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "Port scanning completed successfully.\n";
    return 0;
}
