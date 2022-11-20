#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ARGUMENTS 256
#define MAX_TOKENS 1024
#define MAX_CHARS 4096

void DIE(const char* s) {
	printf("\033[31m%s\n", s);
	exit(-1);
}

typedef enum {
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV
} op_t;

typedef enum {
	EXPRESSION,
	NUMBER
} argument_t;

struct argument {
	argument_t argumentIs;
	long double number;
	struct op* operation;
};

struct op {
	op_t action;
	int amountOfArguments;
	struct argument arguments[MAX_ARGUMENTS];
};

typedef enum {
	TOKEN_END,
	TOKEN_NUMBER,
	TOKEN_ADD,
	TOKEN_SUBTRACT,
	TOKEN_MULTIPLY,
	TOKEN_DIVIDE,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN
} token_t;

typedef enum {
	TOKEN_CATEGORY_NUMBER,
	TOKEN_CATEGORY_OP,
	TOKEN_CATEGORY_PAREN,
	TOKEN_CATEGORY_END
} token_category_t;

typedef struct token {
	token_t type;
	token_category_t category;
	long double number;
} token_expression;

//TODO: CHANGE UNARY MINUS FROM _ TO -
//TODO: CHECK AND MAKE SURE NUMBERS START WITH -, NOT IN MIDDLE
bool isCharDigit(char c) {
	const char* acceptableNumberChars = "_.0123456789";
	for (unsigned int i = 0; i < strlen(acceptableNumberChars); i++) {
		if (acceptableNumberChars[i] == c) {
			return true;
		}
	}
	return false;
}

void tokenize(char* expr, token_expression* out) {
	size_t tokenLength = 0;
	int exprStringPointer = 0;
	while (expr[exprStringPointer] != '\0') {
		if (tokenLength >= MAX_TOKENS)
			DIE("Expression contains too many tokens.");
		token_expression currentToken;
		memset(&currentToken, 0, sizeof(token_expression));
		switch (expr[exprStringPointer]) {
			case '(':
			currentToken.type = TOKEN_OPEN_PAREN;
			currentToken.category = TOKEN_CATEGORY_PAREN;
			exprStringPointer++;
			break;
			case ')':
			currentToken.type =  TOKEN_CLOSE_PAREN;
			currentToken.category = TOKEN_CATEGORY_PAREN;
			exprStringPointer++;
			break;
			case '+':
			currentToken.type = TOKEN_ADD;
			currentToken.category = TOKEN_CATEGORY_OP;
			exprStringPointer++;
			break;
			case '-':
			currentToken.type = TOKEN_SUBTRACT;
			currentToken.category = TOKEN_CATEGORY_OP;
			exprStringPointer++;
			break;
			case '*':
			currentToken.type = TOKEN_MULTIPLY;
			currentToken.category = TOKEN_CATEGORY_OP;
			exprStringPointer++;
			break;
			case '/':
			currentToken.type = TOKEN_DIVIDE;
			currentToken.category = TOKEN_CATEGORY_OP;
			exprStringPointer++;
			break;
			default:
			if (isCharDigit(expr[exprStringPointer])) {
				printf("found a digit, %c\n", expr[exprStringPointer]);
				currentToken.type = TOKEN_NUMBER;
				currentToken.category = TOKEN_CATEGORY_NUMBER;
				int numberStart = exprStringPointer;
				int numberEnd = numberStart;
				for (; isCharDigit(expr[numberEnd]); numberEnd++);
				char numberString[numberEnd - numberStart + 1];
				memcpy(numberString, &expr[numberStart], numberEnd - numberStart);
				numberString[numberEnd - numberStart] = '\0';
				sscanf(numberString, "%Lf", &(currentToken.number));
				exprStringPointer += (numberEnd - numberStart);
			} else {
				DIE("Unrecognized character at ???");
			}
			break;
		}
		memcpy(&out[tokenLength++], &currentToken, sizeof(token_expression));
		printf("added a token of type %i\n", currentToken.type);
	}
	token_expression endToken;
	memset(&endToken, 0, sizeof(token_expression));
	endToken.type = TOKEN_END;
	endToken.category = TOKEN_CATEGORY_END;
	memcpy(&out[tokenLength], &endToken, sizeof(token_expression));
	printf("added an ending token of type (pls 0) %i\n", endToken.type);
}

//returns 1 on error, 0 on success
//syntax check
int checkTokenValidity(token_expression* expr, char* outError) {
	//first, check known invalid token sequences
	token_category_t invalidSequences[3][2] = {
		{TOKEN_CATEGORY_NUMBER, TOKEN_CATEGORY_NUMBER},
		{TOKEN_CATEGORY_OP, TOKEN_CATEGORY_OP},
		{TOKEN_CATEGORY_OP, TOKEN_CATEGORY_END}
	};
	for (int tokenPointer = 0; expr[tokenPointer].type != TOKEN_END; tokenPointer++) {
		for (int currentSequence = 0; currentSequence < 7; currentSequence++) {
			if (expr[tokenPointer].category == invalidSequences[currentSequence][0]) {
				if (expr[tokenPointer + 1].category == invalidSequences[currentSequence][1]) {
					sprintf(outError, "Invalid tokens: categories %i and %i are adjecent.\n", expr[tokenPointer].category, expr[tokenPointer + 1].category);
					return 1;
				}
			}
		}
	}
	//next, check for matching parens
	int parenDepth = 0;
	for (int tokenPointer = 0; expr[tokenPointer].type != TOKEN_END; tokenPointer++) {
		if (expr[tokenPointer].type == TOKEN_OPEN_PAREN)
			parenDepth++;
		if (expr[tokenPointer].type == TOKEN_CLOSE_PAREN)
			parenDepth--;
	}
	if (parenDepth != 0) {
		sprintf(outError, "Unmatched parens: ended expression with depth %i\n", parenDepth);
		return 1;
	}
	return 0;
}

int main(int argc, char const *argv[]) {
	char exprString[MAX_CHARS];
	scanf("%s", exprString);
	printf("got expression %s\n", exprString);
	token_expression exprTokens[MAX_TOKENS];
	memset(exprTokens, 0, sizeof(struct token) * MAX_TOKENS);
	tokenize(exprString, exprTokens);
	for (int i = 0; exprTokens[i].type != TOKEN_END; i++) {
		printf("%i, ", exprTokens[i].type);
	}
	printf("\n");
	char tokenExpressionError[1024];
	if (checkTokenValidity(exprTokens, tokenExpressionError))
		DIE(tokenExpressionError);

	return 0;
}