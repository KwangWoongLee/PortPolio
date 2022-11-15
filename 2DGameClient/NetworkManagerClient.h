#pragma once
#include "stdafx.h"

constexpr size_t BUFF_SIZE = 1024;

enum class eClientState
{
	UNREADY,
	READY,
	WELCOMED,
	TERMINATE

};

class NetworkManagerClient : public Singleton<NetworkManagerClient>
{
public:

	NetworkManagerClient(short port);
	~NetworkManagerClient();


	bool Init();
	void Run();

	void registPacketFunction();

	void RecvPacketProcess();
	void SendPacketProcess();

	void RecvPacket();
	void SendPacket(std::shared_ptr<Packet> packet);


	void	PutRecvPacket(std::shared_ptr<Packet>&& packet);
	std::shared_ptr<Packet> GetRecvPacket();


	//Getter and Setter
	SOCKET& GetSocket() { return mSocket; }

	eClientState GetState() { return mState; }


	std::shared_ptr<Packet> SendHelloPacket();
	std::shared_ptr<Packet> SendInputPacket();
	void SendHeartBeat();
	void SendReqExitPacket();


	void AddGameObjectToNetwork(std::shared_ptr<GameObject> obj) override;
	void RemoveGameObjectToNetwork(size_t networkID);

	bool mShutDown = true;

private:
	bool connect();


	SOCKET mSocket;
	eClientState mState;


	std::atomic<bool>	mConnected;



	SOCKADDR_IN		mServerInfo;

	std::string		mServerIP;
	uint16_t		mServerPort;

	char			mBuffer[BUFF_SIZE];



	float			mLastHelloTime;
	float			mLastPacketSendTime;
	float			mLastHeartBeatTime;


	ThreadSafeQueue<std::shared_ptr<Packet>>	mRecvPacketQueue;

	std::unique_ptr<std::thread>	mRecvThread;

	std::unordered_map<ePacketType, std::function<void(std::shared_ptr<Packet>&)>> mPacketToFunctionMap;

};