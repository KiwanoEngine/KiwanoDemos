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
		square->SetFillColor(Color::Transparent);
		square->SetStrokeColor(Color::White);

		// 设置位置和锚点
		square->SetAnchor(0.5f, 0.5f);
		square->SetPosition(pos);
		square->SetSize(size);

		// 创建物理身体
		square->body_ = PhysicBody::Create(square, PhysicBody::Type::Dynamic);
		// 添加物理形状
		square->body_->AddRectShape(square->GetSize(), 1.f);
		// 将物体添加到物理世界
		world->AddBody(square->body_);
		return square;
	}

	PhysicBodyPtr GetBody()
	{
		return body_;
	}

private:
	PhysicBodyPtr body_;
};
