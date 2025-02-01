#include "VirtualMachine.h"
#include "input.h"
#include <fstream>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <cctype>
#include <iomanip>
using namespace std;

VirtualMachine::VirtualMachine(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	read = new input();
	ui.tableWidget->setColumnWidth(0, 80);
	ui.tableWidget->setColumnWidth(1, 100);
	ui.tableWidget->setColumnWidth(2, 80);
	ui.tableWidget->setColumnWidth(3, 80);
	ui.tableWidget->setColumnWidth(4, 80);
	ui.tableWidget->setColumnWidth(5, 80);
	ui.tableWidget->setColumnWidth(6, 75);
	ui.tableWidget->setColumnWidth(7, 80);
	ui.tableWidget->setColumnWidth(8, 120);
	ui.tableWidget_2->setColumnWidth(0, 120);
	ui.tableWidget_2->setColumnWidth(1, 120);
	ui.CPUStackTable->setColumnWidth(0, 150);
	ui.MemoryTable->setColumnWidth(0, 75);
	ui.MemoryTable->setColumnWidth(1, 75);
	ui.MemoryTable->setColumnWidth(2, 80);
	connect(ui.RunButton, &QPushButton::clicked, this, &VirtualMachine::On_Run);
	connect(ui.LoadButton, &QPushButton::clicked, this, &VirtualMachine::On_Load);
	connect(ui.SaveButton, &QPushButton::clicked, this, &VirtualMachine::On_Save);
	connect(ui.ExitButton, &QPushButton::clicked, this, &VirtualMachine::On_Exit);
	executeTimer = new QTimer(this);
	connect(executeTimer, &QTimer::timeout, this, &VirtualMachine::nxt);
}

void VirtualMachine::On_Load()
{
	QString file = QFileDialog::getOpenFileName(this, "Search File", QDir::currentPath(), "Files(*.uco)");
	if (file.isEmpty())
	{
		QMessageBox::warning(this, "File Open Error", "No file selected or file path is invalid.");
		return;
	}
	try
	{
		Parser(file.toStdString());
		update();
		result.clear();
	}
	catch (const exception& e)
	{
		QMessageBox::critical(this, "File Read Error", QString("An error occurred while reading the file: %1").arg(e.what()));
	}
	catch (...)
	{
		QMessageBox::critical(this, "File Read Error", "An unknown error occurred while reading the file.");
	}
}

void VirtualMachine::On_Save()
{
	CreateFile(QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath(), "Files(*.lst)").toStdString());
	Stat();
}

void VirtualMachine::On_Exit()
{
	this->close();
}

void VirtualMachine::On_Jump()
{
	if (!check)
	{
		msgbox.setText("Load *.uco file");
		msgbox.exec();
		return;
	}
	if (flag)
	{
		if (Instructions[PC].label != "")
		{
			Execute(PC);
			PC++;
		}
		else
		{
			while (PC < Instructions.size() && (Instructions[PC].label == "" || Instructions[PC].label[0] == '$'))
			{
				Execute(PC);
				if (!flag)
				{
					return;
				}
				PC++;
			}
			if (PC < Instructions.size())
			{
				Execute(PC);
				PC++;
			}
		}
		ui.tableWidget->selectRow(PC - 1);
		VirtualMachine::PrintCPU();
		VirtualMachine::PrintMemory();
	}
	else
	{
		ui.tableWidget->selectRow(PC);
		return;
	}
}

void VirtualMachine::On_Step()
{
	if (!check)
	{
		msgbox.setText("Load *.uco file first");
		msgbox.exec();
		return;
	}
	if (flag)
	{
		Execute(PC);
		PC++;
		ui.tableWidget->selectRow(PC - 1);
		VirtualMachine::PrintCPU();
		VirtualMachine::PrintMemory();
	}
	else
	{
		ui.tableWidget->selectRow(PC - 1);
		return;
	}
}

void VirtualMachine::On_Run()
{
	if (!check)
	{
		msgbox.setText("Load *.uco file");
		msgbox.exec();
		return;
	}
	if (flag)
	{
		executeTimer->start(500);
	}
	else
	{
		ui.tableWidget->selectRow(PC - 1);
	}
}

void VirtualMachine::Parser(string path)
{
	ifstream is(path, ifstream::in | ifstream::binary);
	string cur = "", lines = "";
	int lcnt = 0;
	if (is.is_open())
	{
		while (getline(is, cur))
		{
			lines += cur;
			if (cur.empty()) // 빈 줄 처리
			{
				lcnt++;
				continue;
			}
			istringstream stream(cur);
			string label = "", inst = "";
			vector<string> args(3, "");
			if (cur[0] == ' ')  // 레이블이 없는 명령어
			{
				stream >> inst;
			}
			else // 레이블과 명령어 분리
			{
				stream >> label >> inst;
			}
			int len = GetargCount(inst); // 명령어 인수 개수 가져오기
			for (int i = 0; i < len; i++)
			{
				if (!(stream >> args[i]))
				{
					args[i] = "-1";
					break;
				}
			}
			Instruction instruction(label, inst, args[0], args[1], args[2]);
			if (inst == "bgn") // 프로그램 시작 위치 저장
			{
				PC = lcnt;
				instCnt["bgn"]++;
				total_in++;
			}
			Instructions.push_back(instruction); // 명령어 추가
			if (!label.empty()) // 레이블이 있으면 저장
			{ 
				Linfo tlabel;
				tlabel.addr = lcnt + 1;
				tlabel.label = label;
				Labels.push_back(tlabel);
			}
			statics[inst]++; // 명령어 통계 업데이트
			lcnt++;
		}
		check++;
	}
	is.close();
}

int VirtualMachine::GetargCount(string ins)
{
	return argcnt[ins];
}

void VirtualMachine::update()
{
	for (auto& [a, b, c, d, e] : Instructions)
	{
		ui.tableWidget->insertRow(ui.tableWidget->rowCount());
		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(a)));
		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(b)));
		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::fromStdString(c)));
		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::fromStdString(d)));
		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::fromStdString(e)));
	}
	for (auto& [a, b] : Labels)
	{
		ui.tableWidget_2->insertRow(ui.tableWidget_2->rowCount());
		ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(a)));
		ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount() - 1, 1, new QTableWidgetItem(QString::number(b)));
	}
}

void VirtualMachine::Execute(int now)
{
	int inst = 0, temp = 0;
	auto it = opcodes.find(Instructions[now].inst); // 현재 명령어를 opcode로 매핑
	if (it == opcodes.end())
	{
		flag = false;
		return;
	}
	inst = it->second;
	switch (inst)
	{
	case 0: // nop: 아무 작업도 하지 않음
	{ 
		instCnt["nop"]++;
		total_in++;
		break;
	}
	case 1: // bgn: 메모리 블록 시작
	{
		mMemory.PlusM(stoi(Instructions[now].arg1), "bgn");
		instCnt["bgn"]++;
		total_in++;
		break;
	}
	case 2: // sym: 심볼 등록
	{
		instCnt["sym"]++;
		total_in++;
		break;
	}

	case 3: // lod: 메모리 값 로드
	{
		int num = mMemory.GetMnum(stoi(Instructions[now].arg1), stoi(Instructions[now].arg2));
		mCPU.push_back(num);
		instCnt["lod"]++;
		total_in++;
		memoryCnt++;
		break;
	}

	case 4: // lda: 메모리 주소 로드
	{
		int num = mMemory.GetMadr(stoi(Instructions[now].arg1), stoi(Instructions[now].arg2));
		mCPU.push_back(num);
		instCnt["lda"]++;
		total_in++;
		memoryCnt++;
		break;
	}
	case 5: // ldc: 상수를 CPU에 푸시
	{
		mCPU.push_back(stoi(Instructions[now].arg1));
		instCnt["ldc"]++;
		total_in++;
		memoryCnt++;
		break;
	}
	case 6: // str: 값을 메모리에 저장
	{
		if (!mCPU.empty())
		{
			int num = mCPU.back();
			mCPU.pop_back();
			mMemory.Msetter(num, stoi(Instructions[now].arg1), stoi(Instructions[now].arg2));
			instCnt["str"]++;
			total_in++;
			memoryCnt++;
		}
		break;
	}
	case 7: // ldi: 주소를 참조하여 값을 CPU에 푸시
	{
		if (!mCPU.empty())
		{
			int num = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(mMemory.GetMnum(num));
			instCnt["ldi"]++;
			total_in++;
			memoryCnt++;
		}
		break;
	}

	case 8: // sti: 주소에 값을 저장
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int addr = mCPU.back();
			mCPU.pop_back();
			mMemory.Msetter(num, addr);
			instCnt["sti"]++;
			total_in++;
			memoryCnt++;
		}
		break;
	}
	case 9: // not: 값을 비트 반전
	{
		if (!mCPU.empty())
		{
			int num = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(~num);
			instCnt["not"]++;
			total_in++;
		}
		break;
	}
	case 10: // neg: 값을 음수로 변환
	{
		if (!mCPU.empty())
		{
			int num = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(-1 * num);
			instCnt["neg"]++;
			total_in++;
		}
		break;
	}
	case 11: // inc: 값을 증가
	{
		if (!mCPU.empty())
		{
			int num = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(++num);
			instCnt["inc"]++;
			total_in++;
		}
		break;
	}
	case 12:  // dec: 값을 감소
	{
		if (!mCPU.empty())
		{
			int num = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(--num);
			instCnt["dec"]++;
			total_in++;
		}
		break;
	}
	case 13: // dup: 값을 복제
	{
		if (!mCPU.empty())
		{
			int num = mCPU.back();
			mCPU.push_back(num);
			instCnt["dup"]++;
			total_in++;
		}
		break;
	}
	case 14: // add: 두 값을 더함
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp + num);
			instCnt["add"]++;
			total_in++;
		}
		break;
	}
	case 15: // sub: 두 값을 뺌
	{ 
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp - num);
			instCnt["sub"]++;
			total_in++;
		}
		break;
	}
	case 16: // mult: 두 값을 곱함
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp * num);
			instCnt["mult"]++;
			total_in++;
		}
		break;
	}
	case 17: // div: 두 값을 나눔 (0으로 나눌 경우 에러)
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			if (num)
			{
				mCPU.push_back(cmp / num);
			}
			else
			{
				QMessageBox::critical(this, "Runtime Error", "Division by zero.");
				flag = false;
			}
			instCnt["div"]++;
			total_in++;
		}
		break;
	}
	case 18: // mod: 두 값을 나머지 연산 (0으로 나눌 경우 에러)
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			if (num)
			{
				mCPU.push_back(cmp % num);
			}
			else
			{
				QMessageBox::critical(this, "Runtime Error", "Modulo by zero.");
				flag = false;
			}
			instCnt["mod"]++;
			total_in++;
		}
		break;
	}
	case 19: // gt: 두 값 비교, 크면 true
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp > num);
			instCnt["gt"]++;
			total_in++;
		}
		break;
	}
	case 20: // lt: 두 값 비교, 작으면 true
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp < num);
			instCnt["lt"]++;
			total_in++;
		}
		break;
	}
	case 21: // ge: 두 값 비교, 크거나 같으면 true
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp >= num);
			instCnt["ge"]++;
			total_in++;
		}
		break;
	}
	case 22: // le: 두 값 비교, 작거나 같으면 true
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp <= num);
			instCnt["le"]++;
			total_in++;
		}
		break;
	}
	case 23: // eq: 두 값 비교, 같으면 true
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp == num);
			instCnt["eq"]++;
			total_in++;
		}
		break;
	}
	case 24: // ne: 두 값 비교, 다르면 true
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp != num);
			instCnt["ne"]++;
			total_in++;
		}
		break;
	}
	case 25: // and: 두 값을 비트 AND
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp & num);
			instCnt["and"]++;
			total_in++;
		}
		break;
	}
	case 26: // or: 두 값을 비트 OR
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(cmp | num);
			instCnt["or"]++;
			total_in++;
		}
		break;
	}
	case 27: // swp: 두 값의 순서 변경
	{
		if (mCPU.size() > 1)
		{
			int num = mCPU.back();
			mCPU.pop_back();
			int cmp = mCPU.back();
			mCPU.pop_back();
			mCPU.push_back(num);
			mCPU.push_back(cmp);
			instCnt["swp"]++;
			total_in++;
		}
		break;
	}
	case 28: // ujp: 레이블로 무조건 점프
	{
		for (auto& i : Labels)
		{
			if (i.label == Instructions[now].arg1)
			{
				PC = i.addr - 1;
				break;
			}
		}
		instCnt["ujp"]++;
		total_in++;
		break;
	}
	case 29: // tjp: 조건이 참이면 레이블로 점프
	{
		if (!mCPU.empty())
		{
			if (mCPU.back())
			{
				for (auto& i : Labels)
				{
					if (i.label == Instructions[now].arg1)
					{
						PC = i.addr - 1;
						mCPU.pop_back();
						break;
					}
				}
			}
			else
			{
				mCPU.pop_back();
			}
		}
		instCnt["tjp"]++;
		total_in++;
		break;
	}
	case 30: // fjp: 조건이 거짓이면 레이블로 점프
	{
		if (!mCPU.empty())
		{
			if (!mCPU.back())
			{
				for (auto& i : Labels)
				{
					if (i.label == Instructions[now].arg1)
					{
						PC = i.addr - 1;
						mCPU.pop_back();
						break;
					}
				}
			}
			else
			{
				mCPU.pop_back();
			}
		}
		instCnt["fjp"]++;
		total_in++;

		break;
	}
	case 31: // call: 함수 호출 (read/write/lf 포함)
	{
		instCnt["call"]++;
		total_in++;
		string pivot = Instructions[now].arg1;
		if (pivot == "write")
		{
			if (mMemory.GetSP() + 1 < mMemory.GetMstack().size()) 
			{
				int addr = mMemory.GetSP() + 1;
				int value = mMemory.GetMnum(addr);
				result.push_back(to_string(value));
				result.push_back("\t");
			}
			mMemory.SetSP(mMemory.GetSP() - 2);
			mMemory.Refit(mMemory.GetSP() + 1);
			instCnt["write"]++;
			total_in++;
			break;
		}
		if (pivot == "read")
		{
			ui.tableWidget->selectRow(PC);
			read->setModal(true);
			int Dcode = read->exec();
			if (Dcode == QDialog::Accepted)
			{
				int addr = mMemory.GetSP() + 1;
				if (addr < mMemory.GetMstack().size())
				{
					int num = mMemory.GetMnum(addr);
					mMemory.Msetter(read->readnum(), num);
					mMemory.SetSP(mMemory.GetSP() - 2);
					mMemory.Refit(mMemory.GetSP() + 1);
				}
			}
			instCnt["read"]++;
			total_in++;
			break;
		}
		if (pivot == "lf")
		{
			result.push_back("\n");
			mMemory.SetSP(mMemory.GetSP() - 2);
			mMemory.Refit(mMemory.GetSP() + 1);
			instCnt["lf"]++;
			total_in++;
			break;
		}
		else
		{
			for (auto & i : Labels)
			{
				if (i.label == Instructions[now].arg1) 
				{
					PC = i.addr - 2;
					break;
				}
			}
		}
		break;
	}
	case 32: // ret: 함수 반환
	{
		int returnAddr = mMemory.GetMnum(mMemory.GetBP() + 1);
		PC = returnAddr;
		mMemory.DelM();
		instCnt["ret"]++;
		total_in++;
		break;
	}
	case 33: // push: 값을 메모리에 푸시
	{
		if (!mCPU.empty()) {
			int num = mCPU.back();
			mCPU.pop_back();
			mMemory.Msetter(num);
			instCnt["push"]++;
			total_in++;
			memoryCnt++;
		}
		break;
	}
	case 34: // ldp: 메모리 스택에 로컬 변수 준비
	{
		mMemory.PlusM(2, "ldp");
		temp = PC;
		for (;; temp++)
		{
			if (temp >= Instructions.size()) {
				flag = false;
				break;
			}
			if ((Instructions[temp].inst == "call"))
			{
				break;
			}
		}
		if (temp < Instructions.size())
		{
			mMemory.Msetter(temp, mMemory.GetSP());
		}
		instCnt["ldp"]++;
		total_in++;
		break;
	}
	case 35: // proc: 프로시저 시작
	{
		int variableSize = stoi(Instructions[now].arg1);
		mMemory.PlusM(variableSize, string("proc"));
		instCnt["proc"]++;
		total_in++;
		break;
	}
	case 36: // end: 프로그램 종료
	{
		flag = false;
		instCnt["end"]++;
		total_in++;
		break;
	}
	case 37: // read: 입력값을 읽음
	{
		ui.tableWidget->selectRow(PC);
		read->setModal(true);
		int Dcode = read->exec();
		if (Dcode == QDialog::Accepted)
		{
			int addr = mMemory.GetSP() + 1;
			if (addr < mMemory.GetMstack().size())
			{
				int num = mMemory.GetMnum(addr);
				mMemory.Msetter(read->readnum(), num);
				mMemory.SetSP(mMemory.GetSP() - 2);
				mMemory.Refit(mMemory.GetSP() + 1);
			}
		}
		instCnt["read"]++;
		total_in++;
		break;
	}
	case 38:  // write: 값을 출력
	{
		if (mMemory.GetSP() + 1 < mMemory.GetMstack().size())
		{
			int addr = mMemory.GetSP() + 1;
			int  num = mMemory.GetMnum(addr);
			result.push_back(to_string(num));
			result.push_back("\t");
		}
		mMemory.SetSP(mMemory.GetSP() - 2);
		mMemory.Refit(mMemory.GetSP() + 1);
		instCnt["write"]++;
		total_in++;
		break;
	}
	case 39: // lf: 줄바꿈 처리
	{
		result.push_back("\n");
		mMemory.SetSP(mMemory.GetSP() - 2);
		mMemory.Refit(mMemory.GetSP() + 1);
		instCnt["lf"]++;
		total_in++;
		break;
	}
	default:  // 이외 처리
	{
		QMessageBox::critical(this, "Runtime Error", "Unknown instruction encountered.");
		flag = false;
		break;
	}
	}
}

void VirtualMachine::PrintMemory()
{
	vector<int> temp = mMemory.GetMstack();
	ui.MemoryTable->setRowCount(0);
	for (int i = 0; i < temp.size(); i++)
	{
		ui.MemoryTable->insertRow(ui.MemoryTable->rowCount());
		if (i < mMemory.GetDataSize())
		{
			ui.MemoryTable->setItem(ui.MemoryTable->rowCount() - 1, 1, new QTableWidgetItem("1"));
		}
		else
		{
			ui.MemoryTable->setItem(ui.MemoryTable->rowCount() - 1, 1, new QTableWidgetItem("2"));
		}
		ui.MemoryTable->setItem(ui.MemoryTable->rowCount() - 1, 0, new QTableWidgetItem(QString::number(i)));
		ui.MemoryTable->setItem(ui.MemoryTable->rowCount() - 1, 2, new QTableWidgetItem(QString::number(temp[i])));
	}
}

void VirtualMachine::PrintCPU()
{
	vector<int> tmp = mCPU;
	vector<int> vec;
	while (!tmp.empty())
	{
		vec.push_back(tmp.back());
		tmp.pop_back();
	}
	ui.CPUStackTable->setRowCount(vec.size());
	for (int i = 0; i < vec.size(); i++)
	{
		ui.CPUStackTable->setItem(i, 0, new QTableWidgetItem(QString::number(vec[vec.size() - 1 - i])));
	}
}

void VirtualMachine::Stat()
{
	ui.textBrowser->append(QString::fromLocal8Bit("****    Result    ****"));
	ui.textBrowser->append(QString::fromStdString("Mstack Usage: " + to_string(memoryCnt)));
	ui.textBrowser->append(QString::fromStdString("Output:"));
	for (const auto& res : result)
	{
		ui.textBrowser->append(QString::fromStdString(res));
	}
	ui.textBrowser->append(QString::fromLocal8Bit("**********************"));
	ui.textBrowser->append("\n");
	ui.textBrowser->append(QString::fromLocal8Bit("****  Static Instruction Counts ****\n"));
	QString Scount;
	int pnt = 0;
	for (auto& i : opcodes)
	{
		Scount += QString::fromStdString(i.first + " = " + to_string(statics[i.first])) + "\t";
		if ((pnt++ + 1) % 4 == 0)
		{
			Scount += "\n";
		}
	}
	ui.textBrowser->append(Scount);
	ui.textBrowser->append("\n");
	ui.textBrowser->append(QString::fromLocal8Bit("**** Dynamic Instruction Counts ****\n"));
	QString Dcount;
	pnt = 0;
	for (auto& i : opcodes)
	{
		Dcount += QString::fromStdString(i.first + " = " + to_string(instCnt[i.first])) + "\t";
		if ((pnt++ + 1) % 4 == 0)
		{
			Dcount += "\n";
		}
	}
	ui.textBrowser->append(Dcount);
	ui.textBrowser->append("\n");
	QString totalExec = QString::fromLocal8Bit("Total Instruction Execution: ") + QString::number(total_in);
	ui.textBrowser->append(totalExec);
}

void VirtualMachine::CreateFile(string path)
{
	ofstream os(path, ofstream::out);
	if (!os.is_open())
	{
		QMessageBox::critical(this, "File error", "Can not open file");
		return;
	}
	os << left << setw(6) << "addr"
		<< left << setw(10) << "object"
		<< left << setw(8) << "ucode"
		<< left << setw(25) << "source program" << "\n";
	os << string(55, '-') << "\n";
	int addr = 0;
	for (auto& i : Instructions)
	{
		int object = opcodes[i.inst];
		string ucode = (object != -1) ? revopcodes[object] : "unknown";
		string sourceProgram = "";
		if (!i.label.empty())
		{
			sourceProgram += i.label + " ";
		}
		sourceProgram += i.inst;
		if (!i.arg1.empty())
		{
			sourceProgram += " " + i.arg1;
		}
		if (!i.arg2.empty())
		{
			sourceProgram += " " + i.arg2;
		}
		if (!i.arg3.empty())
		{
			sourceProgram += " " + i.arg3;
		}
		os << left << setw(6) << addr
			<< left << setw(10) << ((object != -1) ? to_string(object) : "")
			<< left << setw(8) << ucode
			<< left << setw(25) << sourceProgram
			<< "\n";
		addr += 2;
	}
	os << "\n";
	os << "****    Result    ****\n";
	os << "Mstack Usage: " << memoryCnt << "\n";
	os << "Output:\n";
	for (auto& i : result)
	{
		os << i;
	}
	os << "\n";
	os << "**********************\n\n";
	os << "****  Static Instruction Counts ****\n\n";
	int pnt = 0;
	for (auto& i : opcodes)
	{
		os << left << setw(25) << (i.first + " = " + to_string(statics[i.first]));
		if ((pnt++ + 1) % 4 == 0)
		{
			os << "\n";
		}
		else
		{
			os << "\t";
		}
	}
	os << "\n\n";
	pnt = 0;
	os << "**** Dynamic Instruction Counts ****\n\n";
	for (auto& i : opcodes)
	{
		os << left << setw(25) << (i.first + " = " + to_string(instCnt[i.first]));
		if ((pnt++ + 1) % 4 == 0)
		{
			os << "\n";
		}
		else
		{
			os << "\t";
		}
	}
	os << "\n\n";
	os << "Total Instruction Execution: " << total_in << "\n";
	os.close();
	QMessageBox::information(this, "Success", "Successfully saved the file");
}

void VirtualMachine::nxt()
{
	if (flag && PC < Instructions.size())
	{
		Execute(PC);
		PC++;
		ui.tableWidget->selectRow(PC - 1);
		PrintCPU();
		PrintMemory();
		if (PC >= Instructions.size() || !flag)
		{
			executeTimer->stop();
			Stat();
		}
	}
	else
	{
		executeTimer->stop();
	}
}

