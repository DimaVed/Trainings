#pragma once

#include "object_holder.h"

#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Ast {
  class Statement;
}

class TestRunner;

namespace Runtime {

class Object {
public:
  virtual ~Object() = default;
  virtual void Print(std::ostream& os) = 0;
};

template <typename T>
class ValueObject : public Object {
public:
   ValueObject(T v) : value(v) {
  }

  void Print(std::ostream& os) override {
     // std::cout <<value<< std::endl;
    os << value;
  }

  const T& GetValue() const {
    return value;
  }

private:
  T value;
};

using String = ValueObject<std::string>;
using Number = ValueObject<int>;

class Bool : public ValueObject<bool> {
public:
  using ValueObject<bool>::ValueObject;
  void Print(std::ostream& os) override;
};

struct Method {
  std::string name;
  std::vector<std::string> formal_params;
  std::unique_ptr<Ast::Statement> body;
};

class Class : public Object {
public:
  explicit Class (const std::string &name, std::vector<Method> methods, const Class* parent);
  const Method* GetMethod(const std::string& name) const;
  const std::string& GetName() const;
  void Print(std::ostream& os) override;
  const Class* parent_;
  std::unordered_map<std::string,Method> methods_;
  std::string name_;

};

class ClassInstance : public Object {
public:
  explicit ClassInstance(const Class& cls);

  void Print(std::ostream& os) override;

  ObjectHolder Call(const std::string& method, const std::vector<ObjectHolder>& actual_args);
  bool HasMethod(const std::string& method, size_t argument_count) const;

  Closure& Fields();
  const Closure& Fields() const;
  Closure closure_;
  const Class &cls_;
};

void RunObjectsTests(TestRunner& test_runner);

}
