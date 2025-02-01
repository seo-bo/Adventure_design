#pragma once
#include <string>
using namespace std;

class Instruction
{
public:
	string label; // ��ɾ��� ���̺�
	string inst; // ��ɾ� �̸�
	string arg1, arg2, arg3; // �μ���
	Instruction(string_view a, string_view b, string_view p1, string_view p2, string_view p3)
		: label(a), inst(b), arg1(p1), arg2(p2), arg3(p3) {}
};
