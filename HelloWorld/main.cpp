// Copyright (C) 2019 Nomango

#include <kiwano/kiwano.h>

using namespace kiwano;

class HelloWorld
	: public Stage
{
public:
	HelloWorld()
	{
		// 创建图片精灵
		SpritePtr sprite = Sprite::Create("logo.png");

		// 修改精灵位置, 使精灵在屏幕上居中
		sprite->SetPosition(640 / 2, 480 / 2);

		// 修改精灵锚点, 使图片中心对齐屏幕中心
		sprite->SetAnchor(0.5, 0.5);

		// 修改缩放率, 图片缩小到 0.5 倍
		sprite->SetScale(0.5f, 0.5f);

		// 添加到舞台中
		this->AddChild(sprite);
	}
};

void Startup()
{
	// 创建舞台
	StagePtr scene = new HelloWorld;

	// 进入舞台
	Director::GetInstance().EnterStage(scene);
}

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// 设置背景色
	Renderer::GetInstance().SetClearColor(Color(0xE5E5E5));

	// 创建窗口
	WindowPtr window = Window::Create("Hello World", 640, 480);

	// 创建运行器
	RunnerPtr runner = Runner::Create(window, Startup);

	// 运行
	Application::GetInstance().Run(runner);
	return 0;
}
