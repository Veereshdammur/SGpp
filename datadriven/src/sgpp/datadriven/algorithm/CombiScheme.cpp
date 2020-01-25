// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <sgpp/datadriven/algorithm/CombiScheme.hpp>

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <map>
#include <set>
#include <numeric>
#include <unordered_set>
#include <utility>
#include <vector>

namespace sgpp {
namespace datadriven {

void CombiScheme::initialize(size_t dim, size_t level) {
    dimension = dim;
    this->level = level;
    initIndexSet();
}

void CombiScheme::initializeGaSG(size_t dim, size_t level, const std::set<std::set<size_t>>& interactions) {
    dimension = dim;
    this->level = level;
    initIndexSetGaSG(interactions);
}

bool CombiScheme::isRefinable(std::vector<size_t> levelvec) {
    if (index_set.find(levelvec) != index_set.end()) {
        return index_set[levelvec];
    }
    return false;
}

bool CombiScheme::refineComponent(std::vector<size_t> levelvec) {
    if (!isRefinable(levelvec)) {
        return false;
    }
    index_set[levelvec] = false;
    for (size_t dim = 0; dim < dimension; dim++) {
        refineScheme(dim, levelvec);
    }
    return true;
}

bool CombiScheme::refineComponentGaSG(std::vector<size_t> levelvec, const std::set<std::set<size_t>>& interactions) {
    if (!isRefinable(levelvec)) {
        return false;
    }
    index_set[levelvec] = false;
    for (size_t dim = 0; dim < dimension; dim++) {
        refineSchemeGaSG(dim, levelvec, interactions);
    }
    return true;
}

void CombiScheme::insertParents(std::vector<size_t>& levelvec, const std::set<std::set<size_t>>& interactions, std::set<size_t>& prevInter) {
    for (size_t d = 0; d < dimension; d++) {
        if (levelvec[d] < 2) {
            continue;
        }
        std::vector<size_t> levelvec_copy = levelvec;
        levelvec_copy[d]--;
        if (index_set.find(levelvec_copy) != index_set.end()) {
            continue;
        }
        if (levelvec_copy[d] == 1) {
            prevInter.erase(d);
        }
        index_set[levelvec_copy] = false;
        for (size_t i = 0; i < dimension; i++) {
            if (d == i) {
                continue;
            }
            if (levelvec_copy[i] == 1) {
                prevInter.insert(i);
            }
            levelvec_copy[i]++;
            if (interactions.find(prevInter) == interactions.end()) {
                levelvec_copy[i]--;
                if (levelvec_copy[i] == 1) {
                    prevInter.erase(i);
                }
                continue;
            }
            if (index_set.find(levelvec_copy) == index_set.end()) {
                levelvec_copy[i]--;
                if (levelvec_copy[i] == 1) {
                    prevInter.erase(i);
                }
                index_set[levelvec_copy] = true;
                break;
            }
            levelvec_copy[i]--;
            if (levelvec_copy[i] == 1) {
                prevInter.erase(i);
            }
        }
        insertParents(levelvec_copy, interactions, prevInter);
        if (levelvec_copy[d] == 1) {
            prevInter.insert(d);
        }
    }
}

void CombiScheme::refineSchemeGaSG(size_t dim, std::vector<size_t> levelvec, const std::set<std::set<size_t>>& interactions) {
    levelvec[dim]++;
    std::set<size_t> inter;
    for (size_t i = 0; i < dimension; i++) {
        if (levelvec[i] > 1) {
            inter.insert(i);
        }
    }
    if (interactions.find(inter) == interactions.end()) {
        return;
    }
    index_set[levelvec] = true;
    insertParents(levelvec, interactions, inter);
}

void CombiScheme::refineScheme(size_t dim, std::vector<size_t> levelvec) {
    levelvec[dim]++;
    for (size_t d = 0; d < dimension; d++) {
        std::vector<size_t> levelvec_copy = levelvec;
        levelvec_copy[d]--;
        // check if parents are all there
        if (index_set.find(levelvec_copy) == index_set.end() && levelvec_copy[d] > 0) {
            return;
        }
    }
    index_set[levelvec] = true;
}

void CombiScheme::getArrangements(std::vector<std::vector<size_t>>& arrangements, size_t size, size_t minDim, std::vector<size_t>& curArr, size_t freeLevels) {
    for (size_t i = minDim; i < size; i++) {
        std::vector<size_t> newArr = curArr;
        newArr.at(i)++;
        arrangements.push_back(newArr);
        if (freeLevels > 1) {
            getArrangements(arrangements, size, i, newArr, freeLevels - 1);
        }
    }
}

void CombiScheme::initIndexSetGaSG(const std::set<std::set<size_t>>& interactions) {
    std::vector<size_t> rootVec = std::vector<size_t>(dimension, 1);
    index_set[rootVec] = (interactions.size() <= 1);
    for (const std::set<size_t>& interaction: interactions) {
        size_t size = interaction.size();
        if (size >= level || size == 0) {
            continue;
        }
        std::vector<size_t> myMinVec = rootVec;
        std::vector<size_t> whichDims;
        for (const size_t& dim: interaction) {
            myMinVec.at(dim) = 2;
            whichDims.push_back(dim);
        }
        if (size+1 == level) {
            index_set[myMinVec] = true;
            continue;
        }
        index_set[myMinVec] = false;
        size_t freeLevels = level - size - 1;
        std::vector<std::vector<size_t>> arrangements;
        std::vector<size_t> initialArr = std::vector<size_t>(size, 2);
        getArrangements(arrangements, size, 0, initialArr, freeLevels);
        for (const std::vector<size_t>& arr: arrangements) {
            size_t sum = 0;
            std::vector<size_t> thisVec = rootVec;
            for (size_t i = 0; i < size; i++) {
                sum += arr.at(i);
                thisVec.at(whichDims.at(i)) = arr.at(i);
            }
            if (sum == (2*size) + freeLevels) {
                index_set[thisVec] = true;
            }
            else {
                index_set[thisVec] = false;
            }
        }
    }
}

void CombiScheme::initIndexSet() {
    for (const std::vector<size_t>& v : getGrids(dimension, level)) {
        index_set[v] = true;
    }
    for (size_t q = 1; q < std::min(dimension, level); q++) {
        for (const std::vector<size_t>& v : getGrids(dimension, level - q)) {
            index_set[v] = false;
        }
    }
}

std::unordered_set<std::vector<size_t>, VectorHash> CombiScheme::getGrids(
    size_t dim, size_t values) {
    std::unordered_set<std::vector<size_t>, VectorHash> grids;
    if (dim == 1) {
         grids.insert(std::vector<size_t>(1, values));
         return grids;
    }
    for (size_t index = 0; index < values; index++) {
        for (std::vector<size_t> v : getGrids(dim - 1, values - index)) {
            v.insert(v.begin(), index + 1);
            grids.insert(v);
        }
    }
    return grids;
}

std::vector<std::pair<std::vector<size_t>, int>> CombiScheme::getCombiScheme() {
    std::vector<std::pair<std::vector<size_t>, int>> grid_array;
    std::map<std::vector<size_t>, int> grid_dict;

    for (auto const& index : index_set) {
        std::vector<std::vector<int>> stencil_elements;
        for (size_t d = 0; d < dimension; d++) {
            for (std::vector<int>& stencil_element : stencil_elements) {
                stencil_element.emplace_back(0);
            }
            if (stencil_elements.empty()) {
                std::vector<int> first{0};
                stencil_elements.push_back(first);
            }
            if (index.first[d] > 1) {
                // hier verdoppeln, zweite hälfte letztes element -1 statt 0
                std::vector<std::vector<int>> new_stencil_elements;
                for (std::vector<int>& stencil_element : stencil_elements) {
                    std::vector<int> new_stencil_element = stencil_element;
                    new_stencil_element.back() = -1;
                    new_stencil_elements.push_back(new_stencil_element);
                }
                for (std::vector<int>& stencil_element : new_stencil_elements) {
                    stencil_elements.push_back(stencil_element);
                }
            }
        }
        for (std::vector<int>& stencil_element : stencil_elements) {
            int update_coefficient = std::accumulate(stencil_element.begin(),
                                                     stencil_element.end(), 0);
            update_coefficient = (abs(update_coefficient - 1) % 2) - (abs(update_coefficient) % 2);
            std::vector<size_t> new_stencil_element(dimension, 0);
            std::transform(stencil_element.begin(), stencil_element.end(),
                           index.first.begin(), new_stencil_element.begin(), std::plus<int>());
            grid_dict[new_stencil_element] += update_coefficient;
        }
    }

    for (auto it = grid_dict.cbegin(); it != grid_dict.cend() ; it++) {
        if (it->second != 0) {
            grid_array.emplace_back(it->first, it->second);
        }
    }
    return grid_array;
}

} /* namespace datadriven */
} /* namespace sgpp */
