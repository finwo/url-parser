#include "url-parser.h"
#include "test.h"

void test_tcp_empty_host_with_port() {
    struct parsed_url *purl = parse_url("tcp://:6379");
    ASSERT("tcp empty host with port", purl != NULL);
    ASSERT_STRING_EQUALS("tcp", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_STRING_EQUALS("6379", purl->port);
    ASSERT_STRING_EQUALS(NULL, purl->path);
    parsed_url_free(purl);
}

void test_redis_empty_host_with_port() {
    struct parsed_url *purl = parse_url("redis://:6379");
    ASSERT("redis empty host with port", purl != NULL);
    ASSERT_STRING_EQUALS("redis", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_STRING_EQUALS("6379", purl->port);
    parsed_url_free(purl);
}

void test_http_empty_host_with_port() {
    struct parsed_url *purl = parse_url("http://:8080");
    ASSERT("http empty host with port", purl != NULL);
    ASSERT_STRING_EQUALS("http", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_STRING_EQUALS("8080", purl->port);
    parsed_url_free(purl);
}

void test_empty_host_no_port() {
    struct parsed_url *purl = parse_url("tcp://:");
    ASSERT("tcp empty host no port", purl != NULL);
    ASSERT_STRING_EQUALS("tcp", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_STRING_EQUALS(NULL, purl->port);
    parsed_url_free(purl);
}

void test_empty_host_only_slashes() {
    struct parsed_url *purl = parse_url("tcp://");
    ASSERT("tcp only slashes", purl != NULL);
    ASSERT_STRING_EQUALS("tcp", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_STRING_EQUALS(NULL, purl->port);
    parsed_url_free(purl);
}

int main() {
    RUN(test_tcp_empty_host_with_port);
    RUN(test_redis_empty_host_with_port);
    RUN(test_http_empty_host_with_port);
    RUN(test_empty_host_no_port);
    RUN(test_empty_host_only_slashes);
    return TEST_REPORT();
}
