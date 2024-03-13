
#include "request_queue.h"
using namespace std;

 RequestQueue::RequestQueue(SearchServer& search_server) :
      time_(0),count_(0) ,search_server_(search_server)
{}
    
    int RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
        
         return AddFindRequest(
            raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
    }
    int RequestQueue::AddFindRequest(const string& raw_query) {
        return AddFindRequest(raw_query,DocumentStatus::ACTUAL);
        
    }
    int RequestQueue::GetNoResultRequests() const {
        
        return count_;
    }
