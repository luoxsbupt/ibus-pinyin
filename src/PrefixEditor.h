#ifndef __PY_PREFIX_EDITOR_H_
#define __PY_PREFIX_EDITOR_H_

#include "PinyinEditor.h"

namespace PY {

class PrefixEditor {

public:
    PrefixEditor (void);
    virtual ~PrefixEditor (void);

    gboolean insert (gint ch);
    const String & prefix (void) const;
    gint prefixLength (void) const;
    bool isEmpty (void) const;

    /*
    gboolean removeCharBefore (void);
    gboolean removeCharAfter (void);
    gboolean removeWordBefore (void);
    gboolean removeWordAfter (void);

    gboolean moveCursorLeft (void);
    gboolean moveCursorRight (void);
    gboolean moveCursorLeftByWord (void);
    gboolean moveCursorRightByWord (void);
    gboolean moveCursorToBegin (void);
    gboolean moveCursorToEnd (void);

    gboolean reset (void);

private:
    void updatePinyin (void);
    */

private:
    gint m_cursor;
    String m_text;
};

};

#endif
