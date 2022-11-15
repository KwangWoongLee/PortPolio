#pragma once
#include "stdafx.h"

class NetworkManager : public Singleton<NetworkManager>
{
public:
	NetworkManager() {};
	virtual ~NetworkManager();

	void Init(short port);

private:
	SOCKET mSocket;

};

