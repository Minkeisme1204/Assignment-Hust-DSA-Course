#include <stdlib.h>
#include <stdio.h>
typedef struct Word {
    char word[50];
    int count;
    struct Word* next;
} Word;


Word* createWord(const char* word) {
    Word* newWord = (Word*)malloc(sizeof(Word));
    int i = 0;
    while (word[i] != '\0') {
        newWord->word[i] = word[i];
        i++;
    }
    newWord->word[i] = '\0';
    newWord->count = 1;
    newWord->next = NULL;
    return newWord;
}

int compareWords(const char* word1, const char* word2) {
    int i = 0;
    while (word1[i] != '\0' && word2[i] != '\0') {
        if (word1[i] != word2[i]) {
            return 0; // Not the same word
        }
        i++;
    }
    return word1[i] == word2[i];
}

void addWord(Word** head, const char* word) {
    Word* temp = *head;
    
    // If the list is empty, add the first Word
    if (*head == NULL) {
        *head = createWord(word);
        return;
    }

    // Traverse the list to find if the word already exists
    while (temp) {
        if (compareWords(word, temp->word)) {
            temp->count++; // Increment the count if the word exists
            return;
        }
        if (temp->next == NULL) {
            break;
        }
        temp = temp->next;
    }

    // Add a new word at the end of the list if not found
    Word* newWord = createWord(word);
    temp->next = newWord;
}

Word* findMostFrequentWord(Word* head) {
    Word* maxWord = head;
    while (head) {
        if (head->count > maxWord->count) {
            maxWord = head;
        }
        head = head->next;
    }
    return maxWord;
}

void removeDuplicateWords(Word** head) {
    Word* current = *head;
    Word* prev = NULL;
    while (current && current->next) {
        if (compareWords(current->word, current->next->word)) {
            // If the next word is the same, remove it
            Word* duplicate = current->next;
            current->next = duplicate->next;
            free(duplicate);
        } else {
            current = current->next;
        }
    }
}

int countUniqueWords(Word* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

void printSentence(Word* head) {
    printf("After deleting double presented word: ");
    while (head) {
        printf("%s ", head->word);
        head = head->next;
    }
    printf("\n");
}


void processSentence(Word** head, char* sentence) {
    char word[50];
    int i = 0, j = 0;
    while (sentence[i] != '\0') {
        if (sentence[i] == ' ' || sentence[i] == '\n') {
            if (j > 0) {
                word[j] = '\0';
                addWord(head, word);
                j = 0;
            }
        } else {
            word[j++] = sentence[i];
        }
        i++;
    }
    if (j > 0) {
        word[j] = '\0';
        addWord(head, word);
    }
}

int main() {
    Word* head = NULL;
    char sentence[500];
    
    printf("Enter: ");
    fgets(sentence, sizeof(sentence), stdin);
    
    int i = 0;
    while (sentence[i] != '\n' && sentence[i] != '\0') {
        i++;
    }
    sentence[i] = '\0';

    processSentence(&head, sentence);
    removeDuplicateWords(&head);
    
    Word* mostFrequent = findMostFrequentWord(head);
    int uniqueCount = countUniqueWords(head);
    
    printf("Max presented word: %s (%d times)\n", mostFrequent->word, mostFrequent->count);
    printf("Number of words: %d\n", uniqueCount);

    printSentence(head);
    
    return 0;
}