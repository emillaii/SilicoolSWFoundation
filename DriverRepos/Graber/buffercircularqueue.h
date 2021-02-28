#ifndef BUFFERCIRCULARQUEUE_H
#define BUFFERCIRCULARQUEUE_H
#include <Windows.h>

class BufferCircularQueue
{
public:
    BufferCircularQueue(unsigned queueSize, int bufferSize);
    virtual ~BufferCircularQueue();

    bool Enqueue(const unsigned char *buffer, bool isDeepCopy = true, DWORD timeout = INFINITE);
    unsigned char *AcquireRearBuffer(DWORD timeout = INFINITE);
    void ForwardRearPointer();

    bool Dequeue(unsigned char *buffer, bool isDeepCopy = true, DWORD timeout = INFINITE);
    unsigned char *AcquireHeadBuffer(DWORD timeout = INFINITE);
    void ForwardHeadPointer();
#if 0
    bool IsEmpty();
    bool IsFull();
#endif
    inline int GetBufferSize() const {
        return m_bufferSize;
    }

private:
    unsigned m_queueSize;
    int m_bufferSize;
    unsigned m_head;
    unsigned m_rear;
    HANDLE m_semaphoreFreeSpace;
    HANDLE m_semaphoreFilledSpace;
    unsigned char** m_data;
};

#endif // BUFFERCIRCULARQUEUE_H
