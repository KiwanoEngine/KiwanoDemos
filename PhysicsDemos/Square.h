// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Square);
class Square
	: public ShapeActor
	, public physics::Body
{
public:
	static SquarePtr Create(physics::World* world, const Point& pos, const Size& size)
	{
		SquarePtr square = new Square;

		// 设置形状及颜色
		square->SetShape(Shape::CreateRect(Rect(0, 0, size.x, size.y)));
		square->SetFillColor(Color::Transparent);
		square->SetStrokeColor(Color::White);

		// 设置位置和锚点
		square->SetAnchor(0.5f, 0.5f);
		square->SetPosition(pos);
		square->SetSize(size);

		// 初始化物理身体
		square->InitBody(world, square);
		// 设置物理身体类型为动态
		square->SetType(physics::Body::Type::Dynamic);
		// 添加物理形状
		square->AddRectShape(square->GetSize(), 1.f);

		return square;
	}

	physics::BodyPtr GetBody()
	{
		return this;
	}
};
