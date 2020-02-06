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
		// ������״����ɫ
		SetShape(Shape::CreateRect(Rect(0, 0, size.x, size.y)));
		SetFillColor(Color::Transparent);
		SetStrokeColor(Color::White);

		// ����λ�ú�ê��
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);
		SetSize(size);

		// ��ʼ����������
		InitBody(world, this);
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
