#ifndef UNISOCKET_H
#define UNISOCKET_H

    //WINDOWS COMPILATION
    #if defined(_WIN32)
    
        //Windows 'winsock2.h' configuration
        #ifndef _WIN32_WINNT
            #define _WIN32_WINNT 0x600
        #endif

        //Windows Headers
        #define WIN32_LEAN_AND_MEAN
        #include <winsock2.h>
        #include <ws2tcpip.h>

        //Only available if compiling with MSVC
        #ifdef _MSC_BUILD
            #pragma comment(lib, "ws2_32.lib")
        #endif   

        //Windows Macros
        #define UNISOCKET_WINDOWS //Easy way to check what platform program is compiled on

        #define ISVALIDSOCKET(s) (s != INVALID_SOCKET)
        #define GETSOCKETERRNO() WSAGetLastError()
        #define CLOSESOCKET(s) closesocket(s)
        #define UNISOCKET_CLEANUP() WSACleanup()

        /*Define 'UNISOCKET_RETURN_WSADATA' only if you wish to use the filled WSADATA struct in your program.*/
        #ifdef UNISOCKET_RETURN_WSADATA
            #define UNISOCKET_STARTUP(wsaDataAddr) WSAStartup(MAKEWORD(2, 2), wsaDataAddr)
        #else
            #define UNISOCKET_STARTUP() UNISOCKET_WSAStartup()
        #endif

        //Windows Functions

        /*Encapsulates Winsock2's 'WSAStartup()'. Returns '0' in for unix systems. While this function is available to use directly, 
        it is best to use the 'UNISOCKET_STARTUP()' function macro instead. This is the default 'UNISOCKET_STARTUP()' function macro defined.*/
        static inline int UNISOCKET_WSAStartup(void)
        {
            WSADATA wsaData;
            return WSAStartup(MAKEWORD(2, 2), &wsaData);
        }

    //UNIX COMPILATION    
    #else
        //Unix Headers
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <unistd.h>
        #include <errno.h>

        //Unix Macros
        #define UNISOCKET_UNIX  //Easy way to check what platform program is compiled on  

        #define SOCKET int
        #define ISVALIDSOCKET(s) (s >= 0)
        #define GETSOCKETERRNO() (errno)
        #define CLOSESOCKET(s) close(s)
        #define UNISOCKET_STARTUP() 0
        #define UNISOCKET_CLEANUP() 0
    #endif

    //Windows and Unix Macros
    #define SHUTDOWN_RD(s) shutdown(s, 0)
    #define SHUTDOWN_WR(s) shutdown(s, 1)
    #define SHUTDOWN_RDWR(s) shutdown(s, 2)
    #define SHUTDOWN(s, h) shutdown(s, h)
 #endif   