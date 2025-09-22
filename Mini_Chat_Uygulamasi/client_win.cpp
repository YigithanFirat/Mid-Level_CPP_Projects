// client_win.cpp
// Compile (MinGW): g++ -std=c++11 client_win.cpp -lws2_32 -o client_win
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
            std::cout << "\n[!] Sunucu bağlantıyı kapattı veya hata.\n";
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
    if (argc != 3) {
        std::cerr << "Kullanım: " << argv[0] << " <server_ip> <port>\n";
        return 1;
    }
    const char* server_ip = argv[1];
    int port = atoi(argv[2]);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) { std::cerr << "WSAStartup failed\n"; return 1; }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) { std::cerr << "socket failed\n"; WSACleanup(); return 1; }

    sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &serv.sin_addr);

    if (connect(sock, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) { std::cerr << "connect failed\n"; closesocket(sock); WSACleanup(); return 1; }

    std::cout << "Sunucuya bağlandı: " << server_ip << ":" << port << "\n";

    std::atomic<bool> running(true);
    std::thread r(recv_thread, sock, std::ref(running));
    std::thread s(send_thread, sock, std::ref(running));

    std::cout << "Mesaj yaz, gönder. Çıkmak için '/quit' yaz.\n> " << std::flush;

    s.join();
    running = false;
    shutdown(sock, SD_BOTH);
    r.join();

    closesocket(sock);
    WSACleanup();
    std::cout << "Client kapandı.\n";
    return 0;
}