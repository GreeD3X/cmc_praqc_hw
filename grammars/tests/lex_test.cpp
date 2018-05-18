#include <iostream>
#include "filter.hpp"
#include "iterlex.hpp"


int main(){
	std::cout << "Testing Lexeme functions" << std::endl;
	try{
		Lex_seq lex;
		for(auto x: lex){
			std::cout << x.get_name() << ' '<< x.get_lt() << std::endl;
		}
	}
	catch(const Lex_exception& e){
		std::cout << "Caught: " << e.what() << std::endl;
	}
}
