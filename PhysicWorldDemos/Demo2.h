// Copyright (C) 2019 Nomango

#pragma once
#include "Circle.h"

KGE_DECLARE_SMART_PTR(Demo2);
KGE_DECLARE_SMART_PTR(Car);
KGE_DECLARE_SMART_PTR(Ground);

class Demo2
	: public physics::World
{
	ActorPtr map_;
	CarPtr car_;

public:
	Demo2();

	void OnUpdate(Duration dt) override;

	static inline StagePtr Create()
	{
		return new Demo2;
	}
};


// ����
class Ground
	: public PathActor
{
	physics::BodyPtr ground_;
	Vector<Point> path_points_;	// ·����

public:
	Ground(physics::World* world, Point const& pos)
	{
		// ����
		SetFillColor(Color::Transparent);
		SetStrokeColor(Color::White);
		SetPosition(pos);

		// ����·����
		GeneratePathPoints();

		// ����·�������·��
		BeginPath(path_points_[0]);
		for (size_t i = 1; i < path_points_.size(); ++i)
		{
			AddLine(path_points_[i]);
		}
		EndPath(false);

		// ����·�������������
		ground_ = new physics::Body(world, this);
		{
			physics::EdgeShape shape;
			for (size_t i = 1; i < path_points_.size(); ++i)
			{
				shape.Set(path_points_[i - 1], path_points_[i]);
				ground_->AddFixture(&shape, physics::Fixture::Param(0.f, 0.6f));
			}
		}
	}

	void GeneratePathPoints()
	{
		path_points_.push_back(Point(5, -400));
		path_points_.push_back(Point(5, 0));
		path_points_.push_back(Point(200, 0));

		float hs[] = { -25, -100, -300, 0, 0, 100, 200, 200, 125, 0 };
		float x = 200, dx = 500;

		for (int i = 0; i < 10; ++i)
		{
			float y = hs[i];
			path_points_.push_back(Point(x + dx, y));
			x += dx;
		}

		for (int i = 0; i < 10; ++i)
		{
			float y = hs[i];
			path_points_.push_back(Point(x + dx, y));
			x += dx;
		}

		path_points_.push_back(Point(x, -400));
	}
};


// С��
class Car
	: public Actor
{
	PathActorPtr chassis_;
	physics::BodyPtr chassis_body_;

	physics::WheelJointPtr joint1_;
	physics::WheelJointPtr joint2_;

public:
	Car(physics::World* world, Point const& pos)
	{
		// ����С������
		chassis_ = new PathActor;
		{
			chassis_->SetPosition(pos + Point(0, -100));
			AddChild(chassis_);

			// С�����ɵ�
			Vector<Point> vertices = { Point(-150, 50), Point(150, 50), Point(150, 0), Point(0, -90), Point(-115, -90), Point(-150, -20), };
			chassis_->BeginPath(vertices[0]);
			chassis_->AddLines(vertices);
			chassis_->EndPath(true);

			chassis_body_ = new physics::Body(world, chassis_);
			chassis_body_->SetType(physics::Body::Type::Dynamic);
			chassis_body_->AddPolygonShape(vertices, 1.0f);
		}

		// ������������
		CirclePtr lwheel = new Circle(world, pos + Point(-100, -35), 40.0f);
		CirclePtr rwheel = new Circle(world, pos + Point(100, -40), 40.0f);
		{
			AddChild(lwheel);
			AddChild(rwheel);

			auto lwheel_body = lwheel->GetBody();
			auto rwheel_body = rwheel->GetBody();

			// ����Ħ����
			lwheel_body->GetFixtureList().SetFriction(0.9f);
			rwheel_body->GetFixtureList().SetFriction(0.9f);

			// ���������ӹؽ�
			physics::WheelJoint::Param param1(chassis_body_, lwheel_body, lwheel_body->GetBodyPosition(), Vec2(0, 1));
			param1.frequency_hz = 4.0f;				// Ƶ�ʺ������ʸ���ؽڵ���
			param1.damping_ratio = 0.7f;
			param1.enable_motor = true;				// �������
			param1.motor_speed = 0.0f;				// ��ʼ����ٶ�Ϊ��
			param1.max_motor_torque = 2000;			// ������ת��
			joint1_ = new physics::WheelJoint(world, param1);

			// ���������ӹؽ�
			physics::WheelJoint::Param param2(chassis_body_, rwheel_body, rwheel_body->GetBodyPosition(), Vec2(0, 1));
			param2.frequency_hz = 4.0f;
			param2.damping_ratio = 0.7f;
			param2.enable_motor = false;
			param2.motor_speed = 0.0f;
			param2.max_motor_torque = 1000;
			joint2_ = new physics::WheelJoint(world, param2);
		}
	}

	// ��С�������ṩ����
	void SetSpeed(float speed)
	{
		joint1_->SetMotorSpeed(speed);
	}

	// ��ȡ����λ��
	Point GetBodyPosition() const
	{
		return chassis_->GetPosition();
	}
};


Demo2::Demo2()
{
	map_ = new Actor;
	{
		AddChild(map_);

		// ��������
		ActorPtr ground = new Ground(this, Point(0, GetHeight() - 250));
		map_->AddChild(ground);

		// ����С��
		car_ = new Car(this, Point(190, 240));
		map_->AddChild(car_);

		// ����ı�˵��
		TextPtr intro = new Text(L"��������������С��");
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(GetWidth() / 2, GetHeight() - 60);
		AddChild(intro);
	}
}

void Demo2::OnUpdate(Duration dt)
{
	// ��ȡС��λ��
	Point pos = car_->GetBodyPosition();
	if (pos.x > GetWidth() / 2)
	{
		// С��λ�ó�����Ļһ���Сʱ, �ƶ�������ͼ, ���ӽǸ���С��
		map_->SetPositionX(GetWidth() / 2 - pos.x);
	}
	else
	{
		map_->SetPositionX(0.f);
	}

	// ��������
	auto input = Input::GetInstance();
	if (input->IsDown(KeyCode::Right))
	{
		car_->SetSpeed(360.0f * 3);
	}
	else if (input->IsDown(KeyCode::Left))
	{
		car_->SetSpeed(-360.0f * 3);
	}
	else if (input->IsDown(KeyCode::Down))
	{
		car_->SetSpeed(0);
	}
}
