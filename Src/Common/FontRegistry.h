#pragma once
#include <string>

class FontRegistry
{
public:

	enum class FONT_TYPE
	{
		DOT,
		BOKUTATI,
		MAX
	};

	//�t�H���g���
	static constexpr int FONT_TYPES = static_cast<int>(FONT_TYPE::MAX);

	//�t�H���g�l�[��(cpp�Őݒ�)
	static const std::wstring FONT_DOT;
	static const std::wstring FONT_BOKUTATI;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	FontRegistry();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FontRegistry();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �������
	/// </summary>
	void Destroy();

private:

	//�o�^�p�p�X
	std::wstring fontPath_[FONT_TYPES];
};

