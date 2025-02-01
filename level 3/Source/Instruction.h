#pragma once
#include <string>
using namespace std;

class Instruction
{
public:
	string label; // 명령어의 레이블
	string inst; // 명령어 이름
	string arg1, arg2, arg3; // 인수들
	Instruction(string_view a, string_view b, string_view p1, string_view p2, string_view p3)
		: label(a), inst(b), arg1(p1), arg2(p2), arg3(p3) {}
};
