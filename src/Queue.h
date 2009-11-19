#ifndef __QUEUE_H_
#define __QUEUE_H_

#include "Trietype.h"

namespace PY {

class QNode;
class QNode
{
public:
    QNode ()
    {
        data = NULL;
        next = NULL;
    }

    QNode (const TrieNode *node)
    {
        data = node;
        next = NULL;
    }

    ~QNode ()
    {}

public:
    const TrieNode    *data;
    QNode       *next;

};

/* implemented via linklist */
class Queue
{
public:
    Queue ();
    ~Queue ();

public:
    /* public operation */
    void push (TrieNode *node);
    void push (const TrieNode *node);
    void pop (void);
    bool isEmpty (void) const;
    gint size (void) const;
    const TrieNode * front (void) const;
    const TrieNode * back () const;

private:
    /* called by constructor and deconstructor */
    void init (void);
    void destroy (void);

private:
    QNode *m_head;
    QNode *m_rear;
    gint m_size;
};

#endif

};

