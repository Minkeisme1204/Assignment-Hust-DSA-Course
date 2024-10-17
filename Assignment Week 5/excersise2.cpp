#include <iostream>
#include <utility>
#include <cmath>
#include <vector>
#include <algorithm>
#define MAXN 1000002

int isPrime[MAXN];
int isBlum[MAXN*2]; 
  
int primeFilter(int n, int *isPrime) {// Erathonous Prime filter
    for (int i = 2; i <= n; i++) *(isPrime + i) = 1; 
    for (int i = 2; i <= sqrt(n); i++) {
        if (*(isPrime + i) == 1) {
            for (int j = i*i; j <= n; j+=i) *(isPrime + j) = 0;  
        }
    }
    std::cout << "Filter Prime done" << std::endl;
    return 1; 
}

void init() { 
    primeFilter(MAXN - 2, isPrime);
    // for (int i = 2; i <= MAXN; i++) {
    //     if (isPrime[i] == 1) prime.push_back(i);  
    //     else std::cout << i << std::endl; // store prime number
    // }
    // std::cout << prime.size() << std::endl; 
}

std::vector<int> *generateBlum(int n, int *isBlum) {// Param: int n, array to check if Prime 
    std::vector<int> *blum = new std::vector<int>; 
    for(int i = 2; i <= sqrt(n); i++) {
        for(int j = i; j <= n/i; j++) {
            if (isPrime[i] and isPrime[j] and i*j <= n) {
                blum->push_back(i*j);
                *(isBlum + i*j) = 1; 
            } 
        }
    }
    std::sort(blum->begin(), blum->end());  // sort blum numbers
    // for(int i = 0; i < blum->size(); i++) std::cout << blum->size() <<std::endl; 
    return blum;
}

void questionA(std::vector<int> *blum, int *isBlum) {// Find pair of blums that have the is a blum
    int count = 0;
    for (int i = 0; i < blum->size(); i++) {
        for (int j = i; j < blum->size(); j++) {
            if (*(isBlum + (*blum)[i] + (*blum)[j]) == 1) {
                std::cout << blum->at(i) << " " << blum->at(j) << std::endl;  // print blum pairs
                count++; 
            }
        }
    }
    
    if(count) std::cout << "There are " << count << " blum pairs" << std::endl;
    else std::cout << "There is no blum pair\n"; 
}

int main(int argc, char **argv) {
    std::vector<int> prime;
    std::vector<int> *blum;
    int n; 
    std::cin >> n; 
    init();// Filter Prime

    blum = generateBlum(n, isBlum);// Generate Blum array 
    // questionA(blum, isBlum); // Detect all pairs of Blum numbers have the Sum is a Blum number
    
    int m; 
    std::cin >> m;
    std::cout << m << (isBlum[m] ? " is":" is not") << " a blum number" << std::endl;// Check whether a m number is a Blum number
    return 0;
}