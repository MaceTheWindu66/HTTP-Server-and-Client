NOTE: THIS CODE MUST BE RUN IN A LINUX ENVIRONMENT

To run, first you must make the c files into executables. To make, just type make into the terminal.

After this, you will have 2 things made: http_server and http_client.

http_server:
    To run the http_server, do ./http_server <port_number> in your terminal. I made the default port for the server 5000, so the
    command used to run the server most effectively is 
        ./http_server 5000
    
    After entering this command into your terminal, the http_server will begin running and will say "Server listening on port 5000..."
    When a request is sent to the server, the server logs it as 
        Client request:
        <HTTP Request Here>
        Host: linux.wpi.edu
        Connection: close

    To terminate the server / process, just do ctrl+c.

http_client:
    To run the http_client, the following command must be input:
     ./http_client [-options] server_url port_number
    Options:
        -p : Print RTT for the request

    In the output, you can see all of the details of the request: server URL, port number, RTT if specified, resolved IP,
    hostname, path, etc. Then you will get the HTML file 
    Here is an example output from looking up example.com:

./http_client -p www.example.com 80
----------------------------------------------------------------------------------------------
    Server URL: www.example.com
    Port: 80
    RTT measurement enabled
    Hostname: www.example.com
    Path: /
    Socket created successfully
    Resolved IP address: 93.184.215.14
    Connected to the server
    RTT: 4.00 ms
    HTTP Request:
    GET // HTTP/1.1
    Host: www.example.com
    Connection: close


    Request sent successfully
    Response:
    HTTP/1.1 200 Not Modified
    Accept-Ranges: bytes
    Age: 495161
    Cache-Control: max-age=604800
    Content-Type: text/html; charset=UTF-8
    Date: Wed, 20 Nov 2024 23:47:27 GMT
    Etag: "3147526947+ident"
    Expires: Wed, 27 Nov 2024 23:47:27 GMT
    Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
    Server: ECAcc (lac/55F8)
    Vary: Accept-Encoding
    X-Cache: HIT
    Content-Length: 1256
    Connection: close

    <!doctype html>
    <html>
    <head>
        <title>Example Domain</title>

        <meta charset="utf-8" />
        <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <style type="text/css">
        body {
            background-color: #f0f0f2;
            margin: 0;
            padding: 0;
            font-family: -apple-system, system-ui, BlinkMacSystemFont, "Segoe UI", "Open Sans", "Helvetica Neue", Helvetica, Arial, sans-serif;
            
        }
        div {
            width: 600px;
            margin: 5em auto;
            padding: 2em;
            background-color: #fdfdff;
            border-radius: 0.5em;
            box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);
        }
        a:link, a:visited {
            color: #38488f;
            text-decoration: none;
        }
        @media (max-width: 700px) {
            div {
                margin: 0 auto;
                width: auto;
            }
        }
        </style>    
    </head>

    <body>
    <div>
        <h1>Example Domain</h1>
        <p>This domain is for use in illustrative examples in documents. You may use this
        domain in literature without prior coordination or asking for permission.</p>
        <p><a href="https://www.iana.org/domains/example">More information...</a></p>
    </div>
    </body>
    </html>

    Response received successfully
-------------------------------------------------------------------------------------------------

This is how my program is run! Have a great day!
