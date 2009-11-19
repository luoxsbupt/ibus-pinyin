#ifndef __PY_STRING_ARRAY_H_
#define __PY_STRING_ARRAY_H_

#include <cstring>
#include <glib-object.h>
#include "Trietype.h"
#include "Array.h"

namespace PY {

struct WordInfo;
typedef Array<gchararray>   StringArray;
typedef Array<WordInfo>     WordInfoArray;
typedef Array<TrieNode *>   TrieNodeArray;

struct WordInfo
{
public:
    WordInfo ()
    {
        freq = 0;
    }

    WordInfo (const gchar *str, gdouble frequency)
    {
        memcpy (&word, str, strlen (str) + 1);
        freq = frequency;
    }

    gchar word[30];
    gint freq;
};

};

#endif

