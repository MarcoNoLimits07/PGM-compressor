# PGM Image Compression Software

This project implements a simple PGM image compressor and decompressor using Run-Length Encoding (RLE).

## Project Structure

PGM_Compressor/
├── src/
│ ├── PGM.h
│ ├── PGM.cpp
│ └── main.cpp
├── README.md
└── .gitignore

## Build and Run

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/PGM_Compressor.git
    cd PGM_Compressor/src
    ```

2. Compile the project:
    ```sh
    g++ main.cpp PGM.cpp -o PGMCompressor
    ```

3. Run the program:
    ```sh
    ./PGMCompressor
    ```

## Features

- **Load PGM Image**: Reads a PGM image from file.
- **Compress Image**: Compresses the image using Run-Length Encoding (RLE).
- **Decompress Image**: Decompresses the RLE compressed image.

## Author

- Ahmed Marcolino Teca Kanadji
- 2024

## License

This project is licensed under the MIT License.
