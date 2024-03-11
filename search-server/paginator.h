//Вставьте сюда своё решение из урока «‎Очередь запросов».
#include <vector>
#include <iostream>
#include <string>
#pragma once


template <typename Iterator>       
class Paginator {
        
        
        private:
        std::vector <std::pair<Iterator,Iterator>> pages_;
        public:
        Paginator(const Iterator begin,const Iterator end, const size_t page_size){
            std::vector <std::pair<Iterator,Iterator>> pages;
            Iterator it_begin=begin;
            Iterator it_end=begin;
            while (it_end!=end){
                if (distance(it_begin,end)<=page_size){
                    it_end=end;
                }
                else{
                   advance(it_end,page_size);
                }
                std::pair<Iterator,Iterator> page={it_begin,it_end};
                pages.push_back(page);
                it_begin=it_end;
            }
            pages_=pages;
        }
        auto begin() const{
            return pages_.begin();
        }
        
        auto end() const{
            return pages_.end();
        }

        size_t size(){
            return pages_.size();
        }





};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& output, std::pair<Iterator,Iterator> x) {
    auto it=x.first;
    for (int i=0; i<distance(x.first,x.second);++i){
        output <<"{ document_id = "<< it->id<<", relevance = "<<it->relevance<<", rating = "<<it->rating<<" }" ;
        advance(it,1);
    }
    //output <<endl<<"Page break"<<endl;
    //{ document_id = 2, relevance = 0.402359, rating = 2 }{ document_id = 4, relevance = 0.229073, rating = 2 }
    
    return output;
}
