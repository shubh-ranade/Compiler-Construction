#include"lexerDef.h"

char *current_buffer, *previous_buffer, *curr_pos;
FILE* fp;
int line_no = 0;
extern HashTable ht;

char* keywords[NUM_KEYWORDS] = {
	"with",
	"parameters",
	"end",
	"while",
	"type",
	"_main",
	"global",
	"parameter",
	"list",
	"input",
	"output",
	"int",
	"real",
	"endwhile",
	"if",
	"then",
	"endif",
	"read",
	"write",
	"return",
	"call",
	"record",
	"endrecord",
	"else"
};

FILE* getStream(FILE *fp){
	int read;
	char *temp = current_buffer;
	current_buffer = previous_buffer;
	previous_buffer = temp;
	
	//clear current buffer before reading into it
	memset(current_buffer, 0, sizeof(current_buffer));

	if (feof(fp)){
		fclose(fp);
		return NULL;
	}
	
	//loading data from file
	read = fread(current_buffer, sizeof(char), BUF_SIZE, fp);
	
	if(read <= 0){
		// printf("Read error in buffer 1!");
		fclose(fp);
		return NULL;
	}

	current_buffer[BUF_SIZE] = '\0';

	return fp;
}



tokenInfo getNextToken(FILE *fp){
	if(*current_buffer == '\0'){
		fp = getStream(fp);
	}

	tokenInfo tk_info;
	// memset(tk_info.value, 0, sizeof(tk_info.value));
	tk_info.type_of_token = -2;
	tk_info.line_num = line_no;

	tokenInfo tk_eof;
	tk_eof.type_of_token = -1;
	tk_eof.line_num = line_no;

	int state = 0, vind = 0;
	char *ch = curr_pos;
	char value[MAX_FLOAT_LEN + 1];

	while(1){
		if(*ch == '\0'){
			fp = getStream(fp);
			if(fp == NULL)
				return tk_eof;
			ch = current_buffer;
			curr_pos = ch;
		}
		
		switch(state){
			case 0:
				
				//resetting value
				memset(value, 0, MAX_LEXEME_SIZE);
				vind = 0;

				switch(*ch){
					case '-':
						state = 1;
						break;
					
					case '*':
						state = 2;
						break;
					
					case '~':
						state = 3;
						break;

					case '!':
						state = 4;
						break;

					case ':':
						state = 6;
						break;

					case '[':
						state = 7;
						break;

					case ']':
						state = 8;
						break;

					case '<':
						state = 9;
						break;

					case '=':
						state = 15;
						break;

					case '&':
						state = 17;
						break;

					case '#':
						value[vind] = *ch;
						vind++;
						state = 20;
						break;

					case '0' ... '9':
						value[vind] = *ch;
						vind++;
						state = 23;
						break;

					case '_':
						value[vind] = *ch;
						vind++;
						state = 28;
						break;

					case 'b' ... 'd':
						value[vind] = *ch;
						vind++;
						state = 34;
						break;

					case 'a':
					case 'e' ... 'z':
						value[vind] = *ch;
						vind++;
						state = 32;
						break;

					case '@':
						state = 39;
						break;

					case '\n':
						line_no++;
						break;

					case '%':
						state = 43;
						break;

					case '>':
						state = 45;
						break;

					case ')':
						state = 48;
						break;

					case '(':
						state = 49;
						break;

					case ';':
						state = 50;
						break;

					case ',':
						state = 51;
						break;

					case '.':
						state = 52;
						break;

					case '/':
						state = 53;
						break;

					case '+':
						state = 54;
						break;

					default:
						state = 0;
				}

				break;

			case 1:
				tk_info.type_of_token = TK_MINUS;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 2:
				tk_info.type_of_token = TK_MUL;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 3:
				tk_info.type_of_token = TK_NOT;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 4:
				if(*ch == '='){
					state = 5;
				}
				else{
					return tk_info;
				}
				break;

			case 5:
				tk_info.type_of_token = TK_NE;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 6:
				tk_info.type_of_token = TK_COLON;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 7:
				tk_info.type_of_token = TK_SQL;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 8:
				tk_info.type_of_token = TK_SQR;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 9:											//also state 10 (retract)
				if(*ch == '='){
					state = 14;
				}
				else if(*ch == '-'){
					state = 11;
				}
				else{
					tk_info.type_of_token = TK_LT;
					tk_info.line_num = line_no;
					curr_pos = ch;							//See what I did there?
					return tk_info;
				}
				break;							

			case 11:
				if(*ch == '-'){
					state = 12;
				}
				else{
					return tk_info;
				}
				break;

			case 12:
				if(*ch == '-'){
					state = 13;
				}
				else{
					return tk_info;
				}
				break;

			case 13:
				tk_info.type_of_token = TK_ASSIGNOP;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 14:
				tk_info.type_of_token = TK_LE;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 15:
				if(*ch == '='){
					state = 16;
				}
				else{
					return tk_info;
				}
				break;

			case 16:
				tk_info.type_of_token = TK_EQ;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 17:
				if(*ch == '&'){
					state = 18;
				}
				else{
					return tk_info;
				}
				break;

			case 18:
				if(*ch == '&'){
					state = 19;
				}
				else{
					return tk_info;
				}
				break;

			case 19:
				tk_info.type_of_token = TK_AND;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 20:
				if(*ch >= 'a' && *ch <= 'z'){
					value[vind] = *ch;
					vind++;
					state = 21;
				}
				else{
					return tk_info;
				}
				break;

			case 21:													//also state 22 (retract state)
				if((*ch >= 'a' && *ch <= 'z') && vind >= 20){
					while(*ch >= 'a' && *ch <= 'z'){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else if(*ch >= 'a' && *ch <= 'z'){
					value[vind] = *ch;
					vind++;
					state = 21;
				}				
				else
				{
					tk_info.type_of_token = TK_RECORDID;
					tk_info.line_num = line_no;
					strncpy(tk_info.value, value, MAX_FLOAT_LEN);
					curr_pos = ch;										//See? I did it again
					return tk_info;
				}										
				break;			

			case 23:													//also state 27 (retract state)
				if(*ch >= '0' && *ch <= '9' && vind < 39){
					value[vind] = *ch;
					vind++;
				}
				else if(vind >= 39){
					while(*ch >= '0' && *ch <= '9'){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else if(*ch == '.'){
					value[vind] = *ch;
					vind++;
					state = 24;
				}
				else if (vind >= 10){
					while(*ch >= '0' && *ch <= '9'){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else{
					tk_info.type_of_token = TK_NUM;
					tk_info.line_num = line_no;
					strncpy(tk_info.value, value, MAX_FLOAT_LEN);					
					curr_pos = ch;										//See? I did it again
					return tk_info;
				}
				break;

			case 24:
				if(*ch >= '0' && *ch <= '9'){
					value[vind] = *ch;
					vind++;
					state = 25;
				}
				else{
					return tk_info;
				}
				break;

			case 25:
				if(*ch >= '0' && *ch <= '9'){
					value[vind] = *ch;
					vind++;
					state = 26;
				}
				else{
					return tk_info;
				}
				break;

			case 26:
				if(*ch >= '0' && *ch <= '9'){
					while(*ch >= '0' && *ch <= '9'){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else if(vind <= MAX_FLOAT_LEN){
					tk_info.type_of_token = TK_RNUM;
					tk_info.line_num = line_no;
					strncpy(tk_info.value, value, MAX_FLOAT_LEN);
					curr_pos = ch;
					return tk_info;
				}
				else{
					return tk_info;					
				}
				break;

			case 28:
				if((*ch >= 'a' && *ch <= 'z') || (*ch >= 'A' && *ch <= 'Z')){
					value[vind++] = *ch;
					state = 29;
				}
				else{
					return tk_info;					
				}
				break;

			case 29:
				if((*ch >= 'a' && *ch <= 'z') || (*ch >= 'A' && *ch <= 'Z') && vind >= MAX_LEXEME_SIZE){
					while((*ch >= 'a' && *ch <= 'z') || (*ch >= 'A' && *ch <= 'Z') || (*ch >= '0' && *ch <= '9')){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else if ((*ch >= 'a' && *ch <= 'z') || (*ch >= 'A' && *ch <= 'Z')){
					value[vind++] = *ch;
				}
				else if(*ch >= '0' && *ch <= '9'){
					value[vind++] = *ch;
					state = 30;
				}
				else {
					return tk_info;
				}
				break;

			case 30:														//also state 31 (retracting)
				if((*ch >= '0' && *ch <= '9') && vind >= MAX_LEXEME_SIZE){
					while(*ch >= '0' && *ch <= '9'){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else if(*ch >= '0' && *ch <= '9'){
					value[vind++] = *ch;
				}
				else if(vind <= MAX_LEXEME_SIZE){
					tk_info.type_of_token = findKeyword(ht, value);
					tk_info.line_num = line_no;
					strncpy(tk_info.value, value, MAX_FLOAT_LEN);
					curr_pos = ch;
					if(tk_info.type_of_token == -1){
						tk_info.type_of_token = TK_FUNID;
					}
					return tk_info;
				}
				else{
					return tk_info;
				}
				break;

			case 32:
				if(*ch >= 'a' && *ch <= 'z' && vind >= 20){
					while(*ch >= 'a' && *ch <= 'z'){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else if(*ch >= 'a' && *ch <= 'z'){
					value[vind++] = *ch;
				}
				else if(vind <= 20){
					tk_info.type_of_token = findKeyword(ht, value);
					tk_info.line_num = line_no;
					strncpy(tk_info.value, value, MAX_FLOAT_LEN);
					curr_pos = ch;
					if(tk_info.type_of_token == -1){
						tk_info.type_of_token = TK_FIELDID;
					}
					return tk_info;
				}
				else{
					return tk_info;
				}
				break;

			case 34:
				if(*ch >= '2' && *ch <= '7'){
					state = 35;
					value[vind++] = *ch;
				}
				else if(*ch >= 'a' && *ch <= 'z'){
					state = 32;
					value[vind++] = *ch;
				}
				else{
					return tk_info;
				}
				break;

			case 35:
				if(*ch >= '2' && *ch <= '7'){
					state = 38;
					value[vind++] = *ch;
				}
				else if(*ch >= 'b' && *ch <= 'd'){
					state = 36;
					value[vind++] = *ch;
				}
				else{
					tk_info.type_of_token = TK_ID;
					tk_info.line_num = line_no;
					strncpy(tk_info.value, value, MAX_FLOAT_LEN);
					curr_pos = ch;
					return tk_info;
				}
				break;

			case 36:
				if(*ch >= 'b' && *ch <= 'd' && vind >= 20){
					while((*ch >= 'b' && *ch <= 'd') || (*ch >= '2' && *ch <= '7')){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else if(*ch >= 'b' && *ch <= 'd'){
					value[vind++] = *ch;
				}
				else if(*ch >= '2' && *ch <= '7'){
					state = 38;
					value[vind++] = *ch;
				}
				else if(vind <= 20){
					tk_info.type_of_token = TK_ID;
					tk_info.line_num = line_no;
					strncpy(tk_info.value, value, MAX_FLOAT_LEN);
					curr_pos = ch;
					return tk_info;
				}
				else{
					return tk_info;
				}
				break;

			case 38:
				if(*ch >= '2' && *ch <= '7' && vind >= 20){
					while((*ch >= 'b' && *ch <= 'd') || (*ch >= '2' && *ch <= '7')){
						ch++;
						if(*ch == '\0'){
							fp = getStream(fp);
							if(fp == NULL)
								return tk_eof;
							ch = current_buffer;
							curr_pos = ch;
						}
					}
					return tk_info;
				}
				else if(*ch >= '2' && *ch <= '7'){					
					value[vind++] = *ch;
				}
				else if(vind <= 20){
					tk_info.type_of_token = TK_ID;
					tk_info.line_num = line_no;
					strncpy(tk_info.value, value, MAX_FLOAT_LEN);
					curr_pos = ch;
					return tk_info;
				}
				else{
					return tk_info;					
				}
				break;

			case 39:
				if(*ch == '@'){
					state = 40;
				}
				else{
					return tk_info;
				}
				break;

			case 40:
				if(*ch == '@'){
					state = 41;
				}
				else{
					return tk_info;
				}
				break;

			case 41:
				tk_info.type_of_token = TK_OR;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 43:
				while(*ch != '\n'){
					ch++;
					if(*ch == '\0'){
						fp = getStream(fp);
						if(fp == NULL)
							return tk_eof;
						ch = current_buffer;
						curr_pos = ch;
					}
				}
				line_no++;
				state = 0;
				break;

			case 45:
				if(*ch == '='){
					state = 47;
				}
				else{
					tk_info.type_of_token = TK_GT;
					tk_info.line_num = line_no;
					curr_pos = ch;
					return tk_info;
				}
				break;

			case 47:
				tk_info.type_of_token = TK_GE;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 48:
				tk_info.type_of_token = TK_CL;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 49:
				tk_info.type_of_token = TK_OP;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 50:
				tk_info.type_of_token = TK_SEM;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 51:
				tk_info.type_of_token = TK_COMMA;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 52:
				tk_info.type_of_token = TK_DOT;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 53:
				tk_info.type_of_token = TK_DIV;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;

			case 54:
				tk_info.type_of_token = TK_PLUS;
				tk_info.line_num = line_no;
				curr_pos = ch;
				return tk_info;
		}
		ch++;
	}
}