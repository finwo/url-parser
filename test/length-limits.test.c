#include "url-parser.h"
#include "test.h"

void test_scheme_too_long() {
    char scheme[72];
    memset(scheme, 'a', 65);
    scheme[0] = 'h';
    scheme[1] = 't';
    scheme[2] = 't';
    scheme[3] = 'p';
    scheme[65] = '\0';
    char url[128];
    snprintf(url, sizeof(url), "%s://example.com", scheme);
    struct parsed_url *purl = parse_url(url);
    ASSERT("scheme too long returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_SCHEME_TOO_LONG", parse_url_last_error());
}

void test_username_too_long() {
    char user[262];
    memset(user, 'a', 256);
    user[256] = '\0';
    char url[320];
    snprintf(url, sizeof(url), "http://%s@example.com/", user);
    struct parsed_url *purl = parse_url(url);
    ASSERT("username too long returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_USERNAME_TOO_LONG", parse_url_last_error());
}

void test_password_too_long() {
    char pass[262];
    memset(pass, 'a', 256);
    pass[256] = '\0';
    char url[320];
    snprintf(url, sizeof(url), "http://user:%s@example.com/", pass);
    struct parsed_url *purl = parse_url(url);
    ASSERT("password too long returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_PASSWORD_TOO_LONG", parse_url_last_error());
}

void test_host_too_long() {
    char host[262];
    memset(host, 'a', 256);
    host[256] = '\0';
    char url[320];
    snprintf(url, sizeof(url), "http://%s/", host);
    struct parsed_url *purl = parse_url(url);
    ASSERT("host too long returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_HOST_TOO_LONG", parse_url_last_error());
}

void test_port_too_long() {
    struct parsed_url *purl = parse_url("http://example.com:123456/");
    ASSERT("port too long returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_PORT_TOO_LONG", parse_url_last_error());
}

void test_port_invalid_zero() {
    struct parsed_url *purl = parse_url("http://example.com:0/");
    ASSERT("port zero returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_PORT_INVALID", parse_url_last_error());
}

void test_port_invalid_overflow() {
    struct parsed_url *purl = parse_url("http://example.com:65536/");
    ASSERT("port overflow returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_PORT_INVALID", parse_url_last_error());
}

void test_path_too_long() {
    char path[4100];
    memset(path, 'a', 4097);
    path[4097] = '\0';
    char url[4200];
    snprintf(url, sizeof(url), "http://example.com/%s", path);
    struct parsed_url *purl = parse_url(url);
    ASSERT("path too long returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_PATH_TOO_LONG", parse_url_last_error());
}

void test_query_too_long() {
    char query[4100];
    memset(query, 'a', 4097);
    query[4097] = '\0';
    char url[4200];
    snprintf(url, sizeof(url), "http://example.com/?%s", query);
    struct parsed_url *purl = parse_url(url);
    ASSERT("query too long returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_QUERY_TOO_LONG", parse_url_last_error());
}

void test_fragment_too_long() {
    char frag[4100];
    memset(frag, 'a', 4097);
    frag[4097] = '\0';
    char url[4200];
    snprintf(url, sizeof(url), "http://example.com/#%s", frag);
    struct parsed_url *purl = parse_url(url);
    ASSERT("fragment too long returns NULL", purl == NULL);
    ASSERT_STRING_EQUALS("ERR_FRAGMENT_TOO_LONG", parse_url_last_error());
}

int main() {
    RUN(test_scheme_too_long);
    RUN(test_username_too_long);
    RUN(test_password_too_long);
    RUN(test_host_too_long);
    RUN(test_port_too_long);
    RUN(test_port_invalid_zero);
    RUN(test_port_invalid_overflow);
    RUN(test_path_too_long);
    RUN(test_query_too_long);
    RUN(test_fragment_too_long);
    return TEST_REPORT();
}
