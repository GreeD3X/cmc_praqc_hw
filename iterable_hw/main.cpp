#include <iostream>
#include <fstream>
#include <istream>
#include <algorithm>
#include <memory>
#include <iterator>
#include <ctime>
#include <vector>
#include "iterable.hpp"
#include <typeinfo>
#include <stdint.h>

template<class iterator_type, class T>
class FilterIterator: public std::iterator<std::input_iterator_tag, const T, long, const T, const T>{
public:
    FilterIterator(const iterator_type& begin, const iterator_type& end, const std::function<bool (const T&)>& pred): 
    icurrent(begin), iend(end), pred(pred)
    {
    while((icurrent != iend) && (!pred(*icurrent)))
        ++icurrent;
    }

    const T operator*() const { return *icurrent; }
    const T operator->() const { return *icurrent; }

    FilterIterator& operator++() { advance(); return *this; }

    bool operator==(const FilterIterator& other) const { return icurrent == other.icurrent; }
    bool operator!=(const FilterIterator& other) const { return !(operator==(other)); }
    const FilterIterator end() const {return {this->iend, this->iend, this->pred};};
private:
    void advance()
    {
        do
        {
            ++icurrent;
        }
        while((icurrent != iend) && (!pred(*icurrent)));
    } // advance

    iterator_type icurrent;
    iterator_type iend;
    const std::function<bool (const T&)> pred;
};

class JournalEntry{
    public:
        time_t entryTime;
        std::string entryString;
        friend std::ostream& operator<<(std::ostream& os, const  JournalEntry& entry){
            std::tm *eTime = gmtime(&entry.entryTime);
            os << "entry: \"" << entry.entryString << "\" added on: " << \
            1900 + eTime->tm_year << \
            "-" << 1 + eTime->tm_mon  << \
            "-" <<     eTime->tm_mday << \
            " " << 1 + eTime->tm_hour << \
            ":" << 1 + eTime->tm_min;
            return os;
        }

        bool operator==(JournalEntry const& other) const { return (entryTime == other.entryTime) && (entryString == other.entryString);};  

        bool operator!=(JournalEntry const& other) const { return !(*this == other);};  
};

class Journal: public Iterable<
    std::vector<JournalEntry>::iterator,
    std::vector<JournalEntry>::const_iterator,
    FilterIterator<std::vector<JournalEntry>::iterator, JournalEntry>, 
    JournalEntry>{
public:
    std::vector<JournalEntry>::iterator begin()       { return entries.begin(); }
    std::vector<JournalEntry>::const_iterator begin() const { return entries.cbegin(); }
    std::vector<JournalEntry>::iterator end()         { return entries.end(); }
    std::vector<JournalEntry>::const_iterator end()   const { return entries.cend(); }
    FilterIterator<std::vector<JournalEntry>::iterator, JournalEntry> filter(const std::function<bool (const JournalEntry&)>& f) { return { begin(), end(), f }; }
    void add_entry(const JournalEntry& a);
    void remove_entry(int what);
    void remove_entries();
    bool is_empty(){return entries.empty();}
private:
    std::vector<JournalEntry> entries;
};

void Journal::add_entry(const JournalEntry& a){
	entries.push_back(a);
}

void Journal::remove_entry(int what){
    if(what < 0 || (unsigned int)what >= entries.size())
        return;
    entries.erase(entries.begin() + what);
}

void Journal::remove_entries(){
    if(is_empty())
        return;
    entries.erase(entries.begin(), entries.end() );
}

class NumberIterator: public std::iterator<std::input_iterator_tag, const int32_t*, long, const int32_t, const int32_t>{
public:
    NumberIterator(const int32_t* num, const int bit = 0): num(num), cur_bit(bit) {};
    NumberIterator(const NumberIterator &it): num(it.num), cur_bit(it.cur_bit) {};
    bool operator!=(NumberIterator const& other) const { return num != other.num || cur_bit != other.cur_bit;};
    bool operator==(NumberIterator const& other) const { return num == other.num && cur_bit == other.cur_bit;};
    const int32_t operator*() const {return (*num & (1 << cur_bit)) ? 1 : 0;};
    NumberIterator& operator++(){ ++cur_bit; return *this;};
private:
    const int32_t* num;
    int cur_bit;
};

/*template<class iterator_type>
class NumberFilterIterator: public std::iterator<std::input_iterator_tag, const int32_t*, long, const int32_t, const int32_t>{
public:
    NumberFilterIterator(const iterator_type& begin, const iterator_type& end, const std::function<bool (int32_t)>& pred): 
    icurrent(begin), iend(end), pred(pred)
    {
    while((icurrent != iend) && (!pred(*icurrent)))
        ++icurrent;
    }
    bool operator!=(const NumberFilterIterator& other) const { return !(operator==(other)); }
    bool operator==(const NumberFilterIterator& other) const { return icurrent == other.icurrent; }
    const int32_t operator*() const { return *icurrent; }
    NumberFilterIterator& operator++() { advance(); return *this; }
    const NumberFilterIterator end() const {return {this->iend, this->iend, this->pred};};
private:
    void advance()
    {
        do
        {
            ++icurrent;
        }
        while((icurrent != iend) && (!pred(*icurrent)));
    } // advance

    iterator_type icurrent;
    iterator_type iend;
    std::function<bool (int32_t)> pred;
};*/

class Number: public Iterable<
    NumberIterator, 
    NumberIterator, 
    FilterIterator<NumberIterator, int32_t>,
    int32_t>{
public:
    Number(const int32_t& a = 0): value(a){};
    NumberIterator begin()       {return NumberIterator(&value, 0);}
    NumberIterator end()         {return NumberIterator(&value, 32);}
    NumberIterator begin() const {return NumberIterator(&value, 0);}
    NumberIterator end()   const {return NumberIterator(&value, 32);}
    FilterIterator<NumberIterator, int32_t> filter(const std::function<bool (const int32_t&)>& f){return {begin(), end(), f};}
private:
    int32_t value;
};

class FileIterator: public std::iterator<std::input_iterator_tag, const int, long, const int, const int>{
public:
    FileIterator(std::shared_ptr<std::ifstream> file, bool state = true)
	    : file(file), state(state), lastRead(0) {
		    ++(*this);
	    }
    bool operator!=(FileIterator const& other) const { return state != other.state;};
    bool operator==(FileIterator const& other) const { return state == other.state;};
    const int operator*() const {return lastRead;};
    FileIterator& operator++(){
        if(state){
            try{
                file->read((char *)(&lastRead), sizeof(lastRead));
                state = !(file->eof());
            }
            catch(const std::exception& e){
                if(file->eof()){
                    state = !(file->eof());
                    return *this;
                }
                std::cerr << "Couldnt read in file " << e.what() << std::endl;
                throw e;
            }
        }
        return *this;
    };  
private:
    std::shared_ptr<std::ifstream> file;
    bool state;
    int lastRead;
};

class FileI: public Iterable<FileIterator, FileIterator, FilterIterator<FileIterator,int>, int>{
public:
    FileI(std::string s = "\n"): fileName(s), file(){
        file = std::make_shared<std::ifstream> ();
        file->exceptions( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
        try {
            file->open(fileName, std::ios::binary | std::ios::in);
        } 
        catch(const std::exception& e){
            std::cerr << "Couldnt create FileI instance " << e.what() << std::endl;
	    throw e;
        }
        file->exceptions( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
    };
    FileIterator begin() { return {file, this->file->is_open()};}
    FileIterator end() { return {this->file, false};}
    FileIterator begin() const { return {this->file, this->file->is_open()};}
    FileIterator end() const { return {this->file, false};}
    FilterIterator<FileIterator,int> filter(const std::function<bool (const int&)>& f){return {begin(), end(), f};}
private:
    std::string fileName;
    std::shared_ptr<std::ifstream> file;
};

template<class T1, class T2, class T3, class T4>
void printIterable(const Iterable<T1,T2,T3, T4>& Iter){
    std::cout << "-------\nPrinting " << typeid(Iter).name()<< "\n-------" << std::endl;
    for(auto x: Iter){
        std::cout << x << std::endl;
    }
    std::cout << "-------\nFinished printing " << typeid(Iter).name()<< "\n-------" << std::endl;
}

int main(){
    std::cout << "\nJournal part\n------------" << std::endl;
    {
        Journal r;
        std::cout << "Empty journal" << std::endl;
        for(auto x: r){
            std::cout << x << std::endl;
        } 
        std::cout << "Normal journal" << std::endl;
        r.add_entry({time(NULL), "kek"});
        r.add_entry({time(NULL), "abacaba"});
        r.add_entry({time(NULL), "WOW entry"});
        r.add_entry({time(NULL), "Much entry"});
        r.add_entry({time(NULL), "Such entry"});
        for(auto x: r){
            std::cout << x << std::endl;
        }
        printIterable(r);
        auto it = std::find_if(r.begin(), r.end(), [](JournalEntry a){return a.entryString == "Much entry";});
        if(it != r.end()){
            std::cout << "Entry with string \"Much entry\" is found" << std::endl;
        }
        else{

            std::cout << "Entry with string \"Much entry\" is not found" << std::endl;
        }
        r.remove_entries();
    }
    std::cout << "\n32 bit number part\n-------------" << std::endl;
    {
        Number ne;
        std::cout<<"Empty number is equal to 0" << std::endl;
        for(auto x: ne){
            std::cout << (int)x << ' ';
        }
        std::cout<<std::endl;
        Number n(15);
        std::cout << 15 << std::endl;
        for(auto x: n){
            std::cout << (int)x << ' ';
        }
        std::cout<<"\nprinting previous Number with general printing function" << std::endl;
        printIterable(n);
        std::cout << "\nnext\n";

        n = Number(16);
        std::cout << 16 << std::endl;
        for(auto x: n){
            std::cout << (int)x << ' ';
        }
        

        std::cout << "\nnext\n";
        int32_t a = 1;
        a <<= 31; 
        n = Number(a - 1);
        std::cout << "2^^31-1" << std::endl;
        for(auto x: n){
            std::cout << (int)x << ' ';
        }

        std::cout << "\nnext\n";

        a = 1 << 31; 
        n = Number(a - 1 - (1 << 15));
        std::cout << "2^^31 - 1 -2^^15" << std::endl;
        for(auto x: n){
            std::cout << (int)x << ' ';
        }
        std::cout<<std::endl;
        auto it = std::find_if(n.begin(), n.end(), [](int32_t a){return a != 0;});
        if(it != n.end()){
            std::cout << "Number n got some '1's" << std::endl;
        }
        else{

            std::cout << "Number n doesnt have any '1's" << std::endl;
        }

        n = Number();
        std::cout << "Empty" << std::endl;
        for(auto x: n){
            std::cout << (int)x << ' ';
        }
        std::cout<<std::endl;
        auto it1 = std::find_if(n.begin(), n.end(), [](int32_t a){return a != 0;});
        if(it1 != n.end()){
            std::cout << "Number n got some '1's" << std::endl;
        }
        else{

            std::cout << "Number n doesnt have any '1's" << std::endl;
        }
	n = Number(a - 1 - (1 << 15));
	std:: cout << "Using filter on Number 2^^31 - 1 -2 ^^15" << std::endl;
        auto niter = n.filter([](const int32_t& a){return a == 0;});
        std::cout<<"Printing only 0 from number" << std::endl;
        for(;niter !=std::end(niter); ++niter){
            std::cout << *niter << ' ';
        }
        std::cout<<std::endl;
    }
    std::cout << "\nFile part\n---------" << std::endl;
    {
        //FileI fileEmpty;  //Wont be created, with exception thrown
        std::cout << "Non-existing file(Will throw an exception with error message)" << std::endl;
        try{
            FileI fileNonExist("\n\n\n\n\n\n\n");
        }
        catch(const std::exception& e){
            std::cerr << "Caught: " << e.what() << std::endl;
        }
        FileI fileEmpty("empty");
       	std::cout<<"empty file" << std::endl;
       	for(auto x: fileEmpty){
	    std::cout << x <<' ';
       	}/**/
        std::cout << std::endl;
        std::string fileName = "hello1";
        FileI fileI(fileName);
        std::cout<<"Normal file"<<std::endl;
        for(auto x: fileI){
            std::cout << x <<' ';
        }
        std::cout << std::endl;
        std::cout << "printing FileI with general printing function" << std::endl;
        printIterable(FileI("hello1"));
        auto it = std::find_if(FileI("hello1").begin(), fileI.end(), [](int a){return a == 0;});
        if(it != fileI.end()){
            std::cout<<"This file has some '0's int it" << std::endl;
        }
        else{
            std::cout<<"This file hasnt any '0's int it" << std::endl;
        }
        FileI filtered("hello1");
        auto fiter = filtered.filter([](const int& a){return a>49;});
        std::cout<<"Printing only ints>49" << std::endl;
        for(;fiter !=std::end(fiter); ++fiter){
            std::cout << *fiter << ' ';
        }
        std::cout<<std::endl;
    }
    return 0;
}
