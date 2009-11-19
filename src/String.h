#ifndef __PY_STRING_H_
#define __PY_STRING_H_
#include <iostream>
#include <glib.h>
#include <stdarg.h>
#include <cstring>
#include "WideString.h"
using namespace std;

namespace PY {

class String {
public:
    String (const gchar *init) {
        m_string = g_string_new (init);
    }

    String (const gchar *init, gssize len) {
        m_string = g_string_new_len (init, len);
    }

    String (gsize init_size = 0) {
        m_string = g_string_sized_new (init_size);
    }

    String (const WideString &wstr) {
        m_string = g_string_sized_new (wstr.length() * 6);
        assign (wstr);
    }

    String (const String &str) {
        m_string = g_string_sized_new (str.length());
        assign (str);
    }

    ~String (void) {
        g_string_free (m_string, TRUE);
    }

    gsize length (void) const {
        return m_string->len;
    }

    gsize utf8Length (void) const {
        return g_utf8_strlen (m_string->str, m_string->len);
    }

    gboolean isEmpty (void) const {
        return m_string->len == 0;
    }

    String & assign (const gchar *str) {
        g_string_assign (m_string, str);
        return *this;
    }

    String & assign (const String &str) {
        return assign ((const gchar *) str);
    }

    String & assign (const WideString &wstr) {
        truncate (0);
        return append (wstr);
    }

    String & insert (gint pos, gchar ch) {
        g_string_insert_c (m_string, pos, ch);
        return *this;
    }

    String & append (const gchar *str) {
        g_string_append (m_string, str);
        return *this;
    }
    
    String & append (const gunichar *wstr) {
        for (const gunichar *p = wstr; *p != 0; p++)
            appendUnichar (*p);
        return *this;
    }

    String & append (const WideString &wstr) {
        for (guint i = 0; i < wstr.length (); i++) {
            appendUnichar (wstr[i]);
        }
        return *this;
    }

    String & appendUnichar (gunichar ch) {
        g_string_append_unichar (m_string, ch);
        return *this;
    }

    String & append (const gchar *str, gint len) {
        g_string_append_len (m_string, str, len);
        return *this;
    }

    String & printf (const gchar *fmt, ...) {
        va_list args;

        va_start (args, fmt);
        g_string_vprintf (m_string, fmt, args);
        va_end (args);

        return *this;
    }

    String & appendPrintf (const gchar *fmt, ...) {
        va_list args;

        va_start (args, fmt);
        g_string_append_vprintf (m_string, fmt, args);
        va_end (args);

        return *this;
    }

    String & truncate (gint len) {
        g_string_truncate (m_string, len);
        return *this;
    }

    String & erase (gint pos, gint len) {
        g_string_erase (m_string, pos, len);
        return *this;
    }

    String & operator = (const gchar *str) {
        return assign (str);
    }

    String & operator = (const String &str) {
        return assign (str);
    }

    String & operator = (const WideString &wstr) {
        return assign (wstr);
    }

    String & operator += (const gchar *str) {
        return append (str);
    }

    String & operator << (const gchar *str) {
        return append (str);
    }
    
    String & operator << (const gunichar *wstr) {
        return append (wstr);
    }

    String & operator << (const String &str) {
        return append ((const gchar *) str);
    }

    String & operator << (const WideString &wstr) {
        return append (wstr);
    }

    String & operator << (gint d) {
        g_string_append_printf (m_string, "%d", d);
        return *this;
    }

    String & operator << (guint d) {
        g_string_append_printf (m_string, "%u", d);
        return *this;
    }

    String & operator << (gchar ch) {
        g_string_append_c (m_string, ch);
        return *this;
    }

    gchar operator[] (guint i) {
        if (i >= length ())
            return 0;
        return m_string->str[i];
    }

    operator const gchar *() const {
        return m_string->str;
    }

    operator gboolean () const {
        return m_string->len != 0;
    }

    /*
     * following function added by luoxs
     * author : luoxs
     * date : 2009-10-22
     */
    friend ostream & operator << (ostream &os, const String &s)
    {
        os << s.m_string;
        return os;
    }

    friend ostream & operator << (ostream &os, const gchar *str)
    {
        os << str;
        return os;
    }

    gboolean operator != (const String &s) const
    {
        return (memcmp (s.m_string->str, m_string->str, this->length () + 1) ? TRUE : FALSE);
    }

    gboolean operator != (const gchar *s) const
    {
        return (memcmp (s, m_string->str, this->length () + 1) ? TRUE : FALSE);
    }

    gchar *c_str (void) const
    {
        return m_string->str;
    }

    gboolean clear (void)
    {
        g_string_erase (m_string, 0, this->length ());
    }

private:

    GString *m_string;
};

};
#endif

