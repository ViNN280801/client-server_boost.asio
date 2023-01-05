#include "../include/strmanip.hpp"

strmanip::strmanip(strmanip &&rhs)
{
    copy_obj(rhs);
    clear_obj(rhs);
}

strmanip &strmanip::operator=(const strmanip &obj)
{
    if (this == &obj)
        return *this;

    copy_obj(obj);
    return *this;
}

strmanip &strmanip::operator=(strmanip &&rhs) noexcept
{
    if (this == &rhs)
        return *this;

    clear_obj(*this);
    copy_obj(rhs);
    clear_obj(rhs);

    return *this;
}

std::istream &operator>>(std::istream &is, strmanip &obj)
{
    is >> obj.m_str;
    return is;
}

strmanip &strmanip::operator+=(const char *str)
{
    m_str += str;
    return *this;
}

constexpr void strmanip::replaceEvenDigitsTo_KB() noexcept
{
    for (size_t i{0UL}; i < m_str.length(); i++)
    {
        if (isdigit(m_str.at(i)) and not((m_str.at(i) - '0') % 2))
        {
            m_str.erase(i, 1UL);
            m_str.insert(i, "KB");
        }
    }
}
