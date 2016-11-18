typedef enum{
	I_PLUS, I_MINUS, I_DIV, I_TIMES, I_ASSIGN, I_IF, I_GOTO //devem ser diferentes dos ja definidos
}OpKind;

typedef enum{ 
	I_INT, I_VAR, I_EMPTY
}AddrKind;

typedef struct{
	AddrKind kind;
	union{
		int val;
		char* var;
	}content;
}Address;

typedef struct{
	OpKind op;
	Address addr1, addr2, addr3;
}Instr;
