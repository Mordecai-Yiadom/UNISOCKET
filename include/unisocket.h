#ifndef UNISOCKET_H
#define UNISOCKET_H

#ifdef __cplusplus
extern "C"
{
#endif

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
        #ifdef _MSC_VER
            #pragma comment(lib, "ws2_32.lib")
        #endif   
    
        //Windows Macros
 
        /*Defined only if compiled on Windows systems. Use this to check what if you are compiling on Windows or not.*/
        #define UNISOCKET_WINDOWS

        /*Abstracts away Windows's 'INVALID_SOCKET' macro for sockets. On Windows, this macro evaluates to true if: 's != INVALID_SOCKET'.
        On Unix systems, this macro evaluates true if: 's >= 0' */
        #define ISVALIDSOCKET(s) (s != INVALID_SOCKET)

        /*Absracts away Windows's 'WSAGetLastError()'. On Unix systems this macro abstracts 'errno'.*/
        #define GETSOCKETERRNO() WSAGetLastError()

        /*Abstracts away Windows's 'closesocket()'. On Unix systems, this macro absracts the socket operation 'close()'*/
        #define CLOSESOCKET(s) closesocket(s)

        /*Abstracts Windows's 'WSACleanup()'. Returns '0' in for Unix systems. While this function is available to use directly, 
        it is best to use the 'UNISOCKET_CLEANUP()' function macro instead. This is the default 'UNISOCKET_CLEANUP()' function macro defined.*/
        #define UNISOCKET_CLEANUP() WSACleanup()

        /*Define 'UNISOCKET_RETURN_WSADATA' only if you wish to use the filled WSADATA struct in your program.*/
        #ifdef UNISOCKET_RETURN_WSADATA
            #define UNISOCKET_STARTUP(wsaDataAddr) WSAStartup(MAKEWORD(2, 2), wsaDataAddr)
        #else
            /*Abstracts Windows's 'WSAStartup()' function. This must be called before any socket operations happen.*/
            #define UNISOCKET_STARTUP() UNISOCKET_WSAStartup()
        #endif

        //Windows Functions

        /*Abstracts Windows's 'WSAStartup()'. Returns '0' in for Unix systems. While this function is available to use directly, 
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

        /*Defined only if compiled on Unix systems. Use this to check what if you are compiling on a Unix system or not.*/ 
        #define UNISOCKET_UNIX   

        /*Abstracts the 'SOCKET' typedef on Windows. SOCKET when on compiling on Unix is a signed int.*/
        #define SOCKET int

        /*Abstracts away Windows's 'INVALID_SOCKET' macro for sockets. On Windows, this macro evaluates to true if: 's != INVALID_SOCKET'.
        On Unix systems, this macro evaluates true if: 's >= 0' */
        #define ISVALIDSOCKET(s) (s >= 0)

        /*Absracts away Windows's 'WSAGetLastError()'. On Unix systems this macro abstracts 'errno'.*/
        #define GETSOCKETERRNO() (errno)

        /*Abstracts away Windows's 'closesocket()'. On Unix systems, this macro absracts the socket operation 'close()'*/
        #define CLOSESOCKET(s) close(s)

        /*Abstracts Windows's 'WSAStartup()'. Returns '0' in for Unix systems. While this function is available to use directly, 
        it is best to use the 'UNISOCKET_STARTUP()' function macro instead. This is the default 'UNISOCKET_STARTUP()' function macro defined.*/
        #define UNISOCKET_STARTUP() 0

        /*Abstracts Windows's 'WSACleanup()'. Returns '0' in for Unix systems. While this function is available to use directly, 
        it is best to use the 'UNISOCKET_CLEANUP()' function macro instead. This is the default 'UNISOCKET_CLEANUP()' function macro defined.*/
        #define UNISOCKET_CLEANUP() 0
    #endif

    //Windows and Unix Macros

    /*Abstracts 'shutdown()' for both Windows and Unix systems. It closes the passed socket for reading. */
    #define SHUTDOWN_RD(s) shutdown(s, 0)

    /*Abstracts 'shutdown()' for both Windows and Unix systems. It closes the passed socket for writing. */
    #define SHUTDOWN_WR(s) shutdown(s, 1)

    /*Abstracts 'shutdown()' for both Windows and Unix systems. It closes the passed socket for reading and writing. */
    #define SHUTDOWN_RDWR(s) shutdown(s, 2)

#ifdef __cplusplus
    }
#endif

#endif   