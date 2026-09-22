*This project has been created as part of the 42 curriculum by vibarre, vivaz-ca.*

# Webserv

## Description

Webserv is a fully custom HTTP/1.1 server written from scratch in C++98, built to run without
any external or Boost libraries. It parses a configuration file (inspired by NGINX's `server`
block syntax), binds to one or more `interface:port` pairs, and serves static files, handles
file uploads, deletes resources, and executes CGI scripts (e.g. Python) based on file extension.

All client and CGI I/O (accept, read, write) is driven through a single non-blocking `poll()`
loop — the server never issues a `read`/`write`/`recv`/`send` without first checking that the
file descriptor is ready, and it never crashes or hangs on a malformed or slow client.

Key features:
- Configuration file defining multiple listening ports, routes, error pages and body size limits
- `GET`, `POST`, and `DELETE` methods
- Static file serving, directory listing (autoindex) on/off per route, and default index files
- File uploads with a configurable storage directory
- CGI execution (e.g. Python) based on file extension, with request data passed via environment
  variables and stdin/stdout pipes
- Per-route configuration: allowed methods, root/alias directory, redirection, default file,
  autoindex, upload directory, CGI mapping
- Custom default error pages, used when none are configured

## Instructions

### Compilation

```sh
make        # builds the ./webserv executable
make clean  # removes object files
make fclean # removes object files and the executable
make re     # fclean + all
```

Requires a C++98-compatible compiler (`c++`). No external dependencies.

### Running

```sh
./webserv [configuration file]
```

Example, using one of the provided templates:

```sh
./webserv templates/conf/server.conf
```

Then point a browser or `curl`/`telnet` at the configured host/port(s), e.g.:

```sh
curl http://localhost:8089/
```

### Configuration file

The configuration file uses an NGINX-inspired `server { ... }` block syntax. Each `server`
block defines one or more `listen` ports and a set of `location` blocks. Example:

```
server {
	listen 8089;
	client_max_body_size 1024;

	location / {
		root ./var/www;
		index /index.html;
		allowed_methods GET POST;
	}

	location /upload {
		root ./var/www;
		upload_store /uploads;
	}

	location /delete {
		root ./var/www/uploads;
		allowed_methods DELETE;
	}
}
```

Sample configuration files and their matching static/CGI/upload content used to exercise every
feature during evaluation are provided under `templates/conf/` and `var/`.

## Resources

- [RFC 7230 — HTTP/1.1: Message Syntax and Routing](https://www.rfc-editor.org/rfc/rfc7230)
- [RFC 7231 — HTTP/1.1: Semantics and Content](https://www.rfc-editor.org/rfc/rfc7231)
- [NGINX documentation — `server` and `location` block reference](https://nginx.org/en/docs/)
- [Common Gateway Interface (CGI) — RFC 3875](https://www.rfc-editor.org/rfc/rfc3875)
- `poll(2)` / `select(2)` man pages

### AI usage

AI assistance was used during this project for: drafting/organizing the initial task
breakdown (see `todo.MD`), and as a sounding board while debugging the configuration parser
and CGI environment-variable handling. All generated suggestions were reviewed, tested against
real browsers/`curl`/NGINX comparisons, and understood before being integrated — nothing was
merged without being able to explain how it works.
