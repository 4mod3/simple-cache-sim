#include "TraceParser.h"

TraceParser::TraceParser(const string& input_file_path) {
    input.open(input_file_path);
    if (!input.is_open()) {
        perror("[Error opening track file!] exit:");
        exit(1);
    }

    lineNumber = 0;
}

TraceParser::~TraceParser() {
    input.close();
}

bool TraceParser::getNextTraceLine(TraceEvent *cur_event) {
    input >> cur_event->wr_flag >> std::hex >> cur_event->address;
    if (eof()) { return false; }

    lineNumber++;

    return true;
}
