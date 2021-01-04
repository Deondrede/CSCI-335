#pragma once
#include <vector>

class Field{
    private:
        std::vector<std::vector<int>> precomputeVector;
        std::vector<std::vector<int>> original;
    public:
        Field(const std::vector<std::vector<int>>& arr);
        Field(std::vector<std::vector<int>>&& arr);
        int Weight(int x1, int y1, int x2, int y2);
        int PathCost();
};
