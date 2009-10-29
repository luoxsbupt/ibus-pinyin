#include <fstream>
#include <cstdlib>
#include <queue>
#include "Trie.h"
#include "QSort.h"
#include "Queue.h"
using namespace std;

namespace PY {

Trie::Trie ()
{
    m_ofs = NULL;
    m_root = NULL;
    createTrieTree ("wordlist");
}

Trie::~Trie ()
{
    if ( m_ofs != NULL ) {
        m_ofs->close ();
        delete m_ofs;
    }

    destroy ();
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
        record = 1000000 * atof (pos + 1);
        key.len = strlen(buf);

        insert (&key, &record);
    }
    ifs1.close ();

    ifstream ifs2 (".new_word_list", ios_base::in);
    if ( !ifs2.is_open () ) {
        cerr << "Trie::createTrieTree open failed! .new_word_list" << endl;
    }

    while ( ifs2.getline (buf, LINE_BUF_LEN) ) {
        gchar *pos = strchr (buf, '\t');
        *pos = '\0';

        memcpy (key.str, buf, strlen(buf) + 1);
        record = 1000000 * atof (pos + 1);
        key.len = strlen(buf);

        insert (&key, &record);
    }
    ifs2.close ();

    delete [] key.str;
    delete [] buf;
}

#if 0
TrieNode *
Trie::search (const KeyType *elem) const
{
    TrieNode *node = m_root;
    int i = 0;
    int index = 0;

    while ( i < elem->len && node != NULL && node->kind == BRANCH ) {
        index = elem->str[i] - 'a';
        node = node->branch.child[index];
        ++i;
    }

    if ( node != NULL
            && node->kind == LEAF
            && (node->leaf.key.len == elem->len)
            && memcmp (elem->str, node->leaf.key.str, elem->len) == 0 ) {
        return node;
    }

    return NULL;
}
#endif

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
        /* key has been exist, no need to insert */
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

    while ( node != NULL && node->kind == BRANCH && i < str.length () ) {
        node = node->unMem.branch.child[order(str[i])];
        ++len;
        ++i;
    }

    while ( i < str.length () ) {
        ++len;
        ++i;
    }

    gint strLen = str.length ();
    if ( node == NULL ||
        (node->kind == LEAF && str != node->unMem.leaf.key.str) ) {
        cerr << "match failed!" << endl;
        KeyType key;
        RecordType record = 1000000;
        key.str = new gchar [strLen + 1];
        memcpy (key.str, str.c_str(), strLen + 1);
        key.len = strLen;
        insert (&key, &record);
        delete [] key.str;

        /* save new word */
        saveNewWord (str.c_str (), len);
    } else {
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
Trie::saveNewWord (const char *save_ptr, int len)
{
    if ( m_ofs == NULL ) {
        m_ofs = new ofstream;
        m_ofs->open (".new_word_list", ios_base::app);
        if ( !m_ofs->is_open () ) {
            cerr << "open file failed!" << endl;
            return ;
        }
    }

    m_ofs->write (save_ptr, len);
    m_ofs->write ("\t", 1);
    m_ofs->write ("1000000", 7);
    m_ofs->write ("\n", 1);
    m_ofs->flush ();
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
