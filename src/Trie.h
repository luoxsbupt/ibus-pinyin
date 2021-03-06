#ifndef __TRIE_H_
#define __TRIE_H_

#include <iostream>
#include <cstring>
#include "String.h"
#include "StringArray.h"
#include "Trietype.h"
using namespace std;

namespace PY {

class Trie
{
public :
    Trie ();
    ~Trie ();

public :
    void createTrieTree(const gchar *filename);
    TrieNode * search (const KeyType *elem) const;
    void insert (const KeyType *elem, const RecordType *record);
    void prefixMatch (const String &str, TrieNodeArray &nodearray);
    TrieNode *getRoot (void) const        { return m_root; }
    void print (const TrieNodeArray &nodeArray) const;
    void wideTraverse (const TrieNode *node, TrieNodeArray &leafArray) const;

private :
    gint order (const gchar c) const;
    void saveNewWord (const gchar *save_ptr, gint len);
    void sortByFreq (TrieNodeArray &nodearray) const;
    void destroy ();

private :

    TrieNode *m_root;
    ofstream *m_ofs;
};

};

#endif

