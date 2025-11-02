#include "BigIntMod.h"
const BigIntMod ONE = BigIntMod("1");
const BigIntMod TWO = BigIntMod("2");
const BigIntMod ZERO = BigIntMod("0");

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
    trim();
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

bool BigIntMod::isZero() const
{

    if (bits.empty())
        return true;
    return (bits.size() == 1 && bits[0] == 0);
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

BigIntMod BigIntMod::operator=(const string &hexStr)
{
    BigIntMod tmp(hexStr);
    *this = tmp;

    return *this;
}

BigIntMod &BigIntMod::operator+=(const BigIntMod &i_val)
{
    if (sign_ == i_val.sign_)
    {
        size_t n = std::max(bits.size(), i_val.bits.size());
        bits.resize(n, 0); // mở rộng bits của this

        int64_t carry = 0;
        for (size_t i = 0; i < n; ++i)
        {
            int64_t b = (i < i_val.bits.size()) ? i_val.bits[i] : 0;
            bits[i] += b + carry;
            carry = bits[i] / 2; // base 2
            bits[i] %= 2;
        }

        if (carry > 0)
            bits.push_back(carry); // thêm bit cao cuối cùng
    }
    else
    {
        // xử lý số âm
        if (sign_ && !i_val.sign_)
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

    if (sign_ && !i_val.sign_)
    {
        BigIntMod tmp2(i_val);
        tmp2.sign_ = true;
        *this += tmp2;
        return *this;
    }
    if (!sign_ && i_val.sign_)
    {
        BigIntMod tmp2(*this);
        tmp2.sign_ = true;
        *this = tmp2 + i_val;
        this->sign_ = false;
        return *this;
    }

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

BigIntMod &BigIntMod::operator*=(const BigIntMod &i_val)
{
    // handle sign
    BigIntMod ret;
    ret.sign_ = (this->sign_ == i_val.sign_);

    size_t n = bits.size();
    size_t m = i_val.bits.size();
    if (n == 0 || m == 0)
    {
        *this = ZERO;
        return *this;
    }

    ret.bits.assign(n + m, 0); // allocate result bits

    // Convolution in base 2 (bits)
    for (size_t i = 0; i < n; ++i)
    {
        if (bits[i] == 0)
            continue;
        for (size_t j = 0; j < m; ++j)
        {
            if (i_val.bits[j] == 0)
                continue;
            ret.bits[i + j] += 1; // bits[i]*i_val.bits[j] is 1 if both 1
        }
    }

    // Normalize carries base 2
    int carry = 0;
    for (size_t i = 0; i < ret.bits.size(); ++i)
    {
        int64_t v = ret.bits[i] + carry;
        ret.bits[i] = v & 1; // v % 2
        carry = (v >> 1);    // v / 2
    }
    while (carry > 0)
    {
        ret.bits.push_back(carry & 1);
        carry >>= 1;
    }

    ret.trim();
    *this = ret;
    return *this;
}

BigIntMod &BigIntMod::operator/=(const BigIntMod &i_val)
{
    if (*this == i_val)
    {
        *this = ONE;
        return *this;
    }
    // std::cout << *this << std::endl;
    // std::cout << i_val << std::endl;

    BigIntMod ret = ZERO;
    if (sign_ == i_val.sign_)
        ret.sign_ = true;
    else
        ret.sign_ = false;

    BigIntMod divider(i_val);

    this->sign_ = true;
    divider.sign_ = true;

    if (*this < divider)
    {
        *this = ZERO;
        return *this;
    }

    int cnt = 0;
    while (*this > ZERO)
    {
        if (*this >= divider)
        {
            *this -= divider;
            BigIntMod tmp = pow(TWO, cnt);
            ret += tmp;
            divider = mul2(divider);
            cnt++;
        }
        else
        {
            divider = i_val;
            cnt = 0;
            if (*this < divider)
            {
                ret.trim();
                *this = ret;
                return *this;
            }
        }
        divider.sign_ = true;
    }

    ret.trim();
    *this = ret;
    return *this;
}

BigIntMod operator*(const BigIntMod &lhs, const BigIntMod &rhs)
{
    BigIntMod ret(lhs);
    ret *= rhs;
    return ret;
}

BigIntMod operator/(const BigIntMod &lhs, const BigIntMod &rhs)
{
    BigIntMod ret(lhs);
    ret /= rhs;
    return ret;
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

    // same sign
    bool positive = lhs.sign_;

    if (lhs.bits.size() != rhs.bits.size())
    {
        if (positive)
            return lhs.bits.size() < rhs.bits.size();
        else
            return lhs.bits.size() > rhs.bits.size();
    }

    // compare from most significant bit down to 0
    for (size_t i = lhs.bits.size(); i-- > 0;)
    {
        if (lhs.bits[i] < rhs.bits[i])
            return positive ? true : false;
        if (lhs.bits[i] > rhs.bits[i])
            return positive ? false : true;
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

BigIntMod &BigIntMod::operator%=(const BigIntMod &n)
{
    BigIntMod m = n;
    trim();
    m.trim();

    if (m.bits.size() == 1 && m.bits[0] == 0)
        throw invalid_argument("Modulo by zero");

    if (*this < m)
    {
        if (!sign_)
        {
            sign_ = false;
            while (*this + m < m)
                *this += m;
        }
        return *this;
    }
    BigIntMod divisor = m;

    size_t diff = bits.size() - divisor.bits.size();

    // Shift divisor left (i.e., multiply by 2^diff)
    deque<int64_t> shifted = divisor.bits;
    for (size_t i = 0; i < diff; ++i)
        shifted.push_front(0); // thêm bit 0 ở đầu (dịch trái)

    for (size_t i = 0; i <= diff; ++i)
    {
        BigIntMod temp;
        temp.bits = shifted;
        temp.trim();

        if (*this >= temp)
        {
            *this -= temp;
        }

        // shift right (divide by 2)
        if (!shifted.empty())
            shifted.pop_front(); // bỏ bit thấp nhất
    }

    trim();
    return *this;
}

BigIntMod mul2Mod(const BigIntMod &a, const BigIntMod &m)
{

    BigIntMod result = a;
    // Shift left 1 bit → nhân với 2
    result.bits.push_front(0);
    result.trim();
    result %= m;
    return result;
}

BigIntMod addMod(const BigIntMod &a, const BigIntMod &b, const BigIntMod &m)
{
    return (a + b) % m;
}
BigIntMod subMod(const BigIntMod &a, const BigIntMod &b, const BigIntMod &m)
{
    return (a - b) % m;
}

BigIntMod mulMod(const BigIntMod &a, const BigIntMod &b, const BigIntMod &m)
{
    BigIntMod x = a % m;
    BigIntMod y = b % m;
    BigIntMod res = ZERO;

     for (size_t i = 0; i < y.bits.size(); ++i)
    {
        if (y.bits[i] == 1)
        {
            // res = (res + x) % m  (use direct addition then conditional reduce)
            res += x;
            if (res >= m)
                res -= m;
        }
        // x = (x << 1) % m
        // implement shift-left 1 bit without full division
        // shift bits
        x.bits.push_front(0); // multiply by 2
        x.trim();
        if (x >= m)
            x -= m;
    }
    return res;
}

BigIntMod powMod(const BigIntMod &base, const BigIntMod &exponent, const BigIntMod &m)
{
    BigIntMod x = base % m;
    BigIntMod a = exponent;
    BigIntMod y = ONE;

    for (size_t i = a.bits.size(); i-- > 0;)
    {
        // square
        y = mulMod(y, y, m);
        if (a.bits[i] == 1)
        {
            y = mulMod(y, x, m);
        }
    }
    return y;
}

BigIntMod invMod(const BigIntMod &a, const BigIntMod &m)
{
    return BigIntMod();
}

BigIntMod operator%(const BigIntMod &lhs, const BigIntMod &rhs)
{
    BigIntMod result = lhs;
    result %= rhs;
    return result;
}

BigIntMod pow(const BigIntMod &i_val, const BigIntMod &i_exp)
{
    if (i_exp == ZERO || i_val == ONE)
    {
        return ONE;
    }
    if (i_val == ZERO)
    {
        return ZERO;
    }

    BigIntMod i(ZERO);
    BigIntMod ret(i_val);
    BigIntMod exp(i_exp);

    exp -= ONE;

    while (i < exp)
    {
        ret *= i_val;
        i += ONE;
    }

    return ret;
}

BigIntMod pow(const BigIntMod &base, int exponent)
{
    if (exponent < 0)
        throw std::invalid_argument("Negative exponent not supported");
    BigIntMod result = ONE;
    BigIntMod b = base;
    int e = exponent;
    while (e > 0)
    {
        if (e & 1)
            result *= b;
        e >>= 1;
        if (e)
            b *= b;
    }
    return result;
}

BigIntMod mul2(const BigIntMod &i)
{
    BigIntMod result = i;
    result.bits.push_front(0);
    result.trim();
    return result;
}
