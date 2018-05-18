CXX := g++-7
.PHONY := all clean lex_tests valgrind run rundbg

TARGET := hello

src_dirs := source
inc_dirs := include
search_wildcars := $(addsuffix /*.cpp,$(src_dirs))
VPATH := $(src_dirs)

C_F := -g -std=c++17 -Wall -Werror
L_F :=  

$(TARGET): $(notdir $(patsubst %.cpp,%.o, $(wildcard $(search_wildcars) ) ) )
	$(CXX) $^ $(L_F) -o $@

%.o: %.cpp
	$(CXX) -c -MD $(addprefix -I , $(inc_dirs)) $(C_F) $<

clean:
	rm -rf $(TARGET) *.o *.d

all: clean $(TARGET)

lex_test.o: tests/lex_test.cpp
	$(CXX) -c -MD $(addprefix -I , $(inc_dirs)) $(C_F) $< 

lex_tests: lex.o iterlex.o lex_test.o
	$(CXX) $^ $(L_F) -o lex_test
	for file in $(addsuffix /lex*.test,tests ); do \
		echo _________________________;\
		echo File $$file ;\
		echo -------------------------;\
		[ -d $$file ] || cat $$file;\
		echo -------------------------;\
		./lex_test < $$file; \
	done

	rm -rf lex_test

rec_test.o: tests/rec_test.cpp
	$(CXX) -c -MD $(addprefix -I , $(inc_dirs)) $(C_F) $< 

rec_tests: rec_test.o lex.o iterlex.o recursive.o	
	$(CXX) $^ $(L_F) -o rec_test
	for file in $(addsuffix /rec*.test,tests ); do \
		echo _________________________;\
		echo File $$file ;\
		echo -------------------------;\
		[ -d $$file ] || cat $$file;\
		echo -------------------------;\
		./rec_test < $$file; \
	done


valgrind: $(TARGET)
	valgrind --tool=memcheck --leak-check=full ./$(TARGET) 2>&1 | less

run: exec
		
exec: $(TARGET)
	for file in $(addsuffix /*.test,$(tests) ); do \
		./$(TARGET) < $$file; \
	done
	./$(TARGET)

rundbg: execdbg

execdbg: $(TAGRET)
	for file in $(addsuffix /*.test,$(tests) ); do \
		echo _________________________;\
		echo File $$file ;\
		echo -------------------------;\
		[ -d $$file ] || cat $$file;\
		echo -------------------------;\
		./$(TARGET) < $$file; \
	done
	./$(TARGET)
