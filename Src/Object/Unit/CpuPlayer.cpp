#include "CpuPlayer.h"
#include "../../Utility/Utility.h"
#include "../../Utility/PathFinder.h"
#include "../Stage.h"
#include "UnitEffects.h"

namespace
{
	Vector2 mapChipSize = { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE };		//�}�b�v�`�b�v�T�C�Y
	Vector2 localPos = { Stage::MAP_CHIP_SIZE / 2,Stage::MAP_CHIP_SIZE / 2 };	//���΍��W
}

CpuPlayer::CpuPlayer(const CHARA_TYPE charaType, PathFinder& pathFinder, const PATH_FIND_TYPE pathFindType) :
	UnitBase(charaType),
	pathFinder_(pathFinder),
	pathFindType_(pathFindType)
{
	pathIndex_ = -1;	
}

CpuPlayer::~CpuPlayer(void)
{
}

void CpuPlayer::Draw()
{
	UnitBase::Draw(); // ���N���X�̕`����Ăяo��

	Vector2 pos = pos_.ToVector2(); // ���݂̈ʒu���擾
	int length = static_cast<int>(pathFindNames_[static_cast<int>(pathFindType_)].length() * 16); // �o�H�T�����̂̒������v�Z
	pos = Vector2::AddVector2(pos, { -length / 2, -20}); // ���΍��W�����Z

	//�o�H�T�����̂�`��
	DrawFormatString(pos.x,pos.y,Utility::RED, pathFindNames_[static_cast<int>(pathFindType_)].c_str());
}

void CpuPlayer::SetTargetaPos(const Vector2 target)
{
	//���݂̌o�H���폜
	movePath_.clear();
	pathIndex_ = 0;

	//�v���C���[���W���}�b�v���W�֕ϊ�
	Vector2 mapPos = Utility::WorldToMapPos(pos_.ToVector2(), mapChipSize);

	//�o�H��T��
	bool isSearch = false;
	switch (pathFindType_)
	{
	case PATH_FIND_TYPE::DIJKSTRA:
		isSearch = pathFinder_.FindPath(mapPos, target, movePath_);
		break;
	case PATH_FIND_TYPE::RIGHT_PATH_FIND:
		isSearch = pathFinder_.RightHandSearch(mapPos, target, movePath_);
		break;
	case PATH_FIND_TYPE::A_STAR:
		isSearch = pathFinder_.FindPathAStar(mapPos, target, movePath_);
		break;
	}
	//�o�H�����������ꍇ
	if (isSearch)
	{
		//�o�H�����������ꍇmovePath_�ɕۑ������
		//�ŏ��͌��ݒn�Ȃ̂Ŏ��̃}�X����X�^�[�g�����
		pathIndex_++;
	}
}

void CpuPlayer::SetMapToWorldPos(const Vector2 mapPos)
{
	pos_ = Utility::MapToWorldPos(mapPos, mapChipSize).ToVector2F();
	pos_ = Vector2F::AddVector2F(pos_, localPos.ToVector2F());
}

void CpuPlayer::SetParameter()
{
	unitColor_ = Utility::PINK; //���j�b�g�J���[�ݒ�

	//�̗�
	life_ = DEFAULT_LIFE;

	//���΍��W������
	pos_ = Vector2F::AddVector2F(pos_, localPos.ToVector2F());
}

void CpuPlayer::ProcessMoveUpdate(GameScene& parent)
{
	//�o�H���Ȃ��ꍇ���������s���Ȃ�
	if (pathIndex_ >= movePath_.size())
	{
		return;
	}

	//���΍��W������
	Vector2F pos = Vector2F::SubVector2F(pos_, localPos.ToVector2F());

	//���ɐi�ނׂ��}�X
	const Vector2& nextPos = movePath_[pathIndex_];

	//�v���C���[���W���}�b�v���W�֕ϊ�
	Vector2 mapPos = Utility::WorldToMapPos(pos.ToVector2(), mapChipSize);

	//���̃}�X�̃��[���h���W
	Vector2 targetPixelPos = Utility::MapToWorldPos(nextPos, mapChipSize);

	//���������߂�
	Vector2 delta = Vector2::SubVector2(targetPixelPos, pos.ToVector2());

	// �������l�ȓ��Ȃ瓞�B�����Ƃ݂Ȃ�
	if (delta.Length() < MOVE_SPEED)
	{
		pos = Utility::MapToWorldPos(nextPos, mapChipSize).ToVector2F(); // �␳
		pathIndex_++;   // ����
		return;
	}

	// 4�����݂̂Ɍ���iX�D��A�܂���Y�D��j
	// delta �͎��̖ڕW�}�X�̕����x�N�g���i�s�N�Z���P�ʁj
	movePow_ = { 0, 0 };

	if (std::abs(delta.x) > std::abs(delta.y))
	{
		movePow_.x = (delta.x > 0) ? 1 : -1;
	}
	else
	{
		movePow_.y = (delta.y > 0) ? 1 : -1;
	}

	// �ړ�
	pos.x += movePow_.x * MOVE_SPEED;
	pos.y += movePow_.y * MOVE_SPEED;

	//���΍��W������
	pos_ = Vector2F::AddVector2F(pos, localPos.ToVector2F());
}