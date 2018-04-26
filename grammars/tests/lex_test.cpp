#include <iostream>
#include "filter.hpp"
#include "iterlex.hpp"


int main(){
	std::cout << "Testing Lexeme functions" << std::endl;
	try{
		Lex_seq lex;
		for(auto x: lex){
			std::cout << x.lex << std::endl;
		}
	}
	catch(const std::string& s){
		std::cout << "Caught: " << s << std::endl;
	}
}
