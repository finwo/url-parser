# url-parser

A C library for parsing URLs, with support for Unix socket paths and empty hosts.

## Installation

This library can be installed using the [dep](https://github.com/finwo/dep) package manager:

```bash
dep add finwo/url-parser
```

## Usage

```c
#include "url-parser.h"

struct parsed_url *purl = parse_url("http://www.example.com/path?query=value#fragment");

if (purl != NULL) {
    printf("Scheme:   %s\n", purl->scheme);
    printf("Host:     %s\n", purl->host);
    printf("Port:     %s\n", purl->port);
    printf("Path:     %s\n", purl->path);
    printf("Query:    %s\n", purl->query);
    printf("Fragment: %s\n", purl->fragment);
    printf("Username: %s\n", purl->username);
    printf("Password: %s\n", purl->password);
    
    parsed_url_free(purl);
}
```

## Features

### Standard URL Parsing

```c
parse_url("http://example.com/path");
// scheme: "http", host: "example.com", path: "/path"

parse_url("https://example.com:8080/path");
// scheme: "https", host: "example.com", port: "8080", path: "/path"

parse_url("ftp://user:password@ftp.example.com/file");
// scheme: "ftp", host: "ftp.example.com", username: "user", password: "password", path: "/file"

parse_url("http://[::1]:8080/path");
// scheme: "http", host: "::1", port: "8080", path: "/path"
```

### Query and Fragment Only

```c
parse_url("http://example.com?foo=bar");
// scheme: "http", host: "example.com", query: "foo=bar"

parse_url("http://example.com#section");
// scheme: "http", host: "example.com", fragment: "section"
```

### Empty Host

```c
parse_url("tcp://:6379");
// scheme: "tcp", host: NULL, port: "6379"
```

### Unix Socket URLs

The library supports Unix socket paths with various formats:

```c
// Standard unix socket
parse_url("unix:///var/run/redis.sock");
// scheme: "unix", path: "/var/run/redis.sock", host: NULL

// Unix socket with leading slash
parse_url("unix:/path/to/socket");
// scheme: "unix", path: "/path/to/socket"

// Unix socket without slashes
parse_url("unix:redis.sock");
// scheme: "unix", path: "redis.sock"

// Unix socket with credentials
parse_url("unix://user:pass@/path/to/socket");
// scheme: "unix", username: "user", password: "pass", path: "/path/to/socket"

// Redis/Postgres style unix socket
parse_url("redis:///var/run/redis.sock");
// scheme: "redis", path: "/var/run/redis.sock"
```

Note: For path-based schemes (`unix:`, `file:`, `cunix:`), the entire portion after `://` is treated as the path, and no host or port is parsed.

## License

Copyright (c) 2026 finwo. See LICENSE.md for details.
