# Syncronous client-server application

## Description

Program is written in C++. It consists of two programs, the interaction of programs implemented through the use of Boost Asio Library sockets. Thread interaction is synchronized, [thread #2]() does not poll the shared buffer constantly. The synchronization mechanism is not a global variable. The operation of [program #1]() is as independent as possible from the launch status of [program #2](). This means that the sudden shutdown of program #2 does not lead to immediate input problems for the user. When restarting program #2, reconnection is performed. The operation of program #2 is as independent as possible from the launch status of program #2. The sudden shutdown of program #1 does not lead to immediate display problems. Waiting for the connection of program #2 is performed when the connection between the programs is lost.

### Client side

#### 1st thread

Accepts the string that the user enters. Implemented a check that the string consists only of digits and does not exceed 64 characters. After checking, the string is sorted in descending order and all elements whose value is even are replaced with Latin letters "KB". After that, this line is placed in the shared buffer and the stream should wait for further user input.

#### 2nd thread

Processes data that is placed in a shared buffer. After receiving the data, the shared buffer is overwritten. The stream displays the received data on the screen, the total sum of all elements, which are numerical values, is calculated. The amount received is transferred to the [server side](). After that, the thread waits for the following data.

### Server side

Waits for data from the [client side](). When data is received, an analysis is performed of how many characters the transmitted value consists of. If it is more than 2 characters and if it is a multiple of 32, it displays a message about the received data, otherwise an error message is displayed. Then the program continues to wait for data.

## Dependencies

- [C++20](https://en.cppreference.com/w/cpp/20) - Minimum C++ 20 Standard required
- [Boost Asio Library](https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio.html) - Minimum 1.78 version required
- [CMake](https://cmake.org/) - Minimum 3.25 version required

## How to compile

For compile this program, you need to execute following commands in the terminal:

```console
cmake .
cmake --build .
```

Then, you will get smth same:
![](img/1.png)
