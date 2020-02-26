/**
 *  Scanner for the a(c|ab)* regex
 * 
 *  @param  char *      the string to parse
 *  @param  int         the length of the string
 *  @return char *|NULL
 */
char *scanner(char *stream, int pos = 0)
{
    char c;

// label to scan for a character
state_init:
    c = stream[pos++];
    if (c == 'a') goto state_ca; 
    if (pos == 1) goto state_err;
    else goto state_done;

// label to scan for a|ab characters recursively
state_ca:
    c = stream[pos++];
    if (c == 'c') goto state_ca; 
    else if (c == 'a') goto state_ab; 
    else goto state_done;

// label to scan for b character
state_ab:
    c = stream[pos++];
    if (c == 'b') goto state_ca; 
    else goto state_err;

// label to check if the end of the regex was reached
state_done:
    c = stream[pos-1];
    if (c == 32) goto state_succ; 
    else goto state_err;

// label for success
state_succ:
    return stream;

// label for errors
state_err:
    return NULL;
}