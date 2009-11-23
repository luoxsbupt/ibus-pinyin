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
    gboolean isEmpty (void) const;
    gboolean candidateIsUserWord (guint i) const;
    void insertNewNode (const KeyType *key, const RecordType *record);
    void processUserWord (const String &word);
    void reset (void);

    TrieNodeArray candidates (void) const;
    const gchar * candidate (guint i) const;
    void printCandidate (guint i ) const;
    void printAllCandidates (void) const;

private:
    TrieNodeArray m_nodearray;

    /* root node */
    // static Trie     *m_root;
    Trie     *m_root;
};

};

#endif
