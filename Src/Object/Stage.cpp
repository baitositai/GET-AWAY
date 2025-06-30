#include "Stage.h"
#include "../Manager/ResourceManager.h"
#include "../Utility/Utility.h"
#include "../Application.h"
Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Load()
{
	//�X�e�[�W�f�[�^�̓ǂ݂���
	overData_ = Utility::LoadCSVData(Application::PATH_CSV + L"Stage_Over.csv");
	planeData_ = Utility::LoadCSVData(Application::PATH_CSV + L"Stage_Plane.csv");
	hitData_ = Utility::LoadCSVData(Application::PATH_CSV + L"Stage_Hit.csv");

	//���\�[�X�ǂݍ���
	ResourceManager& res = ResourceManager::GetInstance();
	imgMapChips_ = res.Load(ResourceManager::SRC::MAP_CHIPS).handleIds_;
}

void Stage::Init()
{
	// CSV�f�[�^�̃T�C�Y���擾
	size_.y = static_cast<int>(hitData_.size());
	size_.x = static_cast<int>(size_.y > 0 ? hitData_[0].size() : 0);

	// �}�b�v�f�[�^�̏�����
	for (int y = 0; y < size_.y; y++)
	{
		std::vector<MapData> mapDataRow;
		for (int x = 0; x < size_.x; x++)
		{
			MapData mapData;
			mapData.role = hitData_[y][x];
			switch (mapData.role)
			{
				//��Q�����Ȃ��ꍇ
			case -1:
				mapData.color = Utility::GREEN;
				mapData.role = static_cast<int>(ROLE::NONE);	//�Ȃ���ݒ�
				break;

				//��Q��������ꍇ
			default:
				mapData.color = Utility::LIME;
				mapData.role = static_cast<int>(ROLE::WALL);	//�ǂ�ݒ�
				break;
			}
			mapDataRow.push_back(mapData);
		}
		mapDatas_.push_back(mapDataRow);
	}
}

void Stage::Draw()
{
	for (int y = 0; y < size_.y; y++)
	{
		for (int x = 0; x < size_.x; x++)
		{
			//�n��
			DrawGraph(
				x * MAP_CHIP_SIZE,
				y * MAP_CHIP_SIZE,
				imgMapChips_[planeData_[y][x]],	
				true);

			//��Q��
			DrawGraph(
				x * MAP_CHIP_SIZE,
				y * MAP_CHIP_SIZE,
				imgMapChips_[hitData_[y][x]],	
				true);

		}
	}
}

void Stage::OverDraw()
{
	for (int y = 0; y < size_.y; y++)
	{
		for (int x = 0; x < size_.x; x++)
		{
			//�㏑���`��
			DrawGraph(
				x * MAP_CHIP_SIZE,
				y * MAP_CHIP_SIZE,
				imgMapChips_[overData_[y][x]],
				true);
		}
	}
}

bool Stage::IsCollidableMapChip(const Vector2 mapPos)
{
	//��ގ擾
	ROLE type = static_cast<ROLE>(mapDatas_[mapPos.y][mapPos.x].role);

	//�ǂ̏ꍇ
	if (type == ROLE::WALL)
	{
		//�Փ�
		return true;
	}
	return false;
}

std::vector<std::vector<int>> const Stage::GetMapData()
{
	std::vector<std::vector<int>> mapData;
	for (int y = 0; y < size_.y; y++)
	{
		std::vector<int> row;
		for (int x = 0; x < size_.x; x++)
		{
			row.push_back(mapDatas_[y][x].role);
		}
		mapData.push_back(row);
	}
	return mapData;
}