#include <DxLib.h>
#include "FontRegistry.h"
#include "../Application.h"

const std::wstring FontRegistry::FONT_DOT = L"ベストテンDOT";
const std::wstring FontRegistry::FONT_BOKUTATI = L"ぼくたちのゴシック";

FontRegistry::FontRegistry()
{
}

FontRegistry::~FontRegistry()
{
}

void FontRegistry::Init()
{
	//フォント登録を行う
	// フォントパス初期化
	fontPath_[static_cast<int>(FONT_TYPE::DOT)] = L"ベストテンDOT.otf";
	fontPath_[static_cast<int>(FONT_TYPE::BOKUTATI)] = L"bokutachi.otf";

	// フォントをシステムに登録
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
	// フォント登録解除
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
