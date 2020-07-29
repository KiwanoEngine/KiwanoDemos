// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// 角色
KGE_DECLARE_SMART_PTR(Hero);
class Hero
	: public GifSprite
{
public:
	Hero()
	{
		// 加载图片
		if (this->Load("res/images/Kusanagi.gif"))
		{
			// 设置 GIF 动图无限循环
			this->SetLoopCount(-1);
		}
	}

	// 每帧渲染前执行 OnUpdate
	void OnUpdate(Duration dt) override
	{
		// 运动速度（像素/秒）
		static const float speed_linear_per_sec = 100;
		// 旋转速度（度/秒）
		static const float speed_angular_per_sec = 60;

		// 实际速度 = 速度 x 时间
		float speed_linear = speed_linear_per_sec * dt.Seconds();
		float speed_angular = speed_angular_per_sec * dt.Seconds();

		// 获取输入设备
		auto& input = Input::GetInstance();

		// 按下左右键
		if (input.IsDown(KeyCode::Left))
		{
			this->MoveBy(-speed_linear, 0);
		}
		else if (input.IsDown(KeyCode::Right))
		{
			this->MoveBy(speed_linear, 0);
		}

		// 按下上下键
		if (input.IsDown(KeyCode::Up))
		{
			this->MoveBy(0, -speed_linear);
		}
		else if (input.IsDown(KeyCode::Down))
		{
			this->MoveBy(0, speed_linear);
		}

		// 按下鼠标左键，顺时针旋转角色
		if (input.IsDown(MouseButton::Left))
		{
			// 获取当前旋转角度
			float rotation = this->GetRotation();
			// 设置角度值+2
			this->SetRotation(rotation + speed_angular);
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
		HeroPtr hero = new Hero();
		// 在屏幕上居中显示
		hero->SetAnchor(0.5f, 0.5f);
		hero->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// 创建说明文字
		TextActorPtr intro = new TextActor("按上下左右键移动\n按鼠标左键旋转\n点击鼠标右键隐藏");
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
