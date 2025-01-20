# Simulate_CoDec_Information_by_wireless_com
Product for Information Theory course 

# Before compile the two main files, PLEASE Modify the path of output files
Go to the file "config.h" in "lib" folder, change the default path to the path of the folders
containing the output files respectively to the SENDING side and RECEIVING side and ASSET

# Cmd terminal command to compile encode 
    gcc client.c -o client -lws2_32 ../lib/encoder.c ../lib/read_text.c -I../lib

# Cmd terminal command to compile decode
    gcc server.c -o server -lws2_32 ../lib/decoder.c -I../lib

# Cmd terminal command to compile decode from code 
    gcc dec_main_from_code.c -o dec_main_from_code ../lib/decode_from_code.c -I../lib

# Chú ý 
File test nằm ở mục asset
Kết quả nằm ở results
Các chương trình chính ở src
Thư viện nằm trong lib

# Main programs of the system are client.c and server.c
# Statistics of ENCODING in the file "Stats of Huffman Code" 
# INPUT NAME FILE IN FOLDER "asset" 
# PLEASE CONFIG PARAMETERS IN file config.h in FOLDER "lib" BEFORE COMPILING 