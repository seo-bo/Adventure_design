#pragma once
#include <vector>
#include <string>
using namespace std;

class Memory
{
public:
    int GetMnum(int block, int offset); // 특정 블록과 오프셋에서 값을 읽어옴
    int GetMnum(int addr); // 절대 주소에서 값을 읽어옴
    int GetMadr(int block, int offset); // 특정 블록과 오프셋에서 주소를 계산
    void Msetter(int value, int block, int offset); // 특정 블록과 오프셋에 값 저장
    void Msetter(int value, int addr);
    void Msetter(int value);
    void PlusM(int size, string op); // 메모리 할당 (프로시저/시작/로컬 변수)
    void DelM(); // 메모리 해제
    void Refit(int value); // 메모리 크기 조정
    vector<int> GetMstack();
    int GetSP();
    void SetSP(int value);
    int GetBP();
    void SetBP(int value);
    int GetDataSize();

private:
    int BP = -1;           // 베이스 포인터
    int SP = -1;           // 스택 포인터
    vector<int> Mstack;    // 메모리 스택
    int Datasiz = 0;       // 데이터 크기
};
