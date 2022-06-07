// Copyright (C) 2019 Nomango

#include <kiwano/kiwano.h>

using namespace kiwano;

void Setup()
{
	// 创建舞台
	StagePtr stage = new Stage;

	// 创建精灵
	SpritePtr sprite = new Sprite("../logo/logo_text.png");

	// 修改精灵位置, 使精灵在屏幕上居中
	sprite->SetPosition(640 / 2, 480 / 2);

	// 修改精灵锚点, 使图片中心对齐屏幕中心
	sprite->SetAnchor(0.5, 0.5);

	// 修改缩放率, 图片缩小到 0.3 倍
	sprite->SetScale(0.3f, 0.3f);

	// 添加到舞台中
	stage->AddChild(sprite);

	// 进入舞台
	Director::GetInstance().EnterStage(stage);
}

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// 游戏设置
	Settings settings;
	settings.window.title = "Hello World";
	settings.window.width = 640;
	settings.window.height = 480;
	settings.bg_color = Color::Rgb(0xE5E5E5);

	// 运行
	Application::GetInstance().Run(settings, Setup);
	return 0;
}
