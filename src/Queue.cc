#include "Queue.h"

namespace PY {

Queue::Queue ()
{
    m_size = 0;
    m_head = NULL;
    m_rear = NULL;

    init ();
}

Queue::~Queue ()
{
    destroy ();
}

void
Queue::init (void)
{
    /* allocate head node */
    m_head = new QNode;
    if ( !m_head ) {
        return ;
    }

    m_rear = m_head;
}

void
Queue::destroy (void)
{
    QNode *p = m_head;
    QNode *q = NULL;
    while ( p != m_rear && p != NULL ) {
        q = p->next;
        delete p;
        p = q;
    }

    delete p;
}

const TrieNode *
Queue::front (void) const
{
    return m_head->next->data;
}

const TrieNode *
Queue::back (void) const
{
    return m_rear->data;
}

void
Queue::push (TrieNode *node)
{
    QNode *tmp = new QNode (node);
    m_rear->next = tmp;
    m_rear = tmp;

    ++m_size;
}

void
Queue::push (const TrieNode *node)
{
    QNode *tmp = new QNode (node);
    m_rear->next = tmp;
    m_rear = tmp;

    ++m_size;
}

void
Queue::pop (void)
{
    if ( m_head == m_rear ) {
        return ;
    }

    QNode *tmp = m_head->next;
    m_head->next = tmp->next;

    if ( tmp == m_rear ) {
        m_rear = m_head;
    }

    delete tmp;

    --m_size;
}

bool
Queue::isEmpty (void) const
{
    if ( m_head == m_rear ) {
        return true;
    }

    return false;
}

gint
Queue::size () const
{
    return m_size;
}

};
