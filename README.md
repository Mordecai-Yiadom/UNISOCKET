# UNISOCKET

A lightweight, convenient single-header library that handles cross-platform (Windows, MacOS, Linux) *socket programming* in C.

This API was inspired by [Hands-On Network Programming with C](https://www.amazon.com/Hands-Network-Programming-programming-optimized/dp/1789349869?source=ps-sl-shoppingads-lpcontext&ref_=fplfs&psc=1&smid=ATVPDKIKX0DER) . This API uses [`Winsock2`](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/) and [`UNIX`](https://man7.org/linux/man-pages/man0/sys_socket.h.0p.html) sockets (Berkley Sockets); abstracting away their differences, while still retaining full functionality.

This library does NOT replace or modify functionality of the underlying APIs. It only conveniently abstracts away syntaxtic differences from them to produce write-once-run-everywhere source code.

## Include Headers

To use the API include [`unisocket.h`](include/unisocket.h)
```C
#include "unisocket.h"
```
And that's it!


## Compiling

**IMPORTANT NOTE**: Linking is only required if you are compiling on Windows.

### Microsoft Visual C/C++
If you are using Microsoft Visual C/C++, there is not linking needed.

### MinGW
If you are using MinGW to compile, then you must link with `libws2_32.a`

```powershell
gcc source.c -o myProgram.exe -lws2_32
```

## Tutorial: *How to Use UNISOCKET*

### Initalizing UNISOCKET

Initializing is only necessary if compiling on Windows, however for the sake of cross-platform compatablilty, it is best practice to do the following before using any socket functions:

```C
//Must be called before any socket functions are called if on Windows
UNISOCKET_STARTUP();
```
This function macro encapsulates [`Winsock2`](https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2)'s [`WSAStartUp()`](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup) .


If you wish to retain the [`WSADATA`](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/ns-winsock2-wsadata) struct for your program's use then define the macro below:

```C
//'UNISOCKET_RETURN_WSADATA' be defined before including "unisocket.h"
#define UNISOCKET_RETURN_WSADATA
#include "unisocket.h"
```
And then pass in the address of your `WSADATA` struct
```C
WSADATA data;
UNISOCKET_STARTUP(&data);
```

Both macros returns `0` on success. However `UNISOCKET_STARTUP(&data)` returns specific `int` errors upon failing as documented in the [Winsock2 API](https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2) .

### Creating Sockets

To create a socket file descriptor, use the data type `SOCKET`, which is defined
```C
SOCKET socket = socket(...);
```

To check if a socket is valid, it is best to use the function macro showed below:
```C
if(ISVALIDSOCKET(socket)){
    puts("Succesfully created socket!");
}
```
`ISVALIDSOCKET` returns `1` if:

 `socket != INVALID_SOCKET` (Windows) 

OR 

`socket >= 0` (UNIX).

### Closing Sockets

To close sockets, use the function macro shown below:
```C
CLOSESOCKET(socket);
```

If you want to control how a socket closes, you may want to use `shutdown()` defined in both [`Winsock2`](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-shutdown) and [`UNIX`](https://man7.org/linux/man-pages/man2/shutdown.2.html). The function macros that encapsulate them are listed below:
```C
//Closes socket from reciving packets
SHUTDOWN_RD(socket);

//Closes socket from sending packets
SHUTDOWN_WR(socket);

//Closes socket from reciving and sending packets
SHUTDOWN_RDWR(socket);
```
You are highly encouraged to use the `SHUTDOWN_*()` function macros above for the sake of cross-platform compatibility. 

### Querying the Operating System

For your program to know what Operating System it is executing on during runtime, or even compile time, it is best to use the macros dynamically defined in `unisocket.h` as shown below:
```C
#ifdef UNISOCKET_WINDOWS
    windowsRelatedFunction();
#endif

#ifdef UNISOCKET_UNIX
    unixRelatedFunction();
#endif    
```
`UNISOCKET_WINDOWS` is defined if the program is compiled in Windows

`UNISOCKET_UNIX` is defined if a the program is compiled in UNIX (Linux, MacOS)


### Error Checking

To check for socket-related errors, use the function macro defined below:

```C
int error = GETSOCKETERRNO();
```
Because error codes are different per operating system, it is best to use `UNSOCKET_WINDOWS` or `UNISOCKET_UNIX` defined in `unisocket.h` to handle errors as shown below:
```C
#ifdef UNISOCKET_WINDOWS
    handleWindowsErrors(error);
#endif

#ifdef UNISOCKET_UNIX
    handleUnixErrors(error);
#endif
```

### Terminating UNISOCKET

Before exiting your program, it is best practice to call the function macro `UNISOCKET_CLEANUP()`, defined in `unisocket.h`. This encapsulates `Winsock2`'s [`WSACleanup()`](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsacleanup).

For the sake of cross-platform compatibility, it is best practice to use the following function macro when you are finished using sockets in your program:
```C
#include "unisocket.h"

int main()
{
    UNISOCKET_STARTUP();
    ...
    /*Call when finished with socket operations, 
    OR at the end of your program*/
    UNISOCKET_CLEANUP();
    return 0;
}
```
`UNISOCK_CLEANUP()` returns `0` on success. For UNIX, this will always return `0`. 

For Windows, `UNISOCK_CLEANUP()` returns some `int` called `SOCKET_ERROR` defined in `winsock2.h`. The error can be queried using the function macro `GETSOCKETERRNO()` defined in `unisocket.h`
