#ifndef UNISOCKET_H
#define UNISOCKET_H

    //WINDOWS COMPILATION
    #if defined(_WIN32)
        //Windows 'Winsock2' configuration setup
        #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x600
        #endif

        //Windows Headers
        #include <winsock2.h>
        #include <ws2tcpip.h>
        #pragma comment(lib, "ws2_32.lib") /* <- This has no effect when compiled with MinGW. Link with 'ws2_32.lib' if using MinGW*/

        //Windows Macros
        #define UNISOCKET_WINDOWS //Easy way to check if what platform program is compiled on

        #define ISVALIDSOCKET(s) (s != INVALID_SOCKET)
        #define GETSOCKETERRNO() WSAGetLastError()
        #define CLOSESOCKET(s) closesocket(s)
        #define UNISOCKET_CLEANUP() WSACleanup()     
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
        #define UNISOCKET_UNIX  //Easy way to check if what platform program is compiled on  

        #define SOCKET int
        #define ISVALIDSOCKET(s) (s >= 0)
        #define GETSOCKETERRNO() (errno)
        #define CLOSESOCKET(s) close(s)
        #define UNISOCKET_CLEANUP() 0
    #endif

    //Windows and Unix Macros
    #define SHUTDOWN_RD(s) shutdown(s, 0)
    #define SHUTDOWN_WR(s) shutdown(s, 1)
    #define SHUTDOWN_RDWR(s) shutdown(s, 2)
    #define SHUTDOWN(s, h) shutdown(s, h)

    /*Define 'UNISOCKET_RETURN_WSADATA' only if you wish to use the filled WSADATA struct in your program.*/
    #ifdef UNISOCKET_RETURN_WSADATA
        #define UNISOCKET_STARTUP(wsaDataAddr) unisocket_startup_return_WSADATA(wsaDataAddr)
    #else
        #define UNISOCKET_STARTUP() unisocket_startup_default()
    #endif

    /*Encapsulates Winsock2's 'WSAStartup()'. Returns '0' in for unix systems. While this function is available to use directly, 
    it is best to use the 'UNISOCKET_STARTUP(wsaDataAddr)' function macro instead. To use 'UNISOCKET_STARTUP(wsaDataAddr)', define 'UNISOCKET_RETURN_WSADATA' before including 'unisocket.h'.*/
    static inline int unisocket_startup_return_WSADATA(WSADATA *wsaData)
    {   
        #if defined _WIN32
            return WSAStartup(MAKEWORD(2, 2), wsaData);
        #else
            return 0;        
        #endif
    }

    /*Encapsulates Winsock2's 'WSAStartup()'. Returns '0' in for unix systems. While this function is available to use directly, 
    it is best to use the 'UNISOCKET_STARTUP()' function macro instead. This is the default 'UNICODE_STARTUP()' function macro defined.*/
    static inline int unisocket_startup_default(void)
    {
        #if defined _WIN32
            WSADATA wsaData;
            return WSAStartup(MAKEWORD(2, 2), &wsaData);
        #else
            return 0;        
        #endif
    }
 #endif   