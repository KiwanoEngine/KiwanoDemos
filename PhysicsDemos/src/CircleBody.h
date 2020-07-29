// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Circle);
class Circle
	: public ShapeActor
{
public:
	Circle(PhysicWorldPtr world, const Point& pos, float radius)
	{
		// ʹ����״����������Բ��
		ShapeMaker maker;
		// ��Բ�Ŀ�ʼ��״·��
		maker.BeginPath(Point(radius, radius));
		// ��Ӱ뾶��
		maker.AddLine(Point(radius, 0));
		// ���Բ�ε�����Բ��
		maker.AddArc(Point(radius, 2 * radius), Size(radius, radius), 0.0f);
		maker.AddArc(Point(radius, 0), Size(radius, radius), 0.0f);
		// ������״·��
		maker.EndPath();

		// ������״����ɫ
		this->SetShape(maker.GetShape());
		this->SetStrokeColor(Color::White);

		// ����λ�ú�ê��
		this->SetAnchor(0.5f, 0.5f);
		this->SetPosition(pos);
		this->SetSize(Size(radius * 2, radius * 2));

		// ������������
		PhysicBodyPtr body = new PhysicBody(world, PhysicBody::Type::Dynamic);
		// ���������״
		body->AddCircleShape(radius, 1.f);
		// ��������ӵ���������
		this->AddComponent(body);
	}
};
