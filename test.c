#include "nini.h"
#include <string.h>

const char *testfile = 
"# Comment  ...........\n"
"rootkey = Value\r\n"
"\r\n"
" [ Section@name!@+12346   ]\r\n"
"sec.key-a_b-1 = 444\r\n"
"sec.key2 = Test 123456 with spaces\r\n"
    "[ this is a section with spaces nthat will be removed ]\n"
"this_one_has_no_eol = 123456";


void test_cb(const char *section, const char *key, const char *value, void *userData)
{
    printf("[%s] >%s< = >%s<\r\n", section, key, value);
}

int main (void) {
    ini_parse_file(testfile, strlen(testfile), test_cb, NULL);
}
