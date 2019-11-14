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

		// ����������������Ϊ��̬
		SetType(physics::Body::Type::Dynamic);
		// ���������״
		AddBoxShape(GetSize(), 1.f);
	}

	physics::BodyPtr GetBody()
	{
		return this;
	}
};
