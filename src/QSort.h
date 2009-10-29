#ifndef __Q_SORT_H_
#define __Q_SORT_H_

#include "StringArray.h"

namespace PY {

class QSort
{
public :
    QSort () {}
    virtual ~QSort () {}

public :
    void quicksort (TrieNodeArray &nodearray, gint low, gint high) const;

private :
    friend gint compareFreq (TrieNode *node1, TrieNode *node2);
    gint partition (TrieNodeArray &nodearray, gint low, gint high) const;
};

gint
compareFreq (TrieNode *node1, TrieNode *node2)
{
    if ( node1->unMem.leaf.info < node2->unMem.leaf.info ) {
        return -1;
    } else if ( node1->unMem.leaf.info > node2->unMem.leaf.info ) {
        return 1;
    } else {
        return 0;
    }
}

void
QSort::quicksort (TrieNodeArray &nodearray, gint low, gint high) const
{
    gint position = 0;

    while ( low < high ) {
        position = partition (nodearray, low, high);
        quicksort (nodearray, low, position - 1);
        low = position + 1;
    }
}

gint
QSort::partition (TrieNodeArray &nodearray, gint low, gint high) const
{
    /* save nodearray[low] */
    TrieNode *base = nodearray[low];

    while ( low < high ) {
        while ( low < high && compareFreq (nodearray[high], base) <= 0 ) {
            --high;
        }
        nodearray[low] = nodearray[high];

        while ( low < high && compareFreq (base, nodearray[low]) <= 0 ) {
            ++low;
        }
        nodearray[high] = nodearray[low];
    }

    nodearray[high] = base;
    return low;
}

};

#endif
