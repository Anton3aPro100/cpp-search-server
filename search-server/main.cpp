
#include <algorithm> 
#include <iostream> 
#include <set> 
#include <string> 
#include <utility> 
#include <vector> 
#include <map> 
#include <cmath> 
using namespace std; 
const int MAX_RESULT_DOCUMENT_COUNT = 5; 
string ReadLine() { 
    string s; 
    getline(cin, s); 
    return s; 
} 
int ReadLineWithNumber() { 
    int result = 0; 
    cin >> result; 
    ReadLine(); 
    return result; 
} 
vector<string> SplitIntoWords(const string& text) { 
    vector<string> words; 
    string word; 
    for (const char c : text) { 
        if (c == ' ') { 
            if (!word.empty()) { 
                words.push_back(word); 
                word.clear(); 
            } 
        } 
        else { 
            word += c; 
        } 
    } 
    if (!word.empty()) { 
        words.push_back(word); 
    }
    return words; 
} 
struct Document { 
    int id; 
    double relevance; 
}; 
class SearchServer { 

    public: 
    void SetStopWords(const string& text) { 
        for (const string& word : SplitIntoWords(text)) { 
            stop_words_.insert(word); 
        } 
    } 
    
    struct Query{ 
    set <string> plus_words; 
    set <string> minus_words; 
}; 

void AddDocument(const int& document_id, const string& document) { 
/* Я много и долго пытался переписать этот метод, пытался и сделать все в одном цикле,
и считать до цикла 1.size, и использовать мтод из библиотеки алгоритмов для подсчета количества въождений эемента в вектор. И у всех этих попыток тренажер засчитывал только 11 из 18 и писал подсказку что реализация слишком медленная.
А как было писал - Успех!
Поэтому оставил как есть. 

Так же не понял чем надо заменить insert для множества.

Не могли бы вы засчитать эту версию, все-таки достаточно логичные действия там, и два цикла не вложенны же друг в друга...
А то я и так еле еле успеваю к дедлайну...
И мне очень интересно просто увидеть как эту функцию можно более оптиально написать
*/ 
        const vector<string> words = SplitIntoWordsNoStop(document); 
        double words_size=words.size(); 
        map <string,int> tf; 

        for (const string& word: words){ 

            if (tf.count(word)>0) { 

                tf[word]+=1; 

            } 

            else{ 

                tf.insert({word,1}); 

            } 
        } 

        for (pair<string,int> word_tf:tf) { 

            index_[word_tf.first].insert({document_id,double(word_tf.second)/words_size}); 
    } 
}


    vector<Document> FindTopDocuments(const string& raw_query) const  {
        const Query query_words = ParseQuery(raw_query); 
        auto matched_documents = FindAllDocuments(query_words); 
        sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) { return lhs.relevance > rhs.relevance; }); 

        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) { 
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT); 

        } 
        return matched_documents; 
    } 

 void SetDocumentsCount(int documents_count){ 
     documents_count_=documents_count; 
 } 

private: 
    map<string,set<pair<int,double>>> index_; 
    set<string> stop_words_; 
    int documents_count_=0; 
    double idf_calculate(const string& word) const{
    // написал отдельный метод для подсчета IDF, но если его применяю, вместо вычислений 
//без функции, тренажер пишет, что слишком медленно, хотя теперь только в одной из задач.
// Думаю, что разница незначительна, но так тренажер видимо настроен 
       
        
        double idf=double(documents_count_); 
        idf/=index_.at(word).size(); 
        idf=log(idf); 
        return idf;
    }
    bool IsStopWord(const string& word) const { 
        return stop_words_.count(word) > 0; 
    }
    vector<string> SplitIntoWordsNoStop(const string& text) const { 
        vector<string> words; 
        for (const string& word : SplitIntoWords(text)) { 
            if (!IsStopWord(word)) { 
                words.push_back(word); 
            } 
        } 
        return words; 
    } 
    Query ParseQuery(const string& text) const { 
        Query query_words; 
        for (const string& word : SplitIntoWordsNoStop(text)) { 
            if (word[0]=='-'){ 
                query_words.minus_words.insert(word.substr(1)); 
            } 

            else{ 
                query_words.plus_words.insert(word); 
            } 
        } 

        return query_words; 

    } 

 

    vector<Document> FindAllDocuments(const Query& query_words) const { 
        vector<Document> matched_documents; 
        map<int,double> documents_hit; 
        for (int id=0; id<documents_count_;++id ) documents_hit[id]=-1; 
        for (const string& word : query_words.plus_words){
            if (index_.count(word)>0){
               double idf=double(documents_count_); 
               idf/=index_.at(word).size(); 
               idf=log(idf); 
               
           for (const pair<int,double>& tf :index_.at(word) ){ 
               if (documents_hit[tf.first]==-1)  documents_hit[tf.first]=0; 
                documents_hit[tf.first]+=tf.second*idf; 
                } 
            } 
        } 

       for (const string& word : query_words.minus_words){ 
           if (index_.count(word)>0){ 

           for (pair<int,double> tf :index_.at(word) ){ 
                    documents_hit[tf.first]=-1; 
                } 
            } 
        }
        for (int id=0; id<documents_count_;++id ) { 
            if (documents_hit[id]>=0) matched_documents.push_back({id,documents_hit[id]}); 
        } 
        return matched_documents; 
    } 
}; 

 
SearchServer CreateSearchServer() { 
    SearchServer search_server; 
    search_server.SetStopWords(ReadLine()); 
    const int document_count = ReadLineWithNumber(); 
    for (int document_id = 0; document_id < document_count; ++document_id) { 
        search_server.AddDocument(document_id, ReadLine()); 
    } 
    search_server.SetDocumentsCount(document_count); 
    return search_server; 
} 

int main() { 
   const SearchServer search_server = CreateSearchServer();
   const string query = ReadLine(); 
   for ( auto& [document_id, relevance] : search_server.FindTopDocuments(query)) { 
       cout << "{ document_id = "s << document_id << ", " << "relevance = "s << relevance << " }"s << endl; 
   } 
}