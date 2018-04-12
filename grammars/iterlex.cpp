#include <iterable.hpp>
#include <lex.hpp>


class lex_seq: public Iterable<
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
