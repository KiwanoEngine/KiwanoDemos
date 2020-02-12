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

	TextActorPtr CreateSimpleText()
	{
		TextActorPtr text = TextActor::Create("A simple text");

		// 设置文字填充颜色
		text->SetFillColor(Color::White);
		return text;
	}

	TextActorPtr CreateTextWithOutline()
	{
		TextActorPtr text = TextActor::Create("A text with outline");
		text->SetFillColor(Color::White);

		// 设置文字描边颜色和宽度
		text->SetOutlineColor(Color::BlueViolet);
		text->SetOutlineWidth(2.0f);
		return text;
	}

	TextActorPtr CreateTextWithBoldStyle()
	{
		TextActorPtr text = TextActor::Create("A text with bold style");
		text->SetFillColor(Color::White);

		// 设置字体宽度
		text->SetFontWeight(FontWeight::Bold);
		return text;
	}

	TextActorPtr CreateTextWithUnderlineAndStrikethrough()
	{
		TextActorPtr text = TextActor::Create("A text with underline and strikethough");
		text->SetFillColor(Color::White);

		// 设置下划线和删除线
		text->SetUnderline(true);
		text->SetStrikethrough(true);
		return text;
	}

	TextActorPtr CreateTextWithCustomBrush()
	{
		TextActorPtr text = TextActor::Create("A text with custom brush");

		// 加粗字体和加宽轮廓
		text->SetFontWeight(FontWeight::Bold);
		text->SetOutlineWidth(2.0f);

		// 手动更新文本布局
		text->UpdateLayout();

		// 创建线性渐变样式
		LinearGradientStyle fill_style = LinearGradientStyle(
			Point(text->GetWidth() / 3, 0.0f),  // 线性样式起点
			Point(text->GetWidth() * 2 / 3, text->GetHeight()),  // 线性样式终点
			{ GradientStop(0.0f, Color::Yellow), GradientStop(1.0f, Color::Green) }
		);

		// 创建线性渐变画刷
		BrushPtr fill_brush = Brush::Create(fill_style);
		text->SetFillBrush(fill_brush);

		// 创建径向渐变样式
		RadialGradientStyle outline_style = RadialGradientStyle(
			Point(text->GetWidth() / 2, text->GetHeight() / 2),  // 径向圆心
			Vec2(),  // 径向偏移
			Vec2(text->GetWidth(), text->GetHeight()),  // 径向半径
			{ GradientStop(0.0f, Color::Red), GradientStop(1.0f, Color::Blue) }
		);

		// 创建径向渐变画刷
		BrushPtr outline_brush = Brush::Create(outline_style);
		text->SetOutlineBrush(outline_brush);

		return text;
	}

	TextActorPtr CreateTextWithSystemFont()
	{
		TextActorPtr text = TextActor::Create("A text with system font");
		text->SetFillColor(Color::White);

		// 设置字体族
		text->SetFontFamily("Times New Roman");
		return text;
	}

	TextActorPtr CreateTextWithCustomFont()
	{
		TextActorPtr text = TextActor::Create("A text with custom font");
		text->SetFillColor(Color::White);

		// 加载字体文件
		FontPtr font = Font::Create("res/fonts/Gothica-Book.ttf");
		text->SetFont(font);
		text->SetFontFamily("Gothica");
		return text;
	}

	TextActorPtr CreateMultipleLinesText()
	{
		TextActorPtr text = TextActor::Create("MULTIPLE LINES TEXT\nMULTIPLE LINES TEXT LONGER\nMULTIPLE LINES TEXT");
		text->SetFillColor(Color::White);

		// 设置行间距
		text->SetLineSpacing(0.0f);
		// 设置多行文本居中
		text->SetAlignment(TextAlign::Center);
		return text;
	}
};
