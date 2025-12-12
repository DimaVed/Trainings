#include <cmath>
#include <assert.h>
#include <map>
#include <vector>
#include <string>

#pragma warning( push )
#pragma warning( disable : 26812)
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/variant2/variant.hpp>
#include <boost/json.hpp>
//#include <boost/version.hpp>
#include <type_traits>
#include <iostream>
#include <ostream>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/core/nvp.hpp>
#include <type_traits>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#pragma warning( pop ) 
using namespace   Catch::Matchers;
using namespace boost::describe;


// универсальные функции для сереализации/ десериализации enum 

template<class E> char const* enum_to_string(E e)
{
    char const* r = "(unnamed)";

    boost::mp11::mp_for_each< boost::describe::describe_enumerators<E> >([&](auto D) {

        if (e == D.value) r = D.name;

        });

    return r;
}

[[noreturn]] void throw_invalid_name(char const* name, char const* type)
{
    throw std::runtime_error(
        std::string("Invalid enumerator name '") + name
        + "' for enum type '" + type + "'");
}

template<class E> E string_to_enum(char const* name)
{
    bool found = false;
    E r = {};

    boost::mp11::mp_for_each< boost::describe::describe_enumerators<E> >([&](auto D) {

        if (!found && std::strcmp(D.name, name) == 0)
        {
            found = true;
            r = D.value;
        }

        });

    if (found)
    {
        return r;
    }
    else
    {
        throw_invalid_name(name, typeid(E).name());
    }
}




// Универсальный оператор сравнения для объектов с описанием 
template<class T,
    class Bd = describe_bases<T, mod_any_access>,
    class Md = describe_members<T, mod_any_access>,
    class En = std::enable_if_t<!std::is_union<T>::value> >
    bool operator==(T const& t1, T const& t2)
{
    bool r = true;

    boost::mp11::mp_for_each<Bd>([&](auto D) {

        using B = typename decltype(D)::type;
        r = r && (B const&)t1 == (B const&)t2;

        });

    boost::mp11::mp_for_each<Md>([&](auto D) {

        r = r && t1.*D.pointer == t2.*D.pointer;

        });

    return r;
}


// универсальный метод serialize
template<class Archive, class T,
    class D1 = boost::describe::describe_bases<T, boost::describe::mod_public>,
    class D2 = boost::describe::describe_bases<T,
    boost::describe::mod_protected | boost::describe::mod_private>,
    class D3 = boost::describe::describe_members<T,
    boost::describe::mod_public | boost::describe::mod_protected>,
    class D4 = boost::describe::describe_members<T, boost::describe::mod_private>,
    class En = std::enable_if_t< boost::mp11::mp_empty<D2>::value&&
    boost::mp11::mp_empty<D4>::value && !std::is_union<T>::value> >

    void serialize(Archive& ar, T& t, boost::serialization::version_type)
{
    int k = 0;

    // public bases: use base_object

    boost::mp11::mp_for_each<D1>([&](auto D) {

        using B = typename decltype(D)::type;

        char name[32];
        std::sprintf(name, "base.%d", ++k);

        ar& boost::make_nvp(name, boost::serialization::base_object<B>(t));

        });

    // public (and protected) members

    boost::mp11::mp_for_each<D3>([&](auto D) {

        ar& boost::make_nvp(D.name, t.*D.pointer);

        });
}



// Макрос для определения перечисления с одновременным описанием
BOOST_DEFINE_ENUM_CLASS(Animals, dog, cat,elepant)





struct point {
    int x, y;
};

BOOST_DESCRIBE_STRUCT(point, (), (x, y));

 struct cat {
     point coords;
     bool is_alive = true;
	 std::string name;
	 int age;
     std::vector <int> times;
     std::map <std::string, double> result;
 };

 BOOST_DESCRIBE_STRUCT(cat, (), (coords, is_alive ,name, age, times, result))

TEST_CASE (" json reflection", "[Reflection]") {

    // класс кота
     cat Vasyan{ {1,1}, true, "vasyan", 3, {}, {} };
    Vasyan.times = std::vector <int>{ 1,2,3,4,5 };
    Vasyan.result["res1"] = 234.;
        Vasyan.result["res2"] = 123214534.;

    // сериализация в json
   auto json_obj = boost::json::value_from(Vasyan);
   auto json_str = boost::json::serialize(json_obj);
   std::cout << json_str << std::endl;
   auto json2 = boost::json::parse(json_str);
 

   cat Vasyan2 = boost::json::value_to <cat>(json_obj);
   CHECK(Vasyan.name == Vasyan2.name);
   // Использование автосгенерированного оператора
   CHECK(Vasyan == Vasyan2);


}
 TEST_CASE("ENUM reflection ", "[Reflection]") {
     // Сериализация через строку 
     auto animal_type = string_to_enum<Animals>("cat");
     auto str = enum_to_string<Animals>(animal_type);
     CHECK(str == std::string{ "cat" });

 }
 TEST_CASE("Output Reflection ", "[Reflection]") {

     point tmp_point{ 1,1 };

     // Печать точки  
     //std::cout << tmp_point << std::endl;
     // для работы оператора необходимо перегрузить операторы вывода для словаря и вектора и вытащить его в общую область

 }

 TEST_CASE("NOt full data for class ", "[Reflection]") {

     //  is_alive and name undefined 

     std::string raw = R"({"coords":{"x":1,"y":1},"age":3,"times":[1,2,3,4,5],"result":{"res1":2.34E2,"res2":1.23214534E8}})";
     auto inp = boost::json::parse(raw);
     try {
         cat Vasyan2 = boost::json::value_to <cat>(inp);
     }
     // if some class data undefined you will get an exception
     catch (std::exception& ex) {
         std::cout << "exception" << ex.what();
     }


 }


 TEST_CASE("Archive serialization Reflection ", "[Reflection]") {

     // класс кота
     cat vasyan{ {1,1}, true, "vasyan", 3, {}, {} };
     vasyan.times = std::vector <int>{ 1,2,3,4,5 };
     vasyan.result["res1"] = 234.;
     vasyan.result["res2"] = 123214534.;

     std::ostringstream os;

     {
         boost::archive::xml_oarchive ar(os);
         ar << boost::make_nvp("c1", vasyan);
     }

     std::string s = os.str();

     std::cout << s << std::endl;

     cat vasyan2;

     {
         std::istringstream is(s);
         boost::archive::xml_iarchive ar(is);
         ar >> boost::make_nvp("c2", vasyan2);
     }

     {
         boost::archive::text_oarchive ar(std::cout);
         ar << vasyan2;
     }
     CHECK(vasyan == vasyan2);



 }








