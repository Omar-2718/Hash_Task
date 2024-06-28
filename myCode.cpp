#include<iostream>
#include <vector>
#include<map>
#include "string"
#include "unordered_set"
#include "sensors.h"
#include "bits/stdc++.h"
using namespace std;
int hash_num(int val,int _n){
    long long n = _n;
    return (val%n + n)%n;

}
int hash_string(string str,int _n){
    long long sum = 0;
    long long n = _n;
    int base = 26;
    for(int i =0;i<str.size();i++){
        sum = sum * base +  str[i] - 'a';
        sum = (sum%n + n)%n;
    }
    return sum;

}
int hash_string_lower_upper_digits(string str,int _n){
    //// lowers from [0-25], upper [26-51] and digits [52-61]
    long long sum = 0;
    long long n = _n;
    int base = 2*26 + 10;
    for(int i =0;i<str.size();i++){
        if(islower(str[i])){
            sum = sum * base + str[i] - 'a';
        }
        else if(isupper(str[i])){
            sum = sum * base + str[i] - 'A' + 26;
        }
        else{
            sum = sum * base + str[i] - '0' + 52;
        }
        sum = (sum%n + n)%n;
    }
    return sum;

}
int hash_string_fold(string str,int n_ = 65407) {
    long long sum = 0;
    long long n = n_;
    for(int i = 0;i<str.size();i+=4){
        sum += hash_string_lower_upper_digits(str.substr(i,i+4),n_);
        sum %= n;
    }
    return sum;
}

struct SomeObject {
    const static int INTERNAL_LIMIT = 2147483647;
    string str1, str2;
    int number;

    // Convert all 3 elements as a hash value

    int hash() {
        return hash_string_fold(str1 + str2 + to_string(number),INTERNAL_LIMIT);
    }

};





struct PhoneEntry{
    const static int INTERNAL_LIMIT= 2147483647;

    string name,phone_number;
    int hash(){
        return hash_string(name,INTERNAL_LIMIT);
    }
    PhoneEntry(string name,string phone_number):name(name),phone_number(phone_number){

    }
    void print(){
        cout << name << " " << phone_number << '\n';
    }
};

struct Node{
    PhoneEntry val;
    Node *next = {};
public:Node(PhoneEntry val) : val(val){}

};
class List{
public:
    Node *head = {};
    List(Node* head): head(head){

    }
    void insert(Node* newPhoneEntry){
        Node* idx = head;
        while (idx->next){
            idx = idx->next;
        }
        idx->next = newPhoneEntry;
    }
    void print_all(){
        Node* idx = head;

        while (idx){
            idx->val.print();
            idx = idx->next;
        }
    }


};

class PhoneHashTable{
private:
    int table_size;
    int items = 0;
    vector<vector<PhoneEntry>> table;
    double limit_load_factor;

public:PhoneHashTable(int table_size,double limit_load_factor) : table_size(table_size),limit_load_factor(limit_load_factor){
        table.resize(table_size);
    }
    void print_all(){
        for(int i =0;i<table_size;i++){
            if(table[i].empty())
                continue;
            cout << "Hash value of : " << i << "\n";
            for(int j = 0;j<table[i].size();j++){
                    table[i][j].print();
            }
        }
    }
    bool get(PhoneEntry & phoneEntry){
        int idx = phoneEntry.hash() % table_size;
        for(int i =0;i<table[idx].size();i++){
            if(table[idx][i].name == phoneEntry.name) {
                phoneEntry = table[idx][i];
                return true;
            }
        }
        return false;
    }
    bool remove(PhoneEntry phoneEntry){
        int idx = phoneEntry.hash() % table_size;
        for(int i =0;i<table[idx].size();i++){
            if(table[idx][i].name == phoneEntry.name){
                swap(table[idx][i],table[idx].back());
                table[idx].pop_back();
                items--;
                return true;
            }
        }
        return false;
    }
    void rehashing(){
        cout << "Rehashing\n";
        this->print_all();
        vector<vector<PhoneEntry>>arr(table_size*2);
        for(int i =0;i<table.size();i++){
            for(int j = 0;j<table[i].size();j++){
                arr[table[i][j].hash() % (table_size*2)].push_back(table[i][j]);
            }
        }
        table.resize(table_size*2);table = arr;
        table_size *= 2;
        cout << "Rehashed\n";
        this->print_all();
    }
    void put(PhoneEntry phoneEntry){
        int idx = phoneEntry.hash() % table_size;
        for(int i =0;i<table[idx].size();i++){
            if(table[idx][i].name == phoneEntry.name){
                table[idx][i].phone_number = phoneEntry.phone_number;
                return;
            }
        }
        table[idx].push_back(phoneEntry);
        items++;
        if((double)items/table_size > limit_load_factor){
            rehashing();
        }
    }
};
class PhoneHashTableList{
private:
    int table_size;
    int items = 0;
//    vector<vector<PhoneEntry>> table;
    List** table;
    double limit_load_factor;

public:PhoneHashTableList(int table_size,double limit_load_factor) : table_size(table_size),limit_load_factor(limit_load_factor){
        table = new List*[table_size]{};
        // List* table -> list
        // List** table -> *table , *table2
        // table is a pointer for multiple lists
        // table = array of list pointers
    }
    void print_all(){
        for(int i =0;i<table_size;i++){
            table[i]->print_all();
        }
    }
    bool get(PhoneEntry & phoneEntry){
        int idx = phoneEntry.hash() % table_size;
        Node* st = table[idx]->head;

        while (st){
            if(st->val.name == phoneEntry.name){
                phoneEntry =st->val;
                return true;
            }
            st = st->next;
        }
        return false;

    }
    bool remove(PhoneEntry phoneEntry){
        int idx = phoneEntry.hash() % table_size;
        Node* i = table[idx]->head;
        Node* prev{};

        while (i){
            if(i->val.name == phoneEntry.name){
                Node* temp = i;
                prev->next = i->next;
                delete temp;
                items--;
                return true;
            }
            prev = i;
            i = i->next;
        }
        return false;
    }
    void rehashing(){
        cout << "Rehashing\n";
        this->print_all();
        List** temp = new List*[table_size*2]{};
        for(int i =0;i<table_size;i++){
            Node* j = table[i]->head;
            while (j){
                if(!temp[j->val.hash() % (table_size*2)]){
                    temp[j->val.hash() % (table_size*2)]->head = j;
                }
                temp[j->val.hash() % (table_size*2)]->insert(j);
                j = j->next;
            }
        }
        swap(temp,table);
        delete[] temp;
        table_size *= 2;
        cout << "Rehashed\n";
        this->print_all();
    }
    void put(PhoneEntry phoneEntry){
        int idx = phoneEntry.hash() % table_size;
        if(!table[idx]){
            table[idx] = new List(new Node(phoneEntry));
            return;
        }
        Node* i = table[idx]->head;
        while (i){
            if(i->val.name == phoneEntry.name){
                i->val.phone_number = phoneEntry.phone_number;
                return;
            }
            i = i->next;
        }
        table[idx]->insert(new Node(phoneEntry));

    }
};
int distinct_substr(string s){
    unordered_set<string>st;
    for(int i =0;i<s.size();i++){
        for(int j = 1;j<=s.size();j++){
            st.insert(s.substr(i,j));
        }
    }
    return st.size();
}
int count_substr_match(string a,string b){
    unordered_set<string>st1,st2;
    for(int i =0;i<a.size();i++){
        for(int j = 1;j<=a.size();j++){
            st1.insert(a.substr(i,j));
        }
    }
    for(int i =0;i<b.size();i++){
        for(int j = 1;j<=b.size();j++){
            st2.insert(b.substr(i,j));
        }
    }
    int ans = 0;
    for(auto e : st1){
        if(e != "" && st2.find(e) != st2.end()){
            ans++;
        }
    }
    return ans;

}
int unique_anagrams(string s){
    unordered_set<string>st;
    for(int i =0;i<s.size();i++){
        for(int j = 1;j<=s.size();j++){
            string tem = s.substr(i,j);
            sort(tem.begin(),tem.end());
            st.insert(tem);
        }
    }
}
int main(){
        cout << count_substr_match("aaab","ab");
//    PhoneHashTableList table(3,1);
//    table.put(PhoneEntry("mostafa", "604-401-120"));
//    table.put(PhoneEntry("mostafa", "604-401-777"));	// update
//    table.put(PhoneEntry("ali", "604-401-343"));
//    table.put(PhoneEntry("ziad", "604-401-17"));
//    table.put(PhoneEntry("hany", "604-401-758"));
//
//    table.put(PhoneEntry("belal", "604-401-550"));
//    table.put(PhoneEntry("john", "604-401-223"));
//
//    PhoneEntry e("mostafa", "");
//    if (table.get(e))
//        cout << e.phone_number << "\n";	// 604-401-777
//
//    table.print_all();
//    // Hash 0: (ali, 604-401-343)  (hany, 604-401-758)  (john, 604-401-223)
//    // Hash 1: (mostafa, 604-401-777)  (ziad, 604-401-17)
//    // Hash 2: (belal, 604-401-550)
//
//    cout << table.remove(PhoneEntry("smith", "")) << "\n"; // 0
//    cout << table.remove(PhoneEntry("hany", "")) << "\n";  // 1
//    cout << table.remove(PhoneEntry("belal", "")) << "\n";  // 1
//    table.print_all();
//    // Hash 0: (ali, 604-401-343)  (john, 604-401-223)
//    // Hash 1: (mostafa, 604-401-777)  (ziad, 604-401-17)
////    Node* head = new Node(PhoneEntry("Omar","01067649517"));
////    List list(head);
////    list.insert(new Node(PhoneEntry("Yousef","0100")));
////    list.insert(new Node(PhoneEntry("Ziad","34325")));
////    list.print_all();


}
