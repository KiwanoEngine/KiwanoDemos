// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class TextDemo
	: public Stage
{
public:
	static RefPtr<Stage> Create()
	{
		return new TextDemo;
	}

	static String DemoName()
	{
		return "Text Demo";
	}

	TextDemo()
	{
		Vector<RefPtr<TextActor>> texts;

		texts.push_back(CreateSimpleText());
		texts.push_back(CreateTextWithOutline());
		texts.push_back(CreateTextWithBoldStyle());
		texts.push_back(CreateTextWithUnderlineAndStrikethrough());
		texts.push_back(CreateTextWithCustomBrush());
		texts.push_back(CreateTextWithSystemFont());
		texts.push_back(CreateTextWithCustomFont());
		texts.push_back(CreateMultipleLinesText());

		// �������ı���ɫ��ӵ�����
		float offset_y = 20.0f;
		for (auto text : texts)
		{
			text->SetAnchor(0.5f, 0.0f);
			text->SetPosition(this->GetWidth() / 2, offset_y);
			this->AddChild(text);

			offset_y += 50.0f;
		}
	}

	RefPtr<TextActor> CreateSimpleText()
	{
		RefPtr<TextActor> text = new TextActor("A simple text");

		// �������������ɫ
		text->SetFillColor(Color::White);
		return text;
	}

	RefPtr<TextActor> CreateTextWithOutline()
	{
		// �������������ɫ�Ϳ��
		RefPtr<TextActor> text = new TextActor("A text with outline");
		text->SetFillColor(Color::White);
		text->SetOutlineColor(Color::BlueViolet);
		text->SetOutlineStrokeStyle(new StrokeStyle(3.0f));
		return text;
	}

	RefPtr<TextActor> CreateTextWithBoldStyle()
	{
		// ����������
		TextStyle style;
		style.font = new Font("", 18, FontWeight::Bold);

		RefPtr<TextActor> text = new TextActor("A text with bold style", style);
		text->SetFillColor(Color::White);
		return text;
	}

	RefPtr<TextActor> CreateTextWithUnderlineAndStrikethrough()
	{
		// �����»��ߺ�ɾ����
		TextStyle style;
		style.show_underline = true;
		style.show_strikethrough = true;

		RefPtr<TextActor> text = new TextActor("A text with underline and strikethough", style);
		text->SetFillColor(Color::White);
		return text;
	}

	RefPtr<TextActor> CreateTextWithCustomBrush()
	{
		TextStyle style;
		// �Ӵ�����ͼӿ�����
		style.font = new Font("", 18, FontWeight::Bold);

		RefPtr<TextActor> text = new TextActor("A text with custom brush", style);
		text->SetOutlineStrokeStyle(new StrokeStyle(3.0f));

		// �������Խ�����ʽ
		LinearGradientStyle fill_style = LinearGradientStyle(
			Point(text->GetWidth() / 3, 0.0f),  // ������ʽ���
			Point(text->GetWidth() * 2 / 3, text->GetHeight()),  // ������ʽ�յ�
			{ GradientStop(0.0f, Color::Yellow), GradientStop(1.0f, Color::Green) }
		);

		// �������Խ��仭ˢ
		RefPtr<Brush> fill_brush = new Brush(fill_style);
		text->SetFillBrush(fill_brush);

		// �������򽥱���ʽ
		RadialGradientStyle outline_style = RadialGradientStyle(
			Point(text->GetWidth() / 2, text->GetHeight() / 2),  // ����Բ��
			Vec2(),  // ����ƫ��
			Vec2(text->GetWidth(), text->GetHeight()),  // ����뾶
			{ GradientStop(0.0f, Color::Red), GradientStop(1.0f, Color::Blue) }
		);

		// �������򽥱仭ˢ
		RefPtr<Brush> outline_brush = new Brush(outline_style);
		text->SetOutlineBrush(outline_brush);

		return text;
	}

	RefPtr<TextActor> CreateTextWithSystemFont()
	{
		// ����������
		TextStyle style;
		style.font = new Font("Times New Roman", 18);

		RefPtr<TextActor> text = new TextActor("A text with system font", style);
		text->SetFillColor(Color::White);
		return text;
	}

	RefPtr<TextActor> CreateTextWithCustomFont()
	{
		// ���������ļ�
		Font::Preload("res/fonts/Gothica-Book.ttf");

		TextStyle style;
		style.font = new Font("Gothica", 18);

		RefPtr<TextActor> text = new TextActor("A text with custom font", style);
		text->SetFillColor(Color::White);
		return text;
	}

	RefPtr<TextActor> CreateMultipleLinesText()
	{
		TextStyle style;
		// ���ö����ı�����
		style.alignment = TextAlign::Center;

		String content = "MULTIPLE LINES TEXT\nMULTIPLE LINES TEXT LONGER\nMULTIPLE LINES TEXT";
		RefPtr<TextActor> text = new TextActor(content, style);
		text->SetFillColor(Color::White);
		return text;
	}
};
