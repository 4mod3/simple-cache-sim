#ifndef TRACE_PARSER_H
#define TRACE_PARSER_H

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
using namespace std;

struct TraceEvent {
    char wr_flag; // '1': write, '0': read
    uint64_t address; 
};

class TraceParser {
    private:
        ifstream input;
        int lineNumber;

    public:
        TraceParser(const string& input_file_path);

        ~TraceParser();
        TraceParser(TraceParser&&) = default;

        bool getNextTraceLine(TraceEvent *cur_event); // if eof occur, return false, else true

        bool eof() const { return input.eof(); }

};

#endif /* ifndef TRACE_PARSER_H*/
