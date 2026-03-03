/*_
 * Copyright (c) 2026 finwo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to use, copy,
 * modify, and distribute the Software, subject to the following conditions:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions, and the following disclaimer.
 *
 *  2. Redistributions in binary form, or any public offering of the Software
 *     (including hosted or managed services), must reproduce the above copyright
 *     notice, this list of conditions, and the following disclaimer in the
 *     documentation and/or other materials provided.
 *
 *  3. Any redistribution or public offering of the Software must clearly attribute
 *     the Software to the original copyright holder, reference this License, and
 *     include a link to the official project repository or website.
 *
 *  4. The Software may not be renamed, rebranded, or marketed in a manner that
 *     implies it is an independent or proprietary product. Derivative works must
 *     clearly state that they are based on the Software.
 *
 *  5. Modifications to copies of the Software must carry prominent notices stating
 *     that changes were made, the nature of the modifications, and the date of the
 *     modifications.
 *
 * Any violation of these conditions terminates the permissions granted herein.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "url-parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Prototype declarations
 */
static __inline__ int _is_scheme_char(int);

/*
 * Check if scheme is a path-based scheme (unix socket, file path, etc.)
 */
static __inline__ int
_is_path_scheme(const char *scheme)
{
    if ( NULL == scheme ) {
        return 0;
    }
    if ( 0 == strncmp(scheme, "unix", 4) && scheme[4] == '\0' ) {
        return 1;
    }
    if ( 0 == strncmp(scheme, "file", 4) && scheme[4] == '\0' ) {
        return 1;
    }
    if ( 0 == strncmp(scheme, "cunix", 5) && scheme[5] == '\0' ) {
        return 1;
    }
    return 0;
}

/*
 * Check whether the character is permitted in scheme string
 */
static __inline__ int
_is_scheme_char(int c)
{
    return (!isalpha(c) && '+' != c && '-' != c && '.' != c) ? 0 : 1;
}

/*
 * See RFC 1738, 3986
 */
struct parsed_url *
parse_url(const char *url)
{
    struct parsed_url *purl;
    const char *tmpstr;
    const char *curstr;
    int len;
    int i;
    int userpass_flag;
    int bracket_flag;
    int is_path;

    /* Allocate the parsed url storage */
    purl = malloc(sizeof(struct parsed_url));
    if ( NULL == purl ) {
        return NULL;
    }
    purl->scheme = NULL;
    purl->host = NULL;
    purl->port = NULL;
    purl->path = NULL;
    purl->query = NULL;
    purl->fragment = NULL;
    purl->username = NULL;
    purl->password = NULL;

    curstr = url;

    /*
     * <scheme>:<scheme-specific-part>
     * <scheme> := [a-z\+\-\.]+
     *             upper case = lower case for resiliency
     */
    /* Read scheme */
    tmpstr = strchr(curstr, ':');
    if ( NULL == tmpstr ) {
        parsed_url_free(purl);
        return NULL;
    }
    /* Get the scheme length */
    len = tmpstr - curstr;
    /* Check restrictions */
    for ( i = 0; i < len; i++ ) {
        if ( !_is_scheme_char(curstr[i]) ) {
            parsed_url_free(purl);
            return NULL;
        }
    }
    /* Copy the scheme to the storage */
    purl->scheme = malloc(sizeof(char) * (len + 1));
    if ( NULL == purl->scheme ) {
        parsed_url_free(purl);
        return NULL;
    }
    (void)strncpy(purl->scheme, curstr, len);
    purl->scheme[len] = '\0';
    /* Make the character to lower if it is upper case. */
    for ( i = 0; i < len; i++ ) {
        purl->scheme[i] = tolower(purl->scheme[i]);
    }

    /* Check if this is a path-based scheme */
    is_path = _is_path_scheme(purl->scheme);

    /* Skip ':' */
    tmpstr++;
    curstr = tmpstr;

    /*
     * Normalize: ensure we have // after scheme
     * If missing, treat everything as path
     */
    if ( '/' != curstr[0] || '/' != curstr[1] ) {
        /* No // - entire rest is path */
        tmpstr = curstr;
        while ( '\0' != *tmpstr ) {
            tmpstr++;
        }
        len = tmpstr - curstr;
        if ( len > 0 ) {
            purl->path = malloc(sizeof(char) * (len + 1));
            if ( NULL == purl->path ) {
                parsed_url_free(purl);
                return NULL;
            }
            (void)strncpy(purl->path, curstr, len);
            purl->path[len] = '\0';
        }
        return purl;
    }

    /* Skip the "//" */
    curstr += 2;

    /*
     * Detect and consume username:password, consume @
     */
    tmpstr = curstr;
    while ( '\0' != *tmpstr && '@' != *tmpstr ) {
        tmpstr++;
    }

    if ( '@' == *tmpstr ) {
        /* Has userinfo */
        /* First check if there's a password (look for : before @) */
        const char *colon = curstr;
        int has_password = 0;
        while (colon < tmpstr) {
            if (':' == *colon) {
                has_password = 1;
                break;
            }
            colon++;
        }

        /* Read username */
        const char *username_start = curstr;
        if (has_password) {
            len = colon - curstr;
        } else {
            len = tmpstr - curstr;
        }
        if ( len > 0 ) {
            purl->username = malloc(sizeof(char) * (len + 1));
            if ( NULL == purl->username ) {
                parsed_url_free(purl);
                return NULL;
            }
            (void)strncpy(purl->username, username_start, len);
            purl->username[len] = '\0';
        }

        /* Skip to password or @ */
        if (has_password) {
            curstr = colon + 1;
            /* Read password */
            tmpstr = curstr;
            while ( '\0' != *tmpstr && '@' != *tmpstr ) {
                tmpstr++;
            }
            len = tmpstr - curstr;
            if ( len > 0 ) {
                purl->password = malloc(sizeof(char) * (len + 1));
                if ( NULL == purl->password ) {
                    parsed_url_free(purl);
                    return NULL;
                }
                (void)strncpy(purl->password, curstr, len);
                purl->password[len] = '\0';
            }
            curstr = tmpstr;
        } else {
            /* No password - advance past username to @ */
            curstr = username_start + len;
        }
        /* Skip @ */
        while ( '@' == *curstr ) {
            curstr++;
        }
    }

    /*
     * If NOT a path scheme, detect and consume host:port
     */
    if ( !is_path ) {
        if ( '[' == *curstr ) {
            bracket_flag = 1;
            curstr++;
        } else {
            bracket_flag = 0;
        }

        /* Read host */
        tmpstr = curstr;
        while ( '\0' != *tmpstr ) {
            if ( bracket_flag && ']' == *tmpstr ) {
                break;
            } else if ( !bracket_flag && (':' == *tmpstr || '/' == *tmpstr || '?' == *tmpstr || '#' == *tmpstr) ) {
                break;
            }
            tmpstr++;
        }
        len = tmpstr - curstr;

        if ( len > 0 ) {
            purl->host = malloc(sizeof(char) * (len + 1));
            if ( NULL == purl->host ) {
                parsed_url_free(purl);
                return NULL;
            }
            (void)strncpy(purl->host, curstr, len);
            purl->host[len] = '\0';
        }
        curstr = tmpstr;

        /* Skip ']' if IPv6 */
        if ( ']' == *curstr ) {
            curstr++;
        }

        /* Read port */
        if ( ':' == *curstr ) {
            curstr++;
            tmpstr = curstr;
            while ( '\0' != *tmpstr && '/' != *tmpstr && '?' != *tmpstr && '#' != *tmpstr ) {
                tmpstr++;
            }
            len = tmpstr - curstr;
            if ( len > 0 ) {
                purl->port = malloc(sizeof(char) * (len + 1));
                if ( NULL == purl->port ) {
                    parsed_url_free(purl);
                    return NULL;
                }
                (void)strncpy(purl->port, curstr, len);
                purl->port[len] = '\0';
            }
            curstr = tmpstr;
        }
    }

    /* End of string? */
    if ( '\0' == *curstr ) {
        return purl;
    }

    /* Parse path */
    tmpstr = curstr;
    while ( '\0' != *tmpstr && '?' != *tmpstr && '#' != *tmpstr ) {
        tmpstr++;
    }
    len = tmpstr - curstr;
    if ( len > 0 ) {
        purl->path = malloc(sizeof(char) * (len + 1));
        if ( NULL == purl->path ) {
            parsed_url_free(purl);
            return NULL;
        }
        (void)strncpy(purl->path, curstr, len);
        purl->path[len] = '\0';
    }
    curstr = tmpstr;

    /* Parse query */
    if ( '?' == *curstr ) {
        curstr++;
        tmpstr = curstr;
        while ( '\0' != *tmpstr && '#' != *tmpstr ) {
            tmpstr++;
        }
        len = tmpstr - curstr;
        if ( len > 0 ) {
            purl->query = malloc(sizeof(char) * (len + 1));
            if ( NULL == purl->query ) {
                parsed_url_free(purl);
                return NULL;
            }
            (void)strncpy(purl->query, curstr, len);
            purl->query[len] = '\0';
        }
        curstr = tmpstr;
    }

    /* Parse fragment */
    if ( '#' == *curstr ) {
        curstr++;
        tmpstr = curstr;
        while ( '\0' != *tmpstr ) {
            tmpstr++;
        }
        len = tmpstr - curstr;
        if ( len > 0 ) {
            purl->fragment = malloc(sizeof(char) * (len + 1));
            if ( NULL == purl->fragment ) {
                parsed_url_free(purl);
                return NULL;
            }
            (void)strncpy(purl->fragment, curstr, len);
            purl->fragment[len] = '\0';
        }
    }

    return purl;
}

/*
 * Free memory of parsed url
 */
void
parsed_url_free(struct parsed_url *purl)
{
    if ( NULL != purl ) {
        if ( NULL != purl->scheme ) {
            free(purl->scheme);
        }
        if ( NULL != purl->host ) {
            free(purl->host);
        }
        if ( NULL != purl->port ) {
            free(purl->port);
        }
        if ( NULL != purl->path ) {
            free(purl->path);
        }
        if ( NULL != purl->query ) {
            free(purl->query);
        }
        if ( NULL != purl->fragment ) {
            free(purl->fragment);
        }
        if ( NULL != purl->username ) {
            free(purl->username);
        }
        if ( NULL != purl->password ) {
            free(purl->password);
        }
        free(purl);
    }
}
