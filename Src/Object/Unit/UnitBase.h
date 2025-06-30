#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"

class GameScene;
class Animation2DController;
class UnitEffects;
enum class EFFECT_TYPE;

//���j�b�g���
enum class CHARA_TYPE
{
	BAKU,
	REI,
	MAX
};

//���j�b�g�̃X�e�[�g���
enum class UNIT_STATE
{
	NONE,
	RESPWAN,
	PLAY,
	DIE,
};

//���j�b�g�̃R���f�B�V�������
enum class UNIT_CONDITION
{
	NONE,
	DAMAGE,
};

class UnitBase
{
public:

	//���a
	static constexpr float RADIUS = 14.0f;

	//�ړ����x
	static constexpr float MOVE_SPEED = 3.0f;

	//�A�j���[�V�������x
	static constexpr float ANIM_SPEED = 0.1f;

	//�g�嗦
	static constexpr float RATE_SCALE = 1.2f;

	//���G����
	static constexpr float INVINCIBLE_TIME = 2.0f;

	//�_�ŗp�A���t�@�l
	static constexpr float BLINKING_ALPHA = 128.0f;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="charaType">�L�����N�^�[���</param>
	UnitBase(const CHARA_TYPE charaType);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UnitBase();

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
	/// <param name="parent">�Q�[���N���X</param>
	void Update(GameScene & parent);

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="damage"�_���[�W�l</param>
	void SetDamage(const int damage);
	
	/// <summary>
	/// ��ԕύX
	/// </summary>
	/// <param name="state">�ύX��̏��</param>
	inline void ChangeState(const UNIT_STATE state) { state_ = state; }

	/// <summary>
	/// �G�t�F�N�g�̍Đ�
	/// </summary>
	/// <param name="effectTYpe">�G�t�F�N�g���</param>
	void PlayEffect(const EFFECT_TYPE effectType);

	/// <summary>
	/// ���W��Ԃ�
	/// </summary>
	/// <returns>���W</returns>
	inline const Vector2 GetPos() { return pos_.ToVector2(); }

	/// <summary>
	/// ���S�����Ԃ�
	/// </summary>
	/// <returns>���S����</returns>
	inline const bool IsDeath()const { return state_ == UNIT_STATE::DIE; }

	/// <summary>
	/// ���G���ǂ�������
	/// </summary>
	/// <returns>���G���Ԃ�0�ȏ�̏ꍇtrue�Ԃ�</returns>
	inline const bool IsInvincible()const { return invincibleCnt_ > 0.0f; }

protected:

	//�A�j���[�V�����֘A
	static constexpr int ANIM_STOP_INDEX = 0;		//��~��Ԃ̃C���f�b�N�X
	static constexpr int ANIM_RUN_INDEX = 1;		//���s��Ԃ̃C���f�b�N�X
	static constexpr int ANIM_RUNS = 2;				//���s��Ԃ̃A�j���[�V������
	static constexpr int ANIM_ADD_INDEX = 3;		//�A�j���[�V�����̃I�t�Z�b�g

	//�摜
	int* imgChara_;

	//���j�b�g�J���[
	int unitColor_;

	//���C�t
	int life_;

	//���W
	Vector2F pos_;

	//�ړ���
	Vector2 movePow_;

	//�A�j���[�V�����X�s�[�h
	float animSpeed_;

	//�A�j���[�V�����J�n�C���f�b�N�X
	//�A�j���[�V�����I���C���f�b�N�X
	int animStartIndex_;
	int animFinishIndex_;

	//�������]
	bool dir_;

	//�G�t�F�N�g
	std::unique_ptr<UnitEffects> effects_;

private:	

	//�`��A���t�@�l
	int drawAlpha_;

	//���G���ԗp�J�E���g
	float invincibleCnt_;
	
	//���j�b�g�̎��
	CHARA_TYPE charaType_;	

	//�X�e�[�g���
	UNIT_STATE state_;

	//�R���f�B�V�������
	UNIT_CONDITION condition_;

	//2D�A�j���[�V�����R���g���[���[
	std::unique_ptr<Animation2DController> animController_;

	//�X�e�[�g��Ԃ��Ƃ̏����Ǘ�
	std::unordered_map<UNIT_STATE, std::function<void(GameScene&)>> stateMap_;

	//�R���f�B�V������Ԃ��Ƃ̏����Ǘ�
	std::unordered_map<UNIT_CONDITION, std::function<void()>> conditionMap_;

	//��Ԃ̏�����o�^
	void RegisterStateUpdate(const UNIT_STATE state, std::function<void(GameScene&)> func);
	void RegisterCondition(const UNIT_CONDITION condition, std::function<void()> func);

	//�L�����N�^�[�̃��\�[�X��ǂݍ���
	void CharacterResourceLoad();

	//��ԕʍX�V����
	void UpdateStateNone(GameScene&);		//�Ȃ�
	void UpdateStatePlay(GameScene&);		//�v���C
	void UpdateStateRespawn(GameScene&);	//���X�|�[��
	void UpdateStateDie(GameScene&);		//���S

	//�p�����[�^���̐ݒ�
	virtual void SetParameter() = 0;

	//�A�j���[�V����������
	void InitAnimation();

	//�ړ��̍X�V
	virtual void ProcessMoveUpdate(GameScene& parent) = 0;

	//�A�j���[�V�����̍X�V
	void AnimationUpdate();

	//�R���f�B�V�������
	inline void const ChangeCondition(const UNIT_CONDITION condition) { condition_ = condition; }
	void ConditionNone();
	void ConditionDamage();

};