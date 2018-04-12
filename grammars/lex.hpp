#ifndef  LEXHPP
#define  LEXHPP
#include <iostream>

enum  type_of_lex { 
    LEX_NULL, /*0*/
    LEX_AND, LEX_BEGIN, LEX_WRITE, /*18*/ 
    LEX_FIN, /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_GEQ, /*35*/
    LEX_NUM, /*36*/
    LEX_ID, /*37*/
    POLIZ_LABEL, /*38*/
    POLIZ_ADDRESS, /*39*/
    POLIZ_GO, /*40*/
    POLIZ_FGO   }; /*41*/
class Lex{ 
public:
    type_of_lex get_type();
    int get_value();
private:
    type_of_lex t_lex = LEX_NULL;
    int v_lex = 0;
}

Lexeme get_lex(){
    
}

#endif
