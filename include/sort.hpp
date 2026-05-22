#pragma once

#include <limits>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

class sort {
    private:
        template <typename T>
        void swap(std::vector<T>&vec, unsigned int i, unsigned int j){
            T swp = vec[i];
            vec[i] = vec[j];
            vec[j] = swp;
        }

    public:
        template <typename T>
        void bubble(std::vector<T>& vec){
            if (vec.empty()) return; 

            for (unsigned int i = 0; i < vec.size() - 1; i++) {
                for (int j = i+1; j < vec.size(); j++) {
                    if (vec[i] > vec [j]) {
                        swap(vec, i, j);
                    }
                }
            }
        }

        template <typename T>
        void insertion(std::vector<T>& vec) {
            if (vec.empty()) return; 

            for (unsigned int i = 1; i < vec.size(); i++) {
                T key = vec[i];
                int j = i - 1;
                while(j >= 0 && vec[j] > key) {
                    vec[j+1] = vec[j];
                    j = j - 1;
                }
                vec[j+1] = key;
            }
        }

        template <typename T>
        void selection(std::vector<T>& vec) {
            if (vec.empty()) return; 

            for (unsigned int i = 0; i < vec.size()-1; i++) {
                unsigned int min = i;
                for (int j = i+1; j < vec.size(); j++) {
                    if (vec[j] < vec[min]) {
                        min = j;
                    }
                }
                swap(vec, i, min);
            }
        }


        //
        // questa esercitazione!!!
        //


        template <typename T>
        void merge(std::vector<T>& vec, int p, int q, int r) {
            int n1 = q-p+1;
            int n2 = r-q;

            std::vector<T> vecL, vecR;
            vecL.resize(n1+1);
            vecR.resize(n2+1);

            for (int i = 0; i < n1; i++) {
                vecL[i] = vec[p+i];
            }
            for (int j = 0; j < n2; j++) {
                vecR[j] = vec[q+j+1];
            }

            if constexpr (std::is_same_v<T, std::string>) { // controllo se T è stringa
                vecL[n1] = '}'; 
                vecR[n2] = '}';
            } else {
                vecL[n1] = std::numeric_limits<T>::max(); // generalizzo la costante MAX_INT
                vecR[n2] = std::numeric_limits<T>::max();
            }
            

            int i = 0;
            int j = 0;
            for (int k = p; k <= r; k++) {
                if (vecL[i] <= vecR[j]) {
                    vec[k] = vecL[i];
                    i++;
                } else {
                    vec[k] = vecR[j];
                    j++;
                }
            }

        }

        template <typename T>
        void mergesort(std::vector<T>& vec, int p, int r) {
            if (vec.empty()) return; 

            if(p < r) {
                int q = std::floor((p+r)/2);
                mergesort(vec, p, q);
                mergesort(vec, q+1, r);
                merge(vec, p, q, r);
            }
        }



        template <typename T>
        int partition(std::vector<T>& vec, int p, int r) {
            T x = vec[r];
            int i = p - 1;
            for (int j = p; j <= r-1; j++) {
                if(vec[j] <= x) {
                    i = i+1;
                    swap(vec, i, j);
                }
            }
            swap(vec, i+1, r);
            return i+1;
        }

        template <typename T>
        void quicksort(std::vector<T>& vec, int p, int r) {
            if (vec.empty()) return; 

            if(p < r) {
                int q = partition(vec, p, r);
                quicksort(vec, p, q-1);
                quicksort(vec, q+1, r);
            }
        }

        template <typename T>
        void opt_quicksort(std::vector<T>& vec) {
            int len = vec.size();

            if (len < 150) {
                insertion(vec);
            } else {
                quicksort(vec, 0, len-1);
            }
        }

        template <typename T>
        bool is_sorted(std::vector<T>& vec) {
            if (vec.empty()) {
                return true;
            }

            for (int i = 0; i < vec.size()-1; i++) {
                if (vec[i] > vec[i+1]) {
                    return false;
                }
            }
            return true;
        }

        template <typename T>
        void print(std::vector<T>& vec) {
            
            for (int i = 0; i < vec.size(); i++) {
                std::cout << vec[i] << '\n';
            }
        }
};
