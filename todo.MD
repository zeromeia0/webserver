**TASK DIVISION (A.I GENERATED FOR BETTER VISUALIZATION)**

1. Parse server.conf [PARSING]
    - Read config file
    - Tokenize directives
    - Build ServerConfig and RouteConfig
    - Validate configuration

REJECT UNKNOWN DIRECTIVES
MISSING SEMICOLLUMN
UNEXPECTED SEMICOLUMN
MISSING OPENING BRACE
EXTRA CLOSING BRACE
INVALID NUMBER OF ARGUMENTS
NESTED BLOCKS
INVALID PORT
DUPLICATED LITEN POSTS
INVALID HTTP METHODS
INVALID AUTOINDEX VALUE
INVALID UPLOAD_ENABLE VALUE
INVALID ERROR CODE
EMPTY ROUTE PATH
DUPLICATE ROUTHE PATHS
empty root
empty redirect
NO SERVER BLOCK
NO LISTEN DIRECTIVE
NO LOCATION BLOCK
ROUTE MATCHING SANITY (STARTS WITH /)

2. Create/start server [EXECUTION]
    - socket()
    - bind()
    - listen()
    - Initialize poll/select
3. Accept client connection [EXECUTION]
    - accept()
    - Store client socket
4. Receive HTTP request [EXECUTION]
    - recv()
    - Read raw request bytes
5. Parse HTTP request [PARSING]
    - Parse request line
    - Parse headers
    - Parse body
    - Build HttpRequest object
6. Validate request [PARSING]
    - Check HTTP syntax
    - Check method validity
    - Check client_max_body_size
    - Match route
    - Check allowed_methods
    - Decide status code (200, 404, 405, 413, etc.)
7. Execute request [EXECUTION]
    - Read requested file
    - Generate directory listing
    - Store uploaded file
    - Delete file
    - Execute CGI
    - Follow redirect rules
8. Generate HTTP response [PARSING]
    - Build status line
    - Build headers
    - Build response object
9. Send HTTP response [EXECUTION]
    - send()
10. Close connection / Keep-Alive [EXECUTION]
    - close()
    - or keep socket open