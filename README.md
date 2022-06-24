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
./steg -x -p resources/ladoLSB1.bmp -o resources/out_reveal_4 -s lsb1
./steg -e -p resources/lado.bmp -o resources/embed.bmp -i resources/out_reveal_1.png -s lsb1

<!-- ./steg -x -p resources/ladoLSB1.bmp -o resources/out_reveal_4 -s lsb1 -->

./steg -e -p resources/lado.bmp -o resources/embed_4.bmp -i resources/out_reveal_1.png -s lsb4

./steg -e -p resources/lado.bmp -o resources/embed_i.bmp -i resources/out_reveal_i.png -s lsbi
