#include "url-parser.h"
#include "test.h"

void test_http_url() {
    struct parsed_url *purl = parse_url("http://www.example.com/path/to/resource?query=value#fragment");
    ASSERT("http scheme", purl != NULL);
    ASSERT_STRING_EQUALS("http", purl->scheme);
    ASSERT_STRING_EQUALS("www.example.com", purl->host);
    ASSERT_STRING_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("/path/to/resource", purl->path);
    ASSERT_STRING_EQUALS("query=value", purl->query);
    ASSERT_STRING_EQUALS("fragment", purl->fragment);
    ASSERT_STRING_EQUALS(NULL, purl->username);
    ASSERT_STRING_EQUALS(NULL, purl->password);
    parsed_url_free(purl);
}

void test_https_with_port() {
    struct parsed_url *purl = parse_url("https://www.example.com:8080/path");
    ASSERT("https with port", purl != NULL);
    ASSERT_STRING_EQUALS("https", purl->scheme);
    ASSERT_STRING_EQUALS("www.example.com", purl->host);
    ASSERT_STRING_EQUALS("8080", purl->port);
    ASSERT_STRING_EQUALS("/path", purl->path);
    parsed_url_free(purl);
}

void test_ftp_with_credentials() {
    struct parsed_url *purl = parse_url("ftp://user:password@ftp.example.com/file.txt");
    ASSERT("ftp with credentials", purl != NULL);
    ASSERT_STRING_EQUALS("ftp", purl->scheme);
    ASSERT_STRING_EQUALS("ftp.example.com", purl->host);
    ASSERT_STRING_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("/file.txt", purl->path);
    ASSERT_STRING_EQUALS("user", purl->username);
    ASSERT_STRING_EQUALS("password", purl->password);
    parsed_url_free(purl);
}

void test_http_ipv6() {
    struct parsed_url *purl = parse_url("http://[::1]:8080/path");
    ASSERT("http IPv6", purl != NULL);
    ASSERT_STRING_EQUALS("http", purl->scheme);
    ASSERT_STRING_EQUALS("::1", purl->host);
    ASSERT_STRING_EQUALS("8080", purl->port);
    ASSERT_STRING_EQUALS("/path", purl->path);
    parsed_url_free(purl);
}

void test_query_only() {
    struct parsed_url *purl = parse_url("http://example.com?foo=bar");
    ASSERT("query only", purl != NULL);
    ASSERT_STRING_EQUALS("http", purl->scheme);
    ASSERT_STRING_EQUALS("example.com", purl->host);
    ASSERT_STRING_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS(NULL, purl->path);
    ASSERT_STRING_EQUALS("foo=bar", purl->query);
    parsed_url_free(purl);
}

void test_fragment_only() {
    struct parsed_url *purl = parse_url("http://example.com#section");
    ASSERT("fragment only", purl != NULL);
    ASSERT_STRING_EQUALS("http", purl->scheme);
    ASSERT_STRING_EQUALS("example.com", purl->host);
    ASSERT_STRING_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS(NULL, purl->path);
    ASSERT_STRING_EQUALS(NULL, purl->query);
    ASSERT_STRING_EQUALS("section", purl->fragment);
    parsed_url_free(purl);
}

void test_no_path() {
    struct parsed_url *purl = parse_url("http://www.example.com");
    ASSERT("no path", purl != NULL);
    ASSERT_STRING_EQUALS("http", purl->scheme);
    ASSERT_STRING_EQUALS("www.example.com", purl->host);
    ASSERT_STRING_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS(NULL, purl->path);
    parsed_url_free(purl);
}

void test_username_only() {
    struct parsed_url *purl = parse_url("ftp://user@ftp.example.com/file");
    ASSERT("username only", purl != NULL);
    ASSERT_STRING_EQUALS("ftp", purl->scheme);
    ASSERT_STRING_EQUALS("ftp.example.com", purl->host);
    ASSERT_STRING_EQUALS("user", purl->username);
    ASSERT_STRING_EQUALS(NULL, purl->password);
    parsed_url_free(purl);
}

int main() {
    RUN(test_http_url);
    RUN(test_https_with_port);
    RUN(test_ftp_with_credentials);
    RUN(test_http_ipv6);
    RUN(test_query_only);
    RUN(test_fragment_only);
    RUN(test_no_path);
    RUN(test_username_only);
    return TEST_REPORT();
}
