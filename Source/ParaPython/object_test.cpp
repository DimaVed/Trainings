#include "object.h"
#include "statement.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
using namespace   Catch::Matchers;

#include <sstream>

using namespace std;

namespace Runtime {
TEST_CASE("ParaPython TestNumber", "[ParaPython]") {
  Number num(127);

  ostringstream os;
  num.Print(os);
  CHECK(os.str() == "127");
  CHECK(num.GetValue()== 127);
}

void TestString() {
  String word("hello!");

  ostringstream os;
  word.Print(os);
  CHECK(os.str() == "hello!");
  CHECK(word.GetValue() == "hello!");
}
TEST_CASE("ParaPython TestFields", "[ParaPython]")
 {
  vector<Method> methods;

  methods.push_back({
    "__init__", {}, {
      make_unique<Ast::FieldAssignment>(
        Ast::VariableValue{"self"}, "value", make_unique<Ast::NumericConst>(0)
      )
    }
  });
  methods.push_back({
    "value", {}, {make_unique<Ast::VariableValue>(vector<string>{"self", "value"})}
  });
  methods.push_back({
    "add",
    {"x"},
    {
      make_unique<Ast::FieldAssignment>(
        Ast::VariableValue{"self"},
        "value", make_unique<Ast::Add>(
          make_unique<Ast::VariableValue>(vector<string>{"self", "value"}),
          make_unique<Ast::VariableValue>("x")
        )
      )
    }
  });

  Class cls("BoxedValue", std::move(methods), nullptr);
  ClassInstance inst(cls);

  inst.Call("__init__", {});

  for (int i = 1, expected = 0; i < 10; expected += i, ++i) {
    auto fv = inst.Call("value", {});
    auto obj = fv.TryAs<Runtime::Number>();
    CHECK(obj);
    CHECK(obj->GetValue() == expected);

    inst.Call("add", {ObjectHolder::Own(Number(i))});
  }
}
TEST_CASE("ParaPython TestBaseClass", "[ParaPython]") {
  vector<Method> methods;
  methods.push_back({
    "GetValue", {}, make_unique<Ast::VariableValue>(vector{"self"s, "value"s})
  });
  methods.push_back({
    "SetValue", {"x"}, make_unique<Ast::FieldAssignment>(
      Ast::VariableValue{"self"s}, "value", make_unique<Ast::VariableValue>("x")
    )
  });

  Class cls("BoxedValue", std::move(methods), nullptr);

  CHECK(cls.GetName() == "BoxedValue");
  {
    auto m = cls.GetMethod("GetValue");
    CHECK(m);
    CHECK(m->name == "GetValue");
    CHECK(m->formal_params.empty());
  }
  {
    auto m = cls.GetMethod("SetValue");
    CHECK(m);
    CHECK(m->name == "SetValue");
    CHECK(m->formal_params.size() == 1u);
  }
  CHECK(!cls.GetMethod("AsString"));
}


TEST_CASE("ParaPython TestInheritance", "[ParaPython]")
 {
  vector<Method> methods;
  methods.push_back({
    "GetValue", {}, make_unique<Ast::VariableValue>(vector{"self"s, "value"s})
  });
  methods.push_back({
    "SetValue", {"x"}, make_unique<Ast::FieldAssignment>(
      Ast::VariableValue{"self"s}, "value", make_unique<Ast::VariableValue>("x")
    )
  });

  Class base("BoxedValue", std::move(methods), nullptr);

  methods.clear();
  methods.push_back({
    "GetValue", {"z"}, make_unique<Ast::VariableValue>("z")
  });
  methods.push_back({
    "AsString", {}, make_unique<Ast::StringConst>("value"s)
  });
  Class cls("StringableValue", std::move(methods), &base);

  CHECK(cls.GetName() == "StringableValue");
  {
    auto m = cls.GetMethod("GetValue");
    CHECK(m);
    CHECK(m->name == "GetValue");
    CHECK(m->formal_params.size() == 1u);
  }
  {
    auto m = cls.GetMethod("SetValue");
    CHECK(m);
    CHECK(m->name == "SetValue");
    CHECK(m->formal_params.size()== 1u);
  }
  {
    auto m = cls.GetMethod("AsString");
    CHECK(m);
    CHECK(m->name== "AsString");
    CHECK(m->formal_params.empty());
  }
  CHECK(!cls.GetMethod("AsStringValue"));
}


} /* namespace Runtime */
