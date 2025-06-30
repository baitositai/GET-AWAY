#pragma once
#include <string>
#include <memory>

class FpsControl;
class FontRegistry;

class Application
{

public:

	// �X�N���[���T�C�Y
	static constexpr int SCREEN_SIZE_X = 1024;
	static constexpr int SCREEN_SIZE_Y = 640;

	// �X�N���[���T�C�Y�n�[�t
	static constexpr int SCREEN_HALF_X = SCREEN_SIZE_X / 2;
	static constexpr int SCREEN_HALF_Y = SCREEN_SIZE_Y / 2;

	// �f�[�^�p�X�֘A
	//-------------------------------------------
	static const std::wstring PATH_IMAGE;
	static const std::wstring PATH_MODEL;
	static const std::wstring PATH_EFFECT;
	static const std::wstring PATH_SOUND;
	static const std::wstring PATH_FONT;
	static const std::wstring PATH_TEXT;
	static const std::wstring PATH_JSON;
	static const std::wstring PATH_CSV;
	//-------------------------------------------

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);

	// �Q�[�����[�v�̊J�n
	void Run(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// �����������^���s�̔���
	bool IsInitFail(void) const;

	// ��������^���s�̔���
	bool IsReleaseFail(void) const;

private:

	// �ÓI�C���X�^���X
	static Application* instance_;

	// ���������s
	bool isInitFail_;

	// ������s
	bool isReleaseFail_;

	//FPS
	std::unique_ptr<FpsControl> fps_;

	//�t�H���g�o�^
	std::unique_ptr<FontRegistry> fontReg_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	Application(void);
	Application(const Application& manager) = default;
	~Application(void) = default;

	// Effekseer�̏�����
	void InitEffekseer(void);

};