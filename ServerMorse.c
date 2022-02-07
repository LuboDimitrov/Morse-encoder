
//gcc ServerMorse.c -o ServerMorse.exe -lwsock32
//gcc 8.1.0 or higher required
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_PORT 2022
#define BUFF_LEN 256

//Por defecto usamos el protocolo TCP
#define DEFAULT_PROTO SOCK_STREAM

/*
Proficiency in Morse code is measured in how many words per minute someone can send or receive. 
This seems a tricky concept given that you can send more short words per minute than long ones. 
The work-around is to standardise on the word "PARIS" as the standard word, so if Morse is being 
sent at 20 words per minute (or "20 wpm") then the word "PARIS" (or, more precisely "PARIS " with a space on the end)
could be sent 20 times in a minute.

The neat thing about "PARIS " is that it's a nice even 50 units long. It translates to ".--. .- .-. .. .../" so there are:

10 dots: 10 units;
4 dahs: 12 units;
9 intra-character spaces: 9 units;
4 inter-character spaces: 12 units;
1 word space: 7 units.
A grand total of 10+12+9+12+7=50 units.

Given this (and the fact that there are 60 seconds in a minute) we can therefore make a formula to find the length of a dot, 
tdot in seconds for a given wpm speed, 
swpm:

words per minute = swpm
minutes per word = 1/swpm
seconds per word = 60/swpm
dots per word = 50
seconds per dot = tdot = 60/50swpm

*/

//milisegundos
#define DOT 60
#define DASH 180
#define WORDS 420

SOCKET listen_socket, msgsock;

//Función que indica al usuario como ejecutar el programa
void Usage(char *progname)
{

    fprintf(stderr, "Usage: %s -p [protocol] -e [port_num] -i [ip_address]\n", progname);
    fprintf(stderr, "Where:\n\t- protocol is one of TCP or UDP\n");
    fprintf(stderr, "\t- port_num is the port to listen on\n");
    fprintf(stderr, "\t- ip_address is the ip address (in dotted\n");
    fprintf(stderr, "\t  decimal notation) to bind to.\n");
    fprintf(stderr, "\t- Hit Ctrl-C to terminate server program...\n");
    fprintf(stderr, "\t- The defaults are TCP, port 2022 and INADDR_ANY.\n");
    WSACleanup();
    exit(1);
}

//Buffer donde guardamos el mensaje codificado
char encoded [BUFF_LEN];

void pitido(int duracion){
    Beep(550,duracion);
}

void dot(){
    strcat(encoded, ".");
    pitido(DOT);
}

void dash(){
    strcat(encoded, "-");
    pitido(DASH);
}

//El espacio entre partes de una misma letra
// equivale a una unidad
void betweenParts(){
    Sleep(DOT);
}

//El espacio entre letras equivale a 3 unidades
void betweenLetters(){
    strcat(encoded, " ");
    Sleep(DASH);
}

//Función que codifica el mensaje a morse
void toMorse(char buff []){

    memset(encoded, '\0', sizeof(encoded));
    for(int i=0; i<strlen(buff); i++){
        switch (buff[i])
        {
        case 'A':
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'B':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'C':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'D':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'E':
            dot();
            betweenLetters();
            break;
        case 'F':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'G':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'H':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'I':
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'J':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'K':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'L':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'M':
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'N':
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'O':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'P':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'Q':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'R':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'S':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'T':
            dash();
            betweenLetters();
            break;
        case 'U':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'V':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'W':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case 'X':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'Y':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case 'Z':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case ' ':
            strcat(encoded, " / ");
            Sleep(WORDS);
            break;
        case '1':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '2':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '3':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '4':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '5':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '6':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '7':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '8':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '9':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '0':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '.':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case ',':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '?':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '!':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '/':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '(':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case ')':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        //En el caso del apóstrofe, no podemos poner
        //''' por eso usamos su valor en el código ASCII
        case (int)39:
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '&':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case ':':
            dash();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case ';':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '=':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '+':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '-':
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '_':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '"':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        case '$':
            dot();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenLetters();
            break;
        case '@':
            dot();
            betweenParts();
            dash();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenParts();
            dash();
            betweenParts();
            dot();
            betweenLetters();
            break;
        
        default:
            break;
        } 
    } send(msgsock, encoded, sizeof(encoded), 0);
}

int main(int argc, char **argv)

{

    char Buffer[BUFF_LEN];
    char *ip_address = NULL; //"192.168.1.43";
    unsigned short port = DEFAULT_PORT;
    int retval;
    int fromlen;
    int i;
    int socket_type = DEFAULT_PROTO;
    struct sockaddr_in local, from;

    WSADATA wsaData;

    //En el caso de que el usuario no ejecute el programa con las opciones
    //por defecto, parseamos los argumentos para asignar
    //el protocolo usado, el puerto y la IP del servidor.
    if (argc > 1)
    {
        for (i = 1; i < argc; i++)
        {
            if ((argv[i][0] == '-'))
            {
                // Convertimos los caracteres a minúscula
                switch (tolower(argv[i][1]))
                {
                    // tipo de protocolo
                case 'p':
                    if (!stricmp(argv[i + 1], "TCP"))
                        socket_type = SOCK_STREAM;
                    else if (!stricmp(argv[i + 1], "UDP"))
                        socket_type = SOCK_DGRAM;
                    else
                        Usage(argv[0]);
                    i++;
                    break;
                    // direccion IP
                case 'i':
                    ip_address = argv[++i];
                    break;
                    // numero de puerto
                case 'e':
                    port = atoi(argv[++i]);
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
        fprintf(stderr, "Server: WSAStartup() failed with error %d\n", retval);
        WSACleanup();
        return -1;
    }
    else
        printf("Server: WSAStartup() is OK.\n");
    //En caso de que el usuario no proporcione el puerto llamamos a usage() para
    //indicarle como hacerlo.
    if (port == 0)
    {
        Usage(argv[0]);
    }
    //sin family -> Familia de direcciones (TCP/IP)
    local.sin_family = AF_INET;
    //Direccion IPv4
    local.sin_addr.s_addr = (!ip_address) ? INADDR_ANY : inet_addr(ip_address);
    //Número de puerto
    local.sin_port = htons(port);

    //TCP socket
    //AF_INET para comunicaciones entre procesos que usan IPv4
    listen_socket = socket(AF_INET, socket_type, 0);
    //Tratamiento de errores
    if (listen_socket == INVALID_SOCKET)
    {
        fprintf(stderr, "Server: socket() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    else
        printf("Server: socket() is OK.\n");

    // bind() asocia una combinación de dirección local y puerto con el socket

    if (bind(listen_socket, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Server: bind() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    else
        printf("Server: bind() is OK.\n");

    //Escuchamos en el caso de que usemos TCP.

    if (socket_type != SOCK_DGRAM)
    {
        //Tratamiento de errores
        if (listen(listen_socket, 5) == SOCKET_ERROR)
        {
            fprintf(stderr, "Server: listen() failed with error %d\n", WSAGetLastError());
            WSACleanup();
            return -1;
        }
        else
            printf("Server: listen() is OK.\n");
    }
    printf("Server: I'm listening and waiting connection on port %d, protocol %s\n", port, (socket_type == SOCK_STREAM) ? "TCP" : "UDP");
  
    if (ip_address == NULL){
        printf("Server: I'm at localhost\n");
    }else{
        printf("Server: I'm at  %s\n", ip_address);
    }
    while (1)
    {
        fromlen = sizeof(from);
        //Solo usamos accept() si es TCP ya que en UDP no escuchamos
        if (socket_type != SOCK_DGRAM)
        {
            msgsock = accept(listen_socket, (struct sockaddr *)&from, &fromlen);
            printf("Server: Device with IP: %s is attempting to connect\n", inet_ntoa(from.sin_addr));
            //Tratamiento de errores
            if (msgsock == INVALID_SOCKET)
            {
                fprintf(stderr, "Server: accept() error %d\n", WSAGetLastError());
                WSACleanup();
                return -1;
            }
            else
                printf("Server: accept() is OK.\n");
                printf("Server: accepted connection from %s, port %d\n", inet_ntoa(from.sin_addr), htons(from.sin_port));
        }
        else
            msgsock = listen_socket;

  
        //En el caso de SOCK_STREAM (TCP) usamos recv y send()
        //En UDP usamos recvfrom() y sendto() en bucle

        // TCP
        if (socket_type != SOCK_DGRAM)
            retval = recv(msgsock, Buffer, sizeof(Buffer), 0);

        else
        //UDP
        {
            retval = recvfrom(msgsock, Buffer, sizeof(Buffer), 0, (struct sockaddr *)&from, &fromlen);
            printf("Server: Received datagram from %s\n", inet_ntoa(from.sin_addr));
        }
        //Tratamiendo de errores
        if (retval == SOCKET_ERROR)
        {
            fprintf(stderr, "Server: recv() failed: error %d\n", WSAGetLastError());
            closesocket(msgsock);
            continue;
        }
        else
            printf("Server: recv() is OK.\n");
        if (retval == 0)
        {
            printf("Server: Client closed connection.\n");
            closesocket(msgsock);
            continue;
        }

        printf("Server: Message received from client -> %s\n", Buffer);
        toMorse(Buffer);

        printf("Server: Sending the encoded message to client...\n");
        // TCP
        if (socket_type != SOCK_DGRAM)
            retval = send(msgsock, Buffer, sizeof(Buffer), 0);

        else
        //UDP
            retval = sendto(msgsock, Buffer, sizeof(Buffer), 0, (struct sockaddr *)&from, fromlen);

        if (retval == SOCKET_ERROR)
        {
            fprintf(stderr, "Server: send() failed: error %d\n", WSAGetLastError());
        }
        else
            printf("Server: send() is OK.\n");
        //TCP
        if (socket_type != SOCK_DGRAM)
        {
            printf("Server: I'm waiting more connections...\n");
            closesocket(msgsock);
        }

        else
        //UDP
            printf("Server: UDP server looping back for more requests\n");

        continue;
    }

    return 0;
}