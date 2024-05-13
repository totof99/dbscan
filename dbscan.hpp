#include <vector>
#include <unordered_map>

template<class Element, class DistanceType, class DistanceComputer>
std::vector<std::size_t> range_query(const std::vector<Element>& elements, std::size_t q, DistanceType epsilon2, const DistanceComputer& computeDistance2)
{
    std::vector<std::size_t> neighbors;
    for(std::size_t p = 0; p < elements.size(); ++p)
    {
        if(q != p && computeDistance2(elements[q], elements[p]) <= epsilon2)
        {
            neighbors.emplace_back(p);
        }
    }
    return neighbors;
}

static const int NOISE = -1;

//adapted from wikipedia pseudocode 
template<class Element, class DistanceType, class DistanceComputer>
std::unordered_map<std::size_t, int> dbscan(const std::vector<Element>& elements, DistanceType epsilon, std::size_t minClusterSize, DistanceComputer computeDistance2)
{
    const auto epsilon2 = epsilon*epsilon;
    int clusterCounter = 0;
    std::unordered_map<std::size_t, int> label;//index of element is key, value is cluster
    for(std::size_t p = 0; p < elements.size(); ++p)
    {
        if(label.find(p) != label.end()) 
            continue; //already processed
        auto seed = range_query(elements, p, epsilon2, computeDistance2);
        if(seed.size()+1 < minClusterSize) //if P is not a core point, +1 because range_query exclude origin
        {
            label[p] = NOISE;
            continue;
        }
        ++clusterCounter;
        label[p] = clusterCounter;
        std::vector<bool> presentInSet(elements.size(), false);
        for(std::size_t q : seed)
            presentInSet[q] = true;
            
        for(std::size_t index = 0; index < seed.size(); ++index)//size will change
        {
            std::size_t q = seed[index];
            auto [found, inserted] = label.emplace(q, clusterCounter);
            if(!inserted)
            {
                if(found->second == NOISE)
                    found->second = clusterCounter;
                else
                    continue; //already processed
            }
            const auto& neighbors = range_query(elements, q, epsilon2, computeDistance2);
            if(neighbors.size()+1 >= minClusterSize) //if Q is a core point, +1 because range_query exclude origin
                for(std::size_t n : neighbors)
                    if(!presentInSet[n])
                    {
                        presentInSet[n] = true;
                        seed.emplace_back(n);
                    }
        }
    }
    return label;
}