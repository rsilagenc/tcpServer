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
    // new_socket is the file descriptor for the new socket
    // valread is the number of bytes read by the read() system call
    int server_fd, new_socket, valread;
    
    // struct sockaddr_in is a structure that contains an internet address
    struct sockaddr_in address;
    
    // opt is used to set the socket options
    int opt = 1;
    
    // addrlen is the size of the address structure
    int addrlen = sizeof(address);
    

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    
    {
        std::cerr << "Socket failed" << std::endl;
        return 1;
    }
    std::cout << "Socket is created" << std::endl;

    // from address structure, set the address family to AF_INET
    address.sin_family = AF_INET;
    // from address structure, set the address to INADDR_ANY that means any adress for socket binding
    address.sin_addr.s_addr = INADDR_ANY;
    
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
   
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        throw std::runtime_error("Bind failed");
    }
    std::cout << "Binding" << std::endl;
    

    if (listen(server_fd, SOMAXCONN) == -1)
   
    {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;

        /*this is a throw exception. this exception will propagate up the call stack
        until it's caught by a 'catch' block. if it's not caught, the program will
        terminate*/
        throw std::runtime_error("Listen failed");
        
    }
    std::cout << "Listening" << std::endl;
    

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  
    {
        throw std::runtime_error("Accept failed");
    }

    std::cout << "Accepted" << new_socket<< std::endl;

   
    // The string "server.log" passed to the std::ofstream constructor is the name of the file to be opened.
    // This file will be created if it does not already exist.
    // If it does exist, the existing content will be deleted because by default std::ofstream opens files in overwrite mode.
    // so, we add the 'app' part because app means append mode
    // and in the append mode the file won't be overwrited
    // new data will be written to the end of the file
    std::ofstream logFile("server.log", std::ios::app);
    logFile << "Client connected" << std::endl;

     // it creates a buffer that has a size of 1024 characters and initialize them to zero.
    //  {0} means initializer list. so in this code that means before reading any data my buffer will be null.
    // because compiler will initialize the array elements with values in the list. in this case it's zero.
    // so that means the array's all elements are zero. in char a zero is equivalent to null character '\0'
    // and this is used for marking end of the string in c++

    // so, this code creates a buffer that is filled with null characters.
    char bufferRead[1024] = {0};
        
    // we passed the read function to bytesRead function and now bytesRead is read's return value
    // on success, the number of bytes read is returned
    ssize_t bytesRead;

    while (true)
    {
        
        bytesRead = recv(new_socket, bufferRead, sizeof(bufferRead) - 1, 0);
        if (bytesRead == -1)
        {
        throw std::runtime_error("Read failed");
        }

        // this is used for null-terminate the string in the buffer
        // so, we can determine where the string ends
        bufferRead[bytesRead] = '\0';
        std::cout << "Received: " << bufferRead << std::endl;
        break;
        
    }


std::vector<int>receivedNumbers;
std::vector<int>sentNumbers;


for (int i = 0; i < 10; ++i)
{
receivedNumbers.push_back(bufferRead[i] - 48);
}

std::cout << "print: ";
for (int i = 0; i < receivedNumbers.size(); ++i)
{
    std::cout << receivedNumbers[i] << " ";
}

for (int j = 0; j < 10; ++j)
{
    sentNumbers.push_back(j);
}
    std::cout << "print j: ";
    for (int j = 0; j < sentNumbers.size(); ++j)
    {
        std::cout << sentNumbers[j] << " ";
    }


std::cout<< " " << std::endl;

std::vector<int> diffVector;
for (int diff : sentNumbers)
{
    if(std::find(receivedNumbers.begin(), receivedNumbers.end(), diff) == receivedNumbers.end())
    {
        diffVector.push_back(diff);
    }
    std::cout << " " << std::endl;
}
int bufferSent[1024];
std::copy(diffVector.begin(), diffVector.end(), bufferSent);

send(new_socket, bufferSent, sizeof(int) * diffVector.size(), 0);
for (size_t i = 0; i < diffVector.size(); ++i) {
    std::cout << bufferSent[i] << " ";
}
std::cout << std::endl;

      

    char bufferWrite[1024] = {0};
    ssize_t bytesWritten;

    
        bytesWritten = write(new_socket, bufferWrite, sizeof(bufferWrite));

        if (bytesWritten == -1)
        {
        throw std::runtime_error("Writing failed");
        }

    bufferWrite[bytesWritten] = '\0';
    std::cout << "Written: " << bufferWrite << std::endl;


    
    
    close(server_fd);

    return 0;
}