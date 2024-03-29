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

		// 将所有文本角色添加到场景
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

		// 设置文字填充颜色
		text->SetFillColor(Color::White);
		return text;
	}

	RefPtr<TextActor> CreateTextWithOutline()
	{
		// 设置文字描边颜色和宽度
		RefPtr<TextActor> text = new TextActor("A text with outline");
		text->SetFillColor(Color::White);
		text->SetOutlineColor(Color::BlueViolet);
		text->SetOutlineStrokeStyle(new StrokeStyle(3.0f));
		return text;
	}

	RefPtr<TextActor> CreateTextWithBoldStyle()
	{
		// 设置字体宽度
		TextStyle style;
		style.font = new Font("", 18, FontWeight::Bold);

		RefPtr<TextActor> text = new TextActor("A text with bold style", style);
		text->SetFillColor(Color::White);
		return text;
	}

	RefPtr<TextActor> CreateTextWithUnderlineAndStrikethrough()
	{
		// 设置下划线和删除线
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
		// 加粗字体和加宽轮廓
		style.font = new Font("", 18, FontWeight::Bold);

		RefPtr<TextActor> text = new TextActor("A text with custom brush", style);
		text->SetOutlineStrokeStyle(new StrokeStyle(3.0f));

		// 创建线性渐变样式
		LinearGradientStyle fill_style = LinearGradientStyle(
			Point(text->GetWidth() / 3, 0.0f),  // 线性样式起点
			Point(text->GetWidth() * 2 / 3, text->GetHeight()),  // 线性样式终点
			{ GradientStop(0.0f, Color::Yellow), GradientStop(1.0f, Color::Green) }
		);

		// 创建线性渐变画刷
		RefPtr<Brush> fill_brush = new Brush(fill_style);
		text->SetFillBrush(fill_brush);

		// 创建径向渐变样式
		RadialGradientStyle outline_style = RadialGradientStyle(
			Point(text->GetWidth() / 2, text->GetHeight() / 2),  // 径向圆心
			Vec2(),  // 径向偏移
			Vec2(text->GetWidth(), text->GetHeight()),  // 径向半径
			{ GradientStop(0.0f, Color::Red), GradientStop(1.0f, Color::Blue) }
		);

		// 创建径向渐变画刷
		RefPtr<Brush> outline_brush = new Brush(outline_style);
		text->SetOutlineBrush(outline_brush);

		return text;
	}

	RefPtr<TextActor> CreateTextWithSystemFont()
	{
		// 设置字体族
		TextStyle style;
		style.font = new Font("Times New Roman", 18);

		RefPtr<TextActor> text = new TextActor("A text with system font", style);
		text->SetFillColor(Color::White);
		return text;
	}

	RefPtr<TextActor> CreateTextWithCustomFont()
	{
		// 加载字体文件
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
		// 设置多行文本居中
		style.alignment = TextAlign::Center;

		String content = "MULTIPLE LINES TEXT\nMULTIPLE LINES TEXT LONGER\nMULTIPLE LINES TEXT";
		RefPtr<TextActor> text = new TextActor(content, style);
		text->SetFillColor(Color::White);
		return text;
	}
};
