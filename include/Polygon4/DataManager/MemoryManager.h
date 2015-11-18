#pragma once

#include <memory>
#include <vector>

class MemoryManager
{
    class Base
    {
    public:
        virtual ~Base(){}
    };

    template <typename T>
    class Derived : public Base
    {
    public:
        template <class ... Args>
        T *create(Args&&... args)
        {
            p = std::make_unique<T>(std::forward<Args>(args)...);
            return p.get();
        }
    private:
        std::unique_ptr<T> p;
    };

    using value_type = std::unique_ptr<Base>;

public:
    ~MemoryManager()
    {
        clear();
    }

    template <class T, class ... Args>
    T *create(Args&&... args)
    {
        auto d = std::make_unique<Derived<T>>();
        auto p = d->create(std::forward<Args>(args)...);
        m.push_back(std::move(d));
        return p;
    }

    void clear()
    {
        m.clear();
    }

private:
    std::vector<value_type> m;
};
