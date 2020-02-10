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


// 地面
class Ground
	: public ShapeActor
{
	physics::BodyPtr body_;

public:
	static GroundPtr Create(physics::World* world, Point const& pos)
	{
		GroundPtr ground = new Ground;

		// 设置形状颜色和位置
		ground->SetFillColor(Color::Transparent);
		ground->SetStrokeColor(Color::White);
		ground->SetPosition(pos);

		// 生成路径点
		Vector<Point> path_points = ground->GeneratePathPoints();

		// 根据路径点绘制路径
		ShapeSink sink;
		sink.BeginPath(path_points[0]);
		for (size_t i = 1; i < path_points.size(); ++i)
		{
			sink.AddLine(path_points[i]);
		}
		sink.EndPath();
		ground->SetShape(sink.GetShape());

		// 根据路径点生成物理边
		physics::BodyPtr body = physics::Body::Create(world, ground, physics::Body::Type::Static);
		for (size_t i = 1; i < path_points.size(); ++i)
		{
			body->AddEdgeShape(path_points[i - 1], path_points[i], 0.0f, 0.6f);
		}

		ground->body_ = body;
		return ground;
	}

	Vector<Point> GeneratePathPoints()
	{
		Vector<Point> path_points;	// 路径点

		path_points.push_back(Point(5, -400));
		path_points.push_back(Point(5, 0));
		path_points.push_back(Point(200, 0));

		float hs[] = { -25, -100, -300, 0, 0, 100, 200, 200, 125, 0 };
		float x = 200, dx = 500;

		for (int i = 0; i < 10; ++i)
		{
			float y = hs[i];
			path_points.push_back(Point(x + dx, y));
			x += dx;
		}

		for (int i = 0; i < 10; ++i)
		{
			float y = hs[i];
			path_points.push_back(Point(x + dx, y));
			x += dx;
		}

		path_points.push_back(Point(x, -400));

		return path_points;
	}
};


// 小车
class Car
	: public Actor
{
	ShapeActorPtr chassis_;
	physics::BodyPtr chassis_body_;

	physics::WheelJointPtr left_joint_;
	physics::WheelJointPtr right_joint_;

public:
	static CarPtr Create(physics::World* world, Point const& pos)
	{
		CarPtr car = new Car;

		// 创建小车躯干
		ShapeActorPtr chassis = ShapeActor::Create();
		chassis->SetPosition(pos + Point(0, -100));

		// 小车躯干点
		Vector<Point> vertices = { Point(-150, 50), Point(150, 50), Point(150, 0), Point(0, -90), Point(-115, -90), Point(-150, -20), };

		ShapeSink sink;
		sink.BeginPath(vertices[0]);
		sink.AddLines(vertices);
		sink.EndPath(true);

		chassis->SetShape(sink.GetShape());
		chassis->SetFillColor(Color::Transparent);
		chassis->SetStrokeColor(Color::White);

		physics::BodyPtr chassis_body = physics::Body::Create(world, chassis, physics::Body::Type::Dynamic);
		chassis_body->AddPolygonShape(vertices, 1.0f);

		// 创建左右轮子
		CirclePtr lwheel = Circle::Create(world, pos + Point(-100, -35), 40.0f);
		CirclePtr rwheel = Circle::Create(world, pos + Point(100, -40), 40.0f);

		auto lwheel_body = lwheel->GetBody();
		auto rwheel_body = rwheel->GetBody();

		// 设置摩擦力
		lwheel_body->GetFixtureList().begin()->SetFriction(0.9f);
		rwheel_body->GetFixtureList().begin()->SetFriction(0.9f);

		// 创建左轮子关节
		physics::WheelJoint::Param param1(chassis_body, lwheel_body, lwheel_body->GetBodyPosition(), Vec2(0, 1));
		param1.frequency_hz = 4.0f;				// 频率和阻尼率赋予关节弹性
		param1.damping_ratio = 0.7f;
		param1.enable_motor = true;				// 启用马达
		param1.motor_speed = 0.0f;				// 初始马达速度为零
		param1.max_motor_torque = 2000;			// 最大马达转矩

		physics::JointPtr left_joint = physics::WheelJoint::Create(world, param1);

		// 创建右轮子关节
		physics::WheelJoint::Param param2(chassis_body, rwheel_body, rwheel_body->GetBodyPosition(), Vec2(0, 1));
		param2.frequency_hz = 4.0f;
		param2.damping_ratio = 0.7f;
		param2.enable_motor = false;
		param2.motor_speed = 0.0f;
		param2.max_motor_torque = 1000;

		physics::JointPtr right_joint = physics::WheelJoint::Create(world, param2);

		car->AddChild(chassis);
		car->AddChild(lwheel);
		car->AddChild(rwheel);

		car->chassis_ = chassis;
		car->chassis_body_ = chassis_body;
		car->left_joint_ = left_joint;
		car->right_joint_ = right_joint;
		return car;
	}

	// 给小车后轮提供动力
	void SetSpeed(float speed)
	{
		left_joint_->SetMotorSpeed(speed);
	}

	// 获取身体位置
	Point GetBodyPosition() const
	{
		return chassis_->GetPosition();
	}
};


Demo2::Demo2()
{
	map_ = Actor::Create();
	{
		AddChild(map_);

		// 创建地面
		ActorPtr ground = Ground::Create(this, Point(0, GetHeight() - 250));
		map_->AddChild(ground);

		// 创建小车
		car_ = Car::Create(this, Point(190, 240));
		map_->AddChild(car_);

		// 添加文本说明
		TextActorPtr intro = TextActor::Create("按←→↓键控制小车");
		intro->SetFillColor(Color::White);
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(GetWidth() / 2, GetHeight() - 60);
		AddChild(intro);
	}
}

void Demo2::OnUpdate(Duration dt)
{
	// 获取小车位置
	Point pos = car_->GetBodyPosition();
	if (pos.x > GetWidth() / 2)
	{
		// 小车位置超过屏幕一半大小时, 移动整个地图, 让视角跟随小车
		map_->SetPositionX(GetWidth() / 2 - pos.x);
	}
	else
	{
		map_->SetPositionX(0.f);
	}

	// 按键处理
	auto& input = Input::GetInstance();
	if (input.IsDown(KeyCode::Right))
	{
		car_->SetSpeed(360.0f * 3);
	}
	else if (input.IsDown(KeyCode::Left))
	{
		car_->SetSpeed(-360.0f * 3);
	}
	else if (input.IsDown(KeyCode::Down))
	{
		car_->SetSpeed(0);
	}
}
