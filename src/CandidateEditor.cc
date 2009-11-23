#include <fstream>
#include "CandidateEditor.h"

namespace PY {

CandidateEditor::CandidateEditor (void)
{
    m_root = new Trie;
}

CandidateEditor::~CandidateEditor (void)
{
    if ( m_root ) {
        delete m_root;
        m_root = NULL;
    }
}

void 
CandidateEditor::setRootNode (Trie *rootnode)
{
    m_root = rootnode;
}

void
CandidateEditor::update (const String &prefix)
{
    m_nodearray.removeAll ();
    if ( prefix.length () != 0 ) {
        m_root->prefixMatch (prefix, m_nodearray);
    }
}

gboolean
CandidateEditor::isEmpty (void) const
{
    return m_nodearray.isEmpty ();
}

gboolean
CandidateEditor::candidateIsUserWord (guint i) const
{
    return m_nodearray[i]->unMem.leaf.info.isUserWord;
}

void
CandidateEditor::insertNewNode (const KeyType *key, const RecordType *record)
{
    m_root->insert (key, record);
}

void
CandidateEditor::processUserWord (const String &word)
{
    /* add to trietree */
    guint strLen = word.length ();

    KeyType key;
    key.str = new gchar [strLen + 1];
    strncpy (key.str, word.c_str (), strLen + 1);
    key.len = strLen;

    RecordType record;
    record.freq = 1000000;
    record.isUserWord = true;

    m_root->insert (&key, &record);
    delete [] key.str;

    /* save to cache file */
    ofstream ofs (".user_word", ios_base::out | ios_base::app);
    if ( !ofs.is_open () ) {
        cerr << "open \".user_wrod\" failed!" << endl;
    }

    ofs.write (word, word.length ());
    ofs.write ("\t", 1);
    ofs.write ("1000000", 7);
    ofs.write ("\n", 1);
    ofs.flush ();
    ofs.close ();
}

void
CandidateEditor::reset (void)
{
    m_nodearray.removeAll ();
}

const gchar *
CandidateEditor::candidate (guint i) const
{
    return m_nodearray[i]->unMem.leaf.key.str;
}

TrieNodeArray
CandidateEditor::candidates (void) const
{
    return m_nodearray;
}

void
CandidateEditor::printCandidate (guint i) const
{
    cout << i << "\t" << m_nodearray[i]->unMem.leaf.key.str << endl;
}

void
CandidateEditor::printAllCandidates (void) const
{
    for ( guint i = 0; i < m_nodearray.length (); ++i ) {
        cout << m_nodearray[i]->unMem.leaf.key.str << endl;
    }
}

};

