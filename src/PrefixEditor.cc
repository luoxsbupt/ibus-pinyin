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
PrefixEditor::prefix (void)
{
    guint i = 0;
    m_prefix.clear ();
    for ( ; i < m_cursor; ++i) {
        m_prefix.insert (i, m_text[i]);
    }

    return m_prefix;
}

const String &
PrefixEditor::text (void) const
{
    return m_text;
}

guint
PrefixEditor::prefixLength (void)
{
    return (this->prefix ()).length ();
}

guint
PrefixEditor::textLength (void) const
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

gboolean
PrefixEditor::removeCharBefore (void)
{
    if (G_UNLIKELY (m_cursor == 0)) {
        return FALSE;
    }
    
    m_text.erase (--m_cursor, 1);
    return TRUE;
}

gboolean
PrefixEditor::removeCharAfter (void)
{
    if (G_UNLIKELY (m_cursor >= m_text.length ())) {
        return FALSE;
    }

    m_text.erase (m_cursor, 1);

    return TRUE;
}


gboolean
PrefixEditor::moveCursorLeft (void)
{
    if (G_UNLIKELY (m_cursor == 0)) {
        return TRUE;
    }

    --m_cursor;
    return TRUE;
}

gboolean
PrefixEditor::moveCursorRight (void)
{
    if (G_UNLIKELY (m_cursor == m_text.length ())) {
        return TRUE;
    }

    ++m_cursor;
    return TRUE;
}

gboolean
PrefixEditor::moveCursorToBegin (void)
{
    if (G_UNLIKELY (m_cursor == 0))
        return FALSE;

    m_cursor = 0;

    return TRUE;
}

gboolean
PrefixEditor::moveCursorToEnd (void)
{
    if (G_UNLIKELY (m_cursor == m_text.length ()))
        return FALSE;

    m_cursor = m_text.length ();

    return TRUE;
}

};

