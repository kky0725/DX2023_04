#include "framework.h"
#include "ColliderScene.h"

ColliderScene::ColliderScene()
{
	_rectCollider = make_shared<RectCollider>(Vector2(100.0f, 100.0f));
	_circleCollider = make_shared<CircleCollider>(100.0f);

	_rectCollider->SetPosition(CENTER);
	_circleCollider->SetPosition(CENTER + Vector2(200, 0));
}

ColliderScene::~ColliderScene()
{
}

void ColliderScene::Update()
{
	_rectCollider->Update();
	_circleCollider->Update();
}

void ColliderScene::Render()
{
	_rectCollider->Render();
	_circleCollider->Render();
}
