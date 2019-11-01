// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Square);
class Square
	: public Sprite
	, public PhysicBody
{
public:
	Square(PhysicWorld* world, const Point& pos, const Size& size)
		: PhysicBody(world, this)
	{
		Load(L"resources/box.png");
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);
		SetSize(size);

		// ����������������Ϊ��̬
		SetType(PhysicBody::Type::Dynamic);
		// ���������״
		AddBoxShape(GetSize(), 1.f);
	}

	PhysicBodyPtr GetBody()
	{
		return this;
	}
};
