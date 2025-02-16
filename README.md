## Huffman Encoder & Decoder  
### **Author:** Daniel Morales-Garcia  
### **Description**  
This project is an implementation of **Huffman Coding**, a lossless data compression algorithm. The program consists of two main utilities:  
- `huffencode`: Compresses a file using Huffman encoding.  
- `huffdecode`: Decompresses a Huffman-encoded file back to its original form.  

The project reads a file, constructs a Huffman tree based on character frequencies, assigns binary codes to each character, and compresses the data. The encoded file includes a header that allows proper decoding.

---

## **Compilation & Usage**  

### **1. Compilation**  
Use the provided `Makefile` to compile the program:  

```sh
make
```
This will generate two executable files:
- `huffencode`: For encoding files.  
- `huffdecode`: For decoding files.  

To clean up compiled files, use:  

```sh
make clean
```

---

### **2. Encoding a File**  
To encode a file, run:

```sh
./huffencode <input_file> <output_file>
```

Example:

```sh
./huffencode input.txt encoded.huff
```

This will generate a Huffman-encoded file (`encoded.huff`) from the original `input.txt`.

---

### **3. Decoding a File**  
To decode a Huffman-encoded file, run:

```sh
./huffdecode <encoded_file> <output_file>
```

Example:

```sh
./huffdecode encoded.huff output.txt
```

This will restore the original content from `encoded.huff` to `output.txt`.

---

## **Project Structure**  

### **1. Source Files**  
- **`huffman.h`** - Header file defining the Huffman tree structure and function prototypes.  
- **`huffman.c`** - Core implementation of Huffman encoding and decoding.  
- **`huffencode.c`** - Main function for encoding a file.  
- **`huffdecode.c`** - Main function for decoding a file.  
- **`Makefile`** - Compilation instructions.  

---

## **How It Works**  

### **1. Encoding Process (`huffencode`)**
1. Reads the input file and counts the frequency of each character.  
2. Builds a Huffman tree, with frequent characters having shorter binary codes.  
3. Writes a header to the output file containing character frequencies.  
4. Encodes the input file using Huffman codes and writes it to the output file.  

### **2. Decoding Process (`huffdecode`)**
1. Reads the header from the Huffman-encoded file.  
2. Reconstructs the Huffman tree from the header data.  
3. Reads the compressed binary data and translates it back into characters.  
4. Writes the decoded data to the output file.  

---

## **Error Handling**
The program handles various error cases:
- Incorrect number of arguments.
- Missing or unreadable input files.
- Failure to open output files.