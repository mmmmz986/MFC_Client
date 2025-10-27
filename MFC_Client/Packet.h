#pragma once // ��� �ߺ�����

#include "CommonProtocol.h"
#include <string>
#include <vector>
#include <atomic>

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else // ��������
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1
typedef int SOCKET;
#endif

// ======================
// ��Ŷ Ŭ����
// ======================
class Packet
{
public:
    Packet();
    ~Packet();

    // ����
    bool Connect(const std::string& ip, uint16_t port);
    void Disconnect();

    // ��/����
    bool Send(MsgType type, const std::vector<uint8_t>& body);
    bool Receive(MsgType& outType, uint32_t& outImgId, std::vector<uint8_t>& outBody);

private:
    SOCKET sock; // ����
    std::atomic<uint32_t> nextImgId; // �̹��� ID(�ڵ� ����)

    // ��/���� ����
    bool sendAll(const void* data, size_t size);
    bool recvAll(void* data, size_t size);
};