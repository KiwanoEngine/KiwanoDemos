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
		SpritePtr sprite = new Sprite;

		// 加载图片
		sprite->Load(L"../logo/logo_text.png");

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

class HelloWorldApp
	: public Application
{
public:
	void OnReady() override
	{
		// 创建舞台
		StagePtr scene = new HelloWorld;

		// 进入舞台
		Director::Instance().EnterStage(scene);
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// 创建窗口
	Window::Instance().Create(L"Hello World", 640, 480);

	// 设置背景色
	Renderer::Instance().SetClearColor(Color(0xE5E5E5));

	// 创建程序实例
	HelloWorldApp app;

	// 运行
	app.Run();
	return 0;
}
