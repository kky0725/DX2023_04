#include "framework.h"
#include "Cup_Boss.h"

using namespace tinyxml2;

Cup_Boss::Cup_Boss()
{
	CreateAction(L"Resource/CupHead/Clown_Page_Two_Intro.png", "Resource/CupHead/Clown_Page_Two_Intro.xml", "BOSS_INTRO", Vector2(280, 360), false);
	CreateAction(L"Resource/CupHead/Clown_Page_Two_Idle.png", "Resource/CupHead/Clown_Page_Two_Idle.xml", "BOSS_IDLE", Vector2(280, 360));
	CreateAction(L"Resource/CupHead/Clown_Page_Two_Die.png", "Resource/CupHead/Clown_Page_Two_Die.xml", "BOSS_DIE", Vector2(280, 360), false, 0.3f);
	_transform = make_shared<Transform>();
	_collider = make_shared<CircleCollider>(70.0f);
	_transform->SetParent(_collider->GetTransform());

	_transform->SetPosition(Vector2(-15, -100));

	// Action Envent ����
	{
		_actions[0]->SetEndEvent(std::bind(&Cup_Boss::IdleEvent, this));
		_actions[0]->SetAlmostEnd(std::bind(&Cup_Boss::IdleEvent, this));
		_actions[2]->SetEndEvent(std::bind(&Cup_Boss::EndEvent, this));
	}
	_collider->SetPosition(CENTER + Vector2(400,0));

	_intBuffer = make_shared<IntBuffer>();
	_intBuffer->_data.value1 = 1;
	_intBuffer->_data.value2 = 200;

	_sprites[2]->SetPS(ADD_PS(L"Shader/MosaicActionPS.hlsl"));
}

Cup_Boss::~Cup_Boss()
{
}

void Cup_Boss::Update()
{
	if (!_isActive)
		return;
	if (_state == State::DIE && _intBuffer->_data.value2 > 1)
		_intBuffer->_data.value2 -= 5;
	_collider->Update();
	_intBuffer->Update();
	_actions[_state]->Update();
	_sprites[_state]->Update();
	_transform->Update();
}

void Cup_Boss::Render()
{
	if (!_isActive)
		return;
	_transform->SetBuffer(0);
	_intBuffer->SetPsBuffer(1);
	_sprites[_state]->SetCurFrmae(_actions[_state]->GetCurClip());
	_sprites[_state]->Render();
	_collider->Render();

}

void Cup_Boss::CreateAction(wstring srvPath, string xmlPath, string actionName, Vector2 size, bool isLoop, float time)
{
	shared_ptr<SRV> srv = ADD_SRV(srvPath);

	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
	document->LoadFile(xmlPath.c_str());

	XMLElement* texturAtlas = document->FirstChildElement();
	XMLElement* row = texturAtlas->FirstChildElement();

	vector<Action::Clip> clips;

	while (true)
	{
		if (row == nullptr)
			break;

		int x = row->FindAttribute("x")->IntValue();
		int y = row->FindAttribute("y")->IntValue();
		int w = row->FindAttribute("w")->IntValue();
		int h = row->FindAttribute("h")->IntValue();

		Action::Clip clip = Action::Clip(x, y, w, h, srv);
		clips.push_back(clip);

		row = row->NextSiblingElement();
	}

	shared_ptr<Action> action;
	if (isLoop)
	{
		action = make_shared<Action>(clips, actionName);
	}
	else
	{
		action = make_shared<Action>(clips, actionName, Action::Type::END);
	}
	action->Play();
	shared_ptr<Sprite> sprite = make_shared<Sprite>(srvPath, size);

	action->Update();
	sprite->Update();

	sprite->SetCurFrmae(action->GetCurClip());

	_actions.push_back(action);
	_sprites.push_back(sprite);
}

void Cup_Boss::Damaged(int damgae)
{
	if (!_isActive)
		return;
	_hp -= damgae;

	if (_hp < 1)
	{
		_hp = 0;
		DieEvent();
	}
}
