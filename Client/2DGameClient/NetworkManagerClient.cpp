#include "stdafx.h"

std::function<void(NetworkManagerClient*)> RecvThreadFunction = [](NetworkManagerClient* network) {

	while (network->mShutDown)
	{
		network->RecvPacket();
	}
};



NetworkManagerClient::NetworkManagerClient(short port)
	: mSocket(INVALID_SOCKET)
	, mServerIP("")
	, mServerPort(port)
	, mState(eClientState::UNREADY)
	, mLastHelloTime(0.f)
{
	ZeroMemory(mBuffer, sizeof(mBuffer));
	ZeroMemory(&mServerInfo, sizeof(SOCKADDR_IN));


}

NetworkManagerClient::~NetworkManagerClient()
{
	closesocket(mSocket);

	WSACleanup();
}


bool NetworkManagerClient::Init()
{
	WSADATA WSAData;
	SOCKADDR_IN sockAddr;

	try {
		if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
			throw std::format("Winsock Init Failed {}", WSAGetLastError());

		mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (mSocket == INVALID_SOCKET)
			throw std::format("Connect Socket Create Failed {}", WSAGetLastError());

		ZeroMemory(&sockAddr, sizeof(sockAddr));

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(mServerPort);
		std::wstring ip = L"127.0.0.1";
		InetPton(AF_INET, ip.c_str(), &sockAddr.sin_addr);

		int ret = connect();
		if (ret == SOCKET_ERROR)
		{
			std::string except = std::format("Server Socket Connect Failed {}", WSAGetLastError());
			throw std::exception(except.c_str());
		}

		//char* buf = new char[1024];
		//size_t a = sizeof(size_t);
		//size_t type = 2;
		//memcpy(buf, &a, sizeof(size_t));
		//memcpy(buf + sizeof(size_t), &type, sizeof(size_t));
		//memcpy(buf + sizeof(size_t) + sizeof(size_t), "안녕하세요", a);
		//
		//ret = send(mSocket, buf, a+ sizeof(size_t), 0);

		//if (ret == SOCKET_ERROR)
		//{
		//	std::string except = std::format("Server Socket Connect Failed {}", WSAGetLastError());
		//	throw std::exception(except.c_str());
		//}

		//ZeroMemory(&buf, 1024);
		//ret = recv(mSocket, buf, 100, 0);

		//if (ret == SOCKET_ERROR)
		//{
		//	std::string except = std::format("Server Socket Connect Failed {}", WSAGetLastError());
		//	throw std::exception(except.c_str());
		//}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		if (mSocket != NULL)
		{
			closesocket(mSocket);
			mSocket = NULL;
		}
	}


	mShutDown = false;
	mState = eClientState::READY;

	return true;
}
void NetworkManagerClient::Run()
{
	mRecvThread = std::make_unique<std::thread>(RecvThreadFunction, this);

}

void NetworkManagerClient::registPacketFunction()
{
	mPacketToFunctionMap.insert(std::make_pair(ePacketType::SC_RES_WELCOME, [&](const auto& packet) {
		using PK_SC_RES_WELCOME;
		auto typePacket = std::static_pointer_cast<PK_SC_RES_WELCOME>(packet);

		auto map = typePacket->GetState();

		for (auto element : map)
		{
			auto netid = element.first;
			auto obj = element.second;

			std::shared_ptr<GameObject> newObject = ObjectFactory::GetInstance().CreateObject(obj.GetType());

			if (newObject == nullptr)
				continue;

			newObject->SetNetworkID(netid);
			newObject->SetPosition(obj.GetPosition());
			newObject->SetScale(obj.GetScale());


			AddGameObjectToNetwork(newObject);

			World::GetInstance().AddGameObject(newObject);
		}

		mState = eClientState::WELCOMED;
		}));

	mPacketToFunctionMap.insert(std::make_pair(ePacketType::SC_RES_EXIT, [&](const auto& packet) {

		mShutDown = true;
		mState = eClientState::TERMINATE;

		SDL_Quit();

		}));
}

void NetworkManagerClient::RecvPacketProcess()
{
	while (!mRecvPacketQueue.Empty())
	{
		auto packet = GetRecvPacket();

		if (packet == nullptr)
			return;

		auto iter = mPacketToFunctionMap.find(packet->GetType());

		if (iter != mPacketToFunctionMap.end())
			(iter->second)(packet);

	}
}

void NetworkManagerClient::SendPacketProcess()
{
	std::shared_ptr<Packet> packet = nullptr;

	auto time = Clock::GetInstance().GetSystemTimeFloat();

	if (mState == eClientState::READY)
	{

		if (time > mLastHelloTime + 0.4f)
		{
			packet = SendHelloPacket();
			mLastHelloTime = time;
			printf("hello");
		}
	}

	else if (mState == eClientState::WELCOMED)
	{
		if (time > mLastPacketSendTime + 0.03f)
		{
			packet = SendInputPacket();
			mLastPacketSendTime = time;

		}

		//HeartBeat ÆÐÅ¶ Àü¼Û
		if (time > mLastHeartBeatTime + 3.f)
		{
			SendHeartBeat();
			mLastHeartBeatTime = time;
		}

	}


	if (packet != nullptr)
		SendPacket(std::move(packet));

}







// Helper ---------------------------------------------------------------//


std::shared_ptr<Packet> NetworkManagerClient::SendHelloPacket()
{
	using PK_CS_REQ_HELLO;
	std::shared_ptr<PK_CS_REQ_HELLO> packet = std::make_shared<PK_CS_REQ_HELLO>();

	packet->SetID(GetRandomString());

	return packet;
}


std::shared_ptr<Packet> NetworkManagerClient::SendInputPacket()
{
	using PK_CS_SEND_INPUTLIST;
	std::deque<Input>& inputList = ::InputManager::GetInstance().GetInputList();

	if (inputList.empty())
		return nullptr;

	std::shared_ptr<PK_CS_SEND_INPUTLIST> packet = std::make_shared<PK_CS_SEND_INPUTLIST>();

	int loopSize = std::min((int)inputList.size(), 3);

	int startIndex = loopSize < 3 ? 0 : (int)inputList.size() - 3;

	for (int i = startIndex; i < inputList.size(); ++i)
	{
		packet->PushInputType(inputList[i]);
	}

	inputList.clear();

	return packet;
}



void NetworkManagerClient::SendHeartBeat()
{
	std::shared_ptr<PK_CS_NOTIFY_HEARTBEAT> packet = std::make_shared<PK_CS_NOTIFY_HEARTBEAT>();
	SendPacket(std::move(packet));

	printf("heartbeat\n");
}

void NetworkManagerClient::SendReqExitPacket()
{
	using PK_CS_REQ_EXIT;
	std::shared_ptr<PK_CS_REQ_EXIT> packet = std::make_shared<PK_CS_REQ_EXIT>();

	SendPacket(std::move(packet));
}


bool NetworkManagerClient::connect()
{
	if (::connect(mSocket, (SOCKADDR*)&mServerInfo, sizeof(mServerInfo)) == SOCKET_ERROR)
	{
		printf("Connect Error : %d", GetLastError());
		return false;
	}

	mConnected = true;


	return true;
}

void NetworkManagerClient::RecvPacket()
{
	char buf[10240];

	recv(mSocket, buf, 10240, 0);
	size_t offset = 0;
	size_t size[1] = { 0 };
	memcpy(size, buf, sizeof(PACKET_SIZE));

	offset += sizeof(PACKET_SIZE);

	float packetRecvTime[1] = { 0 };
	memcpy(packetRecvTime, buf + offset, sizeof(float));

	offset += sizeof(float);

	std::shared_ptr<Packet> packet = PacketAnalyzer::GetInstance().analyze(buf + offset, size[0] - offset);

	if (packet == nullptr) {
		if (mState == eClientState::TERMINATE)
			return;

		if (WSAECONNRESET == GetLastError())
		{
			mState = eClientState::TERMINATE;
			//bClientShutDown = true;
			return;
		}


		printf("Packet is not analyzed by Analyzer, Error : %d \n", GetLastError());
		ASSERT(false);
		return;
	}

	PutRecvPacket(std::move(packet));
}

void NetworkManagerClient::SendPacket(std::shared_ptr<Packet> packet)
{
	OutputStream stream;
	packet->Encode(stream);

	size_t offset = 0;

	PACKET_SIZE packetLength[1] = { sizeof(PACKET_SIZE) + sizeof(float) + stream.GetLength() };

	memcpy(mBuffer, packetLength, sizeof(PACKET_SIZE));

	offset += sizeof(PACKET_SIZE);

	float packetSendTime[1] = { Clock::GetInstance().GetSystemTimeFloat() };

	memcpy(mBuffer + offset, packetSendTime, sizeof(float));

	offset += sizeof(float);


	memcpy(mBuffer + offset, stream.GetBuffer(), stream.GetLength());

	offset += stream.GetLength();

	send(mSocket, mBuffer, offset, 0);
}



void NetworkManagerClient::PutRecvPacket(std::shared_ptr<Packet>&& packet)
{
	mRecvPacketQueue.Push(packet);
}

std::shared_ptr<Packet> NetworkManagerClient::GetRecvPacket()
{
	return mRecvPacketQueue.Pop();
}



void NetworkManagerClient::AddGameObjectToNetwork(std::shared_ptr<GameObject> obj)
{
	mNetworkIDToGameObject[obj->GetNetworkID()] = obj;
	printf("type : %d \n", obj->GetType());

}