#pragma once
#include <functional>
#include "../../Common/Vector2.h"
class StartSign
{
public:

	//�ړ����x
	static constexpr int MOVE_SPEED = 8;

	//�g�呬�x
	static constexpr float RATE_SPEED = 0.05f;

	//�ҋ@����
	static constexpr float WAIT_TIME = 0.9f;

	/// <summary>
	/// ���
	/// </summary>
	enum class STATE
	{
		READY,
		GO,
		FINISH,
		NONE,
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	StartSign();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~StartSign();

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
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �T�C���̏I����Ԃ�
	/// </summary>
	const bool FinishSign()const;

	/// <summary>
	/// �I���ɕύX
	/// </summary>
	void ChangeFinishSign();

private:

	//���
	STATE state_;

	//�摜
	int* img_;
	int imgFinish_;

	//�摜�p�C���f�b�N�X
	int imgIndex_;

	//�g�嗦
	float rate_;

	//�ҋ@�X�e�b�v
	float waitStep_;
	
	//���W
	Vector2 pos_;

	//��ԕʍX�V����
	std::unordered_map<STATE, std::function<void()>> stateMap_;

	//��ԕʍX�V�����̓o�^
	void RegisterStateUpdate(const STATE state, std::function<void()> func);

	//��ԑJ��
	inline const void ChangeState(const STATE state) { state_ = state; }

	//��ԕʍX�V����
	void UpdateStateReady();
	void UpdateStateGo();
	void UpdateStateFinish();
	void UpdateStateNone();

};

