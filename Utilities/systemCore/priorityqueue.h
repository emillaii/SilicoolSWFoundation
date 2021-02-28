#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QList>
#include <QMutex>
#include <QMutexLocker>

template <typename T>
class PriorityQueue
{
public:
    void enqueue(const T &t)
    {
        QMutexLocker tmpLocker(&locker);
        int index = 0;
        while (index < data.count())
        {
            if (t <= data[index])
            {
                index++;
            }
            else
            {
                break;
            }
        }
        data.insert(index, t);
    }

    T dequeue()
    {
        QMutexLocker tmpLocker(&locker);
        return data.takeFirst();
    }

    bool tryDequeue(T &t)
    {
        QMutexLocker tmpLocker(&locker);
        if (data.isEmpty())
        {
            return false;
        }
        else
        {
            t = data.takeFirst();
            return true;
        }
    }

    const T &head()
    {
        QMutexLocker tmpLocker(&locker);
        return data[0];
    }

    bool tryGetHead(T &t)
    {
        QMutexLocker tmpLocker(&locker);
        if (data.isEmpty())
        {
            return false;
        }
        else
        {
            t = data[0];
            return true;
        }
    }

    int count()
    {
        QMutexLocker tmpLocker(&locker);
        return data.count();
    }

    bool isEmpty()
    {
        QMutexLocker tmpLocker(&locker);
        return data.isEmpty();
    }

    void clear()
    {
        QMutexLocker tmpLocker(&locker);
        data.clear();
    }

private:
    QList<T> data;
    QMutex locker;
};

#endif    // PRIORITYQUEUE_H
