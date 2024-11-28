#include <cmath>
#include <assert.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#define CPP20  true

bool AreEqual(double const d1, double const d2, double const epsilon = 0.001)
{
	return std::fabs(d1 - d2) < epsilon;
}

namespace pressure
{
	enum class scale
	{
		Pa,
		MPa,
		KgfperMm,
		KgfperM
	};

	template <scale S>
	class Quantity
	{
		const double amount_;
	public:
		constexpr explicit Quantity(double const a) : amount_(a) {}

		explicit operator double() const { return amount_; }
	};


#if CPP20
	template <scale S>
	inline bool operator==(Quantity<S> const& lhs, Quantity<S> const& rhs)
	{
		return AreEqual(static_cast<double>(lhs), static_cast<double>(rhs));
	}
	template <scale S>
	inline auto operator <=> (Quantity<S> const& lhs, Quantity<S> const& rhs)
	{
		return  static_cast<double>(lhs) <=> static_cast<double>(rhs);
	}


#else 
	template <scale S>
	inline bool operator==(quantity<S> const& lhs, quantity<S> const& rhs)
	{
		return are_equal(static_cast<double>(lhs), static_cast<double>(rhs));
	}

	template <scale S>
	inline bool operator!=(quantity<S> const& lhs, quantity<S> const& rhs)
	{
		return !(lhs == rhs);
	}

	template <scale S>
	inline bool operator< (quantity<S> const& lhs, quantity<S> const& rhs)
	{
		return static_cast<double>(lhs) < static_cast<double>(rhs);
	}

	template <scale S>
	inline bool operator> (quantity<S> const& lhs, quantity<S> const& rhs)
	{
		return rhs < lhs;
	}

	template <scale S>
	inline bool operator<=(quantity<S> const& lhs, quantity<S> const& rhs)
	{
		return !(lhs > rhs);
	}

	template <scale S>
	inline bool operator>=(quantity<S> const& lhs, quantity<S> const& rhs)
	{
		return !(lhs < rhs);
	}


#endif


	template <scale S>
	constexpr Quantity<S> operator+(Quantity<S> const& q1, Quantity<S> const& q2)
	{
		return Quantity<S>(static_cast<double>(q1) + static_cast<double>(q2));
	}

	template <scale S>
	constexpr Quantity<S> operator-(Quantity<S> const& q1, Quantity<S> const& q2)
	{
		return Quantity<S>(static_cast<double>(q1) - static_cast<double>(q2));
	}

	template <scale S, scale R>
	struct ConversionTraits
	{
		static double convert(double const value) = delete;
	};

	template <>
	struct ConversionTraits<scale::Pa, scale::MPa>
	{
		static double convert(double const value)
		{
			return value / 1.e+6;
		}
	};

	template <>
	struct ConversionTraits<scale::MPa, scale::Pa>
	{
		static double convert(double const value)
		{
			return value * 1.e+6;
		}
	};

	template <>
	struct ConversionTraits<scale::Pa, scale::KgfperMm>
	{
		static double convert(double const value)
		{
			return value / 9.81e+6;
		}
	};

	template <>
	struct ConversionTraits<scale::MPa, scale::KgfperMm>
	{
		static double convert(double const value)
		{
			return value / 9.81;
		}
	};

	template <>
	struct ConversionTraits<scale::KgfperMm, scale::MPa>
	{
		static double convert(double const value)
		{
			return value * 9.81;
		}
	};

	template <>
	struct ConversionTraits<scale::KgfperMm, scale::Pa>
	{
		static double convert(double const value)
		{
			return value * 9.81e+6;
		}
	};
	template <>
	struct ConversionTraits<scale::KgfperM, scale::Pa>
	{
		static double convert(double const value)
		{
			return value * 9.81;
		}
	};
	template <>
	struct ConversionTraits<scale::Pa, scale::KgfperM>
	{
		static double convert(double const value)
		{
			return value / 9.81;
		}
	};

	template <scale R, scale S>
	constexpr Quantity<R> PressureCast(Quantity<S> const q)
	{
		return Quantity<R>(ConversionTraits<S, R>::convert(static_cast<double>(q)));
	}


	constexpr Quantity<scale::Pa> operator "" _Pa(long double const amount)
	{
		return Quantity<scale::Pa> {static_cast<double>(amount)};
	}

	constexpr Quantity<scale::MPa> operator "" _Mpa(long double const amount)
	{
		return Quantity<scale::MPa> {static_cast<double>(amount)};
	}

	constexpr Quantity<scale::KgfperMm> operator "" _KgfperMm(long double const amount)
	{
		return Quantity<scale::KgfperMm> {static_cast<double>(amount)};


	}
	constexpr Quantity<scale::KgfperM> operator "" _KgfperM(long double const amount)
	{
		return Quantity<scale::KgfperM> {static_cast<double>(amount)};
	}
}

TEST_CASE ("Pressure conversion", "[operator overloads]") {

	using namespace   Catch::Matchers;
	//REQUIRE_THAT(b, Catch::Matchers::WithinRel(a, std::numeric_limits<double>::epsilon()));

	using namespace pressure;

	auto p1{ 1e+5_Pa };
	auto p2{ 0.1_Mpa };
	auto p3{ 0.0101936799184506_KgfperMm };
	auto p4{ 10193.6799184506_KgfperM };
	auto p5{ 2e+5_Pa };


	{
		auto ppa = PressureCast<scale::Pa>(p2);
		REQUIRE_THAT(double (p1), WithinRel(double (ppa), 1e-8));
	}
	{
		auto p_mpa = PressureCast<scale::MPa>(p1);
		REQUIRE_THAT(double(p2), WithinRel(double(p_mpa), 1e-8));
	}
	{
		auto pkgfmm = PressureCast<scale::KgfperMm>(p2);
		REQUIRE_THAT(double(p3), WithinRel(double(pkgfmm), 1e-8));
		
	}
	{
		auto pkgfm = PressureCast<scale::KgfperM>(p1);
		CHECK_THAT(double(p4), WithinRel(double(pkgfm), 1e-8));
	}
	{
		// Работа оператора <=>
		CHECK(p1 < p5);
		CHECK(p1 != p5);
		CHECK(p1 <= p5);
	}


}
