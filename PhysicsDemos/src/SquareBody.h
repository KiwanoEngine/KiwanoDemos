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
		// ������״����ɫ
		this->SetShape(Shape::CreateRect(Rect(0, 0, size.x, size.y)));
		this->SetStrokeColor(Color::White);

		// ����λ�ú�ê��
		this->SetAnchor(0.5f, 0.5f);
		this->SetPosition(pos);
		this->SetSize(size);

		// ������������
		PhysicBodyPtr body = new PhysicBody(world, PhysicBody::Type::Dynamic);
		// ���������״
		body->AddRectShape(this->GetSize(), 1.f);
		// ��������ӵ���������
		this->AddComponent(body);
	}
};
