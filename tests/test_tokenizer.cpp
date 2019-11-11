#include "catch2/catch.hpp"
#include "tokenizer/tokenizer.h"
#include "fmt/core.h"

#include <sstream>
#include <vector>

// 下面是示例如何书写测试用例
TEST_CASE("Test hello world.") {
	std::string input =
		"begin\n"
		"    var a = 1;\n"
		"    const b = 1\n"
		"    print(a+b);\n"
		"end\n";
	std::stringstream ss;
	ss.str(input);

	miniplc0::Tokenizer tkz(ss);
	auto result = tkz.AllTokens();
	if (result.second.has_value()) {
		FAIL();
	}

    std::vector<miniplc0::Token> output = {
	        miniplc0::Token(miniplc0::TokenType::BEGIN, std::string("begin"), { 0, 0 }, { 0, 5 }),
	        miniplc0::Token(miniplc0::TokenType::VAR, std::string("var"), { 1, 4 }, { 1, 7 }),
	        miniplc0::Token(miniplc0::TokenType::IDENTIFIER, std::string("a"), { 1, 8 }, { 1, 9 }),
	        miniplc0::Token(miniplc0::TokenType::EQUAL_SIGN, '=', { 1, 10 }, { 1, 11 }),
	        miniplc0::Token(miniplc0::TokenType::UNSIGNED_INTEGER, std::string("1"), { 1, 12 }, { 1, 13 }),
	        miniplc0::Token(miniplc0::TokenType::SEMICOLON, ';', { 1, 13 }, { 1, 14 }),
	        miniplc0::Token(miniplc0::TokenType::CONST, std::string("const"), { 2, 4 }, { 2, 9 }),
	        miniplc0::Token(miniplc0::TokenType::IDENTIFIER, std::string("b"), { 2, 10 }, { 2, 11 }),
	        miniplc0::Token(miniplc0::TokenType::EQUAL_SIGN, '=', { 2, 12 }, { 2, 13 }),
	        miniplc0::Token(miniplc0::TokenType::UNSIGNED_INTEGER, std::string("1"), { 2, 14 }, { 2, 15 }),
	        miniplc0::Token(miniplc0::TokenType::PRINT, std::string("print"), { 3, 4 }, { 3, 9 }),
	        miniplc0::Token(miniplc0::TokenType::LEFT_BRACKET, '(', { 3, 9 }, { 3, 10 }),
	        miniplc0::Token(miniplc0::TokenType::IDENTIFIER, std::string("a"), { 3, 10 }, { 3, 11 }),
	        miniplc0::Token(miniplc0::TokenType::PLUS_SIGN, '+', { 3, 11 }, { 3, 12 }),
	        miniplc0::Token(miniplc0::TokenType::IDENTIFIER, std::string("b"), { 3, 12 }, { 3, 13 }),
	        miniplc0::Token(miniplc0::TokenType::RIGHT_BRACKET, ')', { 3, 13 }, { 3, 14 }),
	        miniplc0::Token(miniplc0::TokenType::SEMICOLON, ';', { 3, 14 }, { 3, 15 }),
	        miniplc0::Token(miniplc0::TokenType::END, std::string("end"), { 4, 0 }, { 4, 3 })
	};
	REQUIRE( (result.first == output) );
}