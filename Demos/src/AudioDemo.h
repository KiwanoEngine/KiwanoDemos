// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <kiwano-audio/kiwano-audio.h>

class AudioDemo
	: public Stage
{
	RefPtr<audio::SoundPlayer> sound_player;	// ���ֲ�����
	RefPtr<TextActor> volume_text;				// ��������
	RefPtr<TextActor> state_text;				// ����״̬����
	bool is_playing = true;

public:
	static RefPtr<Stage> Create()
	{
		return new AudioDemo;
	}

	static String DemoName()
	{
		return "Audio Demo";
	}

	AudioDemo()
	{
		// ��������
		sound_player = new audio::SoundPlayer;

		// Ԥ������Ƶ�����Է�ֹ����ʱ����
		sound_player->Preload("res/sounds/splash.mp3");

		// �������֣������������ò���ѭ��������-1 ��ʾѭ�����ţ�
		sound_player->Play("res/sounds/splash.mp3", -1);

		// ����˵������
		RefPtr<TextActor> intro = new TextActor("�����¼���������\n���ո����ͣ�����");
		intro->SetFillColor(Color::White);
		intro->SetAlignment(TextAlign::Center);
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50);

		// ������������
		volume_text = new TextActor("��ǰ������");
		volume_text->SetFillColor(Color::White);
		volume_text->SetAnchor(0.5f, 0.5f);
		volume_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 30);

		// ����״̬����
		state_text = new TextActor("��ǰ״̬��");
		state_text->SetFillColor(Color::White);
		state_text->SetAnchor(0.5f, 0.5f);
		state_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 60);

		// ��ӵ���̨
		this->AddChild(intro);
		this->AddChild(volume_text);
		this->AddChild(state_text);
	}

	void OnUpdate(Duration dt) override
	{
		if (sound_player == nullptr)
			return;

		// ��ȡ�����Ͳ���״̬
		float volume = sound_player->GetVolume();

		// �޸��ı�
		volume_text->SetText("��ǰ������" + std::to_string(volume));
		state_text->SetText(is_playing ? "��ǰ״̬�����ڲ���" : "��ǰ״̬��ֹͣ����");

		// ��ȡ�����豸
		auto& input = Input::GetInstance();

		// ���ո����ͣ�����
		if (input.WasPressed(KeyCode::Space))
		{
			is_playing ? sound_player->PauseAll() : sound_player->ResumeAll();
			is_playing = !is_playing;
		}

		// �����¼���������
		if (input.WasPressed(KeyCode::Up))
		{
			sound_player->SetVolume(volume + 0.1f);
		}
		else if (input.WasPressed(KeyCode::Down))
		{
			sound_player->SetVolume(volume - 0.1f);
		}
	}
};
