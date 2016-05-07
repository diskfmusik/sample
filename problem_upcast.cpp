#include <stdio.h>
#include <memory>

class GameObject;
class Player;
typedef std::shared_ptr < GameObject > GameObjectSP;
typedef std::shared_ptr < Player > PlayerSP;


class GameObject
{
public:
	GameObject() { printf("GameObject : コンストラクタ\n"); }
	/*virtual*/ ~GameObject() { printf("GameObject : デストラクタ\n"); }
};


class Player : public GameObject
{
	int* money;

public:
	Player()
	{
		money = new int[1000000];
		printf("Player : コンストラクタ\n");
	}

	/*virtual*/ ~Player()
	{
		delete[] money;
		printf("Player : デストラクタ\n");
	}

	static PlayerSP Create()
	{
		PlayerSP obj = PlayerSP(new Player());
		return obj;
	}
};


void main()
{

	//GameObjectSP sp = GameObjectSP(new Player());

	{
		GameObjectSP sp = GameObjectSP(Player::Create());

		//GameObject* obj = new Player();

		//delete obj;
	}



	getchar();
}