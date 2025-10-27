#pragma once
#include <cstdint>

// ======================
// ���� �������� ����
// ======================

// �޽��� Ÿ��(1Byte)
enum class MsgType : uint8_t
{
    IMG_REQ = 1, // �̹��� ���� ��û
    IMG_RES = 2, // �̹��� ���� ����
    RESULT_REQ = 3, // ��� ��û
    RESULT_RES = 4, // ��� ����
    HEARTBEAT = 9  // ���� ������
};

// ������
constexpr int MAX_JSON_SIZE = 4096;   // ��Ÿ ���� �ִ� ũ��
constexpr int MAX_IMAGE_SIZE = 1 * 1024 * 1024; // �ִ� 1MB �̹���
constexpr int HEADER_SIZE = 9;             // 1 + 4 + 4 = 9����Ʈ

#pragma pack(push, 1)
struct PacketHeader
{
    uint8_t msgType; // 1Byte
    uint32_t bodyLen; // 4Byte
    uint32_t imgId; // 4Byte
};
#pragma pack(pop)