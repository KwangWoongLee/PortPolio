#include "stdafx.h"

Game::Game()
    :mIsRunning(true)
    ,mWindow(nullptr)
	, mRenderer(nullptr)
	, mIsUpdatingActors(false)
	, mMapSizeX(15)
	, mMapSizeY(12)
	, mTileSizeX(0)
	, mTileSizeY(0)
{
}

bool Game::Init()
{
	// SDL �ʱ�ȭ
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// SDL ������ ����
	mWindow = SDL_CreateWindow("2d Game Client", 30, 30, 512, 384, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//SDL ������ ���� , ��������ȭ �� �׷��� �ϵ���� ���
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	//PNG�� �����ϴ� SDL Image �ʱ�ȭ
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	mInput = new Input();
	if (!mInput->Init())
	{
		SDL_Log("Failed to initialize input system");
		return false;
	}

	NetworkManager::GetInstance().Init(8080);

	LoadData();

	mTicksCount = SDL_GetTicks();

    return true;
}


void Game::LoadData()
{
	//TODO : �پ��� ���͸� ����� �� ���Ϳ� ������Ʈ�� �߰��غ���.
	Actor* bgActor = new Actor(this);
	bgActor->SetPosition(Vector2(256.f, 192.f));
	bgActor->SetScale(1.0f);

	SpriteComponent* bgComponent = new SpriteComponent(bgActor, 10);
	bgComponent->SetTexture(GetTexture("Assets/bg.bmp"));


	ReadTileMap("Assets/MapLayer2.csv");
	SetTileSizeXY(32, 32);

	for (int row = 0; row < mMapSizeY; ++row)
	{
		for (int col = 0; col < mMapSizeX; ++col)
		{
			if (mTileMapToInt[row][col] != -1)
			{
				Tile* tile = new Tile(this, mTileMapToInt[row][col]);
				switch (mTileMapToInt[row][col])
				{
				case -1:
					mTileMapToType[row][col] = Tile::TileType::ENone;
					tile->SetTileType(Tile::TileType::ENone);
					break;
				case 3:
				case 8:
				case 9:
				case 10:
					mTileMapToType[row][col] = Tile::TileType::EBlock;
					tile->SetTileType(Tile::TileType::EBlock);
					break;
				default:
					mTileMapToType[row][col] = Tile::TileType::EBush;
					tile->SetTileType(Tile::TileType::EBush);
					break;
				}
				
				tile->SetPosition(Vector2(col * GetTileSizeX() + GetTileSizeX() * 0.5f, row * GetTileSizeY() + GetTileSizeY() * 0.5f));
			}
		}
	}

	Player* p = new Player(this);
	p->SetPosition(Vector2(10.0f, 10.0f));

}

void Game::UnloadData()
{
	// actor �Ҹ��ڿ��� actor ��ü�� ��� ������ ������ ��츦 ���
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}



void Game::ProcessInput()
{
	mInput->PrepareForUpdate();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	KeyboardState keyState = mInput->GetKeyboardState();

	if (keyState.GetKeyState(SDL_SCANCODE_ESCAPE)
		== EReleased)
	{
		mIsRunning = false;
	}


	mIsUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mIsUpdatingActors = false;
}

void Game::UpdateGame()
{
	//������ ���� - ������ ������ ���� ��Ÿ�ð��� ��ǥ���� ������� �ش� �ð�(16ms)���� ���
	//����ȯ���� �޶� FPS�� �޶����� ������ �޶��� �� �ֱ� ������ ���
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	mTicksCount = SDL_GetTicks();

	//��Ÿ �ִ� �ð����� ����
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	//��Ÿ �ð��� ������ �̹� ������ ���Ӽ��� ������Ʈ
	mIsUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mIsUpdatingActors = false;

	//�̹� �����ӿ� ������Ʈ ���� ���� Actor���� mActors�� �߰�
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();


	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::State::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}


	for (auto actor : deadActors)
	{
		delete actor;
	}

	
}

void Game::GenerateOutput()
{
	//�ĸ����(����) �ʱ�ȭ
	//�׸��� ���� ����
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
	//���� �׸��� �������� �ʱ�ȭ
	SDL_RenderClear(mRenderer);

	//TODO:�׸���
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	
	//�������(���÷���)�� �ĸ���� ��ü
	SDL_RenderPresent(mRenderer);
}

void Game::SetTileSizeXY(int x, int y)
{
	mTileSizeX = x;
	mTileSizeY = y;
}


void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	//Actor ������Ʈ ���̶��, �ӽ�Actor Vector�� Pending
	if (mIsUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	//actor ��ü�� delete�� ��, ~Actor()�� ���� ȣ��
	//actor pending vector�� ����actor vector ��ο� �ִٸ� �����ش�.  

	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//TODO : swap �� pop_back�� ������ ? -> stl�����ϸ鼭 ã�ƺ���
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}


	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	//�׸��� ������ ���� sprite vector�� �ڵ�����
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;

	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		//texture ��Ȱ��
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		//�÷�Ű ����
		SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 0, 255));


		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}


bool Game::ReadTileMap(const std::string& fileName)
{
	using namespace std;

	ifstream file(fileName);
	if (true == file.fail())
	{
		//���� �б���� TODO:�α� ����ó��
		return false;
	}

	string cell;
	string line;
	while (getline(file, line))
	{
		vector<int> rowInt;
		vector<Tile::TileType> rowType;
		stringstream lineStream(line);
		while (getline(lineStream, cell, ','))
		{
			int cellInt = stoi(cell);
			Tile::TileType type;
			rowInt.emplace_back(cellInt);
			switch (cellInt)
			{
			case -1:
				type = Tile::TileType::ENone;
				break;
			case 3:
			case 8:
			case 9:
			case 10:
				type = Tile::TileType::EBlock;
				break;
			default:
				type = Tile::TileType::EBush;
				break;
			}
			rowType.emplace_back(type);
		}

		mTileMapToInt.emplace_back(rowInt);
		mTileMapToType.emplace_back(rowType);
	}

	return true;
}

void Game::AddTile(Tile* tile)
{
	mTiles.emplace_back(tile);
}

void Game::RemoveTile(Tile* tile, const Vector2& pos)
{
	mTileMapToInt[pos.y / 32][pos.x / 32] = ENone;
	mTileMapToType[pos.y / 32][pos.x / 32] = Tile::TileType::ENone;
	auto iter = std::find(mTiles.begin(),
		mTiles.end(), tile);
	if (iter != mTiles.end())
	{
		mTiles.erase(iter);
	}
}

void Game::AddPlayer(Player* player)
{
	mPlayers.emplace_back(player);
}

void Game::RemovePlayer(Player* player)
{
	auto iter = std::find(mPlayers.begin(),
		mPlayers.end(), player);
	if (iter != mPlayers.end())
	{
		mPlayers.erase(iter);
	}
}

void Game::AddBomb(Bomb* bomb)
{
	mBombs.emplace_back(bomb);
}

void Game::RemoveBomb(Bomb* bomb)
{
	auto iter = std::find(mBombs.begin(),
		mBombs.end(), bomb);
	if (iter != mBombs.end())
	{
		mBombs.erase(iter);
	}
}

void Game::AddBoom(Boom* boom)
{
	mBooms.emplace_back(boom);
}

void Game::RemoveBoom(Boom* boom)
{
	auto iter = std::find(mBooms.begin(),
		mBooms.end(), boom);
	if (iter != mBooms.end())
	{
		mBooms.erase(iter);
	}
}




