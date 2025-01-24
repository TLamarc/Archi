#include "Cluster.hpp"

#include "Fish.hpp"
#include "Predator.hpp"

template <typename cR>
inline cR& Model::Cluster<cR>::front() const{
    return *cluster.front();
}


template <typename cR>
inline void Model::Cluster<cR>::pop_front(){
    cluster.pop_front();
}


template <typename cR>
inline void Model::Cluster<cR>::push_back(const std::shared_ptr<cR> element){
    cluster.push_back(element);
}

template <typename cR>
inline void Model::Cluster<cR>::push_back(const cR& element){
    cluster.push_back(std::make_shared<cR>(element));
}


template <typename cR>
inline size_t Model::Cluster<cR>::size() const{
    return cluster.size();
}