#include <netinet/in.h> // Structure for storing address information 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h> // For socket APIs 
#include <sys/types.h> 
#include <unistd.h>     // For close()
#include "list.h"

#define PORT 9001
#define ACK "ACK"
      
int main(int argc, char const* argv[]) 
{ 
    int n, val, idx;
    // Create server socket 
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 

    // Buffers for receiving and sending data 
    char buf[1024];
    char sbuf[1024];
    char* token;

    // Define server address 
    struct sockaddr_in servAddr; 

    // Linked list 
    list_t *mylist;

    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 

    // Bind socket to the specified IP and port 
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections 
    if (listen(servSockD, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept a client socket 
    int clientSocket = accept(servSockD, NULL, NULL); 
    if (clientSocket < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Create the linked list 
    mylist = list_alloc();

    while(1){
        // Receive messages from client socket 
        n = recv(clientSocket, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            perror("recv failed or connection closed");
            break;
        }
        buf[n] = '\0';
        memset(sbuf, '\0', sizeof(sbuf)); // Clear the send buffer

        // Tokenize the input command 
        token = strtok(buf, " ");
        
        if(strcmp(token,"exit") == 0){
            list_free(mylist);
            snprintf(sbuf, sizeof(sbuf), "Exiting...");
            send(clientSocket, sbuf, strlen(sbuf), 0);
            close(clientSocket);
            close(servSockD);
            exit(0);
        }
        else if(strcmp(token,"get_length") == 0){
            // Call list_length() 
            val = list_length(mylist);
            snprintf(sbuf, sizeof(sbuf), "Length = %d", val);
        }
        else if(strcmp(token,"add_front") == 0){
            token = strtok(NULL, " ");  // Get the value 
            if (token != NULL) {
                val = atoi(token);
                list_add_to_front(mylist,val);
                snprintf(sbuf, sizeof(sbuf), "%s", ACK);
            } else {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing value");
            }
        }
        else if(strcmp(token,"add_back") == 0){
            token = strtok(NULL, " ");  // Get the value 
            if (token != NULL) {
                val = atoi(token);
                list_add_to_back(mylist,val);
                snprintf(sbuf, sizeof(sbuf), "%s", ACK);
            } else {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing value");
            }
        }
        else if(strcmp(token,"add_position") == 0){
            token = strtok(NULL, " ");  // Get the index 
            if (token != NULL) {
                idx = atoi(token);
                token = strtok(NULL, " ");  // Get the value 
                if (token != NULL) {
                    val = atoi(token);
                    list_add_at_index(mylist,val, idx);
                    snprintf(sbuf, sizeof(sbuf), "%s", ACK);
                } else {
                    snprintf(sbuf, sizeof(sbuf), "Error: Missing value");
                }
            } else {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing index");
            }
        }
        else if(strcmp(token,"remove_back") == 0){
            // Call list_remove_from_back() 
            val = list_remove_from_back(mylist);
            if (val != -1)
                snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
            else
                snprintf(sbuf, sizeof(sbuf), "Error: List is empty");
        }
        else if(strcmp(token,"remove_front") == 0){
            // Call list_remove_from_front() 
            val = list_remove_from_front(mylist);
            if (val != -1)
                snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
            else
                snprintf(sbuf, sizeof(sbuf), "Error: List is empty");
        }
        else if(strcmp(token,"remove_position") == 0){
            token = strtok(NULL, " ");
            if (token != NULL) {
                idx = atoi(token);
                // Call list_remove_at_index() 
                val = list_remove_at_index(mylist,idx);
                if (val != -1)
                    snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
                else
                    snprintf(sbuf, sizeof(sbuf), "Error: Invalid index");
            } else {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing index");
            }
        }
        else if(strcmp(token,"get") == 0){
            token = strtok(NULL, " ");  // Get the index 
            if (token != NULL) {
                idx = atoi(token);
                // Call list_get_elem_at() 
                val = list_get_elem_at(mylist, idx);
                if (val != -1)
                    snprintf(sbuf, sizeof(sbuf), "Value = %d", val);
                else
                    snprintf(sbuf, sizeof(sbuf), "Error: Invalid index");
            } else {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing index");
            }
        }
        else if(strcmp(token,"print") == 0){
            // Call listToString() 
            char *list_str = listToString(mylist);
            snprintf(sbuf, sizeof(sbuf), "%s", list_str);
            free(list_str); // Free the allocated memory
        }
        else {
            snprintf(sbuf, sizeof(sbuf), "Invalid command");
        }

        // Send response to client socket 
        send(clientSocket, sbuf, strlen(sbuf), 0);

        memset(buf, '\0', sizeof(buf)); // Clear the receive buffer
    }

    // Clean up in case of loop exit 
    list_free(mylist);
    close(clientSocket);
    close(servSockD);

    return 0; 
}
