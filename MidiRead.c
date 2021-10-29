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

    int dataStart       = 0;
    int dataEnd         = 0;
    int statusByte      = 0;
    int dataByte        = 0;
    int velocityByte    = 0;
    int ticksFirst      = 0;
    int ticksSecond     = 0;

    for (int i = 0; i<101; i++)
    {
        if (buffer[i] == 0xFF && buffer[i+1] == 0x2F)
        {
            printf("End of Track\n");
            break;
        }

        if (buffer[i] == 0x80 || buffer[i] == 0x90)
        {
            dataStart = i;

            statusByte      = buffer[i];
            dataByte        = buffer[i+1];
            velocityByte    = buffer[i+2];
            ticksFirst      = buffer[i-1];
            
            int checkValue = (int)(unsigned char)buffer[i+3];
                    
            switch(checkValue)
            {
                case 0x00:
                    ticksSecond = 0;
                    break;
                case 0x40:
                    // /2 delta ticks
                    ticksSecond = (int)(unsigned char)buffer[i+3];
                    dataEnd += 1;
                    break;
                case 0x80:
                    ticksSecond = 0;
                    break;
                case 0x81:
                    // *delta ticks
                    ticksSecond = (int)(unsigned char)buffer[i+3];
                    dataEnd += 1;
                    break;
                case 0x82:
                    // *2*delta ticks
                    ticksSecond = (int)(unsigned char)buffer[i+3];
                    dataEnd += 1;
                    break;
                case 0x83:
                    // *3*delta ticks
                    ticksSecond = (int)(unsigned char)buffer[i+3];
                    dataEnd += 1;
                    break;
                case 0x90:
                    ticksSecond = 0;
                    break;
                case 0xFF:
                    ticksSecond = 0;
                    break;
                default:
                    ticksSecond = (int)(unsigned char)buffer[i+2];
                    break;
            }

            pushNew(firstLink,head,dataStart,dataEnd,statusByte,dataByte,velocityByte,ticksFirst,ticksSecond);

            if (firstLink == true)
            {   
                // writing first time in list
                firstLink = false;
            }

            dataStart       = 0;
            dataEnd         = 0;
            statusByte      = 0;
            dataByte        = 0;
            velocityByte    = 0;
            ticksFirst      = 0;
            ticksSecond     = 0;
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

