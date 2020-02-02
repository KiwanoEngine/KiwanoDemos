// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Circle);
class Circle
	: public Sprite
{
	physics::BodyPtr body_;

public:
	Circle(physics::World* world, const Point& pos, float radius)
	{
		Load(L"resources/ball.png");
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);
		SetSize(Size(radius * 2, radius * 2));

		// ������������
		body_ = new physics::Body;
		body_->InitBody(world, this);
		// ����������������Ϊ��̬
		body_->SetType(physics::Body::Type::Dynamic);
		// ���������״
		body_->AddCircleShape(radius, 1.f);
	}

	physics::BodyPtr GetBody()
	{
		return body_;
	}
};
