#include "stdafx.h"

NetworkManager::~NetworkManager()
{
	if (mSocket != NULL)
	{
		closesocket(mSocket);
		mSocket = NULL;
	}

	WSACleanup();
}

void NetworkManager::Init(short port)
{
	WSADATA WSAData;
	SOCKADDR_IN sockAddr;

	try {
		if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
			throw std::format("Winsock Init Failed {}", WSAGetLastError());

		mSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (mSocket == INVALID_SOCKET)
			throw std::format("Connect Socket Create Failed {}", WSAGetLastError());

		ZeroMemory(&sockAddr, sizeof(sockAddr));

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(port);
		std::wstring ip = L"127.0.0.1";
		InetPton(AF_INET, ip.c_str(), &sockAddr.sin_addr);

		int ret = connect(mSocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr));
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
		//memcpy(buf + sizeof(size_t) + sizeof(size_t), "æ»≥Á«œººø‰", a);
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
}
