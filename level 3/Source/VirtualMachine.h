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
	void Parser(string path);                  // �Է� ������ �Ľ�
	int GetargCount(string ins);              // ��ɾ��� �μ� ������ ��ȯ
	void CreateFile(string path);             // ���ο� ���� ����
	void Execute(int now);                    // ���� ��ɾ� ����
	void update();                            // CPU �� �޸� ������Ʈ
	void Stat();                              // ��� ������Ʈ
	void PrintCPU();                          // CPU ���� ���
	void PrintMemory();                       // �޸� ���� ���

private slots:
	void On_Load();                           // ���� �ε�
	void On_Save();                           // ���� ����
	void On_Exit();                           // ���α׷� ����
	void On_Step();                           // ���� ��ɾ� ����
	void On_Run();                            // ��ü ����
	void On_Jump();                           // Ư�� ��ɾ�� ����
	void nxt();                               // ���� ��� ����

private:
	Ui::VirtualMachineClass ui;
	bool flag = true;                         // ���� �÷���
	int check = 0, total_in = 0;              // ��ɾ� �� �Է� ����
	int memoryCnt = 0, PC = 0;                // �޸� ���� �� ���α׷� ī����
	unordered_map<string, int> instCnt, statics;  // ��� �� ��ɾ� ī����
	input* read;                              // �Է� â ��ü
	Memory mMemory;                           // �޸� ��ü
	vector<int> mCPU;                         // ���� CPU ����
	vector<string> result;                    // ��� ���
	vector<Linfo> Labels;                     // ���̺� ����Ʈ
	vector<Instruction> Instructions;         // ��ɾ� ����Ʈ
	QMessageBox msgbox;                       // �޽��� �ڽ�
	QTimer* executeTimer;                     // ���� Ÿ�̸�

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
