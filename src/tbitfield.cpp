// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len >= 0)
    {
        BitLen = len;
        MemLen = (len + (sizeof(TELEM) * 8 - 1)) / (8 * sizeof(TELEM));
        pMem = new TELEM[MemLen];
        if (pMem != NULL)
        {
            for (int i = 0; i < MemLen; i++)
                pMem[i] = 0;
        }
    }

    else
    {
        throw ("Ошибка len < 0");
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    if (bf.BitLen >= 0)
    {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        if (pMem == nullptr)
            throw bad_alloc();
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];

    }
    else
    {
        throw("Ошибка len < 0");
    }
}

TBitField::~TBitField()
{
    delete pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n<0 || n >= BitLen)
    {
        throw ("Ошибка. Битовое поле за пределами возможных значений");
    }
    else
    {
        return (n / (sizeof(TELEM) * 8));
    }
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n<0 || n>BitLen)
    {
        throw ("Ошибка. Битовый номер недопустим для поля бит");
    }
    else
    {
        return ((TELEM)1 << (n % (sizeof(TELEM) * 8)));
    }
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw ("Ошибка. Элемент отрицательный или за границей диапазона");
    pMem[GetMemIndex(n)] |= GetMemMask(n);

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw ("Ошибка. Элемент отрицательный или за границей диапазона");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw ("Ошибка. Элемент отрицательный или за границей диапазона");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    if (this != &bf)
    {
        if (bf.MemLen != MemLen)
        {
            MemLen = bf.MemLen;
            delete[] pMem;
            pMem = new TELEM[MemLen];

            if (pMem == nullptr)
                throw bad_alloc();
        }
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];

    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen) 
    {
        for (int i = 0; i < MemLen; i++) 
        {
            if (pMem[i] != bf.pMem[i])
                return 0;
        }
        return 1;
    }
    else return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int i;
    int len = BitLen;
    if (len < bf.BitLen)
        len = bf.BitLen;

    TBitField tbf(len);
    for (i = 0; i < MemLen; i++)
    {
        tbf.pMem[i] = pMem[i];
    }
    for (i = 0; i < bf.MemLen; i++)
    {
        tbf.pMem[i] |= bf.pMem[i];
    }

    return tbf;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int i;
    int len = BitLen;
    if (len < bf.BitLen) 
        len = bf.BitLen;

    TBitField tbf(len);
    for ( i = 0; i < MemLen; i++) 
    {
        tbf.pMem[i] = pMem[i];
    }
    for ( i = 0; i < bf.MemLen; i++)
    {
        tbf.pMem[i] &= bf.pMem[i];
    }

    return tbf;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < MemLen; i++)
    {
        res.pMem[i] = ~pMem[i];
    }

    int bitsLastElem = BitLen % (sizeof(TELEM) * 8);

    if (bitsLastElem != 0)
    {
        res.pMem[MemLen - 1] &= (((TELEM)1 << bitsLastElem) - 1);
    }

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    size_t a;
    for (int i = 0; i < bf.GetLength(); i++)
    {
        istr >> a;
        if (a == 0)
        {
            bf.ClrBit(i);
        }
        else
        {
            bf.SetBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
