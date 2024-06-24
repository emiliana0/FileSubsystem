#pragma once
#include "Vector.hpp"
#include "Pair.hpp"

template<typename K, typename V>
class Map {
private:
    Vector<Pair<K, V>> data;

public:
    void insert(const K& key, const V& value);
    void erase(const K& key);
    V* find(const K& key);
    bool contains(const K& key) const;
    bool empty() const;
    V& operator[](const K& key);
    typename Vector<Pair<K, V>>::Iterator end();
};

template<typename K, typename V>
void Map<K, V>::insert(const K& key, const V& value) {
    for (size_t i = 0; i < data.getSize(); ++i) {
        if (data[i].key == key) {
            data[i].value = value;
            return;
        }
    }
    data.push_back(Pair<K, V>(key, value));
}

template<typename K, typename V>
void Map<K, V>::erase(const K& key) {
    for (size_t i = 0; i < data.getSize(); ++i) {
        if (data[i].key == key) {
            data.erase(i);
            return;
        }
    }
}

template<typename K, typename V>
V* Map<K, V>::find(const K& key) {
    for (size_t i = 0; i < data.getSize(); ++i) {
        if (data[i].key == key) {
            return &data[i].value;
        }
    }
    return nullptr;
}

template<typename K, typename V>
bool Map<K, V>::contains(const K& key) const {
    for (size_t i = 0; i < data.getSize(); ++i) {
        if (data[i].key == key) {
            return true;
        }
    }
    return false;
}

template<typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
    for (size_t i = 0; i < data.getSize(); ++i) {
        if (data[i].key == key) {
            return data[i].value;
        }
    }
    data.push_back(Pair<K, V>(key, V()));
    return data[data.getSize() - 1].value;
}

template<typename K, typename V>
typename Vector<Pair<K, V>>::Iterator Map<K, V>::end() {
    return data.end();
}

template<typename K, typename V>
bool Map<K, V>::empty() const {
    return data.getSize() == 0;
}