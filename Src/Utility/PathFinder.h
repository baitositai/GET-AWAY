#pragma once
#include <vector>
#include <queue>
#include <utility>
#include <climits>
#include <algorithm>
#include "../Common/Vector2.h"


enum class PATH_FIND_TYPE
{
	DIJKSTRA,			//�_�C�N�X�g��
	RIGHT_PATH_FIND,	//�E��T��
	A_STAR,				//A*
};


class PathFinder
{
public:

	// �ő�o�H��
	static constexpr int MAX_PATH_LENGTH = 1000; 

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PathFinder();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PathFinder() = default;

	/// <summary>
	/// �}�b�v��ݒ�
	/// </summary>
	/// <param name="map">�}�b�v�f�[�^</param>
	void SetMap(const std::vector<std::vector<int>>& map);

	/// <summary>
	/// �_�C�N�X�g���ɂ��o�H�T��
	/// </summary>
	/// <param name="start">�T���X�^�[�g�ʒu</param>
	/// <param name="goal">�T���S�[���ʒu</param>
	/// <param name="path">�T�������o�H</param>
	/// <returns>�T�����s�����ꍇfalse��Ԃ�</returns>
	bool FindPath(const Vector2& start, const Vector2& goal, std::vector<Vector2>& path);

	/// <summary>
	/// �E��T���@�Ń��[�g����
	/// </summary>
	/// <param name="start">�T���X�^�[�g�ʒu</param>
	/// <param name="goal">�T���S�[���ʒu</param>
	/// <param name="pathOut">�o�͌o�H</param>
	/// <returns>�T�����s�����ꍇfalse��Ԃ�</returns>
	bool RightHandSearch(const Vector2& start, const Vector2& goal, std::vector<Vector2>& pathOut);

	/// <summary>
	/// A*�@�ɂ��o�H�T��
	/// </summary>
	/// <param name="start">�T���X�^�[�g�ʒu</param>
	/// <param name="goal">�T���S�[���ʒu</param>
	/// <param name="pathOut">�o�͌o�H</param>
	/// <returns>�T�����s�����ꍇfalse��Ԃ�</returns>
	bool FindPathAStar(const Vector2& start, const Vector2& goal, std::vector<Vector2>& pathOut);

private:
	
	// �T���m�[�h�i�ʒu�ƃR�X�g�j���Ǘ�����\����
	struct Node 
	{
		int cost; // �n�_���炱�̃m�[�h�܂ł̃R�X�g�i�����j
		Vector2 pos;

		// �D��x�t���L���[�ŃR�X�g�̏��������ɕ��Ԃ悤�ɔ�r���Z�q���`
		bool operator<(const Node& other) const 
		{
			return cost > other.cost; // ���������ɂȂ�悤�t��
		}

		// ���������ɕ��Ԃ悤��r���Z�q��`
		bool operator>(const Node& other) const
		{
			return cost > other.cost;
		}
	};

	std::vector<std::vector<int>> mapData_;				// �}�b�v�f�[�^�{�́i0���ʍs�ABLOCK���ǁj
	std::vector<std::vector<int>> distData_;			// �n�_����e�}�X�܂ł̍ŒZ�����i�R�X�g�j
	std::vector<std::vector<Vector2>> prevData_;		// �o�H�����p�A�e�}�X�̒��O�̃}�X��ێ�

	// �ړ������i��, �E, ��, ���j
	static constexpr int DIR_NUM = 4;         // �����̐�
	static constexpr int DX[DIR_NUM] = { 0, 1, 0, -1 };
	static constexpr int DY[DIR_NUM] = { -1, 0, 1, 0 };

	// �ʍs�s�l
	static constexpr int BLOCK = 1;

	// �}�b�v�͈̓`�F�b�N
	bool IsInside(int x, int y) const;

	//�E��T��---------------------------------------------------------

	enum class DIR
	{
		UP,
		RIGHT, 
		DOWN, 
		LEFT
	};

	/// <summary>
	/// �}�b�v��ł��̃}�X���ʍs�\�������𔻒f����
	/// </summary>
	bool IsWalkable(const Vector2& pos) const;

	// �E�ɋȂ���
	DIR TurnRight(DIR dir) {return static_cast<DIR>((static_cast<int>(dir) + 1) % 4);}

	// ���ɋȂ���
	DIR TurnLeft(DIR dir) {return static_cast<DIR>((static_cast<int>(dir) + 3) % 4);}

	// �t������
	DIR TurnBack(DIR dir) {return static_cast<DIR>((static_cast<int>(dir) + 2) % 4);}

	// ��������ړ��x�N�g�����擾
	Vector2 DirToVector(DIR dir)
	{
		switch (dir) 
		{
			case DIR::UP:
				return { 0, -1 };

			case DIR::RIGHT:
				return { 1, 0 };

			case DIR::DOWN:
				return { 0, 1 };

			case DIR::LEFT:
				return { -1, 0 };

			default: 
				return { 0, 0 };
		}
	}

	//A*�@---------------------------------------------------------------------------------

	// �q���[���X�e�B�b�N�֐��i�}���n�b�^�������j
	static int Heuristic(const Vector2& a, const Vector2& b);

};

