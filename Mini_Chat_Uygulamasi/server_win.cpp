// server_win.cpp
// Compile (MinGW): g++ -std=c++17 server_win.cpp -o server_win -lws2_32 -pthread
#include <winsock2.h>
#include <ws2tcpip.h>

#include <atomic>
#include <iostream>
#include <string>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

const int BUFFER_SIZE = 4096;

void recv_thread(SOCKET sock, std::atomic<bool>& running) {
    char buf[BUFFER_SIZE];
    while (running) {
        int r = recv(sock, buf, BUFFER_SIZE - 1, 0);
        if (r <= 0) {
            std::cout << "\n[!] Karşı taraf bağlantıyı kapattı veya hata.\n";
            running = false;
            break;
        }
        buf[r] = '\0';
        std::cout << "\r[Peer] " << buf << "\n> " << std::flush;
    }
}

void send_thread(SOCKET sock, std::atomic<bool>& running) {
    std::string line;
    while (running && std::getline(std::cin, line)) {
        if (line == "/quit") {
            running = false;
            break;
        }
        int s = send(sock, line.c_str(), (int)line.size(), 0);
        if (s == SOCKET_ERROR) {
            std::cout << "[!] Gönderme hatası.\n";
            running = false;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Kullanım: " << argv[0] << " <port>\n";
        return 1;
    }
    int port = atoi(argv[1]);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) { std::cerr << "WSAStartup failed\n"; return 1; }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock == INVALID_SOCKET) { std::cerr << "socket failed\n"; WSACleanup(); return 1; }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);

    if (bind(listen_sock, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) { std::cerr << "bind failed\n"; closesocket(listen_sock); WSACleanup(); return 1; }
    if (listen(listen_sock, 1) == SOCKET_ERROR) { std::cerr << "listen failed\n"; closesocket(listen_sock); WSACleanup(); return 1; }

    std::cout << "Sunucu dinlemede. Port: " << port << "\nBağlantı bekleniyor...\n";

    SOCKET client_sock = accept(listen_sock, NULL, NULL);
    if (client_sock == INVALID_SOCKET) { std::cerr << "accept failed\n"; closesocket(listen_sock); WSACleanup(); return 1; }

    std::atomic<bool> running(true);

    // SOCKET değer olarak, running referans olarak gönderiliyor
    std::thread recvThread(recv_thread, client_sock, std::ref(running));
    std::thread sendThread(send_thread, client_sock, std::ref(running));

    std::cout << "Mesaj yaz, gönder. Çıkmak için '/quit' yaz.\n> " << std::flush;

    sendThread.join();
    running = false;
    shutdown(client_sock, SD_BOTH);
    recvThread.join();

    closesocket(client_sock);
    closesocket(listen_sock);
    WSACleanup();
    std::cout << "Sunucu kapatıldı.\n";
    return 0;
}