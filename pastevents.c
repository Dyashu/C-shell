#include "headers.h"

 

 int countPastEvents(struct PastEvent* head) {
    struct PastEvent* current = head;
    int count = 0;
    while (current != NULL) {
        current = current->next;
        count++;
    }
    return count;
}


void addPastEvent(struct PastEvent** head, const char* event) {
    
    struct PastEvent* current = *head;
    while (current != NULL) {
        if (strcmp(current->event, event) == 0) {
         
            return;
        }
        current = current->next;
    }


    struct PastEvent* new_event = (struct PastEvent*)malloc(sizeof(struct PastEvent));
    if (new_event == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

 
    strncpy(new_event->event, event, MAX_EVENT_LENGTH);
    new_event->next = NULL;

   
    new_event->next = *head;
    *head = new_event;

  
    if (countPastEvents(*head) > MAX_PAST_EVENTS) {
        struct PastEvent* temp = *head;
        while (temp->next->next != NULL) {
            temp = temp->next;
        }
        free(temp->next);
        temp->next = NULL;
    }
}




void printPastEvents(struct PastEvent* head) {
    struct PastEvent* current = head;
    int count = 1;
    while (current != NULL) {
        printf("%d. %s\n", count, current->event);
        current = current->next;
        count++;
    }
}



void clearPastEvents(struct PastEvent** head) {
    while (*head != NULL) {
        struct PastEvent* temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}




void executePastEvents(int index, struct PastEvent** head) {
    struct PastEvent* current = *head;
    int count = countPastEvents(*head);

    if (index <= 0 || index > count) {
        printf("Invalid index\n");
        return;
    }

    for (int i = 1; i < count - index + 1; i++) {
        current = current->next;
    }

    printf("Executing: %s\n", current->event);
    system(current->event);
}


