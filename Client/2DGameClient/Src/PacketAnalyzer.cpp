#include "stdafx.h"

std::shared_ptr<Packet> PacketAnalyzer::analyze(const char* buffer, size_t size)
{
	//��Ŷ Ÿ���� �������� �׸�ŭ offset ����.
	size_t offset = 0;

	ePacketType type[1] = { ePacketType::NONE };
	memcpy((void*)type, buffer, sizeof(PACKET_TYPE_SIZE));
	offset += sizeof(PACKET_TYPE_SIZE);

	if (type[0] == ePacketType::NONE)
		return nullptr;

	std::shared_ptr<Packet> packet = PacketFactory::GetInstance().GetPacket(type[0]);


	if (packet)
	{
		if (offset < size)
		{
			InputStream stream((char*)(buffer + offset), size - offset);
			packet->Decode(stream);
		}
	}

	return packet;
}