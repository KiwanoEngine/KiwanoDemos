// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Square);
class Square
	: public ShapeActor
{
public:
	Square(PhysicWorldPtr world, const Point& pos, const Size& size)
	{
		// 设置形状及颜色
		this->SetShape(Shape::CreateRect(Rect(0, 0, size.x, size.y)));
		this->SetStrokeColor(Color::White);

		// 设置位置和锚点
		this->SetAnchor(0.5f, 0.5f);
		this->SetPosition(pos);
		this->SetSize(size);

		// 创建物理身体
		PhysicBodyPtr body = new PhysicBody(world, PhysicBody::Type::Dynamic);
		// 添加物理形状
		body->AddRectShape(this->GetSize(), 1.f);
		// 将物体添加到物理世界
		this->AddComponent(body);
	}
};
