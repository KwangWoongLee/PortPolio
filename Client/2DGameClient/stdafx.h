// 미리 컴파일된 헤더입니다.
#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "dbghelp.lib")

#define NOMINMAX

// 윈도우 관련
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#include <Mmsystem.h>
#include <Ws2spi.h>
#include <Mstcpip.h>
#include <windows.h>
#include <cassert>

// std
#include <iostream>
#include <stdexcept>
#include <format>
#include <memory>
#include <thread>
#include <string>
#include <algorithm>
#include <functional>

#include <fstream>
#include <iostream>
#include <sstream>

//자료구조
#include <iostream>
#include <vector>
#include <unordered_map>
#include <atomic>

//SDL
#include <SDL.h>
#include <SDL_scancode.h>
#include <SDL_image.h>


#include "Math.h"
#include "Vector2.h"
#include "Input.h"
#include "Singleton.h"

#include "Stream.h"
#include "PacketType.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "PacketAnalyzer.h"

#include "NetworkManager.h"
#include "NetworkManagerClient.h"

#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "TileMapComponent.h"

#include "MoveComponent.h"
#include "InputComponent.h"
#include "CollisionComponent.h"

#include "Boom.h"
#include "Bomb.h"
#include "Player.h"
#include "Block.h"

#include "Tile.h"

#include "Game.h"