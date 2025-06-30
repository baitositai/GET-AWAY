#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	static std::wstring PATH_IMG = Application::PATH_IMAGE;
	static std::wstring PATH_MDL = Application::PATH_MODEL;
	static std::wstring PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

#pragma region タイトルロゴ
	std::wstring path_Title = Application::PATH_IMAGE + L"Title/";
	res = std::make_unique<Resource>(Resource::TYPE::IMG, path_Title + L"GameLogo.png");
	resourcesMap_.emplace(SRC::GAME_LOGO, std::move(res));
#pragma endregion 

#pragma region キャラクター
	std::wstring path_chara = Application::PATH_IMAGE + L"Character/";
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_chara + L"Baku.png", CHARA_DIV_X, CHARA_DIV_Y, CHARA_SIZE, CHARA_SIZE);
	resourcesMap_.emplace(SRC::BAKU, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_chara + L"Rei.png", CHARA_DIV_X, CHARA_DIV_Y, CHARA_SIZE, CHARA_SIZE);
	resourcesMap_.emplace(SRC::REI, std::move(res));
#pragma endregion 

#pragma region エフェクト
	std::wstring path_effect = Application::PATH_IMAGE + L"Effect/";
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_effect + L"DamageEffect.png", DAMAGE_EFE_DIV_X, DAMAGE_EFE_DIV_Y, DAMAGE_EFE_SIZE, DAMAGE_EFE_SIZE);
	resourcesMap_.emplace(SRC::DAMAGE_EFE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_effect + L"RespawnEffect.png", RESPAWN_EFE_DIV_X, RESPAWN_EFE_DIV_Y, RESPAWN_EFE_SIZE, RESPAWN_EFE_SIZE);
	resourcesMap_.emplace(SRC::RESPAWN_EFE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_effect + L"ExitEffect.png", EXIT_EFE_DIV_X, EXIT_EFE_DIV_Y, EXIT_EFE_SIZE, EXIT_EFE_SIZE);
	resourcesMap_.emplace(SRC::EXIT_EFE, std::move(res));
#pragma endregion 

#pragma region UI
	std::wstring path_ui = Application::PATH_IMAGE + L"UI/";
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_ui + L"GameMessages.png", GAME_MESS_DIV_X, GAME_MESS_DIV_Y, GAME_MESS_SIZE_X,GAME_START_MESS_SIZE_Y);
	resourcesMap_.emplace(SRC::GAME_MESS, std::move(res));
#pragma endregion 

#pragma region ステージ
	std::wstring path_stage = Application::PATH_IMAGE + L"Stage/";
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, path_stage + L"MapChips.png", MAP_CHIPS_DIV_X, MAP_CHIPS_DIV_Y, MAP_CHIPS_SIZE, MAP_CHIPS_SIZE);
	resourcesMap_.emplace(SRC::MAP_CHIPS, std::move(res));
#pragma endregion 

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
