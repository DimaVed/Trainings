#include <cmath>
#include <assert.h>

bool are_equal(double const d1, double const d2, double const epsilon = 0.001)
{
    return std::fabs(d1 - d2) < epsilon;
}

namespace Pressure
{
    enum class scale
    {
        Pa,
        MPa,
        KgfperMm,
        KgfperM
    };

    template <scale S>
    class quantity
    {
        const double amount;
    public:
        constexpr explicit quantity(double const a) : amount(a) {}

        explicit operator double() const { return amount; }
    };

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

    template <scale S>
    constexpr quantity<S> operator+(quantity<S> const& q1, quantity<S> const& q2)
    {
        return quantity<S>(static_cast<double>(q1) + static_cast<double>(q2));
    }

    template <scale S>
    constexpr quantity<S> operator-(quantity<S> const& q1, quantity<S> const& q2)
    {
        return quantity<S>(static_cast<double>(q1) - static_cast<double>(q2));
    }

    template <scale S, scale R>
    struct conversion_traits
    {
        static double convert(double const value) = delete;
    };

    template <>
    struct conversion_traits<scale::Pa, scale::MPa>
    {
        static double convert(double const value)
        {
            return value/1.e+6 ;
        }
    };

    template <>
    struct conversion_traits<scale::MPa, scale::Pa>
    {
        static double convert(double const value)
        {
            return value *1.e+6;
        }
    };

    template <>
    struct conversion_traits<scale::Pa, scale::KgfperMm>
    {
        static double convert(double const value)
        {
            return value/ 9.81e+6;
        }
    };

    template <>
    struct conversion_traits<scale::MPa, scale::KgfperMm>
    {
        static double convert(double const value)
        {
            return value / 9.81;
        }
    };

    template <>
    struct conversion_traits<scale::KgfperMm, scale::MPa>
    {
        static double convert(double const value)
        {
            return value*9.81;
        }
    };

    template <>
    struct conversion_traits<scale::KgfperMm, scale::Pa>
    {
        static double convert(double const value)
        {
            return value*9.81e+6;
        }
    };
    template <>
    struct conversion_traits<scale::KgfperM, scale::Pa>
    {
        static double convert(double const value)
        {
            return value * 9.81;
        }
    };
    template <>
    struct conversion_traits<scale::Pa, scale::KgfperM>
    {
        static double convert(double const value)
        {
            return value / 9.81;
        }
    };

    template <scale R, scale S>
    constexpr quantity<R> pressure_cast(quantity<S> const q)
    {
        return quantity<R>(conversion_traits<S, R>::convert(static_cast<double>(q)));
    }

   
        constexpr quantity<scale::Pa> operator "" _Pa(long double const amount)
        {
            return quantity<scale::Pa> {static_cast<double>(amount)};
        }

        constexpr quantity<scale::MPa> operator "" _Mpa(long double const amount)
        {
            return quantity<scale::MPa> {static_cast<double>(amount)};
        }

        constexpr quantity<scale::KgfperMm> operator "" _KgfperMm(long double const amount)
        {
            return quantity<scale::KgfperMm> {static_cast<double>(amount)};


        }
        constexpr quantity<scale::KgfperM> operator "" _KgfperM(long double const amount)
        {
            return quantity<scale::KgfperM> {static_cast<double>(amount)};
        }
   
}

int main()
{
    using namespace Pressure;
  
    auto P1{ 1e+5_Pa };
    auto P2{ 0.1_Mpa };
    auto P3{ 0.0101936799184506_KgfperMm };
    auto P4{ 10193.6799184506_KgfperM };
    auto P5{ 2e+5_Pa };


    {
        auto Ppa = pressure_cast<scale::Pa>(P2);        
        assert(P1 == Ppa);
    }
    {
        auto PMpa = pressure_cast<scale::MPa>(P1);
        assert((P2 == PMpa));
    }
    {
        auto Pkgfmm = pressure_cast<scale::KgfperMm>(P2);
        assert((P3 == Pkgfmm));
    }
    {
        auto Pkgfm = pressure_cast<scale::KgfperM>(P1);
        assert((P4 == Pkgfm));
    }
    {
        assert(P1 < P5);
    }

    return 0;
}
