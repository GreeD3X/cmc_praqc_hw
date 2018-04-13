#ifndef  LEXHPP
#define  LEXHPP
#include <iostream>

/*enum  type_of_lex { 
    LEX_NULL, 
    LEX_AND, LEX_BEGIN, LEX_WRITE,  
    LEX_FIN, 
    LEX_SEMICOLON, LEX_COMMA, LEX_GEQ, /
    LEX_NUM, 
    LEX_ID, 
    POLIZ_LABEL, 
    POLIZ_ADDRESS, 
    POLIZ_GO, 
    POLIZ_FGO   
}; 
class Lex{ 
public:
    type_of_lex get_type();
    int get_value();
private:
    type_of_lex t_lex = LEX_NULL;
    int v_lex = 0;
};*/

std::pair<std::string, bool> get_lex();

#endif
