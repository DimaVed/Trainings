#include "lexer.h"


#include <string>
#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
using namespace   Catch::Matchers;
using namespace Parse;



TEST_CASE("ParaPython TestSimpleAssignment", "[ParaPython]"){

  std::istringstream input("x = 42\n");
  Parse::Lexer lexer(input);

  CHECK(lexer.CurrentToken() == Token(TokenType::Id{"x"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{42}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
}


TEST_CASE("ParaPython TestKeywords", "[ParaPython]") {
  std::istringstream input("class return if else def print or None and not True False");
  Parse::Lexer lexer(input);

  CHECK(lexer.CurrentToken()== Token(TokenType::Class{}));
  CHECK(lexer.NextToken()== Token(TokenType::Return{}));
  CHECK(lexer.NextToken()== Token(TokenType::If{}));
  CHECK(lexer.NextToken()== Token(TokenType::Else{}));
  CHECK(lexer.NextToken()== Token(TokenType::Def{}));
  CHECK(lexer.NextToken()== Token(TokenType::Print{}));
  CHECK(lexer.NextToken()== Token(TokenType::Or{}));
  CHECK(lexer.NextToken()== Token(TokenType::None{}));
  CHECK(lexer.NextToken()== Token(TokenType::And{}));
  CHECK(lexer.NextToken()== Token(TokenType::Not{}));
  CHECK(lexer.NextToken()== Token(TokenType::True{}));
  CHECK(lexer.NextToken()== Token(TokenType::False{}));
}
TEST_CASE("ParaPython TestNumbers", "[ParaPython]")
 {
  std::istringstream input("42 15 -53");
  Parse::Lexer lexer(input);

  CHECK(lexer.CurrentToken()== Token(TokenType::Number{42}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{15}));
  // Отрицательные числа формируются на этапе синтаксического анализа
  CHECK(lexer.NextToken()== Token(TokenType::Char{'-'}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{53}));
}
TEST_CASE("ParaPython TestIds", "[ParaPython]")
 {
  std::istringstream input("x    _42 big_number   Return Class  dEf");
  Parse::Lexer lexer(input);

  CHECK(lexer.CurrentToken() == Token(TokenType::Id{"x"}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"_42"}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"big_number"}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"Return"})); // keywords are case-sensitive
  CHECK(lexer.NextToken()== Token(TokenType::Id{"Class"}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"dEf"}));
}
TEST_CASE("ParaPython TestStrings", "[ParaPython]") {
 std::istringstream input(R"('word' "two words" 'long string with a double quote " inside' "another long string with single quote ' inside")");
 Parse::Lexer lexer(input);

  CHECK(lexer.CurrentToken()== Token(TokenType::String{"word"}));
  CHECK(lexer.NextToken()== Token(TokenType::String{"two words"}));
  CHECK(
    lexer.NextToken()== Token(TokenType::String{"long string with a double quote \" inside"})
  );
  CHECK(
    lexer.NextToken()== Token(TokenType::String{"another long string with single quote ' inside"})
  );
}
TEST_CASE("ParaPython TestOperations", "[ParaPython]") {
 std::istringstream input("+-*/= > < != == <> <= >=");
 Parse::Lexer lexer(input);

  CHECK(lexer.CurrentToken()== Token(TokenType::Char{'+'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'-'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'*'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'/'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'>'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'<'}));
  CHECK(lexer.NextToken()== Token(TokenType::NotEq{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eq{}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'<'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'>'}));
  CHECK(lexer.NextToken()== Token(TokenType::LessOrEq{}));
  CHECK(lexer.NextToken()== Token(TokenType::GreaterOrEq{}));
}

TEST_CASE("ParaPython TestIndentsAndNewlines", "[ParaPython]") {
 std::istringstream input(R"(
no_indent
  indent_one
    indent_two
      indent_three
      indent_three
      indent_three
    indent_two
  indent_one
    indent_two
no_indent
)");

 Parse::Lexer lexer(input);

  CHECK(lexer.CurrentToken()== Token(TokenType::Id{"no_indent"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Indent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"indent_one"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Indent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"indent_two"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Indent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"indent_three"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"indent_three"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"indent_three"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Dedent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"indent_two"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Dedent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"indent_one"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Indent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"indent_two"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Dedent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Dedent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"no_indent"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
}

TEST_CASE("ParaPython TestEmptyLinesAreIgnored", "[ParaPython]"){
  std::istringstream input(R"(
x = 1
  y = 2

  z = 3


)");
  Parse::Lexer lexer(input);

  CHECK(lexer.CurrentToken() == Token(TokenType::Id{"x"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{1}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Indent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"y"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{2}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  // Пустая строка, состоящая только из пробельных символов не меняет текущий отступ,
  // поэтому следующая лексема — это Id, а не Dedent
  CHECK(lexer.NextToken()== Token(TokenType::Id{"z"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{3}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Dedent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
}

TEST_CASE("ParaPython TestMythonProgram", "[ParaPython]"){
  std::istringstream input(R"(
x = 4
y = "hello"

class Point:
  def __init__(self, x, y):
    self.x = x
    self.y = y

  def __str__(self):
    return str(x) + ' ' + str(y)

p = Point(1, 2)
print str(p)
)");
  Parse::Lexer lexer(input);

  using namespace Parse::TokenType;

  CHECK(lexer.CurrentToken() == Token(TokenType::Id{"x"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{4}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"y"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::String{"hello"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Class{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"Point"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{':'}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Indent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Def{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"__init__"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'('}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"self"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{','}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"x"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{','}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"y"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{')'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{':'}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Indent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"self"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'.'}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"x"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"x"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"self"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'.'}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"y"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"y"}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Dedent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Def{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"__str__"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'('}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"self"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{')'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{':'}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Indent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Return{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"str"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'('}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"x"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{')'}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'+'}));
  CHECK(lexer.NextToken()== Token(TokenType::String{" "}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'+'}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"str"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'('}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"y"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{')'}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Dedent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Dedent{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"p"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'='}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"Point"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'('}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{1}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{','}));
  CHECK(lexer.NextToken()== Token(TokenType::Number{2}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{')'}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Print{}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"str"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{'('}));
  CHECK(lexer.NextToken()== Token(TokenType::Id{"p"}));
  CHECK(lexer.NextToken()== Token(TokenType::Char{')'}));
  CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
  CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
}


TEST_CASE("ParaPython TestExpect", "[ParaPython]"){
  std::istringstream is("bugaga");
  Lexer lex(is);

  CHECK_NOTHROW (lex.Expect<TokenType::Id>());
  CHECK(lex.Expect<TokenType::Id>().value == "bugaga");
  CHECK_NOTHROW(lex.Expect<TokenType::Id>("bugaga"));
  CHECK_THROWS_AS(lex.Expect<TokenType::Id>("widget"), LexerError);
  CHECK_THROWS_AS(lex.Expect<TokenType::Return>(), LexerError);
}


TEST_CASE("ParaPython TestExpectNext", "[ParaPython]") {
 std::istringstream is("+ bugaga + def 52");
  Lexer lex(is);

  CHECK(lex.CurrentToken()== Token(TokenType::Char{'+'}));
  CHECK_NOTHROW(lex.ExpectNext<TokenType::Id>());
  CHECK_NOTHROW(lex.ExpectNext<TokenType::Char>('+'));
  CHECK_THROWS_AS(lex.ExpectNext<TokenType::Newline>(), LexerError);
  CHECK_THROWS_AS(lex.ExpectNext<TokenType::Number>(57), LexerError);
}


TEST_CASE("ParaPython TestAlwaysEmitsNewlineAtTheEndOfNonemptyLine", "[ParaPython]"){
  {
    std::istringstream is("a b");
    Lexer lexer(is);

    CHECK(lexer.CurrentToken()== Token(TokenType::Id{"a"}));
    CHECK(lexer.NextToken()== Token(TokenType::Id{"b"}));
    CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
    CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
    CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
    CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
  }
  {
    std::istringstream is("+");
    Lexer lexer(is);

    CHECK(lexer.CurrentToken() == Token(TokenType::Char{'+'}));
    CHECK(lexer.NextToken()== Token(TokenType::Newline{}));
    CHECK(lexer.NextToken()== Token(TokenType::Eof{}));
  }
}




