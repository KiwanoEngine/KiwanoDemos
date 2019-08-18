// Copyright (C) 2019 Nomango

#include "MainStage.h"

using namespace kiwano;

class ImGuiApp
	: public Application
{
public:
	ImGuiApp()
	{
		// ��� ImGui ���
		Use(ImGuiModule::GetInstance());

		// ��ʼ��
		Options options(L"ImGui Demo", 1280, 800);
		Init(options);
	}

	void OnReady() override
	{
		MainStagePtr scene = new MainStage;
		Director::GetInstance()->EnterStage(scene);
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	try
	{
		ImGuiApp app;
		app.Run();
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
