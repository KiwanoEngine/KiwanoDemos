// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Square);
class Square
	: public ShapeActor
{
public:
	static SquarePtr Create(PhysicWorldPtr world, const Point& pos, const Size& size)
	{
		SquarePtr square = new Square;

		// 设置形状及颜色
		square->SetShape(Shape::CreateRect(Rect(0, 0, size.x, size.y)));
		square->SetStrokeColor(Color::White);

		// 设置位置和锚点
		square->SetAnchor(0.5f, 0.5f);
		square->SetPosition(pos);
		square->SetSize(size);

		// 创建物理身体
		PhysicBodyPtr body = PhysicBody::Create(world, PhysicBody::Type::Dynamic);
		// 添加物理形状
		body->AddRectShape(square->GetSize(), 1.f);
		// 将物体添加到物理世界
		square->AddComponent(body);
		return square;
	}
};
