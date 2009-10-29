#include <iostream>
#include "Config.h"
#include "PrefixEditor.h"
using namespace std;

namespace PY {

#define MAX_WORD_LEN 30

PrefixEditor::PrefixEditor (void)
{
    m_cursor = 0;
}

PrefixEditor::~PrefixEditor (void)
{
}

gboolean
PrefixEditor::insert (gint ch)
{
    /* is full */
    if (G_UNLIKELY (m_text.length () >= MAX_WORD_LEN))
        return FALSE;

    m_text.insert (m_cursor++, ch);

    return TRUE;
}

const String &
PrefixEditor::prefix (void) const
{
    return m_text;
}

gint
PrefixEditor::prefixLength (void) const
{
    return m_text.length ();
}

bool
PrefixEditor::isEmpty (void) const
{
    if ( 0 == m_text.length () ) {
        return true;
    }

    return false;
}

guint
PrefixEditor::cursor (void) const
{
    return m_cursor;
}

void
PrefixEditor::reset (void)
{
    m_text.clear ();
}

#if 0
void
FullPinyinEditor::updateWordPrefix (void)
{
    if (G_UNLIKELY (m_text.isEmpty ())) {
        m_pinyin.removeAll ();
        m_pinyin_len = 0;
    }
    else {
        m_pinyin_len = m_parser.parse (m_text,              // text
                                       m_cursor,            // text length
                                       Config::option (),   // option
                                       m_pinyin,            // result
                                       MAX_PHRASE_LEN);     // max result length
        }
    }
};

gboolean
FullPinyinEditor::reset (void)
{
    gboolean retval = FALSE;
    if (m_cursor != 0) {
        m_cursor = 0;
        retval = TRUE;
    }

    if (m_text.length () != 0) {
        m_text.truncate (0);
        retval = TRUE;
    }

    if (retval)
        updatePinyin ();

    return retval;
}

gboolean
FullPinyinEditor::removeCharBefore (void)
{
    if (G_UNLIKELY (m_cursor == 0))
        return FALSE;

    m_cursor --;
    m_text.erase (m_cursor, 1);

    updatePinyin ();

    return TRUE;
}

gboolean
FullPinyinEditor::removeCharAfter (void)
{
    if (G_UNLIKELY (m_cursor == m_text.length ()))
        return FALSE;

    m_text.erase (m_cursor, 1);

    return TRUE;
}

gboolean
FullPinyinEditor::removeWordBefore (void)
{
    if (G_UNLIKELY (m_cursor == 0))
        return FALSE;

    guint cursor;

    if (G_UNLIKELY (m_cursor > m_pinyin_len)) {
        cursor = m_pinyin_len;
    }
    else {
        const Pinyin * p = m_pinyin.pop ();
        cursor = m_cursor - p->len;
        m_pinyin_len -= p->len;
    }

    m_text.erase (cursor, m_cursor - cursor);
    m_cursor = cursor;
    return TRUE;
}

gboolean
FullPinyinEditor::removeWordAfter (void)
{
    if (G_UNLIKELY (m_cursor == m_text.length ()))
        return FALSE;

    m_text.erase (m_cursor, -1);
    return TRUE;
}

gboolean
FullPinyinEditor::moveCursorLeft (void)
{
    if (G_UNLIKELY (m_cursor == 0))
        return FALSE;

    m_cursor --;
    updatePinyin ();

    return TRUE;
}

gboolean
FullPinyinEditor::moveCursorRight (void)
{
    if (G_UNLIKELY (m_cursor == m_text.length ()))
        return FALSE;

    m_cursor ++;
    updatePinyin ();

    return TRUE;
}

gboolean
FullPinyinEditor::moveCursorLeftByWord (void)
{
    if (G_UNLIKELY (m_cursor == 0))
        return FALSE;

    if (G_UNLIKELY (m_cursor > m_pinyin_len)) {
        m_cursor = m_pinyin_len;
        return TRUE;
    }

    const Pinyin * p = m_pinyin.pop ();
    m_cursor -= p->len;
    m_pinyin_len -= p->len;

    return TRUE;
}

gboolean
FullPinyinEditor::moveCursorRightByWord (void)
{
    return moveCursorToEnd ();
}

gboolean
FullPinyinEditor::moveCursorToBegin (void)
{
    if (G_UNLIKELY (m_cursor == 0))
        return FALSE;

    m_cursor = 0;
    m_pinyin.removeAll ();
    m_pinyin_len = 0;

    return TRUE;
}

gboolean
FullPinyinEditor::moveCursorToEnd (void)
{
    if (G_UNLIKELY (m_cursor == m_text.length ()))
        return FALSE;

    m_cursor = m_text.length ();
    updatePinyin  ();

    return TRUE;
}

#endif

};
