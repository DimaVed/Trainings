#include "object.h"
#include "object_holder.h"
#include "statement.h"
#include "lexer.h"
#include "parse.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
using namespace   Catch::Matchers;


void RunMythonProgram(istream& input, ostream& output) {
    Ast::Print::SetOutputStream(output);

    Parse::Lexer lexer(input);
    auto program = ParseProgram(lexer);

    Runtime::Closure closure;
    program->Execute(closure);
}

#ifdef RUNPARAPYTHON
int main() {


    RunMythonProgram(cin, cout);

    return 0;
}

#endif // RUNPARAPYTHON


TEST_CASE("ParaPython TestSimplePrints", "[ParaPython]")
{
    istringstream input(R"(
print 57
print 10, 24, -8
print 'hello'
print "world"
print True, False
print
print None
)");

    ostringstream output;
    RunMythonProgram(input, output);

    CHECK(output.str() == "57\n10 24 -8\nhello\nworld\nTrue False\n\nNone\n");
}

TEST_CASE("ParaPython TestAssignments", "[ParaPython]")
 {
    istringstream input(R"(
x = 57
print x
x = 'C++ black belt'
print x
y = False
x = y
print x
x = None
print x, y
)");

    ostringstream output;
    RunMythonProgram(input, output);

    CHECK(output.str() == "57\nC++ black belt\nFalse\nNone False\n");
}

TEST_CASE("ParaPython TestArithmetics", "[ParaPython]")
{
    istringstream input(
"print 1+2+3+4+5, 1*2*3*4*5, 1-2-3-4-5, 36/4/3, 2*5+10/2"
                );
    ostringstream output;
    RunMythonProgram(input, output);
    CHECK(output.str() == "15 120 -13 3 15\n");
}

TEST_CASE("ParaPython TestVariablesArePointers", "[ParaPython]")
{
    istringstream input(R"(
class Counter:
  def __init__():
    self.value = 0

  def add():
    self.value = self.value + 1

class Dummy:
  def do_add(counter):
    counter.add()

x = Counter()
y = x

x.add()
y.add()

print x.value

d = Dummy()
d.do_add(x)

print y.value
)");

    ostringstream output;
    RunMythonProgram(input, output);

    CHECK(output.str()== "2\n3\n");
}


