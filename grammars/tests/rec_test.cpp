#include <iostream>
#include "filter.hpp"
#include "iterlex.hpp"
#include "recursive.hpp"

int main(){
	std::cout << "Testing recursive functions" << std::endl;
	try{
		Lex_seq lex;
		Parser parser(lex);
		parser.parse();
		parser.print_poliz();
	}
	catch(const std::exception& e){
		std::cout << "Caught: " << e.what() << std::endl;
	}
}
