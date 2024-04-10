# Compress and decompress your files with the Huffman Algorithm!

- Support for **all** file types;
- (Still) does not compress folders;
- The generated files have _.huff_ extension;
- This program was made for a college project, so its comments are in Portuguese;

## Requirements
- gcc;
- Support for Makefile (optional, if using Windows);

## How to Execute
### Setup
At the root of the project, run the _Makefile_ (Only once);
```
make
```

If your OS does not support Makefile, you can copy and paste this line into the terminal
```
gcc -I./algorithm -I./algorithm/structures -I./algorithm/utils ./algorithm/main.c ./algorithm/compress.c ./algorithm/decompress.c ./algorithm/structures/huffman_tree.c ./algorithm/structures/linked_list.c ./algorithm/utils/extension.c ./algorithm/utils/header.c -o huff
```

### Running
To compress or decompress a file, move it into /inputs and execute
```
./huff <filename>
```

*_If the file has the .huff extension, it will be decompressed, otherwise it will be compressed_

## Data Structures used
- Linked lists;
- Trees (Huffman Tree);

## Process

...