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

### EVALUATION

Check the code and ask questions
- Ask for an explanation about the basics of an HTTP server.
- The poll() (or equivalent) should be in the main loop and must check file descriptors for both reading and writing simultaneously. - Search for all read/recv/write/send on a socket and check that, if an error is returned, the client is removed.
- Search for all read/recv/write/send and check if the returned value is correctly handled (checking only -1 or only 0 is not enough; both must be handled).
- If errno is checked after read/recv/write/send, the grade is 0 and the evaluation process ends immediately (errno may be logged, not used to drive control flow).
- Writing or reading event-driven descriptors (sockets, pipes/FIFOs, TTY, etc.) without going through poll() (or equivalent) is strictly FORBIDDEN. Regular disk files are exempt.
- Confirm that synchronous I/O on regular disk files (e.g., reading the configuration or small static files) does not stall the event loop.
- Search for the HTTP response status codes list on the internet. During this evaluation, if any status codes are incorrect, do not award any points related to them.
- Set up multiple websites on different interfaces and ports.
- Set up a default error page (try modifying the 404 error page).
- Limit the size of the client request body (use: curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter or longer than body limit").
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

Bonus part
There is a functional session and cookie system on the web server.
The web server supports multiple CGI systems.
