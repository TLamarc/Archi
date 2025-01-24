#pragma once
#include "Creature.hpp"
#include <deque>
#include <memory>

namespace Model{

template <typename cR>
class Cluster{
private:
    std::deque<std::shared_ptr<cR>> cluster;
public:
    Cluster(){};
    Cluster(size_t initial_population);


public:
    cR& front() const;
    void pop_front() ;
    void push_back(const std::shared_ptr<cR>) ;
    void push_back(const cR&) ;
    size_t size() const;

    // to match STL requierments
    class Iterator;
    Iterator begin();
    Iterator end();

    class ConstIterator;
    ConstIterator begin() const;
    ConstIterator end() const;
};


template class Model::Cluster<Model::Fish>;
template class Model::Cluster<Model::Predator>;


// Nested Iterator class
template <typename cR>
class Cluster<cR>::Iterator {
private:
    typename std::deque<std::shared_ptr<cR>>::iterator iter;

public:
    // Constructor
    Iterator(typename std::deque<std::shared_ptr<cR>>::iterator it) : iter(it) {}

    // Dereference operator
    cR& operator*() const {
        return **iter; // Dereference the shared_ptr
    }

    // Pre-increment operator
    Iterator& operator++() {
        ++iter;
        return *this;
    }

    // Equality comparison
    bool operator==(const Iterator& other) const {
        return iter == other.iter;
    }

    // Inequality comparison
    bool operator!=(const Iterator& other) const {
        return iter != other.iter;
    }
};


// Methods to expose iterators
template <typename cR>
typename Cluster<cR>::Iterator Cluster<cR>::begin(){
    return Iterator(cluster.begin());
}

template <typename cR>
typename Cluster<cR>::Iterator Cluster<cR>::end() {
    return Iterator(cluster.end());
}




// Nested ConstIterator class
template <typename cR>
class Cluster<cR>::ConstIterator {
private:
    typename std::deque<std::shared_ptr<cR>>::const_iterator iter;

public:
    // Constructor
    ConstIterator(typename std::deque<std::shared_ptr<cR>>::const_iterator it) : iter(it) {}

    // Dereference operator
    const cR& operator*() const {
        return **iter; // Dereference the shared_ptr
    }

    // Pre-increment operator
    ConstIterator& operator++() {
        ++iter;
        return *this;
    }

    // Equality comparison
    bool operator==(const ConstIterator& other) const {
        return iter == other.iter;
    }

    // Inequality comparison
    bool operator!=(const ConstIterator& other) const {
        return iter != other.iter;
    }
};

// Methods to expose iterators
template <typename cR>
typename Cluster<cR>::ConstIterator Cluster<cR>::begin() const {
    return ConstIterator(cluster.begin());
}

template <typename cR>
typename Cluster<cR>::ConstIterator Cluster<cR>::end() const {
    return ConstIterator(cluster.end());
}




}