
//gcc ClientMorse.c -o ClientMorse.exe -lwsock32
//gcc 8.1.0 or higher required
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define DEFAULT_PORT 2022
#define BUFF_LEN 256

//Por defecto usamos el protocolo TCP
#define DEFAULT_PROTO SOCK_STREAM

void Usage(char *progname)
{
    fprintf(stderr,"Usage: %s -p [protocol] -n [server name/IP] -e [port_num] -l [iterations]\n", progname);
    fprintf(stderr,"Where:\n\tprotocol is one of TCP or UDP\n");
    fprintf(stderr,"\t- server is the IP address or name of server\n");
    fprintf(stderr,"\t- port_num is the port to listen on\n");
    fprintf(stderr,"\t- iterations is the number of loops to execute.\n");
    fprintf(stderr,"\t- (-l by itself makes client run in an infinite loop,\n");
    fprintf(stderr,"\t- Hit Ctrl-C to terminate it)\n");
    fprintf(stderr,"\t- The defaults are TCP , localhost and port 2022\n");
    WSACleanup();
    exit(1);
}

int main(int argc, char **argv)

{
    char Buffer[BUFF_LEN];
    char *server_name= "localhost"; //dejar vacío si el host es otro ordenador
    unsigned short port = DEFAULT_PORT;
    int retval, loopflag = 0;
    int i, loopcount, maxloop=-1;
    unsigned int addr;
    int socket_type = DEFAULT_PROTO;
    struct sockaddr_in server;
    struct hostent *hp;
    WSADATA wsaData;
    SOCKET  conn_socket;

    //En el caso de que el usuario no ejecute el programa con las opcionese
    //por defecto, parseamos los argumentos para asignar el protocolo,
    //el nombre del servidor y el puerto usado
    if (argc >1)
    {
        for(i=1; i<argc; i++)
        {
            if ((argv[i][0] == '-'))
            {
                switch(tolower(argv[i][1]))
                {
                    case 'p':
                        if (!stricmp(argv[i+1], "TCP"))
                            socket_type = SOCK_STREAM;
                        else if (!stricmp(argv[i+1], "UDP"))
                            socket_type = SOCK_DGRAM;
                        else
                            Usage(argv[0]);
                        i++;
                        break;
                    case 'n':
                        server_name = argv[++i];
                        break;
                    case 'e':
                        port = atoi(argv[++i]);
                        break;
                    case 'l':
                        loopflag =1;
                        if (argv[i+1]) {
                            if (argv[i+1][0] != '-')
                                maxloop = atoi(argv[i+1]);
                        }
                        else
                            maxloop = -1;
                        i++;
                        break;
                    default:
                        Usage(argv[0]);
                        break;
                }
            }
            else
                Usage(argv[0]);
        }
    }
    if ((retval = WSAStartup(0x202, &wsaData)) != 0)
    {
       fprintf(stderr,"Client: WSAStartup() failed with error %d\n", retval);
        WSACleanup();
        return -1;
    }
    else
        printf("Client: WSAStartup() is OK.\n");
    if (port == 0)
    {
        Usage(argv[0]);
    }

    //Si el nombre del servidor empieza por una letra llamamos a gethostbyname()
    //en el caso que sea una dirección IPv4 llamamos a gethostbyaddr()

    if (isalpha(server_name[0]))
    {   
        hp = gethostbyname(server_name);
    }
    else
    { 
        addr = inet_addr(server_name);
        hp = gethostbyaddr((char *)&addr, 4, AF_INET);
    }
    if (hp == NULL )
    {
        fprintf(stderr,"Client: Cannot resolve address \"%s\": Error %d\n", server_name, WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    else
        printf("Client: gethostbyaddr() is OK.\n");

    memset(&server, 0, sizeof(server));
    memcpy(&(server.sin_addr), hp->h_addr, hp->h_length);
    server.sin_family = hp->h_addrtype;
    server.sin_port = htons(port);

    // ABRIMOS EL SOCKET

    conn_socket = socket(AF_INET, socket_type, 0);
    if (conn_socket <0 )
    {
        fprintf(stderr,"Client: Error Opening socket: Error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    else
       printf("Client: socket() is OK.\n");

    printf("Client: Client connecting to: %s.\n", hp->h_name);

    //Intentamos conectarnos al servidor
    //Troubleshooting:
        //Si el programa devuelve el código de error 10060 posiblemente sea
        //porque el firewall está bloqueando el puerto TCP especificado
    if (connect(conn_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        fprintf(stderr,"Client: connect() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    else
        printf("Client: connect() is OK.\n");
    loopcount = 0;
    while(1)
    {   //Informamos al usuario de los caracteres que puede introducir y
        //le pedimos al usuario que introduzca un mensaje
        printf("Client: Allowed Characters -> A..Z 0..9 .,?'!/()&:;=+-_%c$@\n", (int)34);
        printf("Client: Enter a Message in UPPERCASE: ");
        fgets(Buffer,BUFF_LEN,stdin);

        retval = send(conn_socket, Buffer, sizeof(Buffer), 0);
        //Tratamiento de errores
        if (retval == SOCKET_ERROR)
        {
            fprintf(stderr,"Client: send() failed: error %d.\n", WSAGetLastError());
            WSACleanup();
            return -1;
        }
        else
            printf("Client: send() is OK.\n");
            printf("Client: Sent message -> %s\n", Buffer);
            retval = recv(conn_socket, Buffer, sizeof(Buffer), 0);
        //Tratamiento de errores
        if (retval == SOCKET_ERROR)
        {
            fprintf(stderr,"Client: recv() failed: error %d.\n", WSAGetLastError());
            closesocket(conn_socket);
            WSACleanup();
            return -1;
        }
        else
            printf("Client: recv() is OK.\n");

        // CERRAMOS EL SOCKET

        if (retval == 0)
        {
            printf("Client: Server closed connection.\n");
            closesocket(conn_socket);
            WSACleanup();
            return -1;
        }

        printf("Client: Received encoded message from server ->  %s\n", Buffer);

        if (!loopflag)
        {
            printf("Client: Terminating connection...\n");
            break;
        }
        else
        {
            if ((loopcount >= maxloop) && (maxloop >0))
            break;
        }
    }
    closesocket(conn_socket);

    //Cerramos qualquier llamada pendiente de bloqueo 
    //o asíncrona de windows sockets 
    WSACleanup();

return 0;

}