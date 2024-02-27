// Copyright (C) 2019 Nomango

#include "MainStage.h"

using namespace kiwano;

class MyRunner : public Runner
{
public:
	MyRunner()
	{
		// ��Ϸ����
		Settings settings;
		settings.window.title = "ImGui Demo";
		settings.window.width = 800;
		settings.window.height = 600;

		SetSettings(settings);

		// ��� ImGui ģ��
		Application::GetInstance().Use(imgui::Module::GetInstance());
	}

	void OnReady() override
	{
		RefPtr<ImGuiStage> scene = new ImGuiStage;
		Director::GetInstance().EnterStage(scene);
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	try
	{
		// ����������
		RefPtr<Runner> runner = new MyRunner();

		// ����
		Application::GetInstance().Run(runner);
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
