#pragma once

#include <vector>

class MemoryManager
{
public:
    ~MemoryManager()
    {
        clear();
    }

    template <class T, class ...Args>
    T *create(Args&&... args)
    {
        auto p = new T(std::forward<Args>(args)...);
        m.push_back(p);
        return p;
    }

    void clear()
    {
        for (auto &v : m)
            delete v;
        m.clear();
    }

private:
    std::vector<void*> m;
};
