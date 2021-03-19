#ifndef DELETER_H
#define DELETER_H

template <typename T>
struct AutoDeleter
{
public:
    AutoDeleter(T *instance) : instance(instance) {}
    ~AutoDeleter()
    {
        if (instance != nullptr)
        {
            delete instance;
        }
    }

private:
    T *instance;
};

#endif    // DELETER_H
