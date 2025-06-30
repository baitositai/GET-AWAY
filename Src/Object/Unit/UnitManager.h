#pragma once
#include <memory>
#include <vector>

class GameScene;
class Player;
class CpuPlayer;
class PathFinder;
enum class CHARA_TYPE;
enum class UNIT_STATE;

class UnitManager
{
public:

	//�^�[�Q�b�g�Đݒ�p�C���^�[�o��
	static constexpr float RE_SET_TARGET_INTERVAL_FPS = 1.0f;

	//�_���[�W��
	static constexpr int DEFAULT_DAMAGE = 1;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pathFinder">�o�H�T��</param>
	UnitManager(PathFinder& pathFinder);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~UnitManager();

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	void Load();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="parent">�e�C���X�^���X</param>
	void Update(GameScene& parent);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	///	�S�Ẵ��j�b�g�̃X�e�[�g��Ԃ�ύX
	/// </summary>
	/// <param name="state">�ύX��̏��</param>
	void ChangeAllUnitState(const UNIT_STATE state);

	/// <summary>
	/// �v���C���[���S�����Ԃ�
	/// </summary>
	/// <returns>���S����</returns>
	const bool IsPlayerDeath() const;

private:

	//�^�[�Q�b�g�Đݒ�C���^�[�o���X�e�b�v
	float reTargetIntervalStep_;

	//�v���C���[
	std::unique_ptr<Player> player_;

	//CPU�z��
	std::vector<std::unique_ptr<CpuPlayer>> cpus_;

	//�o�H�T��
	PathFinder& pathFinder_;

	//CPU�̃^�[�Q�b�g���Đݒ肷��
	void ReSetTargetPos();

	//�L�����N�^�[�̎�ނ������_���œ���
	CHARA_TYPE GetRandomCharaType(void) const;	
	
	//�v���C���[��CPU�̏Փ˔���
	void CollisonPlayerToCpu();

	//CPU���폜
	void DeleteCpu();

	//CPU�𐶐�
	void CreateCpu();
};


