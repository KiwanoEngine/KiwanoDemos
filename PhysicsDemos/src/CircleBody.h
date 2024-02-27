// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Circle
	: public ShapeActor
{
public:
	RefPtr<physics::Body> body;

	Circle(RefPtr<physics::World> world, const Point& pos, float radius)
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
		b2BodyDef def;
		def.type = b2_dynamicBody;
		this->body = world->AddBody(&def);
		this->AddComponent(body);

		b2CircleShape shape;
		shape.m_radius = physics::LocalToWorld(radius);

		b2FixtureDef fd;
		fd.density = 1.0f;
		fd.friction = 0.2f;
		fd.shape = &shape;
		body->GetB2Body()->CreateFixture(&fd);
	}
};
