#include <stdio.h>
#include <stdlib.h> //malloc function
#include <stdbool.h>


typedef struct node
{
    int dataStart;
    int dataEnd;
    int statusByte;
    int dataByte;
    int velocityByte;
    int ticksFirst;
    int ticksSecond;
    struct node *next; /* Zeiger auf das nächste Element */
} midi_t;



int readByte()
{
    int result = 0;
    int exponent = 0;
    int value = 0;

    return result;
}

void print_list(midi_t * head) {
    midi_t * current = head;

    while (current != NULL) {
        printf("dataStart:      %d\n", current->dataStart);
        printf("dataEnd:        %d\n", current->dataEnd);
        printf("statusByte:     %d\n", current->statusByte);
        printf("dataByte:       %d\n", current->dataByte);
        printf("velocityByte:   %d\n", current->velocityByte);
        printf("ticksFirst:     %d\n", current->ticksFirst);
        printf("ticksSecond:    %d\n", current->ticksSecond);
        printf("------------------\n");
        current = current->next;
    }
}

void push(midi_t * head, int dataStart, int dataEnd, int statusByte, int dataByte, int velocityByte, int ticksFirst, int ticksSecond) {
    midi_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = (midi_t *) malloc(sizeof(midi_t));

    current->next->dataStart    = dataStart;
    current->next->dataEnd      = dataEnd;
    current->next->statusByte   = statusByte;
    current->next->dataByte     = dataByte;
    current->next->velocityByte = velocityByte;
    current->next->ticksFirst   = ticksFirst;
    current->next->ticksSecond  = ticksSecond;
    current->next->next = NULL;
}

void pushNew(bool firstLink, midi_t * head, int dataStart, int dataEnd, int statusByte, int dataByte, int velocityByte, int ticksFirst, int ticksSecond) {
    
    if (firstLink == false)
    {
        midi_t * current = head;
        while (current->next != NULL) 
        {
            current = current->next;
        }
        /* now we can add a new variable */
        current->next = (midi_t *) malloc(sizeof(midi_t));

        current->next->dataStart    = dataStart;
        current->next->dataEnd      = dataEnd;
        current->next->statusByte   = statusByte;
        current->next->dataByte     = dataByte;
        current->next->velocityByte = velocityByte;
        current->next->ticksFirst   = ticksFirst;
        current->next->ticksSecond  = ticksSecond;
        current->next->next = NULL;
    }
    else
    {
        midi_t * current = head;
        
        current->dataStart    = dataStart;
        current->dataEnd      = dataEnd;
        current->statusByte   = statusByte;
        current->dataByte     = dataByte;
        current->velocityByte = velocityByte;
        current->ticksFirst   = ticksFirst;
        current->ticksSecond  = ticksSecond;
        current->next = NULL;
    }
}


void firstPush(midi_t * head, int dataStart, int dataEnd, int statusByte, int dataByte, int velocityByte, int ticksFirst, int ticksSecond) {
    midi_t * current = head;

    /* now we can add a new variable */
    //current->next = (midi_t *) malloc(sizeof(midi_t));
    
    current->dataStart    = dataStart;
    current->dataEnd      = dataEnd;
    current->statusByte   = statusByte;
    current->dataByte     = dataByte;
    current->velocityByte = velocityByte;
    current->ticksFirst   = ticksFirst;
    current->ticksSecond  = ticksSecond;
    current->next = NULL;
}

int main() 
{
    // https://www.youtube.com/watch?v=DneLxrPmmsw
    // https://www.c-howto.de/tutorial/arrays-felder/speicherverwaltung/
    // https://www.codevscolor.com/c-program-create-iterate-linked-list

    midi_t *head = NULL, *current;
    head = (midi_t *)malloc(sizeof(midi_t));
   
    unsigned char buffer[101];

    FILE *ptr;
    ptr = fopen("/Users/alexandermathieu/Coding/TestArea/Midi/simple.mid","rb");  // r for read, b for binary
    /*
    if (ptr == NULL){
        perror("Error opening file");
        return(-1);
    }*/
    

    bool firstLink = true;
    fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer

    for (int i = 0; i<101; i++)
    {
        if (buffer[i] == 0xFF && buffer[i+1] == 0x2F)
        {
            printf("End of Track\n");
            break;
        }

        if (buffer[i] == 0x80 || buffer[i] == 0x90)
        {
            if (firstLink == true)
            {   
                // writing first time in list
                pushNew(firstLink,head,i,i+4,buffer[i],buffer[i+1],buffer[i+2],buffer[i-1],buffer[i+3]);
                firstLink = false;
            }
            else
            {
                pushNew(firstLink,head,i,i+4,buffer[i],buffer[i+1],buffer[i+2],buffer[i-1],buffer[i+3]);
            }
        }
    }

    print_list(head);


    return 0;

    // print that shit
    for (int i = 0; i<101; i++)
    {
        printf("%02x\n", buffer[i]); // prints a series of bytes
    }

    FILE *write_ptr;
    write_ptr = fopen("/Users/alexandermathieu/Coding/TestArea/Midi/test2.bin","wb");  // w for write, b for binary
    fwrite(buffer,sizeof(buffer),1,write_ptr); // write 10 bytes from our buffer


    return 0;
}

