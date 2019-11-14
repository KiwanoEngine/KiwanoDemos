// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Square);
class Square
	: public Sprite
	, public physics::Body
{
public:
	Square(physics::World* world, const Point& pos, const Size& size)
		: physics::Body(world, this)
	{
		Load(L"resources/box.png");
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);
		SetSize(size);

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
