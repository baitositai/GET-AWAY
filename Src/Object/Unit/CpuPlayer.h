#pragma once
#include <vector>
#include <string>
#include "UnitBase.h"

class PathFinder;
enum class PATH_FIND_TYPE;

class CpuPlayer : public UnitBase
{
public:

	//�̗�
	static constexpr int DEFAULT_LIFE = 1;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="charaType">�L�������</param>
	/// <param name="pathFinder">�o�H�T��</param>
	/// <param name="pathFindType">�o�H�T�����</param>
	CpuPlayer(const CHARA_TYPE charaType, PathFinder& pathFinder, const PATH_FIND_TYPE pathFindType);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	/// <param name=""></param>
	~CpuPlayer(void) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
	
	/// <summary>
	/// �ړI�n��ݒ肷��
	/// </summary>
	/// <param name="target">�ړI�n</param>
	void SetTargetaPos(const Vector2 target);

	/// <summary>
	/// �}�b�v���W��ݒ�
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	void SetMapToWorldPos(const Vector2 mapPos);

private:

	//�o�H�T��
	PathFinder& pathFinder_;

	//�o�H�T����@
	PATH_FIND_TYPE pathFindType_;

	//�ŒZ�o�H
	std::vector<Vector2> movePath_;

	//���ǂ��Ă���o�H�̃C���f�b�N�X
	int pathIndex_;

	//�p�����[�^�[�ݒ�
	void SetParameter() override;

	//�ړ�����
	void ProcessMoveUpdate(GameScene& parent) override;

	//�o�H�T������
	std::wstring pathFindNames_[3] = {
		L"�_�C�N�X�g��",
		L"�E��T��",
		L"A�X�^�[" };
};

