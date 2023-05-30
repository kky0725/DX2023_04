#include "framework.h"
#include "DunMonster.h"

DunMonster::DunMonster()
{
	_quad = make_shared<Quad>(L"Resource/Goomba.png");
	_collider = make_shared<CircleCollider>(_quad->GetImageSize().x);
	_quad->GetTransform()->SetParent(_collider->GetTransform());

	_collider->GetTransform()->SetPosition({ 800,500 });
	_collider->GetTransform()->SetScale({ 0.3f, 0.3f });

}

DunMonster::~DunMonster()
{
}

void DunMonster::Collider_Update()
{
	if (!_isActive)
		return;
	_collider->Update();
}

void DunMonster::Update()
{
	if (!_isActive)
		return;
	_quad->Update();
}

void DunMonster::Render()
{
	if (_hp > 0)
	{
		_quad->Render();
		_collider->Render();
	}
}

void DunMonster::Damaged(int damgae)
{
	if (!_isActive)
		return;
	_hp -= damgae;
	
	if (_hp < 1)
	{
		_hp = 0;
		_isActive = false;
	}
}
