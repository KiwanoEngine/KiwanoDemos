// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Square);
class Square
	: public ShapeActor
	, public physics::Body
{
public:
	Square(physics::World* world, const Point& pos, const Size& size)
	{
		// 设置形状及颜色
		SetShape(Shape::CreateRect(Rect(0, 0, size.x, size.y)));
		SetFillColor(Color::Transparent);
		SetStrokeColor(Color::White);

		// 设置位置和锚点
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);
		SetSize(size);

		// 初始化物理身体
		InitBody(world, this);
		// 设置物理身体类型为动态
		SetType(physics::Body::Type::Dynamic);
		// 添加物理形状
		AddBoxShape(GetSize(), 1.f);
	}

	physics::BodyPtr GetBody()
	{
		return this;
	}
};
