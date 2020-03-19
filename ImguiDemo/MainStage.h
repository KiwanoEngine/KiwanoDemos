// Copyright (C) 2019 Nomango

#pragma once

#include <kiwano/kiwano.h>
#include <kiwano-imgui/kiwano-imgui.h>

using namespace kiwano;
using namespace kiwano::imgui;

KGE_DECLARE_SMART_PTR(ImGuiStage);
class ImGuiStage
	: public Stage
{
	SpritePtr logo;

public:
	ImGuiStage()
	{
		logo = Sprite::Create("logo.png");
		logo->SetSize(160, 160);
		logo->SetAnchor(0.5f, 0.5f);
		logo->SetPosition(GetWidth() / 2, GetHeight() / 2);
		AddChild(logo);

		// ���� ImGui ͼ��
		ImGuiLayerPtr layer = ImGuiLayer::Create("ControlPanel", Closure(this, &ImGuiStage::ControlPanel));
		AddChild(layer);
	}

	void ControlPanel()
	{
		static bool visibility = true;
		static float rotation = 0.0f;
		static Color clear_color = Color(0xE5E5E5);

		ImGui::Begin("Control Panel");

		ImGui::Text("Use the following widgets to control actor behaviors");

		ImGui::Checkbox("visibility", &visibility);

		if (ImGui::ArrowButton("left", ImGuiDir_Left))
		{
			logo->Move(-3.0f, 0.0f);
		}
		ImGui::SameLine();

		if (ImGui::ArrowButton("up", ImGuiDir_Up))
		{
			logo->Move(0.0f, -3.0f);
		}
		ImGui::SameLine();

		if (ImGui::ArrowButton("down", ImGuiDir_Down))
		{
			logo->Move(0.0f, 3.0f);
		}
		ImGui::SameLine();

		if (ImGui::ArrowButton("right", ImGuiDir_Right))
		{
			logo->Move(3.0f, 0.0f);
		}

		ImGui::SliderFloat("rotation", &rotation, 0.0f, 360.0f);

		ImGui::ColorEdit3("clear color", (float*)&clear_color);

		if (ImGui::Button("reset"))
		{
			visibility = true;
			rotation = 0.0f;

			logo->SetPosition(GetWidth() / 2, GetHeight() / 2);
		}

		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();

		// �޸Ŀɼ���
		logo->SetVisible(visibility);

		// �޸���ת�Ƕ�
		logo->SetRotation(rotation);

		// �޸Ĵ��ڱ���ɫ
		Renderer::GetInstance().SetClearColor(clear_color);
	}
};
