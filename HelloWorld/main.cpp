// Copyright (C) 2019 Nomango

#include "kiwano/kiwano.h"

using namespace kiwano;

class MainScene
	: public Scene
{
public:
	MainScene()
	{
		String imageFile = L"../logo/logo_text_h.png";	// 图片路径
		SpritePtr image = new Sprite(imageFile);		// 创建图片精灵
		image->SetPosition(640 / 2, 480 / 2);			// 修改精灵位置, 使精灵在屏幕上居中
		image->SetScale(0.4f, 0.4f);					// 图片缩小到 0.4 倍
		image->SetAnchor(0.5, 0.5);						// 修改精灵锚点, 使图片中心对齐屏幕中心
		this->AddChild(image);							// 添加到场景中
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// 创建 Kiwano 程序实例
	Application app;

	// 在初始化选项中设置窗口宽高
	Options options;
	options.width = 640;
	options.height = 480;

	// 设置背景色
	options.clear_color = Color(0xE5E5E5);

	// 初始化 app
	app.Init(options);

	// 创建场景并进入
	ScenePtr scene = new MainScene;
	app.EnterScene(scene);

	// 运行
	app.Run();
	return 0;
}
