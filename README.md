# UNISOCKET

A lightweight, single-header library that handles cross-platform (Windows, MacOS, Linux) *socket programming* in C.

This API was inspired by [Hands-On Network Programming with C](https://www.amazon.com/Hands-Network-Programming-programming-optimized/dp/1789349869?source=ps-sl-shoppingads-lpcontext&ref_=fplfs&psc=1&smid=ATVPDKIKX0DER) . This API uses `Winsock2` and `UNIX` sockets (Berkley Sockets); abstracting away their differences, while still retaining full functionality.

## Include Headers

To use the API include `unisocket.h`
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

## Tutorial

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

The `socket(), bind(), send/to(), recv/from() getaddrinfo(), connect(), accept()`, the `FD_SET` functions, and other socket functions are the same for both `Winsock2` and `UNIX` sockets.

### Error Checking

To check for socket-related errors, use the function macro defined below:

```C
int error = GETSOCKETERRNO();
```
