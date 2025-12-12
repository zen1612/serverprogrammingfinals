# serverprogrammingfinals

1. HTTP Server 
 Uses TCP sockets 
 Listens on a port (e.g., 8080) 
 Accepts a client connection 
 Reads a GET request 
 Sends back: 
o HTTP/1.1 status line 
o Content-Type 
o Content-Length 
o Connection: close 
o HTML page body 
Example GET request: 
GET /index.html HTTP/1.1 
Host: 127.0.0.1 
Connection: close 
2. HTTP Client 
 Connect to the server via TCP 
 Send GET request manually 
 Receive the response 
 Parse and print HTTP headers + body 
Deliverables: 
1. Source code (server + client) 
2. Screenshot of GET request + server response 
3. Show your webpage loaded in a browser (http://127.0.0.1:8080) 
4. Wireshark capture of: 
o TCP handshake 
o HTTP GET 
o HTTP Response 
5. Explanation of protocols: 
o Application: HTTP 
o Transport: TCP 
o Network: IP 
o Link: ARP/Ethernet 
6. 5 minute presentation demo
