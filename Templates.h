#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <bits/stdc++.h>
using namespace std;

// ========== REPOSITORY TEMPLATE (like a database table) ==========

template<typename T>
class Repository {
private:
    map<string, T> data;

public:
    void add(string id, T item) { data[id] = item; }
    bool remove(string id) { return data.erase(id) > 0; }
    
    T* find(string id) {
        auto it = data.find(id);
        return (it != data.end()) ? &(it->second) : nullptr;
    }

    bool exists(string id) const { return data.find(id) != data.end(); }

    vector<T> getAll() const {
        vector<T> result;
        for (auto& p : data) result.push_back(p.second);
        return result;
    }

    vector<T> filter(function<bool(const T&)> pred) const {
        vector<T> result;
        for (auto& p : data) if (pred(p.second)) result.push_back(p.second);
        return result;
    }

    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
    void clear() { data.clear(); }

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
};

// ========== LINKED LIST TEMPLATE ==========

template<typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node *next, *prev;
        Node(T d) : data(d), next(nullptr), prev(nullptr) {}
    };
    Node *head, *tail;
    size_t count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~LinkedList() { clear(); }

    void pushBack(T item) {
        Node* n = new Node(item);
        if (!tail) head = tail = n;
        else { tail->next = n; n->prev = tail; tail = n; }
        count++;
    }

    void pushFront(T item) {
        Node* n = new Node(item);
        if (!head) head = tail = n;
        else { n->next = head; head->prev = n; head = n; }
        count++;
    }

    bool popFront(T& item) {
        if (!head) return false;
        item = head->data;
        Node* temp = head;
        head = head->next;
        if (head) head->prev = nullptr; else tail = nullptr;
        delete temp;
        count--;
        return true;
    }

    bool popBack(T& item) {
        if (!tail) return false;
        item = tail->data;
        Node* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr; else head = nullptr;
        delete temp;
        count--;
        return true;
    }

    void clear() {
        while (head) { Node* t = head; head = head->next; delete t; }
        tail = nullptr; count = 0;
    }

    size_t size() const { return count; }
    bool empty() const { return count == 0; }

    vector<T> toVector() const {
        vector<T> result;
        for (Node* c = head; c; c = c->next) result.push_back(c->data);
        return result;
    }
};

// ========== STACK TEMPLATE ==========

template<typename T>
class Stack {
private:
    vector<T> data;
public:
    void push(T item) { data.push_back(item); }
    bool pop(T& item) { if (data.empty()) return false; item = data.back(); data.pop_back(); return true; }
    bool top(T& item) const { if (data.empty()) return false; item = data.back(); return true; }
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
    void clear() { data.clear(); }
};

// ========== QUEUE TEMPLATE ==========

template<typename T>
class Queue {
private:
    vector<T> data;
public:
    void enqueue(T item) { data.push_back(item); }
    bool dequeue(T& item) { if (data.empty()) return false; item = data.front(); data.erase(data.begin()); return true; }
    bool front(T& item) const { if (data.empty()) return false; item = data.front(); return true; }
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
    void clear() { data.clear(); }
    vector<T> toVector() const { return data; }
};

// ========== SORTING TEMPLATES ==========

template<typename T>
void bubbleSort(vector<T>& arr, function<bool(const T&, const T&)> cmp) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (cmp(arr[j + 1], arr[j])) swap(arr[j], arr[j + 1]);
}

template<typename T>
int linearSearch(const vector<T>& arr, function<bool(const T&)> pred) {
    for (int i = 0; i < arr.size(); i++) if (pred(arr[i])) return i;
    return -1;
}

#endif