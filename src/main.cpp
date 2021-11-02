#include "CacheSimUnit.h"
#include <vector>

using namespace std;
int main() {

    vector<CacheSimUnit> cache_unit_list;
    cache_unit_list.emplace_back(CacheSimUnit(0, "trace0.txt"));
    cache_unit_list.emplace_back(CacheSimUnit(1, "trace1.txt"));
    cache_unit_list.emplace_back(CacheSimUnit(2, "trace2.txt"));
    cache_unit_list.emplace_back(CacheSimUnit(3, "trace3.txt"));

    // vector<CacheSimUnit> cache_unit_list{{0, "trace0.txt"}, {1, "trace1.txt"}, {2, "trace2.txt"}, {3, "trace3.txt"}};

    TraceEvent bus_event;
    unsigned short cur_uid = 0;
    int finish_count;

    while(finish_count < 3) {
        finish_count = 0;
        cur_uid = 0;
        for(auto it_sim = cache_unit_list.begin(); it_sim != cache_unit_list.end(); ++it_sim) {
            if(it_sim->run(&bus_event)) {
                std::cout << "====== cur_uid:" << cur_uid << " ======" << endl;
                std::cout << "bus_event: " << bus_event.wr_flag << " " << std::setfill('0') << std::setw(8) << std::hex << bus_event.address << endl;
                std::cout << "-----------------------" << endl;
                for(auto it_status = cache_unit_list.begin(); it_status != cache_unit_list.end(); ++it_status) {
                    it_status->updateStatus(cur_uid, &bus_event); 
                    // output status of all cache units
                    it_status->outputStatus();
                } 
                std::cout << endl;
            } else {
                finish_count++;
            }
            cur_uid++;
        } 
    }    

     
    return 0;
}
