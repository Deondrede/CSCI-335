#include "Field.h"
#include <stdexcept>
#include <iostream>

Field::Field(const std::vector<std::vector<int>>& arr){//arr[M][N]
    std::vector<std::vector<int>> utilVector(arr.size(), std::vector<int>(arr.at(0).size(), 0));
    utilVector[0] = arr[0];

    for (int i{1}; i < arr.size(); i++){
        for (int j{0}; j < arr.at(0).size(); j++){
            utilVector[i][j] = arr[i][j] + utilVector[i-1][j];
        }
    }

    for (int i{0}; i < arr.size(); i++){
        for (int j{1}; j < arr.at(0).size(); j++){
            utilVector[i][j] += utilVector[i][j-1];
        }
    }
    precomputeVector = utilVector;
    original = arr;
}

Field::Field(std::vector<std::vector<int>>&& arr){//arr[M][N]
    std::vector<std::vector<int>> utilVector(arr.size(), std::vector<int>(arr.at(0).size(), 0));

    for (int i{0}; i < arr.at(0).size(); i++) {utilVector[0][i] = arr[0][i];}

    for (int i{1}; i < arr.size(); i++){
        for (int j{0}; j < arr.at(0).size(); j++){
            utilVector[i][j] = arr[i][j] + utilVector[i-1][j];
        }
    }

    for (int i{0}; i < arr.size(); i++){
        for (int j{1}; j < arr.at(0).size(); j++){
            utilVector[i][j] += utilVector[i][j-1];
        }
    }
    precomputeVector = utilVector;
    original = std::move(arr);
    arr.clear();
}

int Field::Weight(int x1, int y1, int x2, int y2){ //(5,4) (0,0)
    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
        throw std::out_of_range("Given field position is out of range. Position must be of type unsigned integer.");
    }

    if (y1 > precomputeVector.size() || x1 > precomputeVector.at(0).size() || y2 > precomputeVector.size() || x2 > precomputeVector.at(0).size()){
        throw std::out_of_range("Given field position is out of range.");
    }

    if (x2 < x1 && y2 < y1){
        std::swap(x1,x2);
        std::swap(y1,y2);
    }

    int sum = 0;
    sum = precomputeVector[y2][x2];
    if (x1 > 0) {sum -= precomputeVector[y1-1][x2];}
    if (y1 > 0) {sum -= precomputeVector[y2][x1-1];}
    if (x1 > 0 && y1 > 0) {sum += precomputeVector[y1-1][x1-1];}
    return sum;
    
    
}

int minNum(const std::vector<std::vector<int>>& arr, int& i, int& j, int counter){//Recursive function for checking duplicates in the 2d array
    int decrement = counter;
    int return_val = 0;
    if (arr[i+counter][j] != arr[i][j+counter]){
        while(decrement >= 1){
            return_val += std::min(arr[i+decrement][j],arr[i][j+decrement]);
            decrement--;
        }
        if (arr[i+counter][j] < arr[i][j+counter]){
            i += counter;
        }
        else {
            j += counter;
        }
        return return_val;
    }
    counter++;
    return minNum(arr, i, j, counter);
}

int Field::PathCost(){
    if (original.empty() == true) {return 0;}
    
    int i = 0;
    int j = 0;
    int sum = original[0][0];
    while (i < original.size()){
        if (j >= original.at(0).size()){    //Should be at the end at this point
            return sum;
        }
        while (j < original.at(0).size()){
            if (i+1 < original.size() && j+1 < original.at(0).size()){     //As long as we're not at the final row and not at the final column then do the following operations
                if (original[i+1][j] == original[i][j+1]){
                    sum += minNum(original, i, j, 1);
                    break;
                }
                else{
                    sum += std::min(original[i][j+1], original[i+1][j]);    //Take the min of the two directions, right and down
                }
                
                if (original[i+1][j] < original[i][j+1]){ //If row < column then we check the next row
                    i++;
                }
                else{
                    j++;
                }
            }
            else if (i+1 < original.size()){
                sum += original[i+1][j];
                i++;
            }
            else if (j+1 < original.at(0).size()){  //If we're at the final row then we take the sum of the rest of the columns in that row
                sum += original[i][j+1];
                j++;
            }
            else{
                j++;
            }
        }
    }
    return sum;
}
