##### Criptografía y seguridad - Instituto Tecnológico de Buenos Aires

# Trabajo Práctico especial: Esteganografía

### Autores

- [Quintairos, Juan Ignacio](https://github.com/juaniq99) - Legajo 59715
- [Borinsky, Camila](https://github.com/camilaborinsky) - Legajo 60083
- [Riera Torraca, Valentino](https://github.com/vriera) - Legajo 60212

## Compilación

```
make all
```

## Ejecución

### Para ocultar archivos sin encriptar

#### Con LSB1

```
./steg -e -p resources/lado.bmp -o resources/embed_lsb1.bmp -i resources/itba.png -s lsb1
./steg --embed -p resources/lado.bmp --out resources/embed_lsb1.bmp --i resources/itba.png --steg lsb1
```

#### Con LSB4

```
./steg -e -p resources/lado.bmp -o resources/embed_lsb4.bmp -i resources/itba.png -s lsb4
./steg --embed -p resources/lado.bmp --out resources/embed_lsb4.bmp --i resources/itba.png --steg lsb4
```

#### Con LSBI

```
./steg -e -p resources/lado.bmp -o resources/embed_lsbi.bmp -i resources/itba.png -s lsbi
./steg --embed -p resources/lado.bmp --out resources/embed_lsbi.bmp --i resources/itba.png --steg lsbi
```

### Para extraer sin encriptar

#### Con LSB1

```
./steg -x -p resources/ladoLSB1.bmp -o resources/extract_lsb1 -s lsb1
./steg --extract -p resources/ladoLSB1.bmp --out resources/extract_lsb1 --steg lsb1
```

#### Con LSB4

```
./steg -x -p resources/ladoLSB4.bmp -o resources/extract_lsb4 -s lsb4
./steg --extract -p resources/ladoLSB4.bmp --out resources/extract_lsb4 --steg lsb4
```

#### Con LSBI

```
./steg -x -p resources/ladoLSBI.bmp -o resources/extract_lsbi -s lsbi
./steg --extract -p resources/ladoLSBI.bmp --out resources/extract_lsbi --steg lsbi
```

### Para ocultar archivos con contenido encriptado

```
./steg -e -p resources/lado.bmp -o resources/embed_lsb1.bmp -i resources/itba.png -s lsb1 -a aes192 -m cbc -k escondite
./steg --embed -p resources/lado.bmp --out resources/embed_lsb1.bmp --i resources/itba.png --steg lsb1 -a aes192 -m cbc --pass escondite
```

### Para extraer archivos con contenido encriptado

```
./steg -x -p resources/ladoLSB1aes192cbc.bmp -o resources/extract_lsb1_aes -s lsb1 -a aes192 -m cbc -k escondite
./steg --extract -p resources/ladoLSB1aes192cbc.bmp --out resources/extract_lsb1_aes --steg lsb1 -a aes192 -m cbc --pass escondite
```
