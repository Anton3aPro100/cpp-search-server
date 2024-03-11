//Вставьте сюда своё решение из урока «Очередь запросов» темы «Стек, очередь, дек».‎

#include"search_server.h"
#include "document.h"
#include <deque>
#pragma once
class RequestQueue {
public:
 RequestQueue(SearchServer& search_server) ;
    
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        // напишите реализацию
        time_++;
        std::vector<Document> rezult;
        bool is_empty=false;
        rezult= search_server_.FindTopDocuments(raw_query, document_predicate);
        if (rezult.empty()){
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
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query) ;
    int GetNoResultRequests() const ;
private:
    struct QueryResult {
        // определите, что должно быть в структуре
        int time_request;
        bool is_empty;
        std::vector<Document> rezult;
    };
    int time_;
    int count_;
    std::deque<QueryResult> requests_;
    const int min_in_day_ = 1440;
    // возможно, здесь вам понадобится что-то ещё
    SearchServer& search_server_;
};