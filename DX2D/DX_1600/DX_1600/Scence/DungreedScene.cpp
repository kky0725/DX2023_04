#include "framework.h"
#include "DungreedScene.h"

#include "../Object/Dungreed/DunPlayer.h"
#include "../Object/Dungreed/DunBullet.h"

DungreedScene::DungreedScene()
{
	_player = make_shared<DunPlayer>();
}

DungreedScene::~DungreedScene()
{
}

void DungreedScene::Update()
{
	if (KEY_PRESS('A'))
	{
		Vector2 movePos = Vector2(-500.0f, 0.0f) * DELTA_TIME;
		_player->Move(movePos);

	}

	if (KEY_PRESS('D'))
	{
		Vector2 movePos = Vector2(500.0f, 0.0f) * DELTA_TIME;
		_player->Move(movePos);
	}

	_player->Update();
}

void DungreedScene::Render()
{
	_player->Render();
}
