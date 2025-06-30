#include <DxLib.h>
#include "FontRegistry.h"
#include "../Application.h"

const std::wstring FontRegistry::FONT_DOT = L"�x�X�g�e��DOT";
const std::wstring FontRegistry::FONT_BOKUTATI = L"�ڂ������̃S�V�b�N";

FontRegistry::FontRegistry()
{
}

FontRegistry::~FontRegistry()
{
}

void FontRegistry::Init()
{
	//�t�H���g�o�^���s��
	// �t�H���g�p�X������
	fontPath_[static_cast<int>(FONT_TYPE::DOT)] = L"�x�X�g�e��DOT.otf";
	fontPath_[static_cast<int>(FONT_TYPE::BOKUTATI)] = L"bokutachi.otf";

	// �t�H���g���V�X�e���ɓo�^
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (AddFontResourceEx(
			(Application::PATH_FONT + fontPath_[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}

void FontRegistry::Destroy()
{
	// �t�H���g�o�^����
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (RemoveFontResourceEx(
			(Application::PATH_FONT + fontPath_[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}
