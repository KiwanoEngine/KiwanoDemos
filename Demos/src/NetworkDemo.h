// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <iostream>

class NetworkDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new NetworkDemo;
	}

	static String DemoName()
	{
		return "Network Demo";
	}

	NetworkDemo()
	{
		// 添加按键监听
		AddListener<KeyDownEvent>(Closure(this, &NetworkDemo::OnKeyDown));

		// 创建说明文字
		TextActorPtr intro = TextActor::Create("按G发送GET请求\n按P发送POST请求\n按U发送PUT请求\n按D发送DELETE请求");
		// 设置文字位置
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		intro->SetFillColor(Color::White);

		// 添加到舞台
		this->AddChild(intro);
	}

	void OnEnter() override
	{
		// 进入舞台时打开控制台
		Logger::GetInstance().ShowConsole(true);
	}

	void OnExit() override
	{
		// 退出舞台时关闭控制台
		Logger::GetInstance().ShowConsole(false);
	}

	void OnKeyDown(Event* evt)
	{
		KGE_ASSERT(evt->IsType<KeyDownEvent>());

		// 按不同键发送不同请求
		auto key_evt = dynamic_cast<KeyDownEvent*>(evt);
		if (key_evt->code == KeyCode::G)
		{
			SendGetRequest();
		}
		else if (key_evt->code == KeyCode::P)
		{
			SendPostRequest();
		}
		else if (key_evt->code == KeyCode::U)
		{
			SendPutRequest();
		}
		else if (key_evt->code == KeyCode::D)
		{
			SendDeleteRequest();
		}
	}

	void SendGetRequest()
	{
		// 发送 GET 请求
		KGE_LOG("Start to send GET request...");

		// 创建HTTP请求
		HttpRequestPtr request = HttpRequest::Create(
			"http://httpbin.org/get",
			HttpType::Get,
			Closure(this, &NetworkDemo::Complete)
		);

		// 发送 HTTP 请求
		HttpModule::GetInstance().Send(request);
	}

	void SendPostRequest()
	{
		// 发送 POST 请求
		KGE_LOG("Start to send POST request...");

		// 创建 JSON 格式的 POST 数据
		Json request_data = {
			{ "string", "a simple string" },
			{ "boolean", true },
			{ "integer", 12 },
			{ "float", 3.125 },
			{ "array", { 1, 2, 3, 4, 4.5 } },
			{ "object", { "key", "value" } },
		};

		HttpRequestPtr request = HttpRequest::Create(
			"http://httpbin.org/post",
			HttpType::Post,
			request_data,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void SendPutRequest()
	{
		// 发送 PUT 请求
		KGE_LOG("Start to send PUT request...");

		// 创建 JSON 格式的 PUT 数据
		Json request_data = Json::array({ 1, 2, 3 });

		HttpRequestPtr request = HttpRequest::Create(
			"http://httpbin.org/put",
			HttpType::Put,
			request_data,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void SendDeleteRequest()
	{
		// 发送 DELETE 请求
		KGE_LOG("Start to send DELETE request...");

		HttpRequestPtr request = HttpRequest::Create(
			"http://httpbin.org/delete",
			HttpType::Delete,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void Complete(HttpRequestPtr request, HttpResponsePtr response)
	{
		// 判断请求是否成功
		if (response->IsSucceed())
		{
			try
			{
				// 将获取到的数据解析成 JSON 格式
				Json response_data = Json::parse(response->GetData());
				Json result = {
					{"HttpCode", response->GetResponseCode()},
					{"Data", response_data},
				};

				KGE_LOG("Response:\n", result.dump(4));
			}
			catch (std::exception&)
			{
				KGE_ERROR("Parse JSON failed!");
			}
		}
		else
		{
			// 请求失败时打印错误信息
			OutputError(response);
		}
	}

	void OutputError(HttpResponsePtr response)
	{
		// 打印 HTTP 响应结果的状态码和错误信息
		Json result = {
				{"HttpCode", response->GetResponseCode()},
				{"Error", response->GetError()},
		};

		String result_str = result.dump(4);
		KGE_ERROR("Response: %s\n", result_str.c_str());
	}
};
