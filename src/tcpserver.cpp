#include <iostream>     // for input/output
#include <string.h>     // for string manipulation functions
#include <sys/socket.h> // for socket programming
#include <arpa/inet.h>  // for internet operations
#include <unistd.h>     // for close function
#include <fstream>      //for logging
#include <vector>       //for vector
#include <algorithm>    //for finding

#define PORT 8080

int main()
{
    // server_fd is the file descriptor for the server socket
    // socket_fd is the file descriptor for the new socket
    int server_fd, socket_fd;

    // struct sockaddr_in is a structure that contains an internet address
    struct sockaddr_in address;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw std::runtime_error("Socket failed" + std::string(strerror(errno)));
    }
    std::cout << "Socket is created" << std::endl;

    // from address structure, set the address family to AF_INET
    address.sin_family = AF_INET;
    // from address structure, set the address to INADDR_ANY that means any adress for socket binding
    address.sin_addr.s_addr = INADDR_ANY;

    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)

    {
        throw std::runtime_error("Bind failed" + std::string(strerror(errno)));
    }
    std::cout << "Binding" << std::endl;

    if (listen(server_fd, SOMAXCONN) == -1)

    {
        /*this is a throw exception. this exception will propagate up the call stack
        until it's caught by a 'catch' block. if it's not caught, the program will
        terminate*/
        throw std::runtime_error("Listen failed" + std::string(strerror(errno)));
    }
    std::cout << "Listening" << std::endl;

    // addrlen is the size of the address structure
    int addrlen = 0;
    if ((socket_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)

    {
        throw std::runtime_error("Accept failed" + std::string(strerror(errno)));
    }

    std::cout << "Accepted" << socket_fd << std::endl;

    // The string "server.log" passed to the std::ofstream constructor is the name of the file to be opened.
    // This file will be created if it does not already exist.
    // If it does exist, the existing content will be deleted because by default std::ofstream opens files in overwrite mode.
    // so, we add the 'app' part because app means append mode
    // and in the append mode the file won't be overwrited
    // new data will be written to the end of the file
    std::ofstream logFile("server.log", std::ios::app);
    logFile << "Client connected" << std::endl;

    //  {0} means initializer list. so in this code that means before reading any data my buffer will be null.
    // so, this code creates a buffer that is filled with null characters.
    char bufferRead[1024] = {0};

    // we passed the read function to bytesRead function and now bytesRead is read's return value
    // on success, the number of bytes read is returned
    ssize_t bytesRead = recv(socket_fd, bufferRead, sizeof(bufferRead) - 1, 0);
    if (bytesRead < 0)
    {
        throw std::runtime_error("Read failed" + std::string(strerror(errno)));
    }

    // this is used for null-terminate the string in the buffer
    // so, we can determine where the string ends
    bufferRead[bytesRead] = '\0';
    std::cout << "Received: " << bufferRead << std::endl;

    std::vector<int> receivedNumbers;

    for (int i = 0; i < 10; ++i)
    {
        receivedNumbers.push_back(bufferRead[i] - 48);
    }
    /* for printing out receivedNumbers vector
     * std::cout << "print: ";
     * for (int i = 0; i < receivedNumbers.size(); ++i)
     *   {
     *   std::cout << receivedNumbers[i] << " ";
     *   }
     */
    std::vector<int> sentNumbers;

    for (int j = 0; j < 10; ++j)
    {
        sentNumbers.push_back(j);
    }
    /* for printing out sentNumbers vector
     *
     * std::cout << "print j: ";
     *  for (int j = 0; j < sentNumbers.size(); ++j)
     *  {
     *     std::cout << sentNumbers[j] << " ";
     *  }
     * std::cout<< " " << std::endl;
     */

    /*
     * for printing out the different elements between receivedNumbers and sentNumbers vectors
     * we defined the diffVector vector
     */
    std::vector<int> diffVector;
    for (int diff : sentNumbers)
    {
        if (std::find(receivedNumbers.begin(), receivedNumbers.end(), diff) == receivedNumbers.end())
        {
            diffVector.push_back(diff);
        }
        std::cout << " " << std::endl;
    }
    /*
     * we defined bufferSent array for sending the values to socket_fd
     */
    int bufferSent[1024];
    std::copy(diffVector.begin(), diffVector.end(), bufferSent);
    send(socket_fd, bufferSent, sizeof(int) * diffVector.size(), 0);
    for (int i = 0; i < diffVector.size(); ++i)
    {
        std::cout << bufferSent[i] << " ";
    }
    std::cout << std::endl;

    char bufferWrite[1024] = {0};
    ssize_t bytesWritten = write(socket_fd, bufferWrite, sizeof(bufferWrite));

    if (bytesWritten < 0)
    {
        throw std::runtime_error("Writing failed" + std::string(strerror(errno)));
    }

    bufferWrite[bytesWritten] = '\0';
    std::cout << "Written: " << bufferWrite << std::endl;

    close(server_fd);

    return 0;
}