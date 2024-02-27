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
		// 使用形状生成器绘制圆形
		ShapeMaker maker;
		// 从圆心开始形状路径
		maker.BeginPath(Point(radius, radius));
		// 添加半径线
		maker.AddLine(Point(radius, 0));
		// 添加圆形的两个圆弧
		maker.AddArc(Point(radius, 2 * radius), Size(radius, radius), 0.0f);
		maker.AddArc(Point(radius, 0), Size(radius, radius), 0.0f);
		// 结束形状路径
		maker.EndPath();

		// 设置形状及颜色
		this->SetShape(maker.GetShape());
		this->SetStrokeColor(Color::White);

		// 设置位置和锚点
		this->SetAnchor(0.5f, 0.5f);
		this->SetPosition(pos);
		this->SetSize(Size(radius * 2, radius * 2));

		// 创建物理身体
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
