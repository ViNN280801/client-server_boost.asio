#pragma once

#include <string>
#include <sstream>
#include <cstring>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <numeric>

template <typename T>
concept String = std::is_convertible_v<T, std::string_view>;

// The only member of the class data is a string, which is usually a number in
// the string representation. The functionality of the class includes various
// transformations with a string that contains a number.
class strmanip
{
private:
    std::string m_str;

protected:
    inline void copy_obj(const strmanip &obj) { m_str = obj.m_str; }
    inline void clear_obj(strmanip &obj) { obj.m_str.clear(); }

public:
    explicit strmanip() : m_str("") {}
    explicit strmanip(const String auto &str) : m_str(str){};
    explicit strmanip(const strmanip &obj) { copy_obj(obj); }
    explicit strmanip(strmanip &&__rhs);
    virtual ~strmanip() { m_str.clear(); }

    inline std::string getStr() { return m_str; }
    inline void setStr(const String auto &str) { m_str = str; }

    strmanip &operator=(const String auto &str) noexcept
    {
        this->m_str = str;
        return *this;
    }

    strmanip &operator=(const strmanip &obj);
    strmanip &operator=(strmanip &&__rhs) noexcept;
    friend std::ostream &operator<<(std::ostream &os, const strmanip &obj)
    {
        os << obj.m_str;
        return os;
    }
    friend std::istream &operator>>(std::istream &is, strmanip &obj);
    friend inline bool operator==(const strmanip &lhs, const strmanip &rhs) { return (lhs.toString() == rhs.toString()) ? true : false; }
    friend inline bool operator==(const strmanip &lhs, const char *rhs) { return (strcmp(lhs.to_c_str(), rhs)); }

    strmanip &operator+=(const char *str);

    constexpr inline void removeEndl() noexcept
    {
        std::erase_if(m_str, [](const auto &el)
                      { return el == '\n'; });
    }

    template <std::integral T>
    constexpr T toNumber() const noexcept
    {
        std::istringstream iss(m_str);

        T num{};
        iss >> num;

        return num;
    }

    constexpr inline const std::string toString() const noexcept { return m_str; }
    constexpr inline const char *to_c_str() const noexcept { return m_str.c_str(); }

    // Lower than or equals
    constexpr inline bool isLength_lteq_64() const noexcept { return m_str.length() <= 64UL ? true : false; }
    constexpr inline bool isDigits() const noexcept { return std::ranges::all_of(std::ranges::cbegin(m_str), std::ranges::cend(m_str), isdigit); }
    constexpr inline bool is_32multiple() const noexcept { return (toNumber<int>() % 32) ? false : true; }
    constexpr inline bool is_3moreDigits() const noexcept { return (isDigits() and m_str.length() > 2UL) ? true : false; }

    constexpr inline virtual void clear() noexcept { m_str.clear(); }
    constexpr inline virtual size_t length() const noexcept { return m_str.length(); }

    template <std::unsigned_integral T>
    constexpr T sumOfDigits() const noexcept
    {
        T sum{0U};

        for (const auto &el : m_str)
        {
            if (isdigit(el))
                sum += el - '0';
        }

        return sum;
    }

    constexpr inline void sortDescending() noexcept { std::ranges::sort(m_str, std::ranges::greater()); }

    constexpr void replaceEvenDigitsTo_KB() noexcept;
};
