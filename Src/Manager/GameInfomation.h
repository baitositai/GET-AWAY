#pragma once
#include <vector>

enum class CHARA_TYPE;
enum class STAGE_TYPE;

class GameInfomation
{
public:

	//�Q�[���V�X�e���̏����i�[
	struct GameInfo
	{
		int playerNum_;			//�v���C���[�̐l��
		int cpuNum_;			//CPU�̐l��
		STAGE_TYPE stageType_;	//�X�e�[�W�̎��
	};

	//�v���C���[�Ɋւ�������i�[
	struct PlayerInfo
	{
		CHARA_TYPE charaType_;	//�L�����N�^�[�̎��
	};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameInfomation();

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �ÓI�C���X�^���X���擾
	/// </summary>
	/// <returns></returns>�C���X�^���X��Ԃ�
	static GameInfomation& GetInstance();

	/// <summary>
	/// �v���C���[�l����ݒ�
	/// </summary>
	/// <param name="playerNum"></param>
	void SetPlayerNum(const int playerNum);

	/// <summary>
	/// �v���C���[�̃��j�b�g�̎�ނ�ݒ�
	/// </summary>
	/// <param name="playerNo"></param>
	/// <param name="unitType"></param>
	void SetPlayerUnitType(const int playerNo, const CHARA_TYPE charaType);

	/// <summary>
	/// �v���C���[�̏���Ԃ�
	/// </summary>
	/// <param name="playerNo"></param>
	/// <returns></returns>
	const PlayerInfo GetPlayerInfomation(const int playerNo) const;

	/// <summary>
	/// �Q�[���̃V�X�e������Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const GameInfo& GetGameInfo(void) const { return gameInfo_; }

	/// <summary>
	/// ���
	/// </summary>
	void Destroy();

private:	

	//�ÓI�C���X�^���X
	static GameInfomation* instance_;

	//�Q�[���V�X�e���̏��
	GameInfo gameInfo_;	

	//�v���C���[�Ɋւ�������i�[����x�N�^�[
	std::vector<PlayerInfo> playerInfo_;
	
	//�R���X�g���N�^
	GameInfomation();

	//�R�s�[�R���X�g���N�^���������،Ăяo���Ȃ��悤�ɂ���
	GameInfomation(const GameInfomation& app) = delete;

	//������Z�q���폜
	void operator = (const GameInfomation& app) = delete;

};

