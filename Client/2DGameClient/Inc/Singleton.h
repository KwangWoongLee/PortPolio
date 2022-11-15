#pragma once
#include "stdafx.h"

// Singleton Ŭ���� ����
// volatile int dummy{}; �� ���Խ��� ����ȭ ����
// Singleton Ŭ������ ��� �޾� ���α׷� ���� �� �ϳ� ��ü�� ����/���
//
// Example:
//
// ���� ��
// class Test : public Singleton< Test >
// {
// }
//
//
// ��� �� 
// Test::GetInstance();

template <typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T mInstance;
		volatile int dummy{};

		return mInstance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;
};
