#include "recursive.hpp"
#include "iterlex.hpp"

#include <iostream>

int main(){
	Lex_seq l_seq;
	Parser parser(l_seq);
	std::cout << "Im in main" << std::endl;
	return 0;
}
