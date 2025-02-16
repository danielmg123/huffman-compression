/**************************************************************
*
* Daniel Morales-Garcia
*
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

/* creating node that can serve as both list and tree node */
struct Node* createNode(unsigned char data, unsigned long frequency, int isLeaf)
{
    struct Node* node = malloc(sizeof(struct Node));
   
    /* values for linked list insertion */
    node -> data = data;
    node -> frequency = frequency;
    node -> next = NULL;
    
    /* values for binary tree insertion */
    node -> left = NULL;
    node -> right = NULL;
    node -> isLeaf = isLeaf;
    return node;
}

/* printing codes to output stream */
void printDataCodes(unsigned short codes[][256], int value)
{
    int i = 0;
    while (codes[value][i] == 1 || codes[value][i] == 0)
    {
        printf("%d", codes[value][i]);
        i++;
    }
    printf("\n");
}

/* printing symbol, frequencies, and codes, and total character to output stream */
void printData(unsigned long *frequency, unsigned char *values, unsigned long totalCh, unsigned short codes[][256])
{
    int i;
    printf("Symbol\tFreq\tCode\n");
    
    /* checking if value 0 is present in data, and printing info */
    if (frequency[0] > 0 && !(codes[0][0] > 1))
    {
        printf("=0\t%lu\t", frequency[0]);
        printDataCodes(codes, 0);
    }
    
    for (i = 0; i < 256; i++)
    {
        if (values[i] != '\0')
        { 
            
            /* if value is present in file, prints value info */
            if (values[i] < 33 || values[i] > 126)
            {
                printf("=%d\t%lu\t", values[i], frequency[i]);
                printDataCodes(codes, i);
            }
            else 
            {
                printf("%c\t%lu\t", values[i], frequency[i]);
                printDataCodes(codes, i);
            }
        }
    }
    
    /* prints total characters in file */
    printf("Total chars = %lu\n", totalCh);   
}

/* returns data from leftmost node */
unsigned char nodeValue(struct Node* node)
{
    struct Node* tempNode = node;
    if (node -> isLeaf == 1) 
    {
        return node -> data;
    }
    else
    {
        /* goes to the left child of each node until reaching NULL, then returning data */
        while (tempNode -> left != NULL) tempNode = tempNode -> left;
    }
    return tempNode -> data;
}

/* inserts leaf data into ordered linked list (ordered by frequency) */
struct Node* insertSortedListLeaves(struct Node* head, unsigned char value, unsigned long frequency)
{
    struct Node* temp = head;
    /* creates new node with value */
    struct Node* newNode = createNode(value, frequency, 1);
    
    unsigned long tempFrequency = frequency;
    
    /* case if list is empty or new node is inserted in beginning */
    if (head == NULL || tempFrequency < head -> frequency)
    {
        newNode -> next = head;
        head = newNode;
        return head;
    }
    
    /* if head freqency matches inserted leaf freqency, compares data */
    else if (tempFrequency == head -> frequency && head -> data > value)
    {
            newNode -> next = head;
            head = newNode;
            return head;
    }

    /* finds location somwhere in middle of ordered linked list to insert new leaf */ 
    while(temp -> next != NULL && 
    (temp -> next -> frequency <= tempFrequency && temp -> next -> data < value))
    {
        temp = temp -> next;
    }
    
    newNode -> next = temp -> next;
    temp -> next = newNode;
    
    /* returns head of list */
    return head;
}

/* creates sorted linked list with values present in file */
struct Node* createLeafList(unsigned char *values, unsigned long *frequency)
{
    int i;
    struct Node* head = NULL;
    if (frequency[0] > 0) head = createNode(values[0], frequency[0], 1);
    for (i = 0; i < 256; i++)
    {
        if (values[i] != '\0')
        {
            head = insertSortedListLeaves(head, values[i], frequency[i]);
        }
    }
    
    return head;
}

/* inserts tree head to sorted linked list (ordered by frequency or frequency of leftmost node)*/
struct Node* insertSortedTreeToList(struct Node* listHead, struct Node* treeHead)
{
    struct Node* temp = listHead;
    
    /* gets value of node to insert to linked list */
    unsigned char treeHeadvalue = nodeValue(treeHead);   
    
    unsigned long tempFrequency = treeHead -> frequency;
    
    /* tree is inserted to beginning of linked list*/
    if (tempFrequency < listHead -> frequency)
    {
        treeHead -> next = listHead;
        listHead = treeHead;
        return listHead;
    }
    
    /* frequencies of tree matches head of linked list, so compares lefmost values */
    else if (tempFrequency == listHead -> frequency)
    {
            if (nodeValue(listHead) > treeHeadvalue)
            {
                treeHead -> next = listHead;
                listHead = treeHead;
                return listHead;
            }
    }
    
    /* inserts tree somewhere in middle of ordered linked list */
    while(temp -> next != NULL && (temp -> next -> frequency <= tempFrequency))
    {
        if (temp -> next -> frequency == tempFrequency && nodeValue(temp -> next) > treeHeadvalue) break;
        temp = temp -> next;
    }  
    treeHead -> next = temp -> next;
    temp -> next = treeHead;
    
    /* returns new listhead */
    return listHead;
}


/* creates a ordered binary tree of 2 values as leaves. least value leaf goes to left) */ 
struct Node* formSubtree(struct Node* head, struct Node* afterHead)
{
    struct Node* tempHead = afterHead -> next;
    
    /* creates new root to tree, frequency in new node is frequencies from leaves added */
    struct Node* treeHead = createNode(0, head -> frequency + afterHead -> frequency, 0);
    
    /* assigning leaves to new root */
    treeHead -> left = head;
    treeHead -> right = afterHead;
    
    /* end of linked list */
    if (tempHead == NULL)
    {
        head -> next = NULL;
        return treeHead;
    }
    
    /* inserts new root to ordered linked list */
    else
    {
        head -> next = NULL;
        afterHead -> next = NULL;
        tempHead = insertSortedTreeToList(tempHead, treeHead);
    }
    return tempHead;
}

/* forms tree from two leaf nodes, frequencies of both leafs added equals */
/* freqency of new root node                                              */ 
struct Node* createHuffmanTree(struct Node* head)
{
    if (head == NULL) return head;
    
    while (head -> next != NULL)
    {
        head = formSubtree(head, head -> next);       
    }
    return head;
}

/* traverses tree. If leaf is found, the path is entered into the code array */
/* going left in tree, adds 0 to codes, right adds 1 to codes                */
void getCodes(struct Node* root, unsigned short codes[][256],
unsigned short tempCode[], int current, int codeBitsLength[]) 
{
    int i;
    
    if (root -> left != NULL)
    {
        tempCode[current] = 0;
        getCodes(root -> left, codes, tempCode, current + 1, codeBitsLength);
    }
    
    if (root -> right != NULL)
    {
        tempCode[current] = 1;
        getCodes(root -> right, codes, tempCode, current + 1, codeBitsLength);
    }
    
    /* leaf is found, and current path is printed as huffman code for value in leaf */
    if (root -> isLeaf == 1)
    {
        codeBitsLength[root -> data] = current;
        for(i = 0; i < current; i++)
        {
            codes[root -> data][i] = tempCode[i];
        }            
    }
}

/* converts unsigned char of total characters into bytes, and prints to header */
void printTotalChToHeader(unsigned long totalCh, FILE *out)
{
    /* converting unsigned long to 8 unsigned chars */
    unsigned char firstByte   = (totalCh & 0x00000000000000ffUL);
    unsigned char secondByte  = (totalCh & 0x000000000000ff00UL) >>  8;
    unsigned char thirdByte   = (totalCh & 0x0000000000ff0000UL) >> 16;
    unsigned char fourthByte  = (totalCh & 0x00000000ff000000UL) >> 24;
    unsigned char fifthByte   = (totalCh & 0x000000ff00000000UL) >> 32;
    unsigned char sixthByte   = (totalCh & 0x0000ff0000000000UL) >> 40;
    unsigned char seventhByte = (totalCh & 0x00ff000000000000UL) >> 48;
    unsigned char eigthByte   = (totalCh & 0xff00000000000000UL) >> 56;
    
    /* printing 8 unsigned chars to header */
    fprintf(out, "%c", firstByte);
    fprintf(out, "%c", secondByte);
    fprintf(out, "%c", thirdByte);
    fprintf(out, "%c", fourthByte);
    fprintf(out, "%c", fifthByte);
    fprintf(out, "%c", sixthByte);
    fprintf(out, "%c", seventhByte);
    fprintf(out, "%c", eigthByte);   
}

/* prints codes to header. If any bits are unused in last byte, it is padded with zeros */
void printSymbolCodeToHeader(unsigned short codes[][256], int codeBitsLength[], int index, FILE* out)
{
    int i = 0;
    int bitInByteCounter = 0;
    int finalShift;
    unsigned char byte = 0;
    
    for (i = 0; i < codeBitsLength[index]; i++)
    {       
        byte = byte << 1;
        if (codes[index][i] == 1) byte = byte | 0x01; 
        
        if (bitInByteCounter == 7)
        {
            fprintf(out, "%c", byte);
            bitInByteCounter = 0;
            byte = 0;          
        }
        else bitInByteCounter++;
    }
    
    /* last byte padded with zeros */
    finalShift = 8 - bitInByteCounter;
    byte = byte << finalShift;
      
    /* if all bits perfectly fit byte, dont print another byte */
    if ((codeBitsLength[index] % 8) != 0) fprintf(out, "%c", byte);
}

/* writes header of huffman file. */
void writeHeader(unsigned char values[], unsigned short codes[][256], unsigned long frequency[],
unsigned long totalCh, FILE* out, int uniqueSymbols, int codeBitsLength[])
{
    int i;   
    fprintf(out, "%c", uniqueSymbols);
    
    for(i = 0; i < 256; i++)
    {
        /* if frequency of value is greater than 0, value is present */
        if (frequency[i] > 0) 
        {
            /* prints value, huffman code length, and code to header */
            fprintf(out, "%c", values[i]);
            fprintf(out, "%c", codeBitsLength[i]);
            printSymbolCodeToHeader(codes, codeBitsLength, i, out);
        }
    }
    
    /* prints total character count to header */
    printTotalChToHeader(totalCh, out);   
}

/* writes encoded data to file */
void writeText(unsigned short codes[][256], FILE* in, FILE *out, int codeBitsLength[])
{
    int ch = 0;
    int i;
    int finalShift;
    int bitInByteCounter = 0;
    unsigned char byte = 0;
    
    rewind(in);
    
    while (ch != EOF)
    {
        ch = fgetc(in);
        
        if (ch < 256 && ch >= 0)
        {
            for (i = 0; i < codeBitsLength[ch]; i++)
            {              
                if (codes[ch][i] == 1)
                {
                    byte = byte << 1;
                    byte = byte | 0x01;
                }     
                else byte = byte << 1;
                
                /* bits has filled byte, so prints byte and starts a new byte */
                if (bitInByteCounter == 7)
                {
                    fprintf(out, "%c", byte);
                    bitInByteCounter = 0;
                    byte = 0;          
                }
                else bitInByteCounter++;
            }
            
        }
    }
    if (bitInByteCounter != 0)
    {
        finalShift = 8 - bitInByteCounter;
        byte = byte << finalShift;
        fprintf(out, "%c", byte);
    }
}

/* frees all nodes */
void freeTree(struct Node* root)
{
    if (root == NULL) return;
    freeTree(root -> left);
    freeTree(root -> right);
    free(root);
}

/* encodes data using huffman algorithm */
void encodeFile(FILE* in, FILE* out)
{
    int ch = 0;
    unsigned long j;
    unsigned long totalCh = 0;
    int uniqueSymbols = 0;
    unsigned char values[256];
    unsigned long frequency[256];
    unsigned short codes[256][256];
    int codeBitsLength[256];
    unsigned short tempCode[15];
    struct Node* head;
    struct Node* root;
    memset(values, '\0', sizeof values);
    memset(frequency, 0, sizeof frequency);
    
    /* reads data from file */
    while (ch != EOF)
    {   
        ch = fgetc(in);
        totalCh += 1;
        
        if (ch < 256 && ch >= 0)
        {       
            if (values[ch] == '\0' && !(frequency[ch] > 0))
            {
                values[ch] = ch;
                uniqueSymbols += 1;
            }
            
            j = frequency[ch] + 1;
            frequency[ch] = j;
        }
    }    
    totalCh -= 1;
    
    /* if there are 256 unique symbols, it is represented as a 0 in header */
    if (uniqueSymbols == 256) uniqueSymbols = 0;
    
    memset(codes, 2, sizeof codes);
    
    /* creates a linked list with leaves, then combines leaves and forms huffman tree */
    head = createLeafList(values, frequency);
    root = createHuffmanTree(head);
    
    /* gets codes from huffman tree */
    getCodes(root, codes, tempCode, 0, codeBitsLength);
    
    /* prints values, frequencies, huffman codes, and total character count to standard out */
    printData(frequency, values, totalCh, codes);   
    
    /* writes header in new encoded huffman file */
    writeHeader(values, codes, frequency, totalCh, out, uniqueSymbols, codeBitsLength);
    
    /* writes encoded text to new huffman file */
    writeText(codes, in, out, codeBitsLength);
    
    /* frees all nodes that were created */
    freeTree(root);
}

/* gets values code from header */ 
void getCodeFromHeader(unsigned short codes[][256], FILE *in, unsigned char value, int codeLength)
{
    int i;
    unsigned char codesByte;
    int bitInByteCounter = 0;
    
    codesByte = fgetc(in);
    
    for(i = 0; i < codeLength; i++)
    {
        /* gets first bit from byte, then shifts */
        if ((codesByte & 0x80) == 0x80) codes[value][i] = 1;
        else codes[value][i] = 0;
        
        codesByte = codesByte << 1;
        
        /* entire byte is read, so get a new byte from header */
        if(bitInByteCounter == 7 && i != codeLength - 1)
        {
            bitInByteCounter = 0;
            codesByte = fgetc(in);
        }
        else bitInByteCounter++;
    }
}

/* gets count of total characters in file from header (8 unsigned chars converted to one long */
unsigned long getTotalChFromHeader(FILE *in)
{
    unsigned char firstByte, secondByte, thirdByte, fourthByte,
    fifthByte, sixthByte, seventhByte, eighthByte;
    
    unsigned long totalCh = 0;
    
    eighthByte = fgetc(in);
    seventhByte = fgetc(in);
    sixthByte = fgetc(in);
    fifthByte = fgetc(in);
    fourthByte = fgetc(in);
    thirdByte = fgetc(in);
    secondByte = fgetc(in);
    firstByte = fgetc(in);

    /* shifting 8 unsigned char to correct position in unsigned long */
    totalCh |= (unsigned long)firstByte  << 56;
    totalCh |= (unsigned long)secondByte << 48;
    totalCh |= (unsigned long)thirdByte   << 40;
    totalCh |= (unsigned long)fourthByte   << 32;
    totalCh |= (unsigned long)fifthByte  << 24;
    totalCh |= (unsigned long)sixthByte   << 16;
    totalCh |= (unsigned long)seventhByte  << 8;
    totalCh |= (unsigned long)eighthByte; 
    
    /* return count of total characters to print to file */
    return totalCh;
}

/* checking if code read so far is a value */
int checkIfValue(unsigned short codes[][256], unsigned short bitsArr[], int codeLength, unsigned char *value)
{
    int valueIndex;
    int i;
    int codesDontMatch;
    
    
    if (codeLength != 255)
    {
        /* loops through all values and compares current bit array to codes */
        for(valueIndex = 0; valueIndex < 256; valueIndex++)
        {
            /* if current code is same length as a values code, and same bit last bit value compare entire code */
            if (codes[valueIndex][codeLength] == bitsArr[codeLength]
            && (codes[valueIndex][codeLength + 1] != 1 && codes[valueIndex][codeLength + 1] != 0))
            {
                codesDontMatch = 0;
                for (i = 0; i < codeLength + 1; i++)
                {
                    /* if code does not match, break */
                    if (codes[valueIndex][i] != bitsArr[i])
                    {
                        codesDontMatch = 1;
                        break;
                    }
                }
                
                /* if code matches, assign value and return 1 */
                if(codesDontMatch == 0) 
                {
                    *value = valueIndex;
                    return 1;
                }
            }
        }
    }
    /* if code is of length 256 */
    else
    {
        for (valueIndex = 0; valueIndex < 256; valueIndex++)
        {
            if(codes[valueIndex][codeLength] == 1 || codes[valueIndex][codeLength] == 0)
            {
                *value = valueIndex;
                return 1;
            }
        }
    }
    
    return 0;
}

/* printing decoded data to file */
void printDataToFile(FILE *in, FILE *out, unsigned short codes[][256], unsigned long totalCh)
{
    int isValue = 0;
    int chIndex = 0;
    unsigned char value;
    unsigned char byte;
    unsigned short bitsArr[256] = {0};
    int codeLength = 0;
    int bitInByte = 0; 
    
    byte = fgetc(in);

    while(totalCh > chIndex)
    {
        /* checking if bit is 1 or 0, then shifting to next bit */
        /* 0x80 is equal to 10000000 in binary */
        if((byte & 0x80) == 0x80)
        {
            bitsArr[codeLength] = 1;
        }
        else
        {
            bitsArr[codeLength] = 0;
        }
        byte = byte << 1;
        
        /* checking is current code is a value */
        isValue = checkIfValue(codes, bitsArr, codeLength, &value);      
        if (isValue == 1) 
        {
            fprintf(out, "%c", value);
            chIndex++;
            codeLength = 0;
        }
        else codeLength++;
        
        /* if entire byte is read, so gets new byte */
        if (bitInByte == 7 && chIndex != totalCh)
        {
            bitInByte = 0;
            byte = fgetc(in);   
        }
        else bitInByte++;
    }
    
}

/* decodes file with huffman codes and prints data to out file */
void decodeFile(FILE *in, FILE *out)
{
    int i;
    unsigned char value;
    int uniqueSymbols;
    unsigned long totalCh;
    unsigned short codes[256][256];
    unsigned char codeLength = 0;;
    
    memset(codes, 2 , sizeof codes);
    
    /* gets unique symbols count from header */
    uniqueSymbols = fgetc(in);
    
    /* if 256 unique symbols are present, it is stored as 0 value in header */
    if (uniqueSymbols == 0) uniqueSymbols = 256;
    
    /* gets value data from header */
    for(i = 0; i < uniqueSymbols; i++)
    {
        value = fgetc(in);
        codeLength = fgetc(in);
        getCodeFromHeader(codes, in, value, codeLength);
    }         
    totalCh = getTotalChFromHeader(in);   
    
    /* prints decoded data to new file */
    printDataToFile(in, out, codes, totalCh);
}
