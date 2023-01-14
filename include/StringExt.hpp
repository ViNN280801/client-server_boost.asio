#include <string>
#include <sstream>
#include <cstring>
#include <concepts>
#include <ranges>
#include <algorithm>

#include <boost/asio/streambuf.hpp>
#include <boost/asio/buffers_iterator.hpp>

template <typename T>
concept String = std::is_convertible_v<T, std::string_view>;

// The only member of the class data is a string, which is usually a number in
// the string representation. The functionality of the class includes various
// transformations with a string that contains a number. To use std::string class functions
// at first, you need to call the "getStr()" method
class StringExt : std::string
{
private:
    std::string m_str;

protected:
    constexpr void copy_obj(const StringExt &obj) { m_str = obj.m_str; }
    constexpr void clear_obj(StringExt &obj) { obj.m_str.clear(); }

public:
    explicit StringExt() : m_str("") {}
    explicit StringExt(const String auto &str) : m_str(str){};
    explicit StringExt(const StringExt &obj) : std::string(obj) { copy_obj(obj); }
    explicit StringExt(StringExt &&);
    virtual ~StringExt() { m_str.clear(); }

    constexpr std::string getStr() { return m_str; }
    constexpr void setStr(const String auto &str) { m_str = str; }

    StringExt &operator=(const String auto &str) noexcept
    {
        this->m_str = str;
        return *this;
    }

    StringExt &operator=(const StringExt &);
    StringExt &operator=(StringExt &&) noexcept;

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

    StringExt &operator+=(const char &);
    StringExt &operator+=(const char *);

    // Removes all specified occurencies by variable 'charToRemove'
    constexpr void removeChar(const char &charToRemove = '\n')
    {
        std::erase_if(m_str, [charToRemove](const auto &ch)
                      { return ch == charToRemove; });
    }

    template <std::unsigned_integral T = unsigned int>
    constexpr T toNumber() const
    {
        std::istringstream iss(m_str);

        T num{0U};
        iss >> num;

        return num;
    }

    constexpr bool isEmpty() const { return m_str.empty(); }
    constexpr bool isLength_lteq_64() const { return m_str.length() <= 64UL ? true : false; }
    constexpr bool isDigits() const { return std::ranges::all_of(m_str, isdigit); }
    constexpr bool is_32multiple() const { return (toNumber() % 32) ? false : true; }
    constexpr bool is_2moreDigits() const { return (isDigits() and (m_str.length() > 2UL)) ? true : false; }

    constexpr virtual void clear() { m_str.clear(); }
    constexpr virtual size_t length() const { return m_str.length(); }

    template <std::unsigned_integral T = unsigned int>
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

    constexpr void sortDescending() { std::ranges::sort(m_str, std::ranges::greater()); }

    void replaceEvenDigitsTo_KB();

    inline const std::string asio_streambuf_to_str(const boost::asio::streambuf &buf) const noexcept
    {
        return std::string(boost::asio::buffers_begin(buf.data()),
                           boost::asio::buffers_begin(buf.data()) + buf.size());
    }
};
