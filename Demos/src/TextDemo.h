// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class TextDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new TextDemo;
	}

	static String DemoName()
	{
		return "Text Demo";
	}

	TextDemo()
	{
		Vector<TextActorPtr> texts;

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

	TextActorPtr CreateSimpleText()
	{
		TextActorPtr text = new TextActor("A simple text");

		// �������������ɫ
		text->SetFillColor(Color::White);
		return text;
	}

	TextActorPtr CreateTextWithOutline()
	{
		// �������������ɫ�Ϳ��
		TextStyle style;
		style.outline_brush = new Brush(Color::BlueViolet);
		style.outline_stroke = new StrokeStyle(3.0f);

		TextActorPtr text = new TextActor("A text with outline", style);
		text->SetFillColor(Color::White);
		return text;
	}

	TextActorPtr CreateTextWithBoldStyle()
	{
		// ����������
		TextStyle style;
		style.font = new Font("", 18, FontWeight::Bold);

		TextActorPtr text = new TextActor("A text with bold style", style);
		text->SetFillColor(Color::White);
		return text;
	}

	TextActorPtr CreateTextWithUnderlineAndStrikethrough()
	{
		// �����»��ߺ�ɾ����
		TextStyle style;
		style.show_underline = true;
		style.show_strikethrough = true;

		TextActorPtr text = new TextActor("A text with underline and strikethough", style);
		text->SetFillColor(Color::White);
		return text;
	}

	TextActorPtr CreateTextWithCustomBrush()
	{
		TextStyle style;
		// �Ӵ�����ͼӿ�����
		style.font = new Font("", 18, FontWeight::Bold);
		style.outline_stroke = new StrokeStyle(3.0f);

		TextActorPtr text = new TextActor("A text with custom brush", style);

		// �������Խ�����ʽ
		LinearGradientStyle fill_style = LinearGradientStyle(
			Point(text->GetWidth() / 3, 0.0f),  // ������ʽ���
			Point(text->GetWidth() * 2 / 3, text->GetHeight()),  // ������ʽ�յ�
			{ GradientStop(0.0f, Color::Yellow), GradientStop(1.0f, Color::Green) }
		);

		// �������Խ��仭ˢ
		BrushPtr fill_brush = new Brush(fill_style);
		text->SetFillBrush(fill_brush);

		// �������򽥱���ʽ
		RadialGradientStyle outline_style = RadialGradientStyle(
			Point(text->GetWidth() / 2, text->GetHeight() / 2),  // ����Բ��
			Vec2(),  // ����ƫ��
			Vec2(text->GetWidth(), text->GetHeight()),  // ����뾶
			{ GradientStop(0.0f, Color::Red), GradientStop(1.0f, Color::Blue) }
		);

		// �������򽥱仭ˢ
		BrushPtr outline_brush = new Brush(outline_style);
		text->SetOutlineBrush(outline_brush);

		return text;
	}

	TextActorPtr CreateTextWithSystemFont()
	{
		// ����������
		TextStyle style;
		style.font = new Font("Times New Roman", 18);

		TextActorPtr text = new TextActor("A text with system font", style);
		text->SetFillColor(Color::White);
		return text;
	}

	TextActorPtr CreateTextWithCustomFont()
	{
		// ���������ļ�
		Font::Preload("res/fonts/Gothica-Book.ttf");

		TextStyle style;
		style.font = new Font("Gothica", 18);

		TextActorPtr text = new TextActor("A text with custom font", style);
		text->SetFillColor(Color::White);
		return text;
	}

	TextActorPtr CreateMultipleLinesText()
	{
		TextStyle style;
		// ���ö����ı�����
		style.alignment = TextAlign::Center;

		String content = "MULTIPLE LINES TEXT\nMULTIPLE LINES TEXT LONGER\nMULTIPLE LINES TEXT";
		TextActorPtr text = new TextActor(content, style);
		text->SetFillColor(Color::White);
		return text;
	}
};
