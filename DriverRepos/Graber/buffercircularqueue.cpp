#include "BufferCircularQueue.h"

BufferCircularQueue::BufferCircularQueue(unsigned queueSize, int bufferSize) :
    m_queueSize(queueSize),
    m_bufferSize(bufferSize),
    m_head(0),
    m_rear(0)
{
    m_semaphoreFreeSpace = CreateSemaphore(
        NULL,       // default security attributes
        queueSize,  // initial count
        queueSize,  // maximum count
        NULL);      // unnamed semaphore
    m_semaphoreFilledSpace = CreateSemaphore(
        NULL,       // default security attributes
        0,          // initial count
        queueSize,  // maximum count
        NULL);      // unnamed semaphore
    m_data = new unsigned char *[m_queueSize];
    for (unsigned i = 0; i < m_queueSize; i++) {
        m_data[i] = new unsigned char[m_bufferSize];
    }
}

BufferCircularQueue::~BufferCircularQueue()
{
    for (unsigned i = 0; i < m_queueSize; i++) {
        delete[] m_data[i];
    }
    delete[] m_data;
    ::CloseHandle(m_semaphoreFilledSpace);
    ::CloseHandle(m_semaphoreFreeSpace);
}

bool BufferCircularQueue::Enqueue(const unsigned char *buffer, bool isDeepCopy, DWORD timeout)
{
    if (::WaitForSingleObject(m_semaphoreFreeSpace, timeout) != WAIT_OBJECT_0)
        return false;

    if (isDeepCopy)
        memcpy(m_data[m_rear], buffer, m_bufferSize);
    else
        m_data[m_rear] = const_cast<unsigned char *>(buffer);

    ForwardRearPointer();
    return true;
}

unsigned char *BufferCircularQueue::AcquireRearBuffer(DWORD timeout)
{
    unsigned char *rear = nullptr;
    DWORD dwWaitResult = WaitForSingleObject(
        m_semaphoreFreeSpace,   // handle to semaphore
        timeout);               // time-out interval
    if (dwWaitResult != WAIT_OBJECT_0)
        return rear;
    rear = m_data[m_rear];
    return rear;
}

void BufferCircularQueue::ForwardRearPointer()
{
    m_rear = (m_rear + 1) % m_queueSize;
    ::ReleaseSemaphore(
        m_semaphoreFilledSpace,  // handle to semaphore
        1,                       // increase count by one
        NULL);
}

bool BufferCircularQueue::Dequeue(unsigned char *buffer, bool isDeepCopy, DWORD timeout)
{
    DWORD dwWaitResult = WaitForSingleObject(
        m_semaphoreFilledSpace,   // handle to semaphore
        timeout);                 // time-out interval
    if (dwWaitResult != WAIT_OBJECT_0)
        return false;
    if (isDeepCopy)
        memcpy(buffer, m_data[m_head], m_bufferSize);
    else
        buffer = m_data[m_head];

    ForwardHeadPointer();
    return true;
}

unsigned char *BufferCircularQueue::AcquireHeadBuffer(DWORD timeout)
{
    unsigned char *head = nullptr;
    DWORD dwWaitResult = WaitForSingleObject(
        m_semaphoreFilledSpace,      // handle to semaphore
        timeout);                    // time-out interval
    if (dwWaitResult != WAIT_OBJECT_0)
        return head;
    head = m_data[m_head];
    return head;
}

void BufferCircularQueue::ForwardHeadPointer()
{
    m_head = (m_head + 1) % m_queueSize;
    ::ReleaseSemaphore(
        m_semaphoreFreeSpace,    // handle to semaphore
        1,                       // increase count by one
        NULL);
}

#if 0
bool BufferCircularQueue::IsEmpty()
{
    return m_head == m_rear;
}

bool BufferCircularQueue::IsFull()
{
    return m_head == (m_rear + 1) % m_queueSize;
}
#endif
