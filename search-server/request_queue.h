#pragma once

#include"search_server.h"
#include "document.h"
#include <deque>

class RequestQueue {
public:
 RequestQueue(SearchServer& search_server) ;
 
    template <typename DocumentPredicate>
    int AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);
    
    int AddFindRequest(const std::string& raw_query, DocumentStatus status);
    int AddFindRequest(const std::string& raw_query) ;
    int GetNoResultRequests() const ;
private:
    struct QueryResult {
        
        uint time_request;
        bool is_empty;
        int rezult;
    };
    uint time_;
    int count_;
    std::deque<QueryResult> requests_;
    const int min_in_day_ = 1440;
    
    SearchServer& search_server_;
};

template <typename DocumentPredicate>
    int RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        time_++;
        int rezult;
        bool is_empty=false;
        rezult= search_server_.FindTopDocuments(raw_query, document_predicate).size();
        if (rezult==0){
            count_+=1;
            is_empty=true;
        }
        requests_.push_back({time_,is_empty,rezult});
        while (requests_.empty()==false){
            if (time_-requests_.front().time_request<min_in_day_)
            {break;}

            if (requests_.front().is_empty){
                count_--;
            }
            requests_.pop_front();
        }
        return rezult;
    }

