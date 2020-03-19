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

		// ������״����ɫ
		square->SetShape(Shape::CreateRect(Rect(0, 0, size.x, size.y)));
		square->SetStrokeColor(Color::White);

		// ����λ�ú�ê��
		square->SetAnchor(0.5f, 0.5f);
		square->SetPosition(pos);
		square->SetSize(size);

		// ������������
		PhysicBodyPtr body = PhysicBody::Create(world, PhysicBody::Type::Dynamic);
		// ���������״
		body->AddRectShape(square->GetSize(), 1.f);
		// ��������ӵ���������
		square->AddComponent(body);
		return square;
	}
};
