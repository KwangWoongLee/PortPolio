#pragma once

class Game
{
public:
	Game(); 

	//���� �⺻ 3����
	//���� �ʱ�ȭ
	bool Init();
	//���� ���� ����
	void RunLoop();
	//���� ����
	void Shutdown();

	//���� ������ (��)�ε�
	void LoadData();
	void UnloadData();
	
	//Actor�߰� �� �����Լ�
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	std::vector<class Actor*>& GetActors() { return mActors; }

	//SpriteComponent �߰�, ����
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	//�̹��� ���Ͽ��� Texture�� ��ȯ
	SDL_Texture* GetTexture(const std::string& fileName);

	//tilemap ��������
	bool ReadTileMap(const std::string& fileName);
	void AddTile(class Tile* bomb);
	void RemoveTile(class Tile* bomb, const Vector2& pos);
	std::vector<class Tile*>& GetTiles() { return mTiles; }
	std::vector<std::vector<int>>& GetTileMapToInt() { return mTileMapToInt; }
	std::vector<std::vector<Tile::TileType>>& GetTileMapToType() { return mTileMapToType; }

	//���� ��ü�� ����
	void AddPlayer(class Player* player);
	void RemovePlayer(class Player* player);
	std::vector<class Player*>& GetPlayers() { return mPlayers; }

	void AddBomb(class Bomb* bomb);
	void RemoveBomb(class Bomb* bomb);
	std::vector<class Bomb*>& GetBombs() { return mBombs; }

	void AddBoom(class Boom* boom);
	void RemoveBoom(class Boom* boom);
	std::vector<class Boom*>& GetBooms() { return mBooms; }



private:
	//���� ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//������Ʈ �Ϸ�� Actor vector
	std::vector<class Actor*> mActors;
	//������Ʈ �Ϸ���� ���� Actor vector;
	std::vector<class Actor*> mPendingActors;

	// SpriteComponent Vector // Game�� �׸��� ���� Sprite Vector
	std::vector<class SpriteComponent*> mSprites;

	// Texture map  <filename, SDL_Texture*> // Texture ��Ȱ���� ���� vector
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	//Ÿ�ϸ�
	std::vector<Tile*> mTiles;
	std::vector<std::vector<int>> mTileMapToInt;
	std::vector<std::vector<Tile::TileType>> mTileMapToType;
	int GetTileSizeX() const { return mTileSizeX; }
	int GetTileSizeY() const { return mTileSizeY; }
	void SetTileSizeXY(int x, int y);


	int	mMapSizeX;
	int mMapSizeY;
	int mTileSizeX;
	int mTileSizeY;

	//SDL ������
	SDL_Window*	mWindow;
	//SDL ������
	SDL_Renderer* mRenderer;
	//InputSystem
	Input* mInput;

	//���� ���� �Ǵ�
	bool		mIsRunning;
	//���� ƽ���� ������ ����
	Uint32		mTicksCount;
	//Actor ������Ʈ �Ϸ� Ȯ�� �Ǵ�
	bool		mIsUpdatingActors;
	

	std::vector<Player*> mPlayers;
	std::vector<Bomb*> mBombs;
	std::vector<Boom*> mBooms;
	

};

