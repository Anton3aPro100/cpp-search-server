//Вставьте сюда своё решение из урока «Очередь запросов» темы «Стек, очередь, дек».‎

#include "request_queue.h"
using namespace std;

 RequestQueue::RequestQueue(SearchServer& search_server) :
      time_(0),count_(0) ,search_server_(search_server)
{}
    
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    
    
    vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
        
         return AddFindRequest(
            raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
    }
    vector<Document> RequestQueue::AddFindRequest(const string& raw_query) {
        return AddFindRequest(raw_query,DocumentStatus::ACTUAL);
        
    }
    int RequestQueue::GetNoResultRequests() const {
        // напишите реализацию
        return count_;
    }
