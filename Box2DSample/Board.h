// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// 固定的木板
KGE_DECLARE_SMART_PTR(Board);
class Board
	: public RectActor
{
public:
	Board(b2World* world, const Size& size, const Point& pos)
	{
		// 设置线条和填充颜色
		Color color = 0x868686;
		SetStrokeColor(color);
		SetFillColor(color);

		// 设置木板的大小、位置和旋转角度
		SetRectSize(size);
		SetAnchor(0.5f, 0.5f);
		SetRotation(10);
		SetPosition(pos);

		// 创建木板的物理形状
		b2PolygonShape groundBox;
		b2Vec2 sz = Stage2World(size / 2);
		groundBox.SetAsBox(sz.x, sz.y);

		// 创建木板在物理世界的位置角度信息
		b2BodyDef groundBodyDef;
		groundBodyDef.position = Stage2World(GetPosition());
		groundBodyDef.angle = Angle2Radian(10);

		// 创建木板的物理世界实体
		b2Body* groundBody = world->CreateBody(&groundBodyDef);
		groundBody->CreateFixture(&groundBox, 0.0f);
	}
};
