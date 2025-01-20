#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../lib/decoder.h"

#pragma comment(lib, "ws2_32.lib")

int setup(WSADATA *wsa, SOCKET *server_socket, SOCKET *client_socket, struct sockaddr_in *server, struct sockaddr_in *client);
int loop(); 
int decode_main();

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int recv_size;
    char *message = malloc(sizeof(char) * BUFFER_SIZE);
    char *reply = malloc(sizeof(char) * BUFFER_SIZE);
    int phase = 0; 

    setup(&wsa, &server_socket, &client_socket, &server, &client);
    
    while (1) {
        // Listen to incoming connections
        listen(server_socket, 3);
        printf("%s\n", SERVER_IP); 
        // Accept and incoming connection
        printf("Waiting for incoming connections...\n");
        int c = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
        if (client_socket == INVALID_SOCKET) {
            printf("accept failed with error code : %d", WSAGetLastError());
            return 1;
        }
        printf("Connection accepted.\n");


        char *filename = malloc(sizeof(char) * 256);
        sprintf(filename, "%s%s", RECEIVE_PATH, "Real_Compressed.bin");

        int check = 0; 
        FILE *receive_file = fopen(filename, "wb");
        fclose(receive_file);
        
        while (recv_size = recv(client_socket, (char*)message, BUFFER_SIZE, 0)) {
            // Receive data from client
            if (recv_size == SOCKET_ERROR) {
                break; 
                printf("recv failed with error code : %d", WSAGetLastError());
            }

            if (message[0] == START_DECODE) {
                phase = 0; 
                sprintf(reply, "RECEIVED ALL DATA\n");
                send(client_socket, reply, strlen(reply), 0);
                break; 
            }
            
            // Print the received data 
            for (int i = 0; i < recv_size; i++) {
                printf("%d)%d, ",i , message[i]);
                check++;
            }
            printf("\nmem: %d\n", check * sizeof(char));// Track the number of element received

            // Write the data into a binary file
            FILE *receive_file = fopen(filename, "ab");
            printf("Wrote: %d\n", fwrite(message, sizeof(char), recv_size, receive_file));
            fclose(receive_file);

            phase++;// Value for reply message to client

            // Reply to client
            sprintf(reply, "Phase %d successfull\n", phase);
            send(client_socket, reply, strlen(reply), 0);

            check = 0; 
        }

        printf("Decoding\n");
        decode_main();

        free(filename);
    } 

    // Clean up and close the connection
    free(message);
    free(reply); 
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}

int setup(WSADATA *wsa, SOCKET *server_socket, SOCKET *client_socket, struct sockaddr_in *server, struct sockaddr_in *client) {
    int c; 

    // Initialize Winsock
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2),wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // Create a socket
    if ((*server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = INADDR_ANY;
    server->sin_port = htons(PORT);

    // Bind
    if (bind(*server_socket, (struct sockaddr *)server, sizeof(*server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }
    printf("Bind done.\n");

}

int decode_main() {
    char *filename = malloc(sizeof(char) * 256);

    sprintf(filename, "%s%s", RECEIVE_PATH, "Real_Compressed.bin");
    FILE *encoded_file = fopen(filename, "rb");
    char *data; 
    float *freq; 
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);

    int size = Extract_Huffman_code(encoded_file, &data, &freq); // Extract information to Buil Decode tree

    for (int i = 0; i < size; i++) {
        printf("%d) %c: %f\n", i, data[i], freq[i]);
    }

    struct decNode *root = Build_Huffman_Tree(data, freq, size);// Build the HUFFMAN tree to decode

    // Write the Huffman codes to a file txt 
    char *display = malloc(sizeof(char) * (size - 1));
    char *code[256];
    for (int i = 0; i < 256; i++) code[i] = malloc(sizeof(char) * (size - 1));
    sprintf(filename, "%s%s", RECEIVE_PATH, "Code.txt"); 
    FILE *code_file = fopen(filename, "w");
    printCodes(code_file, root, display, 0, code);
    fclose(code_file);

    // Write the content to a file txt 
    sprintf(filename, "%s%s", RECEIVE_PATH, "Binary_Compressed.txt");
    FILE *binary_txt_file = fopen(filename, "w");
    size_t element_read; 
    printf("\nTranslate Message\n\n");
    
    while (element_read = fread(buffer, sizeof(char), 1, encoded_file)) {
        printf("%c", buffer[0]);
        Convert2binary(buffer[0], &buffer); 
        fprintf(binary_txt_file, "%s", buffer);
    }
    printf("\nFinish Translate\n\n");

    fclose(binary_txt_file);

    // Decode the compressed file
    printf("\n");
    sprintf(filename, "%s%s", RECEIVE_PATH, "Binary_Compressed.txt");
    binary_txt_file = fopen(filename, "r");
    decode_file_text(binary_txt_file, root);
    fclose(binary_txt_file);

    free(buffer); 
    free_tree(root); 
}