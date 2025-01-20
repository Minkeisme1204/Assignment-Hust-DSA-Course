#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../lib/encoder.h"
#include "../lib/read_text.h"
#include <math.h>

#pragma comment(lib, "ws2_32.lib")

int setup(WSADATA *wsa, SOCKET *server_socket, struct sockaddr_in *server);
int send_bin_file(FILE *compressed_file, char *message, char *reply, SOCKET s);

int main() {    
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message = malloc(sizeof(char) * (BUFFER_SIZE));
    char *server_reply = malloc(sizeof(char) * BUFFER_SIZE); 
    int recv_size;

    setup(&wsa, &s, &server); 

    char file[50];
    printf("Enter the name of the txt file in \"asset\" folder for sending: "); 

    fgets(file, 50, stdin);
    file[strlen(file)-1] = '\0';
    printf("%s\n", file);
    char *filename = malloc(sizeof(char) * 256); 
    sprintf(filename, "%s%s", ASSET_PATH, file);
    if(strcmp(file, "cancel") == 0) return 0;
    
    char *data; 
    float *freq;
    int *present;
    char *code[256]; 

    int size = process_text(filename, &data, &freq, &present);

    for (int i = 0; i < size; i++) {
        printf("%c : %f\n", data[i], freq[i]);
    }

    struct Node *root = Build_Huffman_Tree(data, freq, size);

    printf("Number of char in txt file: %d\n", size);

    char *display = malloc(sizeof(char) * (size - 1));
    for (int i = 0; i < 256; i++) code[i] = malloc(sizeof(char) * (size - 1));

    sprintf(filename, "%s%s", SEND_PATH, "Huffman Dictionary.txt");
    FILE *code_file = fopen(filename, "w");
    FILE *stat_file = fopen("Stats of Huffman Code.txt", "w");
    fprintf(stat_file, "Char\t\tFreqency/Probability\tCodeword\t\tLength\n");

    // Statistics Calculation
    int len = 0; 
    printCodes(code_file, stat_file, root, display, 0, code, &len);
    size--;

    double sum_entropy = 0; 
    double max_entropy = log(size) / log(2.0);

    for (int i = 0; i < size; i++) {
        double entropy = freq[i] * (log(1.0/freq[i]) / log(2));
        sum_entropy += entropy;
    }

    double avarage_len = len / (double)size;
    fprintf(stat_file, "\nNumber of chars in text: %d char\nThe END SIGN is auto padded for DECODE PROCESS\n", size);
    fprintf(stat_file, "Sum of length: %d | Avarage length: %lf | Entropy: %lf | Max entropy: %lf\n", len, avarage_len, sum_entropy, max_entropy);

    double Scompression_ratio = sum_entropy / avarage_len;
    fprintf(stat_file, "Statistic Compression ratio: %lf\n", Scompression_ratio);

    double Ocompression_ratio = max_entropy / avarage_len; 
    fprintf(stat_file, "Optimal Compression ratio: %lf\n\n\n", Ocompression_ratio);
    fclose(code_file);
    fclose(stat_file);
    size++;
    //

    sprintf(filename, "%s%s", ASSET_PATH, file);
    encode_file_text(fopen(filename, "r"), code, data, freq, size);

    free(display);
    free(data);
    free(freq);
    free(present);
    free_tree(root);
    
    printf("Done Encoder! %c\n", 10);
    // printf("%s", filename);
    // Send data to server
    sprintf(filename, "%s%s", SEND_PATH, "Real_Compressed.bin"); 
    
    FILE *compressed_file = fopen(filename, "rb"); 
    fseek(compressed_file, 0, SEEK_SET);

    send_bin_file(compressed_file, message, server_reply, s); 

    closesocket(s);
    WSACleanup();
    return 0;
}

int setup(WSADATA *wsa, SOCKET *server_socket, struct sockaddr_in *server) {
    // Initialize Winsock
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2),wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // Create a socket
    if ((*server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    // Define server address and port
    printf("%s\n", SERVER_IP);
    server->sin_addr.s_addr = inet_addr(SERVER_IP); // IP address of the server
    server->sin_family = AF_INET;
    server->sin_port = htons(PORT);

    // Connect to server
    if (connect(*server_socket, (struct sockaddr *)server, sizeof(*server)) < 0) {
        printf("Connect error\n");
        return 1;
    }
    printf("Connected to server.\n");
}

int send_bin_file(FILE *compressed_file, char *message, char *server_reply, SOCKET s) {
    int memory = 0;  
    size_t element_read; 
    int recv_size; 
    
    // Implement the process of reading and sending the data to the server
    while (element_read = fread(message, sizeof(char), BUFFER_SIZE, compressed_file)) {
        // The Scope is to debug
        for (int i = 0; i < element_read; i++) {
            printf("%d)%d, ",i ,message[i]);
            memory += sizeof(message[i]);
        }
        printf("\nmem: %d\n", memory);

        int debug = send(s, (char*)message, element_read, 0);
    
        // Send the data with the buffer type of (char)
        if (debug < 0) {
            printf("Send failed ERROR FAKE NEWS %d diff %d\n", debug, element_read);
            return 1;
        } 
        printf("Data send: %d bytes\n", debug);

        // // Receive a reply from the server, Once receive the reply message from server, Qualified to continue to send data 
        while(1) {
            recv_size = recv(s, server_reply, BUFFER_SIZE, 0);
            if (recv_size == SOCKET_ERROR) {
                printf("recv failed\n");
            }
            else if (recv_size > 0) {
                server_reply[recv_size] = '\0';
                printf("Server reply: %s\n\n", server_reply, recv_size);
                break;
            }
        }
        // fseek(compressed_file, -(element_read / 2), SEEK_CUR);
    }

    // Send the signal that announce finish sending file data
    message[0] = START_DECODE; 
    if (send(s, (char*)message, 2, 0) < 0) {
        printf("Send failed\n");
        return 1;
    } 
    memory = 0;

}