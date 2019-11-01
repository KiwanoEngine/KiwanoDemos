// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Circle);
class Circle
	: public Sprite
{
	PhysicBodyPtr body_;

public:
	Circle(PhysicWorld* world, const Point& pos, float radius)
	{
		Load(L"resources/ball.png");
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);
		SetSize(Size(radius * 2, radius * 2));

		// ������������
		body_ = new PhysicBody(world, this);
		// ����������������Ϊ��̬
		body_->SetType(PhysicBody::Type::Dynamic);
		// ���������״
		body_->AddCircleShape(radius, 1.f);
	}

	PhysicBodyPtr GetBody()
	{
		return body_;
	}
};
