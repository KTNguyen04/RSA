#include "BigIntMod.h"

array<int64_t, 4> BigIntMod::hexToBits(char hex)
{
    int64_t value;
    if (hex >= '0' && hex <= '9')
        value = hex - '0';
    else if (hex >= 'A' && hex <= 'F')
        value = hex - 'A' + 10;
    else if (hex >= 'a' && hex <= 'f')
        value = hex - 'a' + 10;
    else
        throw std::invalid_argument("Invalid hex digit");

    array<int64_t, 4> bits;
    for (int64_t i = 0; i < 4; ++i)
        bits[i] = (value >> i) & 1; // LSB → MSB order
    return bits;
}

char BigIntMod::bitsToHex(const deque<int64_t> &bits, size_t pos)
{
    int64_t value = 0;
    for (int64_t i = 0; i < 4; ++i)
        if (pos + i < bits.size())
            value |= (bits[pos + i] << i);
    if (value < 10)
        return '0' + value;
    return 'A' + (value - 10);
}

BigIntMod::BigIntMod(const string &i_val)
{
    bits.clear();
    sign_ = true;
    int64_t tmp = 0;
    if (i_val[0] == '-')
    {
        sign_ = false;
        tmp = 1;
    }
    for (string::size_type i = tmp; i < i_val.size(); ++i)
    {
        auto nibble = hexToBits(i_val[i]);
        for (int64_t b : nibble)
            bits.push_back(b);
    }
}

BigIntMod::BigIntMod()
{
    bits.clear();
    sign_ = true;
}

void BigIntMod::trim()
{
    while (bits.size() > 1 && bits[bits.size() - 1] == 0)
        bits.pop_back();
}

BigIntMod BigIntMod::operator=(const BigIntMod &i_val)
{
    bits.clear();
    sign_ = i_val.sign_;
    for (deque<int64_t>::size_type i = 0; i < i_val.bits.size(); ++i)
    {
        bits.push_back(i_val.bits[i]);
    }
    return *this;
}

BigIntMod &BigIntMod::operator+=(const BigIntMod &i_val)
{
    if (sign_ == i_val.sign_)
    {
        while (bits.size() <= i_val.bits.size())
        {
            bits.push_back(0);
        }
        int64_t carry = 0;
        deque<int64_t>::size_type i;
        for (i = 0; i < i_val.bits.size(); ++i)
        {
            bits.at(i) += carry + i_val.bits.at(i);
            if (bits[i] >= 2)
            {
                carry = 1;
                bits[i] -= 2;
            }
            else
                carry = 0;
        }
        for (; i < bits.size(); ++i)
        {
            bits.at(i) += carry + 0;
            if (bits[i] >= 2)
            {
                carry = 1;
                bits[i] -= 2;
            }
            else
                carry = 0;
        }
    }
    else
    {
        if (sign_ == true && i_val.sign_ == false)
        {
            BigIntMod tmp(i_val);
            tmp.sign_ = true;
            *this -= tmp;
        }
        else
        {
            BigIntMod tmp(*this);
            tmp.sign_ = true;
            *this = i_val - tmp;
        }
    }
    trim();
    return *this;
}

BigIntMod &BigIntMod::operator-=(const BigIntMod &i_val)
{
    BigIntMod tmp;
    if (*this < i_val)
    {
        tmp = *this;
        *this = i_val;
        this->sign_ = false;
    }
    else
    {
        tmp = i_val;
    }
    int64_t bor = 0;
    deque<int64_t>::size_type i;
    for (i = 0; i < tmp.bits.size(); ++i)
    {
        bits[i] -= bor + tmp.bits[i];
        if (bits[i] < 0)
        {
            bor = 1;
            bits[i] += 2;
        }
        else
            bor = 0;
    }
    for (; i < bits.size(); ++i)
    {
        bits[i] -= bor;
        if (bits[i] < 0)
        {
            bor = 1;
            bits[i] += 2;
        }
        else
            bor = 0;
    }
    trim();
    return *this;
}

bool operator>(const BigIntMod &lhs, const BigIntMod &rhs)
{
    return (lhs != rhs) && (rhs < lhs);
}

bool operator>=(const BigIntMod &lhs, const BigIntMod &rhs)
{
    return !(lhs < rhs);
}

bool operator<(const BigIntMod &lhs, const BigIntMod &rhs)
{
    if (lhs.sign_ == false && rhs.sign_ == true)
        return true;
    if (lhs.sign_ == true && rhs.sign_ == false)
        return false;
    if (lhs.sign_)
    {
        if (lhs.bits.size() < rhs.bits.size())
            return true;
        else if (lhs.bits.size() > rhs.bits.size())
            return false;
    }
    else
    {
        if (lhs.bits.size() < rhs.bits.size())
            return false;
        else if (lhs.bits.size() > rhs.bits.size())
            return true;
    }
    for (deque<int64_t>::size_type i = lhs.bits.size() - 1; i > 0; --i)
    {
        if (lhs.bits[i] < rhs.bits[i])
        {
            if (lhs.sign_)
                return true;
            else
                return false;
        }
        else if (lhs.bits[i] > rhs.bits[i])
        {
            if (lhs.sign_)
                return false;
            else
                return true;
        }
    }
    if (lhs.bits[0] < rhs.bits[0])
    {
        if (lhs.sign_)
            return true;
        else
            return false;
    }
    return false;
}
bool operator<=(const BigIntMod &lhs, const BigIntMod &rhs)
{
    return !(lhs > rhs);
}

bool operator==(const BigIntMod &lhs, const BigIntMod &rhs)
{
    if (lhs.sign_ != rhs.sign_ || lhs.bits.size() != rhs.bits.size())
        return false;
    for (deque<int64_t>::size_type i = 0; i < lhs.bits.size(); i++)
    {
        if (lhs.bits.at(i) != rhs.bits.at(i))
            return false;
    }
    return true;
}

bool operator!=(const BigIntMod &lhs, const BigIntMod &rhs)
{
    return !(lhs == rhs);
}

BigIntMod operator+(const BigIntMod &lhs, const BigIntMod &rhs)
{
    BigIntMod ret(lhs);
    ret += rhs;
    return ret;
}

BigIntMod operator-(const BigIntMod &lhs, const BigIntMod &rhs)
{
    BigIntMod ret(lhs);
    ret -= rhs;
    return ret;
}

istream &operator>>(istream &in, BigIntMod &o_val)
{
    string tmp;
    in >> tmp;
    o_val = BigIntMod(tmp);
    return in;
}

ostream &operator<<(ostream &out, const BigIntMod &i_val)
{
    if (i_val.bits.empty())
    {
        out << "0";
        return out;
    }

    if (!i_val.sign_)
        out << '-';

    // output 4 bits → 1 hex (still least→most)
    for (size_t i = 0; i < i_val.bits.size(); i += 4)
        out << BigIntMod::bitsToHex(i_val.bits, i);

    return out;
}