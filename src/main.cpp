/**
 *  Entry-point for simple-cache-sim
 *  
 *  by Gins
 */
#include "CacheSimUnit.h"
#include <vector>

using namespace std;
int main() {

    // 实例化Cache单元
    vector<CacheSimUnit> cache_unit_list;
    cache_unit_list.emplace_back(CacheSimUnit(0, "trace0.txt"));
    cache_unit_list.emplace_back(CacheSimUnit(1, "trace1.txt"));
    cache_unit_list.emplace_back(CacheSimUnit(2, "trace2.txt"));
    cache_unit_list.emplace_back(CacheSimUnit(3, "trace3.txt"));

    // 准备模拟调度需要的辅助变量
    TraceEvent bus_event;
    unsigned short cur_uid = 0;
    int finish_count;
    int op_count = 0;

    // 串行调度执行，串行更新不同Cache单元的状态
    while(finish_count < 4) {
        finish_count = 0;
        cur_uid = 0;
        for(auto it_sim = cache_unit_list.begin(); it_sim != cache_unit_list.end(); ++it_sim) {
            if(it_sim->run(&bus_event)) {
                std::cout << "====== event_count: " << std::dec << op_count << " ======" << endl;
                std::cout << "cur_uid: " << cur_uid << endl; 
                std::cout << "bus_event: " << bus_event.wr_flag << " " << std::setfill('0') << std::setw(8) << std::hex << bus_event.address << endl;
                std::cout << "-----------------------" << endl;
                for(auto it_status = cache_unit_list.begin(); it_status != cache_unit_list.end(); ++it_status) {
                    it_status->updateStatus(cur_uid, &bus_event); 
                    // 输出每个Cache单元更新后的状态
                    it_status->outputStatus();
                } 
                std::cout << endl;
                op_count++;
            } else {
                finish_count++;
            }
            cur_uid++;
        } 
    }    

     
    return 0;
}
