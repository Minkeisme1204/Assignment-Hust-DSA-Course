#include "queue.h"

void cleanString(char *string) {
    int i = 0;
    while (string[i]!= '\0') {
        if (string[i] == '\n') {
            string[i] = '\0';
        }
        i++;
    }
}

int main() {
    Queue* queue = createQueue();
    int n = 3; 
    for (int i = 0; i < n; i++) {
        Data *temp = malloc(sizeof(Data));
        fgets(temp->owner_name, 50, stdin);
        cleanString(temp->owner_name);
        fgets(temp->item_name, 50, stdin);
        cleanString(temp->item_name);
        scanf("%d", &temp->money);
        getchar();

        enQueue(queue, temp);
    }

    // Print the first state of the queue
    printf("Queue at the beginning:\n");
    printQueue(queue);

    int income = 0;
    int i = 1; 
    while (!isEmptyQueue(queue)) {
        printf("Updated queue after processing %d bills:\n", i);
        income += queue->front->data->money;
        deQueue(queue);
        printQueue(queue);
        i++;
    }

    printf("\nTotal income from the bills: %d VND\n", income);
    return 0;
}