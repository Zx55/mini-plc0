#include "catch2/catch.hpp"

#include "instruction/instruction.h"
#include "tokenizer/tokenizer.h"
#include "analyser/analyser.h"
#include "simple_vm.hpp"

#include <vector>
#include <sstream>

TEST_CASE("Test 1: Assignment") {
    std::string input =
            "begin\n"
            "var a = 1;\n"
            "var b;\n"
            "var c;\n"
            "var d;\n"
            "var e;\n"
            "b = a;\n"
            "e = b;\n"
            "d = e;\n"
            "c = a;\n"
            "print(c);\n"
            "end";
    std::stringstream ss;
    ss.str(input);

    miniplc0::Tokenizer tkz(ss);
    auto tks = tkz.AllTokens();
    if (tks.second.has_value()) {
        FAIL();
    }

    miniplc0::Analyser analyser(tks.first);
    auto ins = analyser.Analyse();
    if (ins.second.has_value()) {
        FAIL();
    }

    miniplc0::VM vm(ins.first);
    auto result = vm.Run();
    auto ans = std::vector<std::int32_t>{ 1 };
    REQUIRE( result == ans );
}

TEST_CASE("Test 2: Declaration") {
    std::string input =
            "begin\n"
            "const abc = 123;\n"
            "var ABC = 456;\n"
            "print(abc);\n"
            "print(ABC);\n"
            "end";
    std::stringstream ss;
    ss.str(input);

    miniplc0::Tokenizer tkz(ss);
    auto tks = tkz.AllTokens();
    if (tks.second.has_value()) {
        FAIL();
    }

    miniplc0::Analyser analyser(tks.first);
    auto ins = analyser.Analyse();
    if (ins.second.has_value()) {
        FAIL();
    }

    miniplc0::VM vm(ins.first);
    auto result = vm.Run();
    auto ans = std::vector<std::int32_t>{ 123, 456 };
    REQUIRE( result == ans );
}

TEST_CASE("Test 3: Initialization") {
    std::string input =
            "begin\n"
            "var a = 0;\n"
            "var b = 1;\n"
            "var c = a+b;\n"
            "a = b;\n"
            "c = c;\n"
            "c = a+b;\n"
            "a = b;\n"
            "b = c;\n"
            "print(a);\n"
            "print(b);\n"
            "print(c);\n"
            "end";
    std::stringstream ss;
    ss.str(input);

    miniplc0::Tokenizer tkz(ss);
    auto tks = tkz.AllTokens();
    if (tks.second.has_value()) {
        FAIL();
    }

    miniplc0::Analyser analyser(tks.first);
    auto ins = analyser.Analyse();
    if (ins.second.has_value()) {
        FAIL();
    }

    miniplc0::VM vm(ins.first);
    auto result = vm.Run();
    auto ans = std::vector<std::int32_t>{ 1, 2, 2 };
    REQUIRE( result == ans );
}

TEST_CASE("Test 4: Missing begin and end") {
    std::string input =
            "var a = 1;\n"
            "print(a);";
    std::stringstream ss;
    ss.str(input);

    miniplc0::Tokenizer tkz(ss);
    auto tks = tkz.AllTokens();
    if (tks.second.has_value()) {
        FAIL();
    }

    miniplc0::Analyser analyser(tks.first);
    auto ins = analyser.Analyse();
    if (ins.second.has_value()) {
        SUCCEED();
    } else {
        FAIL();
    }
}

TEST_CASE("Test 5: Missing semicolon") {
    std::string input =
            "begin\n"
            "const A = 1;\n"
            "var B = A;\n"
            "print(A)\n"
            "end";
    std::stringstream ss;
    ss.str(input);

    miniplc0::Tokenizer tkz(ss);
    auto tks = tkz.AllTokens();
    if (tks.second.has_value()) {
        FAIL();
    }

    miniplc0::Analyser analyser(tks.first);
    auto ins = analyser.Analyse();
    if (ins.second.has_value()) {
        SUCCEED();
    } else {
        FAIL();
    }
}

TEST_CASE("Test 6: Redeclaration") {
    std::string input =
            "begin\n"
            "const A = 1;\n"
            "var A;\n"
            "end";
    std::stringstream ss;
    ss.str(input);

    miniplc0::Tokenizer tkz(ss);
    auto tks = tkz.AllTokens();
    if (tks.second.has_value()) {
        FAIL();
    }

    miniplc0::Analyser analyser(tks.first);
    auto ins = analyser.Analyse();
    if (ins.second.has_value()) {
        SUCCEED();
    } else {
        FAIL();
    }
}

TEST_CASE("Test 7: Uninitializtion") {
    std::string input =
            "begin\n"
            "var a;\n"
            "print(a);\n"
            "end";
    std::stringstream ss;
    ss.str(input);

    miniplc0::Tokenizer tkz(ss);
    auto tks = tkz.AllTokens();
    if (tks.second.has_value()) {
        FAIL();
    }

    miniplc0::Analyser analyser(tks.first);
    auto ins = analyser.Analyse();
    if (ins.second.has_value()) {
        SUCCEED();
    } else {
        FAIL();
    }
}

TEST_CASE("Test 8: Var-Const") {
    std::string input =
            "begin\n"
            "var a;\n"
            "const b = 1;\n"
            "end";
    std::stringstream ss;
    ss.str(input);

    miniplc0::Tokenizer tkz(ss);
    auto tks = tkz.AllTokens();
    if (tks.second.has_value()) {
        FAIL();
    }

    miniplc0::Analyser analyser(tks.first);
    auto ins = analyser.Analyse();
    if (ins.second.has_value()) {
        SUCCEED();
    } else {
        FAIL();
    }
}