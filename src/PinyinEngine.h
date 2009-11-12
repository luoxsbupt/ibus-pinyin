/* vim:set et sts=4: */
#ifndef __PY_PIN_YIN_ENGINE_H__
#define __PY_PIN_YIN_ENGINE_H__

#include <ibus.h>
#include "Pointer.h"
#include "Database.h"
#include "FullPinyinEditor.h"
#include "PhraseEditor.h"
#include "RawEditor.h"
#include "LookupTable.h"
#include "Property.h"
#include "Config.h"

#include "Trie.h"
#include "PrefixEditor.h"
#include "CandidateEditor.h"

namespace PY {

class PinyinEngine {
public:
    PinyinEngine (IBusEngine *engine);
    ~PinyinEngine (void);

    gboolean processKeyEvent (guint keyval, guint keycode, guint modifiers);
    void focusIn (void);
    void focusOut (void) {
        reset ();
    }

    void reset (gboolean need_update = TRUE) {
        resetQuote ();

        switch ( m_input_mode ) {
            case MODE_INIT:
                m_pinyin_editor->reset ();
                m_phrase_editor.reset ();
                break;
            case MODE_RAW:
                m_input_mode = MODE_INIT;
                m_raw_editor.reset ();
                break;
            case MODE_ENGLISH:
                m_input_mode = MODE_INIT;
                m_mode_chinese = TRUE;
                m_mode_english = FALSE;
                m_prefix_editor->reset ();
                m_candidate_editor->reset ();
                updateUI (need_update);
                if ( m_prefix_editor ) {
                    delete m_prefix_editor;
                    m_prefix_editor = NULL;
                }
                if ( m_candidate_editor ) {
                    delete m_candidate_editor;
                    m_candidate_editor = NULL;
                }
                return ;
            default:
                break;
        }

        updateUI (need_update);
    }

    void resetQuote (void) {
        m_quote = TRUE;
        m_double_quote = TRUE;
    }

    void enable (void) {}
    void disable (void) {}
    void pageUp (void);
    void pageDown (void);
    void cursorUp (void);
    void cursorDown (void);

    void propertyActivate (const gchar *prop_name, guint prop_state);
    void candidateClicked (guint index, guint button, guint state);

    void updateUI (gboolean now = TRUE) {
        if ( m_input_mode == MODE_INIT ) {
            /* for chinese */
            if (G_UNLIKELY (now || m_need_update >= 4)) {
                updateLookupTable ();
                updateAuxiliaryText ();
                updatePreeditText ();
                m_need_update = 0;
            } else {
                if (m_need_update == 0) {
                    g_idle_add ((GSourceFunc) delayUpdateUIHandler, this);
                }
                m_need_update ++;
            }
        } else if ( m_input_mode == MODE_ENGLISH ) {
            /* for english input */
            updateLookupTable ();
            updateAuxiliaryText ();
            updatePreeditText ();
        }
    }

private:
    gboolean processInitMode (guint keyval, guint keycode, guint modifiers);
    gboolean processRawMode (guint keyval, guint keycode, guint modifiers);
    gboolean processEnglishMode (guint keyval, guint keycode, guint modifiers);
    gboolean processStrokeMode (guint keyval, guint keycode, guint modifiers);
    gboolean processExtensionMode (guint keyval, guint keycode, guint modifiers);
    gboolean processPinyin (guint keyval, guint keycode, guint modifiers);
    gboolean processPrefix (guint keyval, guint keycode, guint modifiers);
    gboolean processCapitalLetter (guint keyval, guint keycode, guint modifiers);
    gboolean processNumber (guint keyval, guint keycode, guint modifiers);
    gboolean processNumberInInit (guint keyval, guint keycode, guint modifiers);
    gboolean processNumberInEnglish (guint keyval, guint keycode, guint modifiers);
    gboolean processPunct (guint keyval, guint keycode, guint modifiers);
    gboolean processSpace (guint keyval, guint keycode, guint modifiers);
    gboolean processOthers (guint keyval, guint keycode, guint modifiers);

private:
    gboolean isEmpty (void) { return m_pinyin_editor->isEmpty (); }

    void commit (void);
    void commit (gchar ch);
    void commit (gunichar ch);
    void commit (const gchar *str);
    void commit (const String &str);

    void toggleModeChinese (void);
    void toggleModeEnglish (void);
    void toggleModeFull (void);
    void toggleModeFullPunct (void);
    void toggleModeSimp (void);
    void showSetupDialog (void);

    gboolean selectCandidate (guint i);
    gboolean selectCandidateInPage (guint i);
    gboolean resetCandidate (guint i);
    gboolean resetCandidateInPage (guint i);
    void updatePreeditText (void);
    void updatePreeditTextInInitMode (void);
    void updatePreeditTextInRawMode (void);
    void updatePreeditTextInENGLISHMode (void);
    void updatePreeditTextInInitEditingMode (void);
    void updatePreeditTextInInitTypingMode (void);
    void updateAuxiliaryText (void);
    void updateAuxiliaryTextInInit (void);
    void updateAuxiliaryTextInEnglish (void);
    void updateLookupTable (void);
    void updatePhraseEditor (void);
    void updateCandidateEditor (void);

    static gboolean delayUpdateUIHandler (PinyinEngine *pinyin) {
        if (pinyin->m_need_update > 0)
            pinyin->updateUI (TRUE);
        return FALSE;
    }

    void setInputMode ();
private:
    Pointer<IBusEngine>  m_engine;      // engine pointer

    PinyinEditor *m_pinyin_editor;      // pinyin editor
    PrefixEditor *m_prefix_editor;      // english editor
    PhraseEditor m_phrase_editor;       // phrase editor
    CandidateEditor *m_candidate_editor;    // word editor
    String m_buffer;                    // string buffer

    gint m_need_update;                 // need update preedit, aux, or lookup table

    LookupTable m_lookup_table;
    Property    m_prop_chinese;
    Property    m_prop_full;
    Property    m_prop_full_punct;
    Property    m_prop_simp;
    Property    m_prop_setup;
    PropList    m_props;

    gboolean m_mode_chinese;
    gboolean m_mode_english;
    gboolean m_mode_full;
    gboolean m_mode_full_punct;

    gboolean m_quote;
    gboolean m_double_quote;

    guint    m_prev_pressed_key;
    gboolean m_prev_pressed_key_result;
    gunichar m_prev_commited_char;

    enum {
        MODE_INIT = 0,          // init mode
        MODE_RAW,               // raw mode
        MODE_ENGLISH,           // press v into English input mode
        MODE_STROKE,            // press u into stroke input mode
        MODE_EXTENSION,         // press i into extension input mode
    } m_input_mode;

    RawEditor m_raw_editor;
};

};

#endif
