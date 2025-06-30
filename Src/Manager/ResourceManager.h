#pragma once
#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	//�L�����N�^�[�摜
	static constexpr int CHARA_DIV_X = 3;
	static constexpr int CHARA_DIV_Y = 2;
	static constexpr int CHARA_SIZE = 32;

	//�G�t�F�N�g�摜
	static constexpr int DAMAGE_EFE_DIV_X = 5;
	static constexpr int DAMAGE_EFE_DIV_Y = 1;
	static constexpr int DAMAGE_EFE_SIZE = 64;

	static constexpr int RESPAWN_EFE_DIV_X = 14;
	static constexpr int RESPAWN_EFE_DIV_Y = 1;
	static constexpr int RESPAWN_EFE_SIZE = 64;

	static constexpr int EXIT_EFE_DIV_X = 15;
	static constexpr int EXIT_EFE_DIV_Y = 1;
	static constexpr int EXIT_EFE_SIZE = 64;

	//�Q�[���J�n���b�Z�[�W
	static constexpr int GAME_START_MESS_DIV_X = 1;
	static constexpr int GAME_START_MESS_DIV_Y = 2;
	static constexpr int GAME_START_MESS_SIZE_X = 512;
	static constexpr int GAME_START_MESS_SIZE_Y = 256;

	//�Q�[�����b�Z�[�W
	static constexpr int GAME_MESS_DIV_X = 1;
	static constexpr int GAME_MESS_DIV_Y = 3;
	static constexpr int GAME_MESS_SIZE_X = 512;
	static constexpr int GAME_MESS_SIZE_Y = 256;

	//�}�b�v�`�b�v
	static constexpr int MAP_CHIPS_DIV_X = 8;
	static constexpr int MAP_CHIPS_DIV_Y = 292;
	static constexpr int MAP_CHIPS_SIZE = 32;


	// ���\�[�X��
	enum class SRC
	{
		//�^�C�g��
		GAME_LOGO,

		//�L�����N�^�[
		BAKU,
		REI,

		//�G�t�F�N�g
		DAMAGE_EFE,
		EXIT_EFE,
		RESPAWN_EFE,

		//UI
		GAME_MESS,

		//�X�e�[�W
		MAP_CHIPS
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// �������[�h
	Resource& _Load(SRC src);

};
