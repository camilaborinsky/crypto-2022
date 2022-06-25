# crypto-2022

### Para ocultar archivo

Esteganografiar el archivo de texto “mensaje1.txt” en el archivo portador “imagen1.bmp”
obteniendo un archivo “imagenmas1 .bmp” mediante el algoritmo LSB Improved, con encripción DES en
modo CBC con password “oculto” \
`stegobmp -embed –in “mensaje1.txt” –p “imagen1.bmp” -out “imagenmas1.bmp” –steg LSBI –a des –m cbc -pass “oculto”`\
Esteganografiar el archivo de imagen “mensaje1.txt” en el archivo portador “imagen1 .bmp”
obteniendo un archivo “imagenmas1.bmp” mediante el algoritmo LSB Improved, sin encripción\
`stegobmp -embed –in “mensaje1.txt” –p “imagen1.bmp” -out “imagenmas1.bmp” –steg LSBI` \
No se puede encriptar/desencriptar sin password. Si este dato no está, sólo se esteganografia. \
Son válidas en cambio las siguientes opciones:

- indicar algoritmo y password pero no modo: Se asume CBC por default.
- Indicar modo y password pero no algoritmo: Se asume aes128 por default.
- Indicar sólo password: Se asume algoritmo aes128 en modo CBC por default. \

make clean && make all

EMBED without encryption
LSB1
./steg -e -p resources/lado.bmp -o resources/embed_lsb1.bmp -i resources/itba.png -s lsb1
LSB2
./steg -e -p resources/lado.bmp -o resources/embed_lsb4.bmp -i resources/itba.png -s lsb4
LSBI
./steg -e -p resources/lado.bmp -o resources/embed_lsbi.bmp -i resources/itba.png -s lsbi

EXTRACT without encryption
LSB1
./steg -x -p resources/ladoLSB1.bmp -o resources/extract_lsb1 -s lsb1
LSB4
./steg -x -p resources/ladoLSB4.bmp -o resources/extract_lsb4 -s lsb4
LSBI
./steg -x -p resources/ladoLSBI.bmp -o resources/extract_lsbi -s lsbi

LSB1
./steg -x -p resources/ladoLSB1aes192cbc.bmp -o resources/extract_lsb1_aes192cbc -s lsb1 -a aes192 -m cbc -k escondite
./steg -e -p resources/lado.bmp -o resources/embed_lsb1_aes192cbc.bmp -s lsb1 -a aes192 -m cbc -k escondite -i resources/itba.png
