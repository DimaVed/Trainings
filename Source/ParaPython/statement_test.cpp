#include "statement.h"
#include <sstream>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
using namespace   Catch::Matchers;

using namespace std;



namespace Ast {

using Runtime::Closure;

template <typename T>
void CHECKObjectValueEqual(ObjectHolder obj, T expected) {
  ostringstream one;
  obj->Print(one);

  ostringstream two;
  two << expected;
  auto lhs = one.str();
  auto rhs = two.str();
  CHECK(lhs == rhs);
}


TEST_CASE("ParaPython TestNumericConst", "[ParaPython]"){
  NumericConst num(Runtime::Number(57));
  Closure empty;

  ObjectHolder o = num.Execute(empty);
  CHECK(o==true);
  CHECK(empty.empty());

  ostringstream os;
  o->Print(os);
  CHECK(os.str() == "57");
}

TEST_CASE("ParaPython TestStringConst", "[ParaPython]"){
  StringConst value(Runtime::String("Hello!"));
  Closure empty;

  ObjectHolder o = value.Execute(empty);
  CHECK(o);
  CHECK(empty.empty());

  ostringstream os;
  o->Print(os);
  CHECK(os.str() == "Hello!");
}
TEST_CASE("ParaPython TestVariable", "[ParaPython]"){
  Runtime::Number num(42);
  Runtime::String word("Hello");

  Closure closure = {{"x", ObjectHolder::Share(num)}, {"w", ObjectHolder::Share(word)}};
  CHECK(VariableValue("x").Execute(closure).Get() == &num);
  CHECK(VariableValue("w").Execute(closure).Get() == &word);
  CHECK_THROWS_AS(VariableValue("unknown").Execute(closure), std::runtime_error);
}


TEST_CASE("ParaPython TestAssignment", "[ParaPython]")
{
  Assignment assign_x("x", make_unique<NumericConst>(Runtime::Number(57)));
  Assignment assign_y("y", make_unique<StringConst>(Runtime::String("Hello")));

  Closure closure = {{"y", ObjectHolder::Own(Runtime::Number(42))}};

  {
    ObjectHolder o = assign_x.Execute(closure);
    CHECK(o);
    CHECKObjectValueEqual(o, 57);
  }
  CHECK(closure.find("x") != closure.end());
  CHECKObjectValueEqual(closure.at("x"), 57);

  {
    ObjectHolder o = assign_y.Execute(closure);
    CHECK(o);
    CHECKObjectValueEqual(o, "Hello");
  }
  CHECK(closure.find("y") != closure.end());
  CHECKObjectValueEqual(closure.at("y"), "Hello");
}

TEST_CASE("ParaPython TestFieldAssignment", "[ParaPython]")
{
  Runtime::Class empty("Empty", {}, nullptr);
  Runtime::ClassInstance object{empty};

  FieldAssignment assign_x(
    VariableValue{"self"}, "x", make_unique<NumericConst>(Runtime::Number(57))
  );
  FieldAssignment assign_y(
    VariableValue{"self"}, "y", make_unique<NewInstance>(empty)
  );

  Closure closure = {{"self", ObjectHolder::Share(object)}};

  {
    ObjectHolder o = assign_x.Execute(closure);
    CHECK(o);
    CHECKObjectValueEqual(o, 57);
  }
  CHECK(object.Fields().find("x") != object.Fields().end());
  CHECKObjectValueEqual(object.Fields().at("x"), 57);

  assign_y.Execute(closure);
  FieldAssignment assign_yz(
    VariableValue{vector<string>{"self", "y"}}, "z", make_unique<StringConst>(
      Runtime::String("Hello, world! Hooray! Yes-yes!!!")
    )
  );
  {
    ObjectHolder o = assign_yz.Execute(closure);
    CHECK(o);
    CHECKObjectValueEqual(o, "Hello, world! Hooray! Yes-yes!!!");
  }

  CHECK(object.Fields().find("y") != object.Fields().end());
  auto subobject = object.Fields().at("y").TryAs<Runtime::ClassInstance>();
  CHECK(!(subobject && subobject->Fields().find("z") == subobject->Fields().end()));
 // CHECKObjectValueEqual(subobject->Fields().at("z"), "Hello, world! Hooray! Yes-yes!!!");
}


TEST_CASE("ParaPython TestFieTestPrintVariableldAssignment", "[ParaPython]")
 {
  ostringstream os;
  Print::SetOutputStream(os);

  Closure closure = {{"y", ObjectHolder::Own(Runtime::Number(42))}};

  auto print_statement = Print::Variable("y");
  print_statement->Execute(closure);

  CHECK(os.str() == "42\n");
}
TEST_CASE("ParaPython TestPrintMultipleStatements", "[ParaPython]"){
  ostringstream os;
  Print::SetOutputStream(os);

  Runtime::String hello("hello");
  Closure closure = {
    {"word", ObjectHolder::Share(hello)},
    {"empty", ObjectHolder::None()}
  };

  vector<unique_ptr<Statement>> args;
  args.push_back(make_unique<VariableValue>("word"));
  args.push_back(make_unique<NumericConst>(57));
  args.push_back(make_unique<StringConst>("Python"s));
  args.push_back(make_unique<VariableValue>("empty"));

  Print(std::move(args)).Execute(closure);

  CHECK(os.str() == "hello 57 Python None\n");
}
TEST_CASE("ParaPython TestStringify", "[ParaPython]")
 {
  Closure empty;

  {
    auto res = Stringify(make_unique<NumericConst>(57));
        auto result =res.Execute(empty);
        CHECKObjectValueEqual(result, "57");
    CHECK(result.TryAs<Runtime::String>());
  }
  {
    auto result = Stringify(make_unique<StringConst>("Wazzup!"s)).Execute(empty);
    CHECKObjectValueEqual(result, "Wazzup!"s);
    CHECK(result.TryAs<Runtime::String>());
  }
  {
    vector<Runtime::Method> methods;
    methods.push_back({"__str__", {}, make_unique<NumericConst>(842)});

    Runtime::Class cls("BoxedValue", std::move(methods), nullptr);
    auto item = make_unique<NewInstance>(cls);
    auto resu = Stringify(move(item));
        auto result =  resu.Execute(empty);
        CHECKObjectValueEqual(result, "842"s);
    CHECK(result.TryAs<Runtime::String>());
  }
  {
    Runtime::Class cls("BoxedValue", {}, nullptr);
    Runtime::Closure closure{{"x", ObjectHolder::Own(Runtime::ClassInstance{cls})}};

    std::ostringstream expected_output;
    expected_output << closure.at("x").Get();

    Stringify str(make_unique<VariableValue>("x"));
    CHECKObjectValueEqual(str.Execute(closure), expected_output.str());
  }
}
TEST_CASE("ParaPython TestNumbersAddition", "[ParaPython]"){
  Add sum(
    make_unique<NumericConst>(23),
    make_unique<NumericConst>(34)
  );

  Closure empty;
  CHECKObjectValueEqual(sum.Execute(empty), 57);
}

void TestStringsAddition() {
  Add sum(
    make_unique<StringConst>("23"s),
    make_unique<StringConst>("34"s)
  );

  Closure empty;
  CHECKObjectValueEqual(sum.Execute(empty), "2334");
}

void TestBadAddition() {
  Closure empty;

  CHECK_THROWS_AS(
    Add(make_unique<NumericConst>(42), make_unique<StringConst>("4"s)).Execute(empty),
    std::runtime_error
  );
  CHECK_THROWS_AS(
    Add(make_unique<StringConst>("4"s), make_unique<NumericConst>(42)).Execute(empty),
    std::runtime_error
  );
  CHECK_THROWS_AS(
    Add(make_unique<None>(), make_unique<StringConst>("4"s)).Execute(empty),
    std::runtime_error
  );
  CHECK_THROWS_AS(
    Add(make_unique<None>(), make_unique<None>()).Execute(empty),
    std::runtime_error
  );
}
TEST_CASE("ParaPython TestSuccessfullClassInstanceAdd", "[ParaPython]") {
  vector<Runtime::Method> methods;
  methods.push_back({
    "__add__",
    {"value"},
    make_unique<Add>(make_unique<StringConst>("hello, "s), make_unique<VariableValue>("value"))
  });

  Runtime::Class cls("BoxedValue", std::move(methods), nullptr);

  Closure empty;
  auto res = Add(
    make_unique<NewInstance>(cls), make_unique<StringConst>("world"s)
  );
     auto result  =   res.Execute(empty);
     CHECKObjectValueEqual(result, "hello, world");
}
TEST_CASE("ParaPython TestClassInstanceAddWithoutMethod", "[ParaPython]"){
  Runtime::Class cls("BoxedValue", {}, nullptr);

  Closure empty;
  Add addition(
    make_unique<NewInstance>(cls), make_unique<StringConst>("world"s)
  );
  CHECK_THROWS_AS(addition.Execute(empty), std::runtime_error);
}

TEST_CASE("ParaPython TestCompound", "[ParaPython]")
{
  Compound cpd{
    make_unique<Assignment>("x", make_unique<StringConst>("one"s)),
    make_unique<Assignment>("y", make_unique<NumericConst>(2)),
    make_unique<Assignment>("z", make_unique<VariableValue>("x"s)),
  };

  Closure closure;
  auto result = cpd.Execute(closure);

  CHECKObjectValueEqual(closure.at("x"), "one");
  CHECKObjectValueEqual(closure.at("y"), 2);
  CHECKObjectValueEqual(closure.at("z"), "one");

  CHECK(!result);
}
TEST_CASE("ParaPython TestReturn", "[ParaPython]")
{
   Closure clos;
      Return  a(make_unique<NumericConst>(2));
     auto res = a.Execute(clos);
     CHECKObjectValueEqual(res, 2);
}


} /* namespace Ast */
