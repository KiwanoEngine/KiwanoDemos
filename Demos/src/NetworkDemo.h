// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <iostream>

class NetworkDemo
	: public Stage
{
	String text_;

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
		// 创建GUI控制面板
		ImGuiLayerPtr control_panel = new ImGuiLayer("Control", Closure(this, &NetworkDemo::ControlPanel));
		this->AddChild(control_panel);
	}

	void ControlPanel()
	{
		// imgui窗口属性
		ImGui::SetNextWindowPosCenter(ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(500, 430), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowFocus();

		// 显示imgui窗口
		auto flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;
		ImGui::Begin("Try to send HTTP requests", nullptr, flags);

		// 显示四个按钮用来发送HTTP请求
		if (ImGui::Button("GET"))
		{
			SendGetRequest();
		}
		ImGui::SameLine();

		if (ImGui::Button("POST"))
		{
			SendPostRequest();
		}
		ImGui::SameLine();

		if (ImGui::Button("PUT"))
		{
			SendPutRequest();
		}
		ImGui::SameLine();

		if (ImGui::Button("DELETE"))
		{
			SendDeleteRequest();
		}

		// 显示请求的结果
		ImGui::Text("Output:");

		char* buf = (char*)text_.c_str();
		ImGui::InputTextMultiline("Output", buf, text_.size(), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 25), ImGuiInputTextFlags_ReadOnly);

		// 清空结果内容
		if (ImGui::Button("Clear output"))
		{
			text_.clear();
		}

		// imgui窗口结束
		ImGui::End();
	}

	void SendGetRequest()
	{
		// 发送 GET 请求
		text_.append("Start to send GET request...\n");

		// 创建HTTP请求
		HttpRequestPtr request = new HttpRequest(
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
		text_.append("Start to send POST request...\n");

		// 创建 JSON 格式的 POST 数据
		Json request_data = {
			{ "string", "a simple string" },
			{ "boolean", true },
			{ "integer", 12 },
			{ "float", 3.125 },
			{ "array", { 1, 2, 3, 4, 4.5 } },
			{ "object", {{ "key", "value" }, { "key2", "value2" }} },
		};

		HttpRequestPtr request = new HttpRequest(
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
		text_.append("Start to send PUT request...\n");

		// 创建 JSON 格式的 PUT 数据
		Json request_data = Json::array({ 1, 2, 3 });

		HttpRequestPtr request = new HttpRequest(
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
		text_.append("Start to send DELETE request...\n");

		HttpRequestPtr request = new HttpRequest(
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

				text_.append("Response:\n")
					.append(result.dump(4))
					.append("\n");
			}
			catch (std::exception&)
			{
				text_.append("Parse JSON failed!\n");
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

		text_.append("Response:\n")
			.append(result.dump(4))
			.append("\n");
	}
};
