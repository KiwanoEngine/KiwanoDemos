// Copyright (C) 2019 Nomango

#include "MainStage.h"

using namespace kiwano;

void Startup()
{
	ImGuiStagePtr scene = new ImGuiStage;
	Director::GetInstance().EnterStage(scene);
}

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	try
	{
		// 添加 ImGui 模块
		Application::GetInstance().Use(ImGuiModule::GetInstance());

		// 游戏设置
		Settings s;
		s.window.title = "ImGui Demo";
		s.window.width = 800;
		s.window.height = 600;

		// 运行
		RunnerPtr runner = Runner::Create(s, Startup);
		Application::GetInstance().Run(runner);
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
