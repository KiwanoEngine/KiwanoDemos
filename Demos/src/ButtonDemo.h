// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class ButtonDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new ButtonDemo;
	}

	static String DemoName()
	{
		return "Button Demo";
	}

	ButtonDemo()
	{
		// 添加一个精灵按钮
		AddSpriteButton();

		// 添加一个文字按钮
		AddTextButton();
	}

	void AddSpriteButton()
	{
		// 加载按钮图片
		FramePtr frame = new Frame("res/images/buttons.png");
		// 按钮关闭和打开时的裁剪矩形
		Rect closed = Rect(Point(0, frame->GetHeight() / 2), Point(frame->GetWidth(), frame->GetHeight()));
		Rect opened = Rect(Point(), Point(frame->GetWidth(), frame->GetHeight() / 2));

		// 创建精灵
		SpritePtr sprite = new Sprite(frame);
		sprite->SetSize(100, 50);
		sprite->SetAnchor(0.5f, 0.5f);
		sprite->SetPosition(this->GetWidth() / 2, this->GetHeight() / 2 - 50);
		sprite->SetCropRect(closed);
		this->AddChild(sprite);

		// 创建点击按钮后的回调函数
		auto cb = [=](Button* btn)
		{
			static bool is_opened = false;

			// 强制转换为精灵按钮
			Sprite* sprite = (Sprite*)btn->GetBoundActor();
			// 重新设置按钮的打开和关闭状态
			is_opened = !is_opened;
			sprite->SetCropRect(is_opened ? opened : closed);
		};

		// 创建按钮
		ButtonPtr button = new Button;
		button->SetCallbackOnClicked(cb);
		sprite->AddComponent(button);
	}

	void AddTextButton()
	{
		// 创建文字角色
		TextActorPtr text = new TextActor("Click Me");
		text->SetFillColor(Color::White);
		text->SetFont(new Font("", 18, FontWeight::Bold));
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(this->GetWidth() / 2, this->GetHeight() / 2 + 50);
		this->AddChild(text);

		// 创建按钮的回调函数
		auto cb = [=](Button* btn, Button::Event evt)
		{
			// 强制转换为文本按钮
			TextActor* text = (TextActor*)btn->GetBoundActor();

			switch (evt)
			{
			case Button::Event::MouseEntered:
				text->SetText("MouseEntered");
				break;
			case Button::Event::MouseExited:
				text->SetText("MouseExited");
				break;
			case Button::Event::Pressed:
				text->SetText("Pressed");
				break;
			case Button::Event::Released:
				text->SetText("Released");
				break;
			case Button::Event::Clicked:
				text->SetText("Clicked!");
				break;
			}
		};

		// 创建按钮
		ButtonPtr button = new Button(cb);
		text->AddComponent(button);
	}
};
