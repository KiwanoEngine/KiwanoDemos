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
		auto click = [=](Button*, Actor* target)
		{
			static bool is_opened = false;

			// 强制转换为精灵按钮
			Sprite* sprite = (Sprite*)target;
			// 重新设置按钮的打开和关闭状态
			is_opened = !is_opened;
			sprite->SetCropRect(is_opened ? opened : closed);
		};

		// 创建按钮
		ButtonPtr button = new Button(click);
		sprite->AddComponent(button);
	}

	void AddTextButton()
	{
		// 创建文字角色
		TextActorPtr text = new TextActor("Click Me");
		text->SetFillColor(Color::White);
		text->SetFontWeight(FontWeight::Bold);
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(this->GetWidth() / 2, this->GetHeight() / 2 + 50);
		this->AddChild(text);

		// 创建点击按钮后的回调函数
		auto click = [=](Button*, Actor* target)
		{
			// 强制转换为文本按钮
			TextActor* text = (TextActor*)target;
			text->SetText("Clicked!");
		};

		// 创建鼠标按下按钮后的回调函数
		auto pressed = [=](Button*, Actor* target)
		{
			// 强制转换为文本按钮
			TextActor* text = (TextActor*)target;
			text->SetText("Pressed");
		};

		// 创建鼠标移入按钮后的回调函数
		auto mouseover = [=](Button*, Actor* target)
		{
			// 强制转换为文本按钮
			TextActor* text = (TextActor*)target;
			text->SetText("Mouseover");
		};

		// 创建鼠标移出按钮后的回调函数
		auto mouseout = [=](Button*, Actor* target)
		{
			// 强制转换为文本按钮
			TextActor* text = (TextActor*)target;
			text->SetText("Mouseout");
		};

		// 创建按钮
		ButtonPtr button = new Button(click, pressed, mouseover, mouseout);
		text->AddComponent(button);
	}
};
