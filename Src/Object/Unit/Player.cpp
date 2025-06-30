#include "Player.h"
#include "../../Utility/Utility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/GameInfomation.h"
#include "../../Scene/GameScene.h"
#include "../Stage.h"
#include "UnitEffects.h"


namespace
{
	Vector2 mapChipSize = { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE };		//�}�b�v�`�b�v�T�C�Y
	Vector2 localPos = { Stage::MAP_CHIP_SIZE / 2,Stage::MAP_CHIP_SIZE / 2 };	//���΍��W
}

Player::Player(const CHARA_TYPE charaType, const int playerNo) :
	UnitBase(charaType),
	playerNo_(playerNo)
{	
	//�ړ����쏈���̓o�^
	RegisterProcessMove(MOVE_TYPE::NORMAL, [this](GameScene& parent) {ProcessMoveNormal(parent); });
	RegisterProcessMove(MOVE_TYPE::MASS, [this](GameScene& parent) {ProcessMoveMass(parent); });
	RegisterProcessMove(MOVE_TYPE::NONSTOP, [this](GameScene& parent) {ProcessMoveNonstop(parent); });

	//������
	moveType_ = MOVE_TYPE::NONE;
	movePow_ = {};
	targetPos_ = {};
}

Player::~Player(void)
{
}

void Player::RegisterProcessMove(const MOVE_TYPE moveType, std::function<void(GameScene&)> func)
{
	//�o�^
	processMoveMap_[moveType] = func;
}

void Player::SetParameter()
{
	unitColor_ = Utility::BLUE;

	//�������W
	Vector2 mapPos = { 3,16 };
	pos_ = Utility::MapToWorldPos(mapPos, { Stage::MAP_CHIP_SIZE,Stage::MAP_CHIP_SIZE }).ToVector2F();
	pos_ = Vector2F::AddVector2F(pos_, { Stage::MAP_CHIP_SIZE / 2,Stage::MAP_CHIP_SIZE / 2 });

	//�̗�
	life_ = DEFAULT_LIFE;

	//��ԕύX
	ChangeMoveType(MOVE_TYPE::MASS);
}

void Player::ProcessMoveUpdate(GameScene& parent)
{
	//�ړ���ނ��Ƃ̏���
	processMoveMap_[moveType_](parent);
}

void Player::ProcessMoveNormal(GameScene& parent)
{
	auto& input = InputManager::GetInstance();
	Vector2F move = {};
	Vector2F offSetPos = {};
	bool dir = false;	//false : X����, true : Y����

	//���쏈��(4��������,���������Ȃ�)
	if (input.IsNew(KEY_INPUT_RIGHT)) { move.x += MOVE_SPEED; offSetPos.x += RADIUS; }
	else if (input.IsNew(KEY_INPUT_LEFT)) { move.x -= MOVE_SPEED; offSetPos.x -= RADIUS; }
	else if (input.IsNew(KEY_INPUT_DOWN)) { move.y += MOVE_SPEED; offSetPos.y += RADIUS; dir = true; }
	else if (input.IsNew(KEY_INPUT_UP)) { move.y -= MOVE_SPEED; offSetPos.y -= RADIUS; dir = true; }

	//���͊m�F�i�ړ��ʂ��m�F�ł��Ȃ������ꍇ�j
	if (!(move.x != 0.0f || move.y != 0.0f))
	{
		return;	//��������߂�
	}

	//���͕������Ƃɂ���Ɉʒu�𒲐�
	Vector2F range = {};
	if (!dir) { range.y = RADIUS; }
	else { range.x = RADIUS; }

	//�Փ˔���𒲂ׂ�
	bool isCollision = false;
	for (int i = -1; i < (int)CHECK_COL::MAX - 1; i++)
	{
		//���W�𒲐�����
		Vector2F checkPos = {
			pos_.x + move.x + offSetPos.x + i * range.x,
			pos_.y + move.y + offSetPos.y + i * range.y };

		if (parent.CheckPlayerMapCollision(checkPos.ToVector2()))
		{
			isCollision = true;
			break;
		}
	}

	//�Փ˂��ĂȂ��ꍇ�ړ�����
	if (!isCollision) { pos_ = Vector2F::AddVector2F(pos_, move); }
}

void Player::ProcessMoveMass(GameScene& parent)
{
	auto& input = InputManager::GetInstance();

	//���쏈��(4��������,���������Ȃ�)
	if (movePow_.x == 0 && movePow_.y == 0)	//�ړ����͖���
	{
		if (input.IsNew(KEY_INPUT_RIGHT)) { 
			movePow_ = { 1,0 };}
		else if (input.IsNew(KEY_INPUT_LEFT)) { movePow_ = { -1,0 };}
		else if (input.IsNew(KEY_INPUT_DOWN)) { movePow_ = { 0,1 }; }
		else if (input.IsNew(KEY_INPUT_UP)) { movePow_ = { 0, -1 }; }
		else { movePow_ = { 0,0 }; }

		//���͊m�F�i�ړ��ʂ��m�F�ł��Ȃ������ꍇ�j
		if (movePow_.x == 0 && movePow_.y == 0)
		{
			return;	//��������߂�
		}

		//�ړ���̏Փ˔���
		//���W�𒲐�����
		Vector2F checkPos = {
			pos_.x + movePow_.x * mapChipSize.x,
			pos_.y + movePow_.y * mapChipSize.y };
		//�Փˊm�F
		if (parent.CheckPlayerMapCollision(checkPos.ToVector2()))
		{
			movePow_ = { 0,0 };	//������
			return;//�����I��
		}

		//���݈ʒu�̃}�b�v���W���擾
		Vector2 mapPos = Utility::WorldToMapPos(pos_.ToVector2(), mapChipSize);

		//�ړ���̍��W��ݒ�
		targetPos_ = { 
			static_cast<float>((mapPos.x + movePow_.x) * mapChipSize.x) + localPos.x,
			static_cast<float>((mapPos.y + movePow_.y) * mapChipSize.y) + localPos.y };
	}

	//�ړ�����
	if((movePow_.x != 0 || movePow_.y != 0) && 
		!Vector2::IsSameVector2(targetPos_.ToVector2(), {0,0}))
	{
		//�^�[�Q�b�g�Ɍ������Ĉړ�
		if (std::abs(pos_.x - targetPos_.x) > MOVE_SPEED)
		{
			pos_.x += static_cast<float>(MOVE_SPEED * movePow_.x);
		}
		else
		{
			pos_.x = targetPos_.x;
		}
		if (std::abs(pos_.y - targetPos_.y) > MOVE_SPEED)
		{
			pos_.y += static_cast<float>(MOVE_SPEED * movePow_.y);
		}
		else
		{
			pos_.y = targetPos_.y;
		}
		// �^�[�Q�b�g�ɓ��B�������~
		if (Vector2::IsSameVector2(pos_.ToVector2(), targetPos_.ToVector2()))
		{
			movePow_ = { 0,0 };
		}
	}
	
}

void Player::ProcessMoveNonstop(GameScene& parent)
{
	auto& input = InputManager::GetInstance();

	//���쏈��(4��������,���������Ȃ�)
	if (movePow_.x == 0 && movePow_.y == 0)	//�ړ����͖���
	{
		if (input.IsNew(KEY_INPUT_RIGHT)) { movePow_ = { 1,0 }; }
		else if (input.IsNew(KEY_INPUT_LEFT)) { movePow_ = { -1,0 }; }
		else if (input.IsNew(KEY_INPUT_DOWN)) { movePow_ = { 0,1 }; }
		else if (input.IsNew(KEY_INPUT_UP)) { movePow_ = { 0, -1 }; }

		//���͊m�F�i�ړ��ʂ��m�F�ł��Ȃ������ꍇ�j
		if (movePow_.x == 0 && movePow_.y == 0)
		{
			return;	//��������߂�
		}

		//�ړ���̏Փ˔���
		//���W�𒲐�����
		Vector2F checkPos = {
			pos_.x + movePow_.x * mapChipSize.x,
			pos_.y + movePow_.y * mapChipSize.y };
		//�Փˊm�F
		if (parent.CheckPlayerMapCollision(checkPos.ToVector2()))
		{
			//�����I��
			return;
		}

		//���݈ʒu�̃}�b�v���W���擾
		Vector2 mapPos = Utility::WorldToMapPos(pos_.ToVector2(), mapChipSize);

		//�ړ���̍��W��ݒ�
		targetPos_ = {
			static_cast<float>((mapPos.x + movePow_.x) * mapChipSize.x) + localPos.x,
			static_cast<float>((mapPos.y + movePow_.y) * mapChipSize.y) + localPos.y };
	}

	//�ړ�����
	if (movePow_.x != 0 || movePow_.y != 0)
	{
		//�^�[�Q�b�g�Ɍ������Ĉړ�
		if (std::abs(pos_.x - targetPos_.x) > MOVE_SPEED)
		{
			pos_.x += static_cast<float>(MOVE_SPEED * movePow_.x);
		}
		else
		{
			pos_.x = targetPos_.x;
		}
		if (std::abs(pos_.y - targetPos_.y) > MOVE_SPEED)
		{
			pos_.y += static_cast<float>(MOVE_SPEED * movePow_.y);
		}
		else
		{
			pos_.y = targetPos_.y;
		}
		// �^�[�Q�b�g�ɓ��B�������~
		if (Vector2::IsSameVector2(pos_.ToVector2(), targetPos_.ToVector2()))
		{
			movePow_ = { 0,0 };
		}
	}
}