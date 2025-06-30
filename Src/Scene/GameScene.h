#pragma once
#include <memory>
#include <unordered_map>
#include "SceneBase.h"
#include "../Common/Vector2.h"

class Stage;
class UnitManager;
class PathFinder;	
class StartSign;

/// <summary>
/// ���
/// </summary>
enum class GAME_STATE
{
	READY,
	PLAY,
	RESULT,
};

class GameScene : public SceneBase
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// �ǂݍ��ݏ���
	/// </summary>
	void Load() override;

	/// <summary>
	/// ������
	/// </summary>
	void Init() override;

	/// <summary>
	/// �v���C���[�ƃ}�b�v�`�b�v�̏Փ˔���
	/// </summary>
	/// <param name="playerPos">�v���C���[�ʒu</param>
	/// <returns>���茋��</returns>
	bool CheckPlayerMapCollision(const Vector2 playerPos);

private:	

	//���
	GAME_STATE state_;
	
	//�X�e�[�W
	std::unique_ptr<Stage> stage_;

	//�v���C���[�Ǘ��N���X
	std::unique_ptr<UnitManager> unitMng_;

	//�o�H�T��
	std::unique_ptr<PathFinder> pathFinder_;

	//�X�^�[�g���}
	std::unique_ptr<StartSign> startSign_;

	//��ԕʏ����̊Ǘ�
	std::unordered_map<GAME_STATE, std::function<void()>> stateUpdateMap_;
	std::unordered_map<GAME_STATE, std::function<void()>> stateDrawMap_;

	//�X�V�֐�
	void NormalUpdate() override;

	//�`��֐�
	void NormalDraw() override;

	//�����̕ύX
	void ChangeNormal() override;
	
	//��ԑJ��
	inline const void ChangeState(const GAME_STATE state) { state_ = state; }
	
	//��Ԃ̏�����o�^
	void RegisterStateProcess(const GAME_STATE state, std::function<void()> update, std::function<void() >draw);

	//��ԕʏ���
	void UpdateStateReady();	//����
	void UpdateStatePlay();		//�v���C
	void UpdateStateResult();	//���U���g

	//��ԕʕ`��
	void DrawStateReady();
	void DrawStatePlay();
	void DrawStateResult();

	//�f�o�b�O����
	void DebagUpdate();	
	void DebagDraw();
};