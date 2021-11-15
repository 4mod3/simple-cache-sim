#include "CacheSimUnit.h"

bool CacheSimUnit::addNode(uint64_t tag, CacheLineStatus s) {
    tag_status_t content = std::make_pair(tag, s);
    node_ptr_t new_node_ptr = std::make_shared<CacheLineNode>(content);

    tag_map[tag] = new_node_ptr;

    if(head == nullptr) {
        // the first node
        head = new_node_ptr;
        tail = new_node_ptr;
    } else {
        // add new node to the head
        new_node_ptr->next = head;
        head->last = new_node_ptr;
        head = new_node_ptr;
    }
    cache_line_num++;

    bool res;
    if(cache_line_num > CACHE_LINE_N) {
        // tail swap out  
        res = tag_map.erase(tail->node_value.first);
        tail = tail->last;
        tail->next = nullptr;
        return res;
    }
    return true;
}

node_ptr_t CacheSimUnit::hitCheck(uint64_t tag) {
    const auto check_ptr = tag_map.find(tag);
    return ((check_ptr == tag_map.cend()) ? nullptr : check_ptr->second);

}

void CacheSimUnit::localOp(TraceEvent *core_in) {
    uint64_t tag_in = core_in->address >> (64-TAG_WIDTH);
    const auto check_ptr = hitCheck(tag_in);  
    
    if(check_ptr == nullptr){
        // miss
        if(core_in->wr_flag == '0') {
            addNode(tag_in, S); // error if false
        } else if(core_in->wr_flag == '1') {
            addNode(tag_in, M); // error if false
        }
    } else {
        // hit, then update Status
        if(core_in->wr_flag == '1') {
            check_ptr->node_value.second = M;
        } else if(check_ptr->node_value.second == I) {
            check_ptr->node_value.second = S;
        }

        // move hited-CacheLineNode to the head
        if(check_ptr->next == nullptr && check_ptr->last != nullptr) {
            check_ptr->next = head;
            tail = check_ptr->last;
            head = tail->next;
            tail->next = nullptr;
            check_ptr->last = nullptr;
        } else if(check_ptr->last != nullptr) {
            check_ptr->next->last = check_ptr->last;
            check_ptr->last->next = check_ptr->next;
            check_ptr->next = head;
            head = check_ptr;
            check_ptr->last = nullptr;
        }
    }
}

void CacheSimUnit::updateStatus(const unsigned short uid_in, TraceEvent *bus_in) {
    if(uid == uid_in) {
        return;
    } 

    // firstly, check if bus_event->tag is one of self-CacheLines
    const auto check_ptr = hitCheck(bus_in->address >> (64-TAG_WIDTH));
    if(check_ptr == nullptr){
        return;
    }

    // update Status
    if(bus_in->wr_flag == '1') {
        check_ptr->node_value.second = I;
    } else if(bus_in->wr_flag == '0') {
        if(check_ptr->node_value.second == M) {
            // write back to Mem
            check_ptr->node_value.second = S;
        }
    }
}

bool CacheSimUnit::run(TraceEvent *bus_event) {
    if(SpecParser.getNextTraceLine(bus_event)) {
        localOp(bus_event);
        return true;
    } else {
        return false;
    }
}

void CacheSimUnit::outputStatus() {
    std::cout << "uid: " << uid << std::endl;
    std::cout << "[Tag]    " << "[Status]" << endl;
    for(node_ptr_t i = head; i != nullptr; i=i->next) {
        char status_char;
        switch (i->node_value.second) {
            case I:
                status_char = 'I';  
                break;
            case S:
                status_char = 'S';
                break;
            case M:
                status_char = 'M';
                break;
            default:
               status_char = 'I'; 
        }
        std::cout << std::setfill('0') << std::setw(8) << std::hex << i->node_value.first << " " << status_char << endl;
    }
}
