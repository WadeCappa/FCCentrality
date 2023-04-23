#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <numeric>
#include <bits/stdc++.h>
#include <unordered_set>
#include <cmath>
#include <string>
#include <cstddef>
#include <concepts>

#include "fc_vector.h"

#ifndef NONDOMINATEDVECTORS_H
#define NONDOMINATEDVECTORS_H

template<typename T>
class NonDominatedVectors
{
    private:
        std::vector<std::pair<T, std::mutex>> scores;

    public:
        // T requires (T x) { x + x; }
        void Update(size_t v, T score)
        {
            this->scores[v].second.lock();
            this->scores[v].first = this->scores[v].first + score;
            this->scores[v].second.unlock();
        }

        NonDominatedVectors(size_t n)    
            : scores(
                n
            )
        {
        }

        std::vector<T> Convert() const 
        {
            std::vector<T> res(this->scores.size());
            for (int i = 0; i < this->scores.size(); i++)
            {
                res[i] = this->scores[i].first;
            }
            return res;
        }
};

#endif