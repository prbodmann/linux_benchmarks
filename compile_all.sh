SRC_FILES=(crc32 fft jpeg matmul_500 qsort rijndael susan)

for i in "${SRC_FILES[@]}"; do
    #make -C "$i" clean
    make -C "$i" TARGET=armA5
done;
