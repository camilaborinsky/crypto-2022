# crypto-2022

Esteganografiar el archivo de texto “mensaje1.txt” en el archivo portador “imagen1.bmp”
obteniendo un archivo “imagenmas1 .bmp” mediante el algoritmo LSB Improved, con encripción DES en
modo CBC con password “oculto”
`stegobmp -embed –in “mensaje1.txt” –p “imagen1.bmp” -out “imagenmas1.bmp” –steg LSBI –a des –m cbc -pass “oculto”`

`stegobmp -embed –in “mensaje1.txt” –p “imagen1.bmp” -out “imagenmas1.bmp” –steg LSBI`
