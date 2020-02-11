// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// 角色
KGE_DECLARE_SMART_PTR(Hero);
class Hero
	: public GifSprite
{
public:
	static HeroPtr Create()
	{
		HeroPtr hero = new Hero;

		// 从资源缓存中获取GIF图像
		GifImagePtr img = GifImage::Create("res/Kusanagi.gif");

		// 加载图片
		if (hero->Load(img))
		{
			// 设置 GIF 动图无限循环
			hero->SetLoopCount(-1);
			return hero;
		}
		return nullptr;
	}

	// 每帧渲染前执行 OnUpdate
	void OnUpdate(Duration dt) override
	{
		// 获取输入设备
		auto& input = Input::GetInstance();

		// 按下左右键
		if (input.IsDown(KeyCode::Left))
		{
			this->Move(-2, 0);
		}
		else if (input.IsDown(KeyCode::Right))
		{
			this->Move(2, 0);
		}

		// 按下上下键
		if (input.IsDown(KeyCode::Up))
		{
			this->Move(0, -2);
		}
		else if (input.IsDown(KeyCode::Down))
		{
			this->Move(0, 2);
		}

		// 按下鼠标左键，顺时针旋转角色
		if (input.IsDown(MouseButton::Left))
		{
			// 获取当前旋转角度
			float rotation = this->GetRotation();
			// 设置角度值+2
			this->SetRotation(rotation + 2);
		}

		// 点击鼠标右键，隐藏或显示角色
		if (input.WasPressed(MouseButton::Right))
		{
			// 获取当前显示状态
			bool visible = this->IsVisible();
			// 设置相反的显示状态
			this->SetVisible(!visible);
		}
	}
};

class InputDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new InputDemo;
	}

	static String DemoName()
	{
		return "Input Demo";
	}

	InputDemo()
	{
		// 创建角色
		HeroPtr hero = Hero::Create();
		// 在屏幕上居中显示
		hero->SetAnchor(0.5f, 0.5f);
		hero->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// 创建说明文字
		TextActorPtr intro = TextActor::Create("按上下左右键移动\n按鼠标左键旋转\n点击鼠标右键隐藏");
		// 设置文字位置
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
		intro->SetFillColor(Color::White);
		intro->SetAlignment(TextAlign::Center);

		// 添加到舞台
		this->AddChild(hero);
		this->AddChild(intro);
	}
};
