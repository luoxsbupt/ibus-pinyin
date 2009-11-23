#include <fstream>
#include <cstdlib>
#include "Trie.h"
#include "QSort.h"
#include "Queue.h"
using namespace std;

namespace PY {

Trie::Trie ()
{
    m_root = NULL;
    createTrieTree ("wordlist");
}

Trie::~Trie ()
{
    destroy ();
    m_root = NULL;
}

void
Trie::createTrieTree(const gchar *filename)
{
    RecordType record;
    KeyType key;
    key.str = new gchar [MAX_WORD_LEN];
    memset (key.str, 0, MAX_WORD_LEN);

    ifstream ifs1 ("wordlist", ios_base::in);
    if ( !ifs1.is_open () ) {
        cerr << "Trie::createTrieTree open failed! wordlist" << endl;
        return ;
    }

    gchar *buf = new gchar [LINE_BUF_LEN];
    memset (buf, 0, LINE_BUF_LEN);
    while ( ifs1.getline (buf, LINE_BUF_LEN) ) {
        gchar *pos = strchr (buf, '\t');
        *pos = '\0';

        memcpy (key.str, buf, strlen(buf) + 1);
        record.freq = 1000000 * atof (pos + 1);
        record.isUserWord = false;
        key.len = strlen(buf);

        insert (&key, &record);
    }
    ifs1.close ();

    ifstream ifs2 (".user_word", ios_base::in);
    if ( !ifs2.is_open () ) {
        cerr << "Trie::createTrieTree open failed! .user_word" << endl;
    }

    while ( ifs2.getline (buf, LINE_BUF_LEN) ) {
        gchar *pos = strchr (buf, '\t');
        *pos = '\0';

        memcpy (key.str, buf, strlen(buf) + 1);
        record.freq = 1000000 * atof (pos + 1);
        record.isUserWord = true;
        key.len = strlen(buf);

        insert (&key, &record);
    }
    ifs2.close ();

    delete [] key.str;
    delete [] buf;
}

TrieNode *
Trie::search (const KeyType *elem) const
{
    TrieNode *node = m_root;
    int i = 0;
    int index = 0;

    while ( i < elem->len && node != NULL && node->kind == BRANCH ) {
        index = elem->str[i] - 'a';
        node = node->unMem.branch.child[index];
        ++i;
    }

    if ( node != NULL
            && node->kind == LEAF
            && (node->unMem.leaf.key.len == elem->len)
            && memcmp (elem->str, node->unMem.leaf.key.str, elem->len) == 0 ) {
        return node;
    }

    return NULL;
}

void
Trie::insert (const KeyType *key, const RecordType *record)
{
    if ( key == NULL && record == NULL ) {
        cerr << "Trie::insert empty pointer!" << endl;
        return ;
    }

    TrieNode *leaf = new TrieNode (key, record);

    if ( m_root == NULL ) {
        /* empty tree */
        m_root = new TrieNode;
        m_root->unMem.branch.child[order(key->str[0])] = leaf;

        return ;
    }

    TrieNode *node = m_root;
    TrieNode *parent = NULL;
    int i = 0;

    /* is exist? */
    while ( node != NULL && node->kind == BRANCH && i < key->len ) {
        parent = node;
        node = node->unMem.branch.child[order(key->str[i])];
        ++i;
    }

    if ( node != NULL
        && node->kind == LEAF
        && (node->unMem.leaf.key.len == key->len)
        && memcmp (node->unMem.leaf.key.str, key->str, key->len) == 0 ) {
        /* key exists */
        return ;
    }

    --i;

    /* insert new node */
    if ( node == NULL ) {
        parent->unMem.branch.child[order(key->str[i])] = leaf;
    } else if ( node->kind == LEAF ) {
        TrieNode *q = parent;
        TrieNode *tmp = NULL;

        /* divide branch node */
        do {
            tmp = new TrieNode;
            q->unMem.branch.child[order(key->str[i])] = tmp;
            q = q->unMem.branch.child[order(key->str[i])];
            ++i;
        } while ( key->str[i] == node->unMem.leaf.key.str[i] );

        if ( i >= node->unMem.leaf.key.len ) {
            q->unMem.branch.child[0] = node;
        } else {
            q->unMem.branch.child[order(node->unMem.leaf.key.str[i])] = node;
        }

        if ( i >= key->len ) {
            q->unMem.branch.child[0] = leaf;
        } else {
            q->unMem.branch.child[order(key->str[i])] = leaf;
        }

    } else if ( node->kind == BRANCH ) {
        TrieNode *q = parent;
        q->unMem.branch.child[order(key->str[i])]->unMem.branch.child[0] = leaf;
    }
}

void
Trie::prefixMatch (const String &str, TrieNodeArray &nodearray)
{
    TrieNode *node = m_root;

    guint len = 0;
    guint i = 0;

    while ( node != NULL &&
            node->kind == BRANCH &&
            i < str.length () ) {

            node = node->unMem.branch.child[order(str[i])];
            ++len;
            ++i;
    }

    while ( i < str.length () ) {
        ++len;
        ++i;
    }

    if ( node == NULL ||
        (node->kind == LEAF &&
         strncmp (str, node->unMem.leaf.key.str, str.length ())
        ) ) {
        /* match failed */
        cerr << "match failed!" << endl;
    } else {
        /* match ok */
        cerr << "match succeed!" << endl;
        wideTraverse (node, nodearray);
    }

    if ( nodearray.isEmpty () != true ) {
        sortByFreq (nodearray);
    }
}

void
Trie::wideTraverse (const TrieNode *node, TrieNodeArray &leafArray) const
{
    WordInfo word;
    Queue qe;

    if ( node != NULL ) {
        qe.push (node);
    }

    while ( !qe.isEmpty () ) {
        node = qe.front ();
        qe.pop ();

        if ( node->kind == BRANCH ) {
            for ( gint i = 0; i < 27; ++i ) {
                if ( node->unMem.branch.child[i] != NULL ) {
                    qe.push (node->unMem.branch.child[i]);
                }
            }
        } else if ( node->kind == LEAF ) {
            leafArray.push (const_cast<TrieNode *>(node));
        }
    }
}

void
Trie::destroy ()
{
    Queue qe;
    const TrieNode *node = NULL;

    if ( m_root != NULL ) {
        qe.push (m_root);
    }

    while ( !qe.isEmpty () ) {
        node = qe.front ();
        qe.pop ();

        if ( node->kind == BRANCH ) {
            for ( int i = 0; i < 27; ++i ) {
                if ( node->unMem.branch.child[i] != NULL ) {
                    qe.push (node->unMem.branch.child[i]);
                }
            }
        }

        delete node;
    }
}

void
Trie::print (const TrieNodeArray &nodearray) const
{
    int len = nodearray.length ();
    for ( int i = 0; i < len; ++i ) {
        cout << nodearray[i]->unMem.leaf.key.str << endl;
    }
}

void
Trie::sortByFreq (TrieNodeArray &nodearray) const
{
    QSort sorter;
    sorter.quicksort (nodearray, 0, nodearray.length () - 1);
}

int
Trie::order (const char c) const
{
    int idx = 0;
    if ( c <= 'z' && c >= 'a' ) {
        idx = c - 'a' + 1;
    } else if ( c <= 'Z' && c>= 'A' ) {
        idx = c - 'A' + 1;
    } else {
        cerr << "invalid character!" << endl;
        idx = -1;
    }

    return idx;
}

};

