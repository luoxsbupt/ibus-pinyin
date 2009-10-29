#ifndef __TRIE_TYPE_H_
#define __TRIE_TYPE_H_ #include <cstring>
#include <glib.h>
#include <cstring>

namespace PY {

#define LINE_BUF_LEN 80
#define MAX_WORD_LEN 30

typedef gint RecordType;
struct KeyType
{
    gchar *str;
    gint len;
};

typedef enum
{
    LEAF = 0,
    BRANCH = 1,
}NodeType;

struct TrieNode
{
    TrieNode ()
    {
        /* initialize branch node */
        kind = BRANCH;
        for ( int i = 0; i < 27; ++i ) {
            unMem.branch.child[i] = NULL;
        }
    }

    TrieNode (const KeyType *key, const RecordType *record)
    {
        /* initialize leaf node */
        kind = LEAF;
        unMem.leaf.key.str = new gchar [key->len + 1];
        memcpy (unMem.leaf.key.str, key->str, key->len + 1);
        unMem.leaf.key.len = key->len;
        unMem.leaf.info = *record;
    }

    ~TrieNode ()
    {
        if ( kind == LEAF ) {
            delete [] unMem.leaf.key.str;
        }
    }

    NodeType kind;
    union {
        /* leaf node */
        struct {
            KeyType     key;
            RecordType  info;
        }leaf;
        /* branch node */
        struct {
            // int num;
            struct TrieNode *child[27];
        }branch;
    }unMem;
};

typedef TrieNode * TrieTree;
typedef TrieNode * TrieNodePointer;

};

#endif

