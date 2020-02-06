// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Circle);
class Circle
	: public ShapeActor
{
	physics::BodyPtr body_;

public:
	Circle(physics::World* world, const Point& pos, float radius)
	{
		// ʹ����״����������Բ��
		ShapeSink sink;
		// ��Բ�Ŀ�ʼ��״·��
		sink.BeginPath(Point(radius, radius));
		// ��Ӱ뾶��
		sink.AddLine(Point(radius, 0));
		// ���Բ�ε�����Բ��
		sink.AddArc(Point(radius, 2 * radius), Size(radius, radius), 0.0f);
		sink.AddArc(Point(radius, 0), Size(radius, radius), 0.0f);
		// ������״·��
		sink.EndPath();

		// ������״����ɫ
		SetShape(sink.GetShape());
		SetFillColor(Color::Transparent);
		SetStrokeColor(Color::White);

		// ����λ�ú�ê��
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
