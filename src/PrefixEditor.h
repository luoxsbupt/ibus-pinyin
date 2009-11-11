#ifndef __PY_PREFIX_EDITOR_H_
#define __PY_PREFIX_EDITOR_H_

#include "PinyinEditor.h"

namespace PY {

class PrefixEditor {

public:
    PrefixEditor (void);
    virtual ~PrefixEditor (void);

    gboolean insert (gint ch);
    const String & prefix (void);
    const String & text (void) const;
    guint prefixLength (void);
    guint textLength (void) const;
    bool isEmpty (void) const;
    guint cursor (void) const;
    void reset (void);

    gboolean removeCharAfter (void);
    gboolean removeCharBefore (void);
    gboolean moveCursorLeft (void);
    gboolean moveCursorRight (void);
    gboolean moveCursorToBegin (void);
    gboolean moveCursorToEnd (void);

private:
    gint m_cursor;
    String m_text;
    String m_prefix;
};

};

#endif
