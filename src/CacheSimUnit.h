#include "TraceParser.h"
#include <memory>
#include <unordered_map>
#include <iostream>
#include <iomanip>

#ifndef CACHE_SIM_UNIT_H
#define CACHE_SIM_UNIT_H

#define TAG_WIDTH 58
#define CACHE_LINE_N 16

using namespace std;

enum CacheLineStatus {
    M = 1,
    S = 0,
    I = -1
};

typedef pair<uint64_t, CacheLineStatus> tag_status_t; // <cache block tag, cache block status>
class CacheLineNode {

    public:
        tag_status_t node_value; // only 58 bits LSB used as Tag
        shared_ptr<CacheLineNode> next;
        shared_ptr<CacheLineNode> last;

        CacheLineNode(tag_status_t& node_value):
            node_value(node_value), next(nullptr), last(nullptr) {};

        ~CacheLineNode()=default;
};

typedef shared_ptr<CacheLineNode> node_ptr_t;
class CacheSimUnit{

    private:
        unsigned short uid;
        
        node_ptr_t head;
        node_ptr_t tail;
        int cache_line_num;

        unordered_map<uint64_t, node_ptr_t> tag_map;
                
        TraceParser SpecParser;

        // _function
        node_ptr_t hitCheck(uint64_t tag);

        bool addNode(uint64_t tag, CacheLineStatus s);

        void localOp(TraceEvent *core_in);

    public:

        CacheSimUnit(unsigned short uid, const string& trace_file_path) :
            uid(uid), 
            head(nullptr),
            tail(nullptr),
            cache_line_num(0),
            SpecParser(trace_file_path) {};

        ~CacheSimUnit() = default;
        CacheSimUnit(CacheSimUnit&&) = default;
        CacheSimUnit(const CacheSimUnit&) = delete;

        void outputStatus();
        void updateStatus(const unsigned short uid, TraceEvent *bus_in);
        bool run(TraceEvent *bus_event); // reutrn validness for bus
};

#endif /* ifndef CACHE_SIM_UNIT_H */
