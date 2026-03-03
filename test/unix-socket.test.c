#include "url-parser.h"
#include "test.h"

void test_unix_triple_slash() {
    struct parsed_url *purl = parse_url("unix:///path/to/socket");
    ASSERT("unix triple slash", purl != NULL);
    ASSERT_STRING_EQUALS("unix", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("/path/to/socket", purl->path);
    ASSERT_EQUALS(NULL, purl->username);
    ASSERT_EQUALS(NULL, purl->password);
    parsed_url_free(purl);
}

void test_unix_triple_slash_with_credentials() {
    struct parsed_url *purl = parse_url("unix://user:pass@/path/to/socket");
    ASSERT("unix with credentials", purl != NULL);
    ASSERT_STRING_EQUALS("unix", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("/path/to/socket", purl->path);
    ASSERT_STRING_EQUALS("user", purl->username);
    ASSERT_STRING_EQUALS("pass", purl->password);
    parsed_url_free(purl);
}

void test_unix_single_slash() {
    struct parsed_url *purl = parse_url("unix:/path/to/socket");
    ASSERT("unix single slash", purl != NULL);
    ASSERT_STRING_EQUALS("unix", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("/path/to/socket", purl->path);
    ASSERT_EQUALS(NULL, purl->username);
    ASSERT_EQUALS(NULL, purl->password);
    parsed_url_free(purl);
}

void test_unix_no_slash() {
    struct parsed_url *purl = parse_url("unix:filename");
    ASSERT("unix no slash", purl != NULL);
    ASSERT_STRING_EQUALS("unix", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("filename", purl->path);
    ASSERT_EQUALS(NULL, purl->username);
    ASSERT_EQUALS(NULL, purl->password);
    parsed_url_free(purl);
}

void test_unix_double_slash_no_host() {
    struct parsed_url *purl = parse_url("unix://path/to/socket");
    ASSERT("unix double slash no host", purl != NULL);
    ASSERT_STRING_EQUALS("unix", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("path/to/socket", purl->path);
    ASSERT_EQUALS(NULL, purl->username);
    ASSERT_EQUALS(NULL, purl->password);
    parsed_url_free(purl);
}

void test_unix_double_slash_with_credentials_no_leading_slash() {
    struct parsed_url *purl = parse_url("unix://user:pass@path/to/socket");
    ASSERT("unix double slash with credentials no leading slash", purl != NULL);
    ASSERT_STRING_EQUALS("unix", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("path/to/socket", purl->path);
    ASSERT_STRING_EQUALS("user", purl->username);
    ASSERT_STRING_EQUALS("pass", purl->password);
    parsed_url_free(purl);
}

void test_redis_unix_socket() {
    struct parsed_url *purl = parse_url("redis:///var/run/redis.sock");
    ASSERT("redis unix socket", purl != NULL);
    ASSERT_STRING_EQUALS("redis", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("/var/run/redis.sock", purl->path);
    parsed_url_free(purl);
}

void test_postgres_unix_socket() {
    struct parsed_url *purl = parse_url("postgres:///var/run/postgres.sock");
    ASSERT("postgres unix socket", purl != NULL);
    ASSERT_STRING_EQUALS("postgres", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("/var/run/postgres.sock", purl->path);
    parsed_url_free(purl);
}

void test_unix_socket_with_relative_path() {
    struct parsed_url *purl = parse_url("unix:redis.sock");
    ASSERT("unix relative path", purl != NULL);
    ASSERT_STRING_EQUALS("unix", purl->scheme);
    ASSERT_STRING_EQUALS("redis.sock", purl->path);
    parsed_url_free(purl);
}

void test_unix_socket_credentials_with_port() {
    struct parsed_url *purl = parse_url("unix://user:pass@/path/to/sock:8080");
    ASSERT("unix with credentials and port", purl != NULL);
    ASSERT_STRING_EQUALS("unix", purl->scheme);
    ASSERT_EQUALS(NULL, purl->host);
    ASSERT_EQUALS(NULL, purl->port);
    ASSERT_STRING_EQUALS("/path/to/sock:8080", purl->path);
    ASSERT_STRING_EQUALS("user", purl->username);
    ASSERT_STRING_EQUALS("pass", purl->password);
    parsed_url_free(purl);
}

int main() {
    RUN(test_unix_triple_slash);
    RUN(test_unix_triple_slash_with_credentials);
    RUN(test_unix_single_slash);
    RUN(test_unix_no_slash);
    RUN(test_unix_double_slash_no_host);
    RUN(test_unix_double_slash_with_credentials_no_leading_slash);
    RUN(test_redis_unix_socket);
    RUN(test_postgres_unix_socket);
    RUN(test_unix_socket_with_relative_path);
    RUN(test_unix_socket_credentials_with_port);
    return TEST_REPORT();
}
