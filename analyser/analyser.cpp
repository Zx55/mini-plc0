#include "analyser.h"

#include <climits>

namespace miniplc0 {
	std::pair<std::vector<Instruction>, std::optional<CompilationError>> Analyser::Analyse() {
		auto err = analyseProgram();
		if (err.has_value())
			return std::make_pair(std::vector<Instruction>(), err);
		else
			return std::make_pair(_instructions, std::optional<CompilationError>());
	}

	// <程序> ::= 'begin'<主过程>'end'
	std::optional<CompilationError> Analyser::analyseProgram() {
		// 示例函数，示例如何调用子程序

		// 'begin'
		auto bg = nextToken();
		if (!bg.has_value() || bg.value().GetType() != TokenType::BEGIN)
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrNoBegin);

		// <主过程>
		auto err = analyseMain();
		if (err.has_value())
			return err;

		// 'end'
		auto ed = nextToken();
		if (!ed.has_value() || ed.value().GetType() != TokenType::END)
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrNoEnd);
		return {};
	}

	// <主过程> ::= <常量声明><变量声明><语句序列>
	// 需要补全
	std::optional<CompilationError> Analyser::analyseMain() {
		// 完全可以参照 <程序> 编写

		// <常量声明>
		if (auto err = analyseConstantDeclaration(); err.has_value()) {
		    return err;
		}

		// <变量声明>
		if (auto err = analyseVariableDeclaration(); err.has_value()) {
		    return err;
		}

		// <语句序列>
		if (auto err = analyseStatementSequence(); err.has_value()) {
		    return err;
		}

		return {};
	}

	// <常量声明> ::= {<常量声明语句>}
	// <常量声明语句> ::= 'const'<标识符>'='<常表达式>';'
	std::optional<CompilationError> Analyser::analyseConstantDeclaration() {
		// 示例函数，示例如何分析常量声明

		// 常量声明语句可能有 0 或无数个
		while (true) {
			// 预读一个 token，不然不知道是否应该用 <常量声明> 推导
			auto next = nextToken();
			if (!next.has_value())
				return {};
			// 如果是 const 那么说明应该推导 <常量声明> 否则直接返回
			if (next.value().GetType() != TokenType::CONST) {
				unreadToken();
				return {};
			}

			// <常量声明语句>
			next = nextToken();
			if (!next.has_value() || next.value().GetType() != TokenType::IDENTIFIER)
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrNeedIdentifier);
			if (isDeclared(next.value().GetValueString()))
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrDuplicateDeclaration);
			addConstant(next.value());

			// '='
			next = nextToken();
			if (!next.has_value() || next.value().GetType() != TokenType::EQUAL_SIGN)
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrConstantNeedValue);

			// <常表达式>
			int32_t val;
			auto err = analyseConstantExpression(val);
			if (err.has_value())
				return err;

			// ';'
			next = nextToken();
			if (!next.has_value() || next.value().GetType() != TokenType::SEMICOLON)
				return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrNoSemicolon);
			// 生成一次 LIT 指令加载常量
			_instructions.emplace_back(Operation::LIT, val);
		}
		return {};
	}

	// <变量声明> ::= {<变量声明语句>}
	// <变量声明语句> ::= 'var'<标识符>['='<表达式>]';'
	// 需要补全
	std::optional<CompilationError> Analyser::analyseVariableDeclaration() {
		// 变量声明语句可能有一个或者多个
        while (true) {
            // 预读
            auto next = nextToken();
            if (!next.has_value()) {
                return {};
            }

            // var
            if (next.value().GetType() != TokenType::VAR) {
                unreadToken();
                return {};
            }

            // <标识符>
            next = nextToken();
            if (!next.has_value() || next.value().GetType() != TokenType::IDENTIFIER) {
                return CompilationError(_current_pos, ErrorCode::ErrNeedIdentifier);
            } else if (isDeclared(next.value().GetValueString())) {
                return CompilationError(_current_pos, ErrorCode::ErrDuplicateDeclaration);
            }
            auto token = next.value();

            next = nextToken();
            // [ '=' ]
            if (next.has_value()) {
                if (next.value().GetType() == TokenType::EQUAL_SIGN) {
                    // 考虑初始化
                    // [ '<表达式>' ]
                    if (auto err = analyseExpression(); err.has_value()) {
                        return err;
                    }
                    addVariable(token);
                } else {
                    // 仅仅声明变量
                    unreadToken();
                    addUninitializedVariable(token);
                    _instructions.emplace_back(Operation::LIT, 0);
                }
            }

            // ;
            next = nextToken();
            if (!next.has_value() || next.value().GetType() != TokenType::SEMICOLON) {
                return CompilationError(_current_pos, ErrorCode::ErrNoSemicolon);
            }
        }
		return {};
	}

	// <语句序列> ::= {<语句>}
	// <语句> :: = <赋值语句> | <输出语句> | <空语句>
	// <赋值语句> :: = <标识符>'='<表达式>';'
	// <输出语句> :: = 'print' '(' <表达式> ')' ';'
	// <空语句> :: = ';'
	// 需要补全
	std::optional<CompilationError> Analyser::analyseStatementSequence() {
		while (true) {
			// 预读
			auto next = nextToken();
			if (!next.has_value())
				return {};

			unreadToken();
			if (next.value().GetType() != TokenType::IDENTIFIER &&
				next.value().GetType() != TokenType::PRINT &&
				next.value().GetType() != TokenType::SEMICOLON) {
				return {};
			}

			switch (next.value().GetType()) {
				// 这里需要你针对不同的预读结果来调用不同的子程序
				// 注意我们没有针对空语句单独声明一个函数，因此可以直接在这里返回
			    case TokenType::IDENTIFIER: {
			        if (auto err = analyseAssignmentStatement(); err.has_value()) {
			            return err;
			        }
			        break;
			    }
			    case TokenType::PRINT: {
			        if (auto err = analyseOutputStatement(); err.has_value()) {
			            return err;
			        }
			        break;
			    }
			    case TokenType::SEMICOLON: {
			        // <空语句> ::= ';'
                    nextToken();
			        break;
			    }
			    default:
				    break;
			}
		}
		return {};
	}

	// <常表达式> ::= [<符号>]<无符号整数>
	// 需要补全
	std::optional<CompilationError> Analyser::analyseConstantExpression(int32_t& out) {
		// out 是常表达式的结果
		// 这里你要分析常表达式并且计算结果
		// 注意以下均为常表达式
		// +1 -1 1
		// 同时要注意是否溢出
		int64_t val;

		auto next = nextToken();
	label1:
		if (!next.has_value()) {
		    return CompilationError(_current_pos, ErrorCode::ErrIncompleteExpression);
		}

		switch (next.value().GetType()) {
            case TokenType::MINUS_SIGN: {
                next = nextToken();
                if (!next.has_value() || next.value().GetType() != TokenType::UNSIGNED_INTEGER) {
                    return CompilationError(_current_pos, ErrorCode::ErrIncompleteExpression);
                }

                try {
                    if (val = -1 * static_cast<int64_t>(std::any_cast<int32_t>(next.value().GetValue()));
                        val > static_cast<int64_t>(INT32_MAX) || val < static_cast<int64_t>(INT32_MIN)) {
                        return CompilationError(_current_pos, ErrorCode::ErrIntegerOverflow);
                    }

                    out = static_cast<int32_t>(val);
                    return {};
                } catch (const std::bad_any_cast &) {
                    return CompilationError(_current_pos, ErrorCode::ErrInvalidInput);
                }

                break;
            }

            case TokenType::PLUS_SIGN: {
                next = nextToken();
                goto label1;
            }

            case TokenType::UNSIGNED_INTEGER: {
                val = static_cast<std::int64_t>(std::any_cast<std::int32_t>(next.value().GetValue()));
                if (val > static_cast<std::int64_t>(INT32_MAX) || val < static_cast<std::int64_t>(INT32_MIN)) {
                    return CompilationError(_current_pos, ErrorCode::ErrIntegerOverflow);
                }

                out = static_cast<std::int32_t>(val);
                return {};
            }

            default:
                return CompilationError(_current_pos, ErrorCode::ErrIncompleteExpression);
        }
	}

	// <表达式> ::= <项>{<加法型运算符><项>}
	std::optional<CompilationError> Analyser::analyseExpression() {
		// <项>
		auto err = analyseItem();
		if (err.has_value())
			return err;

		// {<加法型运算符><项>}
		while (true) {
			// 预读
			auto next = nextToken();
			if (!next.has_value())
				return {};
			auto type = next.value().GetType();
			if (type != TokenType::PLUS_SIGN && type != TokenType::MINUS_SIGN) {
				unreadToken();
				return {};
			}

			// <项>
			err = analyseItem();
			if (err.has_value())
				return err;

			// 根据结果生成指令
			if (type == TokenType::PLUS_SIGN)
				_instructions.emplace_back(Operation::ADD, 0);
			else if (type == TokenType::MINUS_SIGN)
				_instructions.emplace_back(Operation::SUB, 0);
		}
		return {};
	}

	// <赋值语句> ::= <标识符>'='<表达式>';'
	// 需要补全
	std::optional<CompilationError> Analyser::analyseAssignmentStatement() {
	    // <标识符>
	    auto next = nextToken();
	    auto id_str = next.value().GetValueString();

	    if (isConstant(id_str)) {
	        return CompilationError(_current_pos, ErrorCode::ErrAssignToConstant);
	    } else if (!isInitializedVariable(id_str) && !isUninitializedVariable(id_str)) {
	        return CompilationError(_current_pos, ErrorCode::ErrInvalidIdentifier);
	    }

	    auto id_index = getIndex(id_str);

	    // '='
	    next = nextToken();
	    if (!next.has_value() || next.value().GetType() != TokenType::EQUAL_SIGN) {
	        return CompilationError(_current_pos, ErrorCode::ErrInvalidVariableDeclaration);
	    }

	    // <表达式>
	    if (auto err = analyseExpression(); err.has_value()) {
	        return err;
	    }
	    _instructions.emplace_back(Operation::STO, id_index);
	    if (isUninitializedVariable(id_str)) {
	        _uninitialized_vars.erase(id_str);
	        _vars[id_str] = id_index;
	    }

	    // ';'
	    next = nextToken();
	    if (!next.has_value() || next.value().GetType() != TokenType::SEMICOLON) {
	        return CompilationError(_current_pos, ErrorCode::ErrNoSemicolon);
	    }

		return {};
	}

	// <输出语句> ::= 'print' '(' <表达式> ')' ';'
	std::optional<CompilationError> Analyser::analyseOutputStatement() {
		// 如果之前 <语句序列> 的实现正确，这里第一个 next 一定是 TokenType::PRINT
		auto next = nextToken();

		// '('
		next = nextToken();
		if (!next.has_value() || next.value().GetType() != TokenType::LEFT_BRACKET)
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrInvalidPrint);

		// <表达式>
		auto err = analyseExpression();
		if (err.has_value())
			return err;

		// ')'
		next = nextToken();
		if (!next.has_value() || next.value().GetType() != TokenType::RIGHT_BRACKET)
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrInvalidPrint);

		// ';'
		next = nextToken();
		if (!next.has_value() || next.value().GetType() != TokenType::SEMICOLON)
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrNoSemicolon);

		// 生成相应的指令 WRT
		_instructions.emplace_back(Operation::WRT, 0);
		return {};
	}

	// <项> :: = <因子>{ <乘法型运算符><因子> }
	// 需要补全
	std::optional<CompilationError> Analyser::analyseItem() {
        // <项>
	    if (auto err = analyseFactor(); err.has_value()) {
            return err;
        }

	    // { <乘法型运算符><因子> }
        while (true) {
            // 预读
            auto next = nextToken();
            if (!next.has_value()) {
                return {};
            }

            auto type = next.value().GetType();
            if (type != TokenType::MULTIPLICATION_SIGN && type != TokenType::DIVISION_SIGN) {
                unreadToken();
                return {};
            }

            // <因子>
            if (auto err = analyseFactor(); err.has_value()) {
                return err;
            }

            if (type == TokenType::MULTIPLICATION_SIGN) {
                _instructions.emplace_back(Operation::MUL, 0);
            } else if (type == TokenType::DIVISION_SIGN) {
                _instructions.emplace_back(Operation::DIV, 0);
            }
        }
		return {};
	}

	// <因子> ::= [<符号>]( <标识符> | <无符号整数> | '('<表达式>')' )
	// 需要补全
	std::optional<CompilationError> Analyser::analyseFactor() {
		// [<符号>]
		auto next = nextToken();
		auto prefix = 1;
		if (!next.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrIncompleteExpression);
		if (next.value().GetType() == TokenType::PLUS_SIGN)
			prefix = 1;
		else if (next.value().GetType() == TokenType::MINUS_SIGN) {
			prefix = -1;
			_instructions.emplace_back(Operation::LIT, 0);
		}
		else
			unreadToken();

		// 预读
		next = nextToken();
		if (!next.has_value())
			return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrIncompleteExpression);
		switch (next.value().GetType()) {
			// 这里和 <语句序列> 类似，需要根据预读结果调用不同的子程序
			// 但是要注意 default 返回的是一个编译错误
		    case TokenType::IDENTIFIER: {
		        // TODO: 补全
		        auto id_str = next.value().GetValueString();

		        if (!isDeclared(id_str)) {
		            return CompilationError(_current_pos, ErrorCode::ErrNotDeclared);
		        }

		        if (isUninitializedVariable(id_str)) {
		            return CompilationError(_current_pos, ErrorCode::ErrNotInitialized);
		        }

		        auto id_index = getIndex(id_str);
		        _instructions.emplace_back(Operation::LOD, id_index);

		        break;
		    }
		    case TokenType::UNSIGNED_INTEGER: {
                auto val = std::any_cast<int32_t>(next.value().GetValue());
                _instructions.emplace_back(Operation::LIT, val);

		        break;
		    }
		    case TokenType::LEFT_BRACKET: {
		        if (auto err = analyseExpression(); err.has_value()) {
		            return err;
		        }

		        next = nextToken();
		        if (!next.has_value() || next.value().GetType() != TokenType::RIGHT_BRACKET) {
		            return CompilationError(_current_pos, ErrorCode::ErrIncompleteExpression);
		        }

		        break;
		    }
		    default:
			    return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrIncompleteExpression);
		}

		// 取负
		if (prefix == -1)
			_instructions.emplace_back(Operation::SUB, 0);
		return {};
	}

	std::optional<Token> Analyser::nextToken() {
		if (_offset == _tokens.size())
			return {};
		// 考虑到 _tokens[0..._offset-1] 已经被分析过了
		// 所以我们选择 _tokens[0..._offset-1] 的 EndPos 作为当前位置
		_current_pos = _tokens[_offset].GetEndPos();
		return _tokens[_offset++];
	}

	void Analyser::unreadToken() {
		if (_offset == 0)
			DieAndPrint("analyser unreads token from the beginning.");
		_current_pos = _tokens[_offset - 1].GetEndPos();
		_offset--;
	}

	void Analyser::_add(const Token& tk, std::map<std::string, int32_t>& mp) {
		if (tk.GetType() != TokenType::IDENTIFIER)
			DieAndPrint("only identifier can be added to the table.");
		mp[tk.GetValueString()] = _nextTokenIndex;
		_nextTokenIndex++;
	}

	void Analyser::addVariable(const Token& tk) {
		_add(tk, _vars);
	}

	void Analyser::addConstant(const Token& tk) {
		_add(tk, _consts);
	}

	void Analyser::addUninitializedVariable(const Token& tk) {
		_add(tk, _uninitialized_vars);
	}

	int32_t Analyser::getIndex(const std::string& s) {
		if (_uninitialized_vars.find(s) != _uninitialized_vars.end())
			return _uninitialized_vars[s];
		else if (_vars.find(s) != _vars.end())
			return _vars[s];
		else
			return _consts[s];
	}

	bool Analyser::isDeclared(const std::string& s) {
		return isConstant(s) || isUninitializedVariable(s) || isInitializedVariable(s);
	}

	bool Analyser::isUninitializedVariable(const std::string& s) {
		return _uninitialized_vars.find(s) != _uninitialized_vars.end();
	}
	bool Analyser::isInitializedVariable(const std::string&s) {
		return _vars.find(s) != _vars.end();
	}

	bool Analyser::isConstant(const std::string&s) {
		return _consts.find(s) != _consts.end();
	}
}