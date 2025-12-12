#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 4096

void error_handling(char *message);
void handle_get_request(int clnt_sock, char *request);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    printf("HTTP server running on port %s...\n", argv[1]);

    clnt_adr_sz = sizeof(clnt_adr);
    while(1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if (clnt_sock == -1)
            error_handling("accept() error");

        str_len = read(clnt_sock, message, BUF_SIZE);
        if (str_len > 0)
        {
            message[str_len] = '\0';
            handle_get_request(clnt_sock, message);
        }

        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void handle_get_request(int clnt_sock, char *request)
{
    if (strncmp(request, "GET /index.html", 15) == 0 || 
        strncmp(request, "GET / ", 6) == 0)
    {
        FILE *fp = fopen("index.html", "r");
        if (!fp) {
            char error[] =
                "HTTP/1.1 500 Internal Server Error\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 21\r\n"
                "Connection: close\r\n\r\n"
                "File not found error";
            write(clnt_sock, error, sizeof(error)-1);
            return;
        }

        char body[BUF_SIZE];
        int body_len = fread(body, 1, sizeof(body), fp);
        fclose(fp);

        char header[256];
        sprintf(header,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n\r\n",
            body_len
        );

        write(clnt_sock, header, strlen(header));
        write(clnt_sock, body, body_len);
    }
    else
    {
        char response[] = 
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "Content-Length: 95\r\n\r\n"
            "<html><body><h1>404 Not Found</h1><p>The requested page does not exist.</p></body></html>";

        write(clnt_sock, response, sizeof(response)-1);
    }
}
