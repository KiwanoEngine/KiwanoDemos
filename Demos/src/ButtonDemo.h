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
		FramePtr button_frame = Frame::Create("res/images/buttons.png");
		float button_width = button_frame->GetWidth();
		float button_height = button_frame->GetHeight();

		FramePtr closed = Frame::Create(button_frame->GetTexture());
		closed->SetCropRect(Rect(Point(0, button_height / 2), Point(button_width, button_height)));

		FramePtr opened = Frame::Create(button_frame->GetTexture());
		opened->SetCropRect(Rect(Point(), Point(button_width, button_height / 2)));

		// 创建点击按钮后的回调函数
		auto click = [=](Button* button)
		{
			static bool is_opened = false;

			// 强制转换为精灵按钮
			SpriteButton* self = (SpriteButton*)button;
			// 重新设置按钮的打开和关闭状态
			is_opened = !is_opened;
			self->SetFrame(is_opened ? opened : closed, false);
		};

		// 创建精灵按钮
		SpriteButtonPtr button = SpriteButton::Create(click);
		button->SetFrame(closed);
		button->SetSize(100, 50);
		button->SetAnchor(0.5f, 0.5f);
		button->SetPosition(this->GetWidth() / 2, this->GetHeight() / 2 - 50);
		this->AddChild(button);
	}

	void AddTextButton()
	{
		// 创建点击按钮后的回调函数
		auto click = [=](Button* button)
		{
			// 强制转换为文本按钮
			TextButton* self = (TextButton*)button;
			self->SetText("Clicked!");
		};

		// 创建鼠标按下按钮后的回调函数
		auto pressed = [=](Button* button)
		{
			// 强制转换为文本按钮
			TextButton* self = (TextButton*)button;
			self->SetText("Pressed");
		};

		// 创建鼠标移入按钮后的回调函数
		auto mouseover = [=](Button* button)
		{
			// 强制转换为文本按钮
			TextButton* self = (TextButton*)button;
			self->SetText("Mouseover");
		};

		// 创建鼠标移出按钮后的回调函数
		auto mouseout = [=](Button* button)
		{
			// 强制转换为文本按钮
			TextButton* self = (TextButton*)button;
			self->SetText("Mouseout");
		};

		// 创建精灵按钮
		TextButtonPtr button = TextButton::Create(click, pressed, mouseover, mouseout);
		button->SetText("Click Me");
		button->SetFillColor(Color::White);
		button->SetFontWeight(FontWeight::Bold);
		button->SetAnchor(0.5f, 0.5f);
		button->SetPosition(this->GetWidth() / 2, this->GetHeight() / 2 + 50);
		this->AddChild(button);
	}
};
