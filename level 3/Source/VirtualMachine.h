#pragma once
#include "ui_VirtualMachine.h"
#include "input.h"
#include "Memory.h"
#include "Instruction.h"
#include <QtWidgets/QMainWindow>
#include <fstream>
#include <QMessageBox>
#include <QTimer>
using namespace std;

struct Linfo
{
	string label;
	int addr;
};

class VirtualMachine : public QMainWindow
{
	Q_OBJECT

public:
	VirtualMachine(QWidget* parent = Q_NULLPTR);
	ifstream is;
	void Parser(string path);                  // 입력 파일을 파싱
	int GetargCount(string ins);              // 명령어의 인수 개수를 반환
	void CreateFile(string path);             // 새로운 파일 생성
	void Execute(int now);                    // 현재 명령어 실행
	void update();                            // CPU 및 메모리 업데이트
	void Stat();                              // 통계 업데이트
	void PrintCPU();                          // CPU 상태 출력
	void PrintMemory();                       // 메모리 상태 출력

private slots:
	void On_Load();                           // 파일 로드
	void On_Save();                           // 파일 저장
	void On_Exit();                           // 프로그램 종료
	void On_Step();                           // 단일 명령어 실행
	void On_Run();                            // 전체 실행
	void On_Jump();                           // 특정 명령어로 점프
	void nxt();                               // 다음 명령 실행

private:
	Ui::VirtualMachineClass ui;
	bool flag = true;                         // 실행 플래그
	int check = 0, total_in = 0;              // 명령어 및 입력 상태
	int memoryCnt = 0, PC = 0;                // 메모리 상태 및 프로그램 카운터
	unordered_map<string, int> instCnt, statics;  // 통계 및 명령어 카운터
	input* read;                              // 입력 창 객체
	Memory mMemory;                           // 메모리 객체
	vector<int> mCPU;                         // 가상 CPU 상태
	vector<string> result;                    // 출력 결과
	vector<Linfo> Labels;                     // 레이블 리스트
	vector<Instruction> Instructions;         // 명령어 리스트
	QMessageBox msgbox;                       // 메시지 박스
	QTimer* executeTimer;                     // 실행 타이머

	unordered_map<string, int> opcodes =
	{
		{"nop", 0}, {"bgn", 1}, {"sym", 2}, {"lod", 3}, {"lda", 4}, {"ldc", 5}, {"str", 6}, {"ldi", 7}, {"sti", 8},
		{"not", 9}, {"neg", 10}, {"inc", 11}, {"dec", 12}, {"dup", 13}, {"add", 14}, {"sub", 15}, {"mult", 16}, {"div", 17}, {"mod", 18},
		{"gt", 19}, {"lt", 20}, {"ge", 21}, {"le", 22}, {"eq", 23}, {"ne", 24}, {"and", 25}, {"or", 26}, {"swp", 27}, {"ujp", 28},
		{"tjp", 29}, {"fjp", 30}, {"call", 31}, {"ret", 32}, {"push", 33}, {"ldp", 34}, {"proc", 35},
		{"end", 36}, {"read", 37}, {"write", 38}, {"lf", 39}
	};

	unordered_map<int, string> revopcodes =
	{
		{0, "nop"}, {1, "bgn"}, {2, "sym"}, {3, "lod"}, {4, "lda"}, {5, "ldc"}, {6, "str"}, {7, "ldi"}, {8, "sti"},
		{9, "not"}, {10, "neg"}, {11, "inc"}, {12, "dec"}, {13, "dup"}, {14, "add"}, {15, "sub"}, {16, "mult"}, {17, "div"}, {18, "mod"},
		{19, "gt"}, {20, "lt"}, {21, "ge"}, {22, "le"}, {23, "eq"}, {24, "ne"}, {25, "and"}, {26, "or"}, {27, "swp"}, {28, "ujp"},
		{29, "tjp"}, {30, "fjp"}, {31, "call"}, {32, "ret"}, {33, "push"}, {34, "ldp"}, {35, "proc"},
		{36, "end"}, {37, "read"}, {38, "write"}, {39, "lf"}
	};

	unordered_map<string, int> argcnt =
	{
		{"bgn", 1}, {"proc", 1}, {"call", 1}, {"ujp", 1}, {"tjp", 1}, {"fjp", 1}, {"ldc", 1},
		{"lod", 2}, {"lda", 2}, {"str", 2}, {"sym", 3}
	};
};
