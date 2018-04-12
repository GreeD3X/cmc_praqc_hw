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

template<class T>
class JournalIterator: public std::iterator<std::input_iterator_tag, T,long, T*, T>{
public:
    JournalIterator(T* p): p(p){};
    JournalIterator(const JournalIterator &it): p(it.p){};
    bool operator!=(JournalIterator const& other) const { return p != other.p;};
    bool operator==(JournalIterator const& other) const { return p == other.p;};
    T& operator*() const {return *p;};
    JournalIterator& operator++(){ ++p; return *this;};
private:
    T* p;
};

template<class T>
class CJournalIterator: public std::iterator<std::input_iterator_tag, const T, long, const T*, const T>{
public:
    CJournalIterator(const T* p): p(p){};
    CJournalIterator(const CJournalIterator &it): p(it.p){};
    bool operator!=(CJournalIterator const& other) const { return p != other.p;};
    bool operator==(CJournalIterator const& other) const { return p == other.p;};
    const T& operator*() const {return *p;};
    CJournalIterator& operator++(){ ++p; return *this;};
private:
    const T* p;
};

template<class iterator_type, class T>
class JournalFilterIterator: public std::iterator<std::input_iterator_tag, const T, long, const T*, const T>{
public:
    JournalFilterIterator(const iterator_type& begin, const iterator_type& end, const std::function<bool (T)>& pred): 
    current(begin), end(end), pred(pred)
    {
    while((current != end) && (!pred(*current)))
        ++current;
    }

    T& operator*() const { return *current; }
    T& operator->() const { return *current; }

    JournalFilterIterator& operator++() { advance(); return *this; }

    bool operator==(const JournalFilterIterator& other) const { return current == other.current; }
    bool operator!=(const JournalFilterIterator& other) const { return !(operator==(other)); }
private:
    void advance()
    {
        do
        {
            ++current;
        }
        while((current != end) && (!pred(*current)));
    } // advance

    iterator_type current;
    iterator_type end;
    std::function<bool (T)> pred;
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
    JournalIterator<JournalEntry>,
    CJournalIterator<JournalEntry>, 
    JournalFilterIterator<JournalIterator<JournalEntry>, JournalEntry>, 
    JournalEntry>{
public:
    long size = 0;
    JournalIterator<JournalEntry>  begin()       { return {entries};}

    CJournalIterator<JournalEntry> begin() const { return {entries};}

    JournalIterator<JournalEntry>  end()         {return {size == 0 ? entries: entries + size};}

    CJournalIterator<JournalEntry> end()   const {return {size == 0 ? entries: entries + size};}

    JournalFilterIterator<JournalIterator<JournalEntry>, JournalEntry> filter(const std::function<bool (JournalEntry)>& f) {return {begin(), end(), f};};

    void add_entry(const JournalEntry& a);
    void remove_entry(int what);
    void remove_entries();

    Journal(){};

    //bool operator!=(Journal const& other) const{ return this->entries != other.entries;}

    //bool operator==(Journal const& other) const{ return this->entries == other.entries;}
    
    //Journal& operator++(){++entries; return *this;};
    //typename Journal::reference operator*() const{return *this;}

    bool is_empty(){return size == 0;}

    ~Journal(){};
private:
    JournalEntry* entries = nullptr;
};

void Journal::add_entry(const JournalEntry& a){
    if(is_empty()){
        try{
            entries = new JournalEntry[1];
        }
        catch(std::exception& ex){
            std::cerr << "Couldnt add entry to the journal " << ex.what() << std::endl;
            delete [] entries;
            entries = nullptr;
            return;
        }
        size = 1;
        entries[0] = a;
        return;
    }
    JournalEntry* temp = nullptr;
    try{
        temp = new JournalEntry[size + 1];
    }       
    catch(std::exception& ex){
        std::cerr << "Couldnt add entry to the journal " << ex.what() << std::endl;
        delete [] temp;
        return;
    }
    for (long i = 0; i < size; i++){ 
        temp[i] = entries[i];
    }
    temp[size] = a;
    delete [] entries;
    entries = temp;
    ++size;
}

void Journal::remove_entry(int what){
    if(what>=size || what < 0)
        return;
    auto temp = new JournalEntry[size - 1];
    for(long i = 0; i < size; i++){
        if(i == what)
            continue;
        temp[i] = entries[i];
    }
    delete [] entries;
    entries = temp;
    --size;
}

void Journal::remove_entries(){
    if(is_empty())
        return;
    delete [] entries;
    size = 0;
    entries = nullptr;
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

class CNumberIterator: public std::iterator<std::input_iterator_tag, const int32_t*, long, const int32_t, const int32_t>{
public:
    CNumberIterator(const int32_t* num, const int bit = 0): num(num), cur_bit(bit) {};
    CNumberIterator(const CNumberIterator &it): num(it.num), cur_bit(it.cur_bit) {};
    bool operator!=(CNumberIterator const& other) const { return num != other.num || cur_bit != other.cur_bit;};
    bool operator==(CNumberIterator const& other) const { return num == other.num && cur_bit == other.cur_bit;};
    const int32_t operator*() const {return (*num & (1 << cur_bit)) ? 1 : 0;};
    CNumberIterator& operator++(){ ++cur_bit; return *this;};
private:
    const int32_t* num;
    int cur_bit;
};

template<class iterator_type>
class NumberFilterIterator: public std::iterator<std::input_iterator_tag, const int32_t*, long, const int32_t, const int32_t>{
public:
    NumberFilterIterator(const iterator_type& begin, const iterator_type& end, const std::function<bool (int32_t)>& pred): 
    current(begin), end(end), pred(pred)
    {
    while((current != end) && (!pred(*current)))
        ++current;
    }
    bool operator!=(const NumberFilterIterator& other) const { return !(operator==(other)); }
    bool operator==(const NumberFilterIterator& other) const { return current == other.current; }
    const int32_t operator*() const { return *current; }
    NumberFilterIterator& operator++() { advance(); return *this; }
private:
    void advance()
    {
        do
        {
            ++current;
        }
        while((current != end) && (!pred(*current)));
    } // advance

    iterator_type current;
    iterator_type end;
    std::function<bool (int32_t)> pred;
};

class Number: public Iterable<
    NumberIterator, 
    CNumberIterator, 
    NumberFilterIterator<NumberIterator>,
    int32_t>{
public:
    Number(const int32_t& a = 0): value(a){};
    NumberIterator begin(){return NumberIterator(&value, 0);};
    NumberIterator end(){return NumberIterator(&value, 32);};
    CNumberIterator begin() const {return CNumberIterator(&value, 0);};
    CNumberIterator end() const {return CNumberIterator(&value, 32);};
    NumberFilterIterator<NumberIterator> filter(const std::function<bool (int32_t)>& f){return {begin(), end(), f};};
private:
    int32_t value;
};

class FileIterator: public std::iterator<std::input_iterator_tag, int, long, int, int>{
public:
    FileIterator(std::ifstream* file, bool state = true): file(file), state(state), lastRead(0) {++(*this);};
    FileIterator(const FileIterator &it): file(it.file), state(it.state), lastRead(it.lastRead) {};
    bool operator!=(FileIterator const& other) const { return state != other.state;};
    bool operator==(FileIterator const& other) const { return state == other.state;};
    const int operator*() const {return lastRead;};
    FileIterator& operator++(){
        if(state){
            try{
                //file >> lastRead;
                /*int k = */file->read((char *)(&lastRead), sizeof(lastRead));
                state = !(file->eof());
            }
            catch(const std::exception& e){
                //std::cerr << "Couldnt read in file " << e.what() << std::endl;
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
    std::ifstream* file;
    bool state;
    int lastRead;
};

class CFileIterator: public std::iterator<std::input_iterator_tag, const int, long, const int, const int>{
public:
    CFileIterator(std::ifstream* file, bool state = true): file(file), state(state), lastRead(0) {++(*this);};
    CFileIterator(const CFileIterator &it): file(it.file), state(it.state), lastRead(it.lastRead) {};
    bool operator!=(CFileIterator const& other) const { return state != other.state;};
    bool operator==(CFileIterator const& other) const { return state == other.state;};
    const int operator*() const {return lastRead;};
    CFileIterator& operator++(){
        if(state){
            try{
                //file >> lastRead;
                /*int k = */file->read((char *)(&lastRead), sizeof(lastRead));
                state = !(file->eof());
            }
            catch(const std::exception& e){
                if(file->eof()){
                    state = !(file->eof());
                    return *this;
                }
                std::cerr << "Couldnt ddread in file " << e.what() << std::endl;
                throw e;
            }
        }
        return *this;
    };
private:
    std::ifstream* file;
    bool state;
    int lastRead;
};

template<class iterator_type>
class FileFilterIterator: public std::iterator<std::input_iterator_tag, int, long, int, int>{
public:
    FileFilterIterator(const iterator_type& ibegin, const iterator_type& iend, const std::function<bool (int)>& pred): 
    icurrent(ibegin), iend(iend), pred(pred){
    while((icurrent != iend) && (!pred(*icurrent)))
        ++icurrent;
    };
    bool operator!=(const FileFilterIterator& other) const { return !(operator==(other)); };
    bool operator==(const FileFilterIterator& other) const { return icurrent == other.icurrent; };
    const int operator*() const { return *icurrent; };
    FileFilterIterator& operator++() { advance(); return *this; };
    const FileFilterIterator end() const {return {this->iend, this->iend, this->pred};};
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
    std::function<bool (int)> pred;
};


class FileI: public Iterable<FileIterator, CFileIterator, FileFilterIterator<FileIterator>, int>{
public:
    FileI(std::string s = "\n"): fileName(s), file(nullptr){
        //std::ifstream::iostate old_state = file.exceptions (); 
        //std::cout<<"KA{A"<<std::endl;
        file = new std::ifstream();
        file->exceptions( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
        try {
            file->open(fileName, std::ios::binary | std::ios::in);
        } 
        catch(const std::exception& e){
            std::cerr << "Couldnt create FileI instance " << e.what() << std::endl;
            delete file;
	    throw e;
        }
        file->exceptions( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
        //file.exceptions(old_state);
    };
    FileIterator begin(){ return {file, this->file->is_open()};};
    FileIterator end(){ return {this->file, false};};
    CFileIterator begin()const{ return {this->file, this->file->is_open()};};
    CFileIterator end()const{ return {this->file, false};};
    FileFilterIterator<FileIterator> filter(const std::function<bool (int)>& f){return {begin(), end(), f};};
    ~FileI(){file->clear(); file->close(); delete file;}
private:
    std::string fileName;
    std::ifstream* file;
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
        auto fiter = filtered.filter([](int a){return a>10;});
        std::cout<<"Printing only ints>10" << std::endl;
        for(;fiter !=std::end(fiter); ++fiter){
            std::cout << *fiter << ' ';
        }
        std::cout<<std::endl;
    }
    return 0;
}
