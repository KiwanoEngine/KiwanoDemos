// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Circle);
class Circle
	: public ShapeActor
{
public:
	static CirclePtr Create(PhysicWorldPtr world, const Point& pos, float radius)
	{
		CirclePtr circle = new Circle;

		// ʹ����״����������Բ��
		ShapeMakerPtr maker = ShapeMaker::Create();
		// ��Բ�Ŀ�ʼ��״·��
		maker->BeginPath(Point(radius, radius));
		// ��Ӱ뾶��
		maker->AddLine(Point(radius, 0));
		// ���Բ�ε�����Բ��
		maker->AddArc(Point(radius, 2 * radius), Size(radius, radius), 0.0f);
		maker->AddArc(Point(radius, 0), Size(radius, radius), 0.0f);
		// ������״·��
		maker->EndPath();

		// ������״����ɫ
		circle->SetShape(maker->GetShape());
		circle->SetStrokeColor(Color::White);

		// ����λ�ú�ê��
		circle->SetAnchor(0.5f, 0.5f);
		circle->SetPosition(pos);
		circle->SetSize(Size(radius * 2, radius * 2));

		// ������������
		PhysicBodyPtr body = PhysicBody::Create(world, PhysicBody::Type::Dynamic);
		// ���������״
		body->AddCircleShape(radius, 1.f);
		// ��������ӵ���������
		circle->AddComponent(body);
		return circle;
	}
};
