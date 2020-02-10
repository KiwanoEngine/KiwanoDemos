// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Circle);
class Circle
	: public ShapeActor
{
	physics::BodyPtr body_;

public:
	static CirclePtr Create(physics::World* world, const Point& pos, float radius)
	{
		CirclePtr circle = new Circle;

		// 使用形状生成器绘制圆形
		ShapeSink sink;
		// 从圆心开始形状路径
		sink.BeginPath(Point(radius, radius));
		// 添加半径线
		sink.AddLine(Point(radius, 0));
		// 添加圆形的两个圆弧
		sink.AddArc(Point(radius, 2 * radius), Size(radius, radius), 0.0f);
		sink.AddArc(Point(radius, 0), Size(radius, radius), 0.0f);
		// 结束形状路径
		sink.EndPath();

		// 设置形状及颜色
		circle->SetShape(sink.GetShape());
		circle->SetFillColor(Color::Transparent);
		circle->SetStrokeColor(Color::White);

		// 设置位置和锚点
		circle->SetAnchor(0.5f, 0.5f);
		circle->SetPosition(pos);
		circle->SetSize(Size(radius * 2, radius * 2));

		// 创建物理身体
		physics::BodyPtr body = physics::Body::Create(world, circle, physics::Body::Type::Dynamic);
		// 添加物理形状
		body->AddCircleShape(radius, 1.f);

		circle->body_ = body;
		return circle;
	}

	physics::BodyPtr GetBody()
	{
		return body_;
	}
};
