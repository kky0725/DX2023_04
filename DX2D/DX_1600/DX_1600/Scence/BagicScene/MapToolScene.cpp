#include "framework.h"
#include "MapToolScene.h"

#include "../../Object/CupHead/Cup_Block.h"

MapToolScene::MapToolScene()
{
	for (int i = 0; i < 50; i++)
	{
		shared_ptr<Cup_Block> block = make_shared<Cup_Block>(Vector2(-3000, -3000));
		block->_isActive = false;
		_blocks.push_back(block);
	}

	_save = make_shared<Button>(L"Resource/UI/Button.png", Vector2(100, 40));
	_save->SetEvent(std::bind(&MapToolScene::Save, this));
	_save->SetPosition(Vector2(-100,0));
	_load = make_shared<Button>(L"Resource/UI/Button.png", Vector2(100, 40));
	_load->SetEvent(std::bind(&MapToolScene::Load, this));
	_load->SetPosition(Vector2(100, 0));

}

MapToolScene::~MapToolScene()
{
}

void MapToolScene::Update()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		for (auto block : _blocks)
		{
			if(block->_isActive == false)
			{
				block->SetPosition(W_MOUSE_POS);
				block->_isActive = true;
				break;
			}
		}
	}

	for (auto block : _blocks)
		block->Update();

	_save->Update();
	_load->Update();
}

void MapToolScene::Render()
{
	for (auto block : _blocks)
		block->Render();
}

void MapToolScene::PostRender()
{
	_save->PostRender();
	_load->PostRender();
}

void MapToolScene::Save()
{
	vector<BlockInfo> blockInfo;

	for (auto block : _blocks)
	{
		if (block->_isActive)
		{
			BlockInfo info;
			Vector2 pos = block->GetPosition();
			info.pos = pos;

			blockInfo.push_back(info);
		}
	}

	BinaryWriter writer = BinaryWriter(L"Save/map.map");
	writer.String("BlockInfo");
	writer.UInt(blockInfo.size());
	writer.Byte(blockInfo.data(), sizeof(BlockInfo) * blockInfo.size());
}

void MapToolScene::Load()
{
	vector<BlockInfo> blockInfo;

	BinaryReader reader = BinaryReader(L"Save/map.map");
	string infoName = reader.String();

	if (infoName == "BlockInfo")
	{
		UINT size = reader.UInt();
		blockInfo.resize(size);

		void* ptr = blockInfo.data();
		reader.Byte(&ptr, size * sizeof(BlockInfo));
	}

	for (int i=0; i < blockInfo.size(); i++)
	{
		if (i >= _blocks.size() - 1)
			break;
		
		_blocks[i]->SetPosition(blockInfo[i].pos);
		_blocks[i]->_isActive = true;
	}
}
