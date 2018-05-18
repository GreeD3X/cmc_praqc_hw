#include <iostream>
#include "filter.hpp"
#include "iterlex.hpp"
#include "recursive.hpp"

int main(){
	std::cout << "Testing recursive functions" << std::endl;
	try{
		Lex_seq lex;// = Lex_seq();
		Parser parser(lex);
		parser.parse();
		std::cout << "Printing poliz" << std::endl;
		parser.print_poliz();
		std::cout << "Printing expressions" << std::endl;
		parser.print_expr();
	}
	catch(const std::exception& e){
		std::cout << "Caught: " << e.what() << std::endl;
	}
}
