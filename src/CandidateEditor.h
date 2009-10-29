#ifndef __PY_CANDIDATE_EDITOR_H_
#define __PY_CANDIDATE_EDITOR_H_

#include "Trietype.h"
#include "String.h"
#include "StringArray.h"
#include "Trie.h"

namespace PY {

class CandidateEditor {
public:
    CandidateEditor (void);
    ~CandidateEditor (void);

    void update (const String &prefix);
    void setRootNode (Trie *root);
    bool isEmpty (void) const;

    TrieNodeArray candidates (void) const;
    const gchar * candidate (guint i) const;
    void printCandidate (guint i ) const;
    void printAllCandidates (void) const;

private:
    String  m_prefix;
    TrieNodeArray m_nodearray;

    /* root node */
    static Trie     *m_root;
};

};

#endif
