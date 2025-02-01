#include "Memory.h"

int Memory::GetMnum(int block, int offset)
{
    if (block == 1)
    {
        return Mstack[offset];
    }
    else
    {
        return Mstack[BP + offset + 2];
    }
}

int Memory::GetMnum(int addr)
{
    return Mstack[addr];
}

int Memory::GetMadr(int block, int offset)
{
    if (block == 1)
    {
        return offset;
    }
    else
    {
        return BP + offset + 2;
    }
}

void Memory::Msetter(int value, int block, int offset)
{
    if (block == 2)
    {
        Mstack[BP + offset + 2] = value;
    }
    else
    {
        Mstack[offset] = value;
    }
}

void Memory::Msetter(int value, int addr)
{
    Mstack[addr] = value;
}

void Memory::Msetter(int value)
{
    Mstack.push_back(value);
}

void Memory::PlusM(int size, string op)
{
    if (op == "proc")
    {
        Mstack.resize(SP + size + 1, -1);
        Mstack[SP - 1] = BP;
        BP = SP - 1;
        SP += size;
    }
    else if (op == "bgn")
    {
        Mstack.resize(size, -1);
        BP = 0;
        SP = size - 1;
        Datasiz = size;
    }
    else if (op == "ldp")
    {
        SP += 2;
        Mstack.resize(SP + 1, -1);
    }
}

void Memory::DelM()
{
    SP = BP - 1;
    BP = Mstack[BP];
    Mstack.resize(SP + 1);
}

void Memory::Refit(int value)
{
    Mstack.resize(value);
}

vector<int> Memory::GetMstack()
{
    return Mstack;
}

int Memory::GetSP()
{
    return SP;
}

void Memory::SetSP(int value)
{
    SP = value;
}

int Memory::GetBP()
{
    return BP;
}

void Memory::SetBP(int value)
{
    BP = value;
}

int Memory::GetDataSize()
{
    return Datasiz;
}
