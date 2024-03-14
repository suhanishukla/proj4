//
//  HashMap.h
//  cs32project4
//
//  Created by Suhani Shukla on 3/11/24.
//

#ifndef HashMap_h
#define HashMap_h
#include <string>
#include <vector>
#include <list>
template<typename T>
class HashMap
{
public:
    HashMap(double maxLoadFactor = 0.75) : m_maxLoad(maxLoadFactor) {
        m_numBuckets = 10;
        table.resize(10);
    }
    ~HashMap() {
        for (auto& bucket : table) {
            for (Node* current : bucket) {
                delete current;
            }
        }
    }
    void reset();
    int size() const {return itemCount;}
    void insert(const std::string& key, const T& value) {
        if ((itemCount + 1) / static_cast<double>(table.size()) > m_maxLoad) {
            rehash();
        }
        size_t hash = std::hash<std::string>()(key);
        size_t index = hash % table.size();
        
        for (Node* current : table[index]) {
            if (current->key == key) {
                current->value = value;
                return;
            }
        }
        Node* newNode = new Node(key, value);
        table[index].push_back(newNode);
        itemCount++;
    }
    T& operator[](const std::string& key) {
        if (find(key) == nullptr) {
            insert(key, T());
        }
        size_t hash = std::hash<std::string>()(key);
        size_t index = hash % table.size();
        
        for (Node* current : table[index]) {
            if (current->key == key) {
                return current->value;
            }
        }
        return table[0].front()->value;
    }
    const T* find(const std::string& key) const {
        size_t hash = std::hash<std::string>()(key);
        size_t index = hash % table.size();
        
        for (const Node* current : table[index]) {
            if (current->key == key) {
                return &(current->value);
            }
        }
        
        return nullptr; // Key not found
    }
    T* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }
private:
    int itemCount;
    int m_numBuckets;
    double m_maxLoad;
    
    struct Node {
        std::string key;
        T value;
        Node* next;
        Node(const std::string& k, const T& v) : key(k), value(v), next(nullptr) {}
    };
    
    std::list<Node*> table;
    //int getBucket(const KeyType& key) const;
    void rehash() {
        // Double the number of buckets
        table.resize(table.size() * 2);
        
        // Initialize new buckets
        for (size_t i = 0; i < table.size(); ++i) {
            table[i] = std::list<Node*>();
        }
        
        // Reinsert all nodes into the new buckets
        for (const auto& oldBucket : table) {
            for (Node* current : oldBucket) {
                size_t newIndex = std::hash<std::string>()(current->key) % table.size();
                table[newIndex].push_back(current);
            }
            
        }
    }
};



#endif /* HashMap_h */
