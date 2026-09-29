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

Here is how the server works:
![alt text](assets/serverSetup.png)

## The full journey `GET /index.html HTTP/1.1` 
0. On the browser side, before your code runs:
- localhost → 127.0.0.1 (via /etc/hosts);
- the TCP handshake is completed by the kernel, which puts the connection in the listening socket's
queue (listen backlog).

1. New connection: case SERVER
- poll() reports POLLIN on the listening socket for 8089.
- createNewClient() → accept() returns a new file descriptor for this client, which is set to
O_NONBLOCK and FD_CLOEXEC, wrapped in a Connection of type CLIENT (with its own Request/Response),
and put in newConns.
- At the end of the loop, it moves into serverConnections, with events = POLLIN.

2. Reading the request: case CLIENT, POLLIN
- On a next poll(), the client's socket reports POLLIN. recv() → the bytes are appended to buffer →
lastActive is updated.
- As long as \r\n\r\n isn't found, you wait for the next recv (with the 16 KB → 431 limit).
- Once found: handleHeaders() → parseHeaders() (method, path, version, headers) → checkHeaders() (..
in the path, version, Host), otherwise 400.

3. Route and validation
- findRoute("/index.html", router): longest prefix, so location /.
- validateRequest(): is the method known (otherwise 501)? Is GET allowed (otherwise 405)? A redirect
(301)? Is Content-Length too big (413)?
- No Content-Length and not chunked → no body → finishRequest(): state = COMPLETED, events = POLLOUT,
and isCgi()? No, .html isn't a CGI extension.

4. Building the response: case CLIENT, POLLOUT → sendDataToClient()
- OUT():
- getPath() = root + path = ./var/www + /index.html = ./var/www/index.html (the full URI, including
    the /);
- stat() finds it exists and isn't a directory → access(R_OK) → readFileContent() → statusCode = 
    200.
- SEND(): Content-Type: text/html via getMimeType(".html"), Content-Length: 1649, Connection: close.
- stringify(): status line + headers + \r\n + body, all in RES->body.

5. Sending
- send() with RES->sent, possibly over several POLLOUT rounds if it's a partial write.
- When sent == body.size() → sendDataToClient() returns 0 → closeConnection(): close(fd), delete,
erase from the vector, curIdx--.

6. After
- The browser displays the HTML, then sends new requests for the CSS and images. Because of Connection: close, each one goes over a new TCP connection, so back to step 1.
- The listening socket, meanwhile, never stopped being watched.

![alt text](assets/requestJourney.png)


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
./webserv server.conf
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

We used AI (Claude) as an assistant, not as the author of the project. The architecture and the core of every feature (the `poll()` loop, request parsing, CGI handling, the configuration parser) were designed and written by us. AI came in afterwards, mainly for:
- **Edge-case testing.** We built the structure of the config tests (`tests/configsFiles/`, `tests/tester.py`) and asked AI: *"based on my current structure, add hardcore tests"*. It generated about 120 malformed or unusual configuration files, after the 6th we already created.
- **Review against the subject.** AI reviewed the codebase and helped us reproduce bugs with `curl`, `nc`, and `siege`.
- **Fixes.** For each bug, we first worked out the cause together. For some fixes AI proposed the code, which we reviewed, integrated ourselves and tested again.
- **Documentation and debugging.** Structuring and wording this README, and helping during debugging sessions when we were stuck. It also helped us debug a potential `atoi` undefined behavior, chunked bodies containing `\r\n`, HTTP/1.0 requests without a `Host` header.


## TO DELETE WHEN DONE
- can i send the tester?
- solve the regular file reading stalling the server
- dupliacted headers received (upper and lower case?)
- if i send two files in one cboundary it works? Different boundary delimiter?
- is POST with upload enabled and folder saolved?
- Is data() forbidden in cpp98?
- Review all throw in setupServer delete everything before closing
- Try testing with telnet and NGINX, etc.. know how to do 
- difference select(), kqueue(), or epoll() vs poll()
- RFCs know better
- only http 1.1 is okay?
- difference requesttimeout and gateway timeout? Your HTTP response status codes must be accurate?
- siege -c 255 -r 5000 "http://localhost:8089/cgi-bin/timeout.js"
- can i keep files uploaded when server ends?
- what are IO operatiosn and what does "non-blocking and use only 1 poll() (or equivalent) for all the I/O operations between the clients and the server (listen included)" means
- are the ServerOut same as a webserver?
- what does this mean: "Regular disk files are exempt." (in "Calling read/recv or write/send on these descriptors without prior readiness...")
- What does a flag concretely does and how is it linked to a pollfd?
- is it compatible with firefox? 
- NGINX may be used to compare headers and answer behaviours (pay attention to differences between HTTP versions).
- Your server must have default error pages if none are provided.
• Your server must be able to listen to multiple ports to deliver different content (see Configuration file).
For MacOS only
Since macOS handles write() differently from other Unix-based OSes, you are allowed to use fcntl(). You must use file descriptors in non-blocking mode to achieve behaviour similar to that of other Unix OSes.
- If no content_length is returned from the CGI, EOF will mark the end of the returned data.
- The CGI should be run in the correct directory for relative path file access.
- You can have other rules or configuration information in your file (e.g., a server name for a website if you plan to implement virtual hosts). -> What are virtual hosts?
- explanation about the basics of an HTTP server.
- Search for all read/recv/write/send and check if the returned value is correctly handled (checking only -1 or only 0 is not enough; both must be handled).
- If errno is checked after read/recv/write/send, the grade is 0 and the evaluation process ends immediately (errno may be logged, not used to drive control flow).
- Writing or reading event-driven descriptors (sockets, pipes/FIFOs, TTY, etc.) without going through poll() (or equivalent) is strictly FORBIDDEN. Regular disk files are exempt.
- Confirm that synchronous I/O on regular disk files (e.g., reading the configuration or small static files) does not stall the event loop.
- "Set up multiple websites on different interfaces and ports" what are interfaces?
- Set up a default error page (try modifying the 404 error page).
- Set up routes on a server to different directories.
- Set up a default file to serve when requesting a directory.
- Set up a list of accepted methods for a specific route (e.g., try DELETE something with and without permission).

Using telnet, curl, and pre-prepared files, demonstrate that the following features function correctly:
- GET, POST, and DELETE requests should work.
- UNKNOWN requests should not result in a crash.
- For every test, you should receive the appropriate status code.
- Upload some files to the server and retrieve them.

Check CGI
- You must test with CGI files containing errors to ensure that server's error handling works correctly. You can use a script containing an infinite loop or an error; you are free to do whatever tests you want within the limits of acceptability that remain at your discretion. The group being evaluated should help you with this.
- The server should never crash, and an error should be displayed if an issue occurs.

Check with a browser
- Use the browser chosen by the team. Open the network tab and try connecting to the server.
- Look at the request header and response header.
- It should be compatible with serving a fully static website.
- Try an incorrect URL on the server.
- Try to list a directory.
- Try a redirected URL.
- Try anything you like.

Port issues
In the configuration file, set up multiple interfaces and ports to provide different websites. Use the browser to verify that the configuration works correctly and serves the appropriate website.
Configure multiple websites on the same interface:port. This should result in an error, unless the team has chosen to implement the virtual host feature. Both approaches are valid, as long as everything works as expected.
Launch multiple webserv programs at the same time with different configuration files but with common interface:ports. Does it work? If it does, ask why. Ensure that, whatever the group's choice was, the program behaviour is coherent and does not crash.

Siege & stress test
Use Siege to run some stress tests.
Availability should be above 99.5% for a simple GET request on an empty page using a siege with the -b flag.
Ensure there are no memory leaks (Monitor the process memory usage; it should not increase indefinitely).
Check that there are no hanging connections.
You should be able to use siege indefinitely without having to restart the server (take a look at siege with -b flag).
When conducting load tests using the siege command, be careful, it depends on your OS. it is crucial to limit the number of connections per second by specifying options such as -c (number of clients), -d (maximum wait time before a client reconnects), and -r (number of attempts). The choice of these parameters is at the evaluator's discretion. However, it is imperative to reach an agreement with the person being evaluated to ensure a fair and transparent assessment of the web server's performance.
