///
/// nini - Nano INI parser
/// 
/// Written by MightyPork, 2018
/// MIT license
/// 


#include "nini.h"

enum nini_state {
    NINI_IDLE,
    NINI_SECTION,
    NINI_KEY,
    NINI_VALUE,
    NINI_COMMENT,
};

static struct {
    uint8_t section_i;
    char section[INI_KEY_MAX];

    uint8_t key_i;
    char key[INI_KEY_MAX];

    uint8_t value_i;
    char value[INI_VALUE_MAX];
    bool val_last_space;

    IniParserCallback cb;
    void *userdata;
    enum nini_state state;
} nini;

void ini_parse_begin(IniParserCallback callback, void *userData)
{
    ini_parse_reset();
    nini.cb = callback;
    nini.userdata = userData;
}

void ini_parse(const char *data, size_t len)
{
    for (; len > 0; len--) {
        char c = *data++;
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            if (nini.state != NINI_VALUE && nini.state != NINI_COMMENT)
                continue;
        }

        switch (nini.state) {
            case NINI_IDLE:
                if (c == '[') {
                    nini.state = NINI_SECTION;
                    nini.section_i = 0;
                }
                else if (c == '#') {
                    nini.state = NINI_COMMENT;
                }
                else {
                    nini.state = NINI_KEY;
                    nini.key_i = 0;
                    nini.value_i = 0;
                    nini.val_last_space = false;
                    nini.key[nini.key_i++] = c;
                }
                break;

            case NINI_COMMENT:
                if (c == '\n' || c == '\r') {
                    nini.state = NINI_IDLE;
                }
                break;

            case NINI_SECTION:
                if (c == ']') {
                    nini.section[nini.section_i] = 0;
                    nini.state = NINI_COMMENT; // discard to EOL
                    break;
                }
                else if (nini.section_i < INI_KEY_MAX - 1) {
                    nini.section[nini.section_i++] = c;
                }
                break;

            case NINI_KEY:
                if (c == '=') {
                    nini.key[nini.key_i] = 0;
                    nini.state = NINI_VALUE;
                }
                else if (nini.key_i < INI_KEY_MAX - 1) {
                    nini.key[nini.key_i++] = c;
                }
                break;

            case NINI_VALUE:
                switch (c) {
                    case ' ':
                    case '\t':
                        if (nini.value_i) nini.val_last_space = true;
                        break;
                    case '\r':
                    case '\n':
                        nini.value[nini.value_i] = 0;
                        nini.state = NINI_IDLE;
                        nini.cb(nini.section, nini.key, nini.value, nini.userdata);
                        break;
                    default:
                        if (nini.val_last_space && nini.value_i < INI_VALUE_MAX - 1) {
                            nini.value[nini.value_i++] = ' ';
                        }

                        if (nini.value_i < INI_VALUE_MAX - 1) {
                            nini.value[nini.value_i++] = c;
                        }
                        nini.val_last_space = false;
                }
        }
    }
}

void *ini_parse_end(void)
{
    if (nini.state == NINI_VALUE) {
        nini.value[nini.value_i] = 0;
        nini.state = NINI_IDLE;
        nini.cb(nini.section, nini.key, nini.value, nini.userdata);
    }

    return nini.userdata;
}

void ini_parse_file(const char *text, size_t len, IniParserCallback callback, void *userData)
{
    ini_parse_begin(callback, userData);
    ini_parse(text, len);
    ini_parse_end();
}

void ini_parse_reset(void)
{
    nini.state = NINI_IDLE;
}
