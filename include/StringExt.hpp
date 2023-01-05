#include <string>
#include <sstream>
#include <cstring>
#include <concepts>
#include <ranges>
#include <algorithm>

template <typename T>
concept String = std::is_convertible_v<T, std::string_view>;

// The only member of the class data is a string, which is usually a number in
// the string representation. The functionality of the class includes various
// transformations with a string that contains a number.
class StringExt : std::string
{
private:
    std::string m_str;

protected:
    inline void copy_obj(const StringExt &obj) { m_str = obj.m_str; }
    inline void clear_obj(StringExt &obj) { obj.m_str.clear(); }

public:
    explicit StringExt() : m_str("") {}
    explicit StringExt(const String auto &str) : m_str(str){};
    explicit StringExt(const StringExt &obj) : std::string(obj) { copy_obj(obj); }
    explicit StringExt(StringExt &&__rhs);
    virtual ~StringExt() { m_str.clear(); }

    constexpr inline std::string getStr() { return m_str; }
    constexpr inline void setStr(const String auto &str) { m_str = str; }

    StringExt &operator=(const String auto &str) noexcept
    {
        this->m_str = str;
        return *this;
    }

    StringExt &operator=(const StringExt &obj);
    StringExt &operator=(StringExt &&__rhs) noexcept;
    friend std::ostream &operator<<(std::ostream &os, const StringExt &obj)
    {
        os << obj.m_str;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, StringExt &obj)
    {
        is >> obj.m_str;
        return is;
    }

    friend inline bool operator==(const StringExt &lhs, const StringExt &rhs) { return strcmp(lhs.c_str(), rhs.c_str()) ? false : true; }
    friend inline bool operator==(const StringExt &lhs, const char *rhs) { return (strcmp(lhs.c_str(), rhs)) ? false : true; }

    StringExt &operator+=(const char *str);

    constexpr inline void removeEndl()
    {
        std::erase_if(m_str, [](const auto &el)
                      { return el == '\n'; });
    }

    template <std::integral T>
    constexpr T toNumber() const
    {
        std::istringstream iss(m_str);

        T num{};
        iss >> num;

        return num;
    }

    // Lower than or equals
    constexpr inline bool isLength_lteq_64() const { return m_str.length() <= 64UL ? true : false; }
    constexpr inline bool isDigits() const { return std::ranges::all_of(std::ranges::cbegin(m_str), std::ranges::cend(m_str), isdigit); }
    constexpr inline bool is_32multiple() const { return (toNumber<int>() % 32) ? false : true; }
    constexpr inline bool is_3moreDigits() const { return (isDigits() and m_str.length() > 2UL) ? true : false; }

    constexpr inline virtual void clear() { m_str.clear(); }
    constexpr inline virtual size_t length() const { return m_str.length(); }

    template <std::unsigned_integral T>
    constexpr T sumOfDigits() const
    {
        T sum{0U};

        for (const auto &el : m_str)
        {
            if (isdigit(el))
                sum += el - '0';
        }

        return sum;
    }

    constexpr inline void sortDescending() { std::ranges::sort(m_str, std::ranges::greater()); }

    constexpr void replaceEvenDigitsTo_KB();
};
