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
		// ���һ�����鰴ť
		AddSpriteButton();

		// ���һ�����ְ�ť
		AddTextButton();
	}

	void AddSpriteButton()
	{
		// ���ذ�ťͼƬ
		FramePtr frame = new Frame("res/images/buttons.png");
		// ��ť�رպʹ�ʱ�Ĳü�����
		Rect closed = Rect(Point(0, frame->GetHeight() / 2), Point(frame->GetWidth(), frame->GetHeight()));
		Rect opened = Rect(Point(), Point(frame->GetWidth(), frame->GetHeight() / 2));

		// ��������
		SpritePtr sprite = new Sprite(frame);
		sprite->SetSize(100, 50);
		sprite->SetAnchor(0.5f, 0.5f);
		sprite->SetPosition(this->GetWidth() / 2, this->GetHeight() / 2 - 50);
		sprite->SetCropRect(closed);
		this->AddChild(sprite);

		// ���������ť��Ļص�����
		auto cb = [=](Button* btn)
		{
			static bool is_opened = false;

			// ǿ��ת��Ϊ���鰴ť
			Sprite* sprite = (Sprite*)btn->GetBoundActor();
			// �������ð�ť�Ĵ򿪺͹ر�״̬
			is_opened = !is_opened;
			sprite->SetCropRect(is_opened ? opened : closed);
		};

		// ������ť
		ButtonPtr button = new Button;
		button->SetCallbackOnClicked(cb);
		sprite->AddComponent(button);
	}

	void AddTextButton()
	{
		// �������ֽ�ɫ
		TextActorPtr text = new TextActor("Click Me");
		text->SetFillColor(Color::White);
		text->SetFont(new Font("", 18, FontWeight::Bold));
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(this->GetWidth() / 2, this->GetHeight() / 2 + 50);
		this->AddChild(text);

		// ������ť�Ļص�����
		auto cb = [=](Button* btn, Button::Event evt)
		{
			// ǿ��ת��Ϊ�ı���ť
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

		// ������ť
		ButtonPtr button = new Button(cb);
		text->AddComponent(button);
	}
};
