#include "read_text.h"

char *extract_char(struct char_text *text, int size) {
    char *ch = malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++)  {
        ch[i] = text[i].data;
        // printf("%c:\n", ch[i]);
    }
    return ch; 
}

float *extract_freq(struct char_text *text, int size) {
    long sum = 0; 
    for (int i = 0; i < size; i++) sum += text[i].present; 
    // printf("%f\n", sum);

    float *freq = malloc(sizeof(float) * size);
    for (int i = 0; i < size; i++)  {
        freq[i] = text[i].present / (float)sum;
        // printf("%c: %f\n",text[i].data, freq[i]);
    }

    float check = 0; 
    for (int i = 0; i < size; i++) check += freq[i];
    printf("%f\n", check);
    printf("Root file has %ld bits\n", sum * 8);
    return freq; 
}

int *extract_present(struct char_text *text, int size) {
    int *present = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)  {
        present[i] = text[i].present;
    }
    return present;
}


int process_text(char *filename, char **data, float **freq, int **present) {
    for (int i = 0; i < 256; i++) count[i] = 0;

    FILE *txt = fopen(filename, "r");
    if(txt == NULL) {
        perror("FILE does not exist");
        exit(EXIT_FAILURE);
    }
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);

    // Get the number of pressence of each character
    int number_of_char = 0;
    while (fgets(buffer, BUFFER_SIZE, txt)) {
        int i = 0; 
        while (buffer[i] != '\n') {
            if (count[buffer[i]] == 0) number_of_char++;
            count[buffer[i]] ++;
            i++; 
        }
        count['\n'] ++;
    }

    struct char_text arr[256];

    // Init and check information about char and pressence 
    int j = 0; 
    for (int i = 1; i < 256; i++) {
        if (count[i] > 0) {
            arr[j].data = i; 
            arr[j].present = count[i]; 
            printf("%d) %c: %d   %d\n", j, arr[j].data, arr[j].present, arr[j].data);
            j++;
        }
    }
    printf("%d\n", number_of_char);
    
    // padding the ending sign (not included in the input text)
    char padding = PADDING_CHAR;
    arr[number_of_char].data = padding;
    arr[number_of_char].present = 0;
    number_of_char++;
    printf("Padding char: %d %c\n", padding, padding);

    *data = extract_char(arr, number_of_char);
    *freq = extract_freq(arr, number_of_char);
    *present = extract_present(arr, number_of_char);
    // for (int i = 0; i < number_of_char; i++) {
    //     printf("%c : %f\n", data[i], freq[i]);
    // }

    free(buffer);
    fclose(txt);
    
    return number_of_char;
}

// int main() {
//     process_text("C:\\Users\\admin\\Desktop\\nguyencaomink\\Workplace\\University Subject Projects\\Simulate_CoDec_Information_by_wireless_com\\asset\\text1.txt");
// }