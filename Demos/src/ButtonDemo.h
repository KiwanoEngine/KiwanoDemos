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
		auto click = [=](Button*, Actor* target)
		{
			static bool is_opened = false;

			// ǿ��ת��Ϊ���鰴ť
			Sprite* sprite = (Sprite*)target;
			// �������ð�ť�Ĵ򿪺͹ر�״̬
			is_opened = !is_opened;
			sprite->SetCropRect(is_opened ? opened : closed);
		};

		// ������ť
		ButtonPtr button = new Button(click);
		sprite->AddComponent(button);
	}

	void AddTextButton()
	{
		// �������ֽ�ɫ
		TextActorPtr text = new TextActor("Click Me");
		text->SetFillColor(Color::White);
		text->SetFontWeight(FontWeight::Bold);
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(this->GetWidth() / 2, this->GetHeight() / 2 + 50);
		this->AddChild(text);

		// ���������ť��Ļص�����
		auto click = [=](Button*, Actor* target)
		{
			// ǿ��ת��Ϊ�ı���ť
			TextActor* text = (TextActor*)target;
			text->SetText("Clicked!");
		};

		// ������갴�°�ť��Ļص�����
		auto pressed = [=](Button*, Actor* target)
		{
			// ǿ��ת��Ϊ�ı���ť
			TextActor* text = (TextActor*)target;
			text->SetText("Pressed");
		};

		// ����������밴ť��Ļص�����
		auto mouseover = [=](Button*, Actor* target)
		{
			// ǿ��ת��Ϊ�ı���ť
			TextActor* text = (TextActor*)target;
			text->SetText("Mouseover");
		};

		// ��������Ƴ���ť��Ļص�����
		auto mouseout = [=](Button*, Actor* target)
		{
			// ǿ��ת��Ϊ�ı���ť
			TextActor* text = (TextActor*)target;
			text->SetText("Mouseout");
		};

		// ������ť
		ButtonPtr button = new Button(click, pressed, mouseover, mouseout);
		text->AddComponent(button);
	}
};
