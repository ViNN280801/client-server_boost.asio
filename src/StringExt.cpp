#include "../include/StringExt.hpp"

StringExt::StringExt(StringExt &&rhs)
{
    copy_obj(rhs);
    clear_obj(rhs);
}

StringExt &StringExt::operator=(const StringExt &obj)
{
    if (this == &obj)
        return *this;

    copy_obj(obj);
    return *this;
}

StringExt &StringExt::operator=(StringExt &&rhs) noexcept
{
    if (this == &rhs)
        return *this;

    clear_obj(*this);
    copy_obj(rhs);
    clear_obj(rhs);

    return *this;
}

StringExt &StringExt::operator+=(const char &ch)
{
    m_str += ch;
    return *this;
}

StringExt &StringExt::operator+=(const char *str)
{
    m_str += str;
    return *this;
}

void StringExt::replaceEvenDigitsTo_KB()
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
