#include <algorithm>
#include <bits/stdc++.h>

#ifndef DSU_UNDO_HPP
#define DSU_UNDO_HPP

#pragma region dsu_undo

class dsu_undo{
public:
    struct Operation{
        int parent_updated;
        int previous_maxsize;
    };
    std::vector<int> pi;
    std::vector<int> size;
    std::vector<Operation> operationStack;
    int _maxSize;
public:
    dsu_undo(int n = 1){
        if (n <= 0){
            std::cerr << "non-positive initializer for dsu\n";
            exit(1);
        }

        this->pi.assign(n, -1);
        this->size.assign(n, 1);

        this->_maxSize = 1;
    }

    int setOf(int i){
        int s = i;
        while(pi[s] != -1){
            s = pi[s];
        }
        return s;
    }

    bool merge(int i, int j){
        int s1 = this->setOf(i);
        int s2 = this->setOf(j);
        int curr_maxsize = this->maxSize();

        if (s1 == s2) return false;

        if (this->size[s1] < this->size[s2]){
            pi[s1] = s2;
            size[s2] += size[s1];

            this->operationStack.push_back({s1, curr_maxsize});

            this->_maxSize = std::max(this->_maxSize, size[s2]);
        }
        else{
            pi[s2] = s1;
            size[s1] += size[s2];

            this->operationStack.push_back({s2, curr_maxsize});

            this->_maxSize = std::max(this->_maxSize, size[s1]);
        }

        return true;
    }
    
    int sizeOf(int i) { return this->size[i]; }
    int maxSize() { return this->_maxSize; }
    
    bool undo(){
        if (this->operationStack.empty()) return false;

        auto op = *(this->operationStack.end() - 1);
        this->operationStack.pop_back();

        int old_parent = setOf(op.parent_updated);

        this->pi[op.parent_updated] = -1;
        this->size[old_parent] -= this->size[op.parent_updated];

        this->_maxSize = op.previous_maxsize;

        return true;
    }

    void new_dsu(int n){
        if (n <= 0){
            std::cerr << "non-positive initializer for dsu\n";
            exit(1);
        }

        this->pi.assign(n, -1);
        this->size.assign(n, 1);

        this->_maxSize = 1;
        this->operationStack.clear();

    }
};

#pragma endregion

#endif
