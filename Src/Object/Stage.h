#pragma once
#include <vector>
#include "../Common/Vector2.h"

//�X�e�[�W�̎��
enum class STAGE_TYPE
{
	TYPE1,
	TYPE2,
	TYPE3,
};

class Stage
{
public:

	//�}�b�v�`�b�v�̖���
	enum class ROLE
	{
		NONE,
		WALL,
		MAX
	};

	//�}�b�v�`�b�v�T�C�Y
	static constexpr int MAP_CHIP_SIZE = 32;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stage();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Stage();

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	void Load();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �ʏ�`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[���㏑�����ĕ`��
	/// </summary>
	void OverDraw();

	/// <summary>
	/// ���̃}�b�v�`�b�v���Փ˔�����������ׂ�
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	/// <returns>�Փ˔��茋��</returns>
	bool IsCollidableMapChip(const Vector2 mapPos);

	/// <summary>
	/// �}�b�v�f�[�^��Ԃ�
	/// </summary>
	/// <returns>�}�b�v�f�[�^</returns>
	std::vector<std::vector<int>> const GetMapData();

private:

	//�}�b�v�f�[�^�̖����ƐF���i�[����\����
	struct MapData
	{
		int role;		//����
		int color;		//�F
	};

	//�X�e�[�W���̊i�[
	std::vector<std::vector<int>> planeData_;	//�n��
	std::vector<std::vector<int>> hitData_;		//��Q��
	std::vector<std::vector<int>> overData_;	//�㏑���`��p

	//�}�b�v�f�[�^
	std::vector<std::vector<MapData>> mapDatas_;

	//�}�b�v�T�C�Y
	Vector2 size_;

	//�}�b�v�`�b�v�摜
	int* imgMapChips_;

};

