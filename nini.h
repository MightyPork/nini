///
/// nini - Nano INI parser
/// 
/// Written by MightyPork, 2018
/// MIT license
/// 

#ifndef INIPARSE_H
#define INIPARSE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// buffer sizes
#define INI_KEY_MAX 20
#define INI_VALUE_MAX 30

/**
 * INI parser callback, called for each found key-value pair.
 *
 * @param section - current section, empty string for global keys
 * @param key - found key (trimmed of whitespace)
 * @param value - value, trimmed of quotes or whitespace
 * @param userData - opaque user data pointer, general purpose
 */
typedef void (*IniParserCallback)(const char *section, const char *key, const char *value, void *userData);

/**
 * Begin parsing a stream
 *
 * @param callback - key callback to assign
 * @param userData - optional user data that will be passed to the callback
 */
void ini_parse_begin(IniParserCallback callback, void *userData);

/**
 * End parse stream.
 * Flushes what remains in the buffer and removes callback.
 *
 * @returns userData or NULL if none
 */
void* ini_parse_end(void);

/**
 * Parse a string (needn't be complete line or file)
 *
 * @param data - string to parse
 * @param len - string length (0 = use strlen)
 */
void ini_parse(const char *data, size_t len);

/**
 * Parse a complete file loaded to string
 *
 * @param text - entire file as string
 * @param len - file length (0 = use strlen)
 * @param callback - key callback
 * @param userData - optional user data for key callback
 */
void ini_parse_file(const char *text, size_t len, IniParserCallback callback, void *userData);

/**
 * Explicitly reset the parser
 */
void ini_parse_reset(void);

#endif // INIPARSE_H
