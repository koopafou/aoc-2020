#include <iomanip>
#include <sstream>
#include "../../utils.hh"

enum TokenType {
	TOK_L,
	TOK_R,
	TOK_ADD,
	TOK_MUL,
	TOK_NUM
};

struct Token {
	TokenType type;
	i64 value;
};

enum ExprType {
	EXPR_ADD,
	EXPR_MUL,
	EXPR_NUM
};

struct Expr {
	ExprType type;
	union {
		i64 value;
		struct {
			u32 left_dex;
			u32 right_dex;
		};
	};
};

#define EXPECT(TK)																\
	do {																						\
		if ((*toks)[tkdex].type != TK)								\
			throw std::runtime_error("expected " #TK);	\
		++tkdex;																			\
	} while (0)

struct ExprPool {
	std::vector<Expr> pool;
	const std::vector<Token>* toks;
	u32 tkdex;
	bool has_precedence;

	u32 read_token_stream (const std::vector<Token>& toks_, bool has_precedence_) {
		toks = &toks_;
		tkdex = 0;
		has_precedence = has_precedence_;
		return read_expr();
	}

	u32 read_expr_single () {
		if ((*toks)[tkdex].type == TOK_L) {
			++tkdex;
			u32 dex = read_expr();
			EXPECT(TOK_R);
			return dex;
		} else {
			Expr expr;
			expr.type = EXPR_NUM;
			expr.value = (*toks)[tkdex].value;
			EXPECT(TOK_NUM);
			u32 dex = pool.size();
			pool.push_back(expr);
			return dex;
		}
	}

	u32 read_expr () {
		u32 left = read_expr_single();
		while (true) {
			if (tkdex == toks->size() || (*toks)[tkdex].type == TOK_R)
				return left;
			Expr expr;
			switch ((*toks)[tkdex].type) {
			case TOK_ADD: expr.type = EXPR_ADD; break;
			case TOK_MUL: expr.type = EXPR_MUL; break;
			default: std::runtime_error("unexpected token"); break;
			}
			++tkdex;
			expr.left_dex = left;
			if (has_precedence)
				expr.right_dex = (expr.type == EXPR_MUL ? read_expr() : read_expr_single());
			else
				expr.right_dex = read_expr_single();
			left = pool.size();
			pool.push_back(expr);
		}
	}

	i64 eval (u32 exprdex) {
		Expr& expr = pool[exprdex];
		switch (expr.type) {
		case EXPR_NUM: return expr.value;
		case EXPR_ADD: return eval(expr.left_dex) + eval(expr.right_dex);
		case EXPR_MUL: return eval(expr.left_dex) * eval(expr.right_dex);
		}
		throw std::runtime_error("unreachable code");
	}
};

int main () {
	std::vector<std::string> eqs;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		eqs.emplace_back(std::move(line));
	}

	std::vector<std::vector<Token>> eqtoks;
	i64 sum = 0;
	for (auto& eq : eqs) {
		std::stringstream sstr(eq);
		std::vector<Token> toks;
		while (sstr) {
			sstr >> std::ws;
			switch (sstr.peek()) {
			case '(': sstr.get(); toks.push_back(Token{TOK_L, 0}); break;
			case ')': sstr.get(); toks.push_back(Token{TOK_R, 0}); break;
			case '+': sstr.get(); toks.push_back(Token{TOK_ADD, 0}); break;
			case '*': sstr.get(); toks.push_back(Token{TOK_MUL, 0}); break;
			case decltype(sstr)::traits_type::eof(): break;
			default: toks.push_back(Token{TOK_NUM, 0}); sstr >> toks.back().value; break;
			}
		}
		eqtoks.emplace_back(std::move(toks));
	}

	ExprPool epool;
	std::vector<u32> root_dexes1;
	std::vector<u32> root_dexes2;
	for (auto& toks : eqtoks) {
		root_dexes1.push_back(epool.read_token_stream(toks, false));
		root_dexes2.push_back(epool.read_token_stream(toks, true));
	}

	i64 acc1 = 0;
	for (u32 exprdex : root_dexes1)
		acc1 += epool.eval(exprdex);
	std::cout << acc1 << std::endl;

	i64 acc2 = 0;
	for (u32 exprdex : root_dexes2)
		acc2 += epool.eval(exprdex);
	std::cout << acc2 << std::endl;

	return 0;
}
