#include "CandidateEditor.h"

namespace PY {

Trie *CandidateEditor::m_root = 0;

CandidateEditor::CandidateEditor (void)
{
    if ( !m_root ) {
        m_root = new Trie;
    }
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

bool
CandidateEditor::isEmpty (void) const
{
    return m_nodearray.isEmpty ();
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

