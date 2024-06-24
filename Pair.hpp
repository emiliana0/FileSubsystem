#pragma once

template<typename K, typename V>
struct Pair {
    K key;
    V value;

    Pair() = default;
    Pair(const K& key, const V& value) : key(key), value(value) {}
};