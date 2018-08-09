#include <iostream>
#include <string>
#include <stdexcept>
#include <type_traits>

using std::swap;
using std::string;
using std::istream;
using std::ostream;
using std::runtime_error;
using std::is_integral;
using std::is_floating_point;
using std::is_same;
using std::common_type_t;
using std::enable_if_t;
using std::is_signed;
using std::is_unsigned;
using std::to_string;

template<typename T>
constexpr enable_if_t<is_integral<T>::value && is_signed<T>::value, T> 
	abs_integral(T val)
	{ 
		return val < 0 ? -val : val;
	}

template<typename T>
constexpr enable_if_t<is_integral<T>::value && is_unsigned<T>::value, T>
	abs_integral(T val)
	{ 
		return val; 
	}

void abs_integral(bool) = delete;

/// Greatest common divisor
template<typename Mt, typename Nt>
constexpr common_type_t<Mt, Nt>
	gcd(Mt m, Nt n)
    {
      return m == 0 ? abs_integral(n) : n == 0 ? abs_integral(m) : gcd(n, m % n);
    }

/// Least common multiple
template<typename Mt, typename Nt>
constexpr common_type_t<Mt, Nt>
    lcm(Mt m, Nt n)
    {
      return (m != 0 && n != 0) ? (abs_integral(m) / gcd(m, n)) * abs_integral(n) : 0;
	}

struct Fraction_error : public runtime_error
{
	explicit Fraction_error(const char* s) : runtime_error(s){}
	explicit Fraction_error(const string& s) : runtime_error(s){}
};

template<typename T, typename = enable_if_t<is_integral<T>::value>>
class Fraction
{	
private:
	T num{0};
	T denom{1};
public:
	constexpr Fraction() noexcept {}
	explicit constexpr Fraction(T n) noexcept : num{n} {}
	explicit constexpr Fraction(T n, T d) noexcept
	{
		if(d == 0)
			throw Fraction_error{"Divide by zero!"};

		num = n;
		denom = d;
	}
	
	constexpr T get_num() const noexcept { return num; }
	constexpr void set_num(T other) noexcept { num = other; }

	constexpr T get_denom() const noexcept { return denom; }
	constexpr void set_denom(T other) 
	{
		if(other == 0)
			 throw Fraction_error{"Division by zero"};
		denom = other; 
	}
	template<typename Y, typename = enable_if_t<is_floating_point<Y>::value>>
	constexpr inline operator Y() const noexcept
	{ 
		return static_cast<Y>(num) / static_cast<Y>(denom);
	}

	constexpr inline operator bool() const noexcept
	{
		return num != 0;
	}

	inline string ToString() const{ return (denom == 1) ? to_string(num) : to_string(num) + " / " + to_string(denom); }
	
	friend istream& operator>>(istream& is, Fraction& a)
	{
		is >> a.num >> a.denom;
		if(a.denom == 0)
			throw Fraction_error{"Division by zero"};
		return is;
	}
	friend ostream& operator<<(ostream& os, Fraction a)
	{
		os << a.ToString();
		return os;
	}
	
	constexpr friend Fraction operator+(Fraction a, Fraction b) noexcept
	{
		a.reduct();
		b.reduct();
		if(a.denom != b.denom){
			common_denom(a, b);
		}
		return Fraction(a.num + b.num, a.denom);
	}
	constexpr Fraction operator+=(Fraction a) noexcept { return *this = *this + a; }
	constexpr Fraction operator+(T n) const noexcept { return Fraction(num + n * denom, denom); }
	constexpr Fraction operator+=(T n) noexcept { return *this = *this + n; }
	constexpr Fraction operator +() const noexcept { return *this; }
	
	constexpr friend Fraction operator-(Fraction a, Fraction b) noexcept
	{
		a.reduct();
		b.reduct();
		if(a.denom != b.denom){
			common_denom(a, b);
		}		
		return Fraction(a.num - b.num, a.denom);
	}
	constexpr Fraction operator-=(Fraction a) noexcept { return *this = *this - a; }
	constexpr Fraction operator-(T n) const noexcept{ return Fraction(num - n * denom, denom); }
	constexpr Fraction operator-=(T n) noexcept {	return *this = *this - n; }
	constexpr Fraction operator -() const noexcept { return Fraction(num * (-1), denom); }
	
	constexpr friend Fraction operator*(Fraction a, Fraction b) noexcept 
	{ 
		a.reduct();
		b.reduct();
		return Fraction(a.num * b.num, a.denom * b.denom);
	}
	constexpr Fraction operator*=(Fraction& a) noexcept { return *this = *this * a; }
	constexpr Fraction operator*(T n) const noexcept{ return Fraction(num * n, denom); }
	constexpr Fraction operator*=(T n) noexcept { return *this = *this * n; }
	
	constexpr friend Fraction operator/(Fraction a, Fraction b)
	{
		a.reduct();
		b.reduct();
		auto __den = a.denom * b.num;
		if(__den == 0)
			throw Fraction_error{"Division by zero"};
		return Fraction(a.num * b.denom, __den);
	}
	constexpr Fraction operator/=(Fraction a){ return *this = *this / a; }
	constexpr Fraction operator/(T n){ return Fraction(num, denom * n); }
	constexpr Fraction operator/=(const T n){ return *this = *this / n; }
	
	constexpr bool operator==(Fraction a) const noexcept
	{
		Fraction _fr(*this);
		common_denom(_fr, a);
		return	_fr.num == a.num;
	}
	constexpr bool operator!=(Fraction a) const noexcept { return !(*this == a); }
	constexpr bool operator<(Fraction a) const noexcept
	{
		Fraction _fr(*this);
		common_denom(_fr, a);
		return	_fr.num < a.num;
	}
	constexpr bool operator>(Fraction a) const noexcept
	{
		Fraction _fr(*this);
		common_denom(_fr, a);
		return	_fr.num > a.num;
	}
	constexpr bool operator<=(Fraction a) const noexcept
	{
		Fraction _fr(*this);
		common_denom(_fr, a);
		return	_fr.num <= a.num;
	}
	constexpr bool operator>=(Fraction a) const noexcept
	{
		Fraction _fr(*this);
		common_denom(_fr, a);
		return	_fr.num >= a.num;
	}
	
	constexpr friend Fraction reducted(Fraction a) noexcept { return a.reduct(); }
	constexpr Fraction reduct() noexcept
	{
		if (num == 0)
			return Fraction(0);
		auto _div = gcd(num, denom);
	
		num /= _div;
		denom /= _div;
	
		return *this;
	}
	
	constexpr friend void common_denom(Fraction& a, Fraction& b) noexcept
	{
		auto _tmp = a.denom;
		a.denom = lcm(a.denom, b.denom);
		a.num *= (a.denom / _tmp);
	
		_tmp = b.denom;
		b.denom = a.denom;
		b.num *= (b.denom / _tmp);
	}
};