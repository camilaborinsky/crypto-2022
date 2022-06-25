# Trabajo Práctico: Esteganografía

1. Discutir los aspectos relativos al documento
   a. Organización formal del documento
   b. La descripción del algoritmo.
   c. La notación utilizada, ¿es clara? ¿hay algún error o contradicción?

La organización formal del documento es académica y apropiada. Primero introduce el tema, brindando la idea básica de la técnica y un poco de historia y los conceptos, luego sigue la explicación de cómo se realizó la implementación y por último la sección con los resultados empíricos y análisis de los mismos.

En cuanto a la claridad en la descripción del algoritmo, creemos que no se logró plasmar la idea del mismo de forma simple. Necesitamos varias leídas del mismo para entender verdaderamente cómo era la implementación. Hubiera servido tener alguna figura que tal vez ayudaba a la comprensión y no solo la explicación en prosa.

La notación utilizada es escasa, se explica bastante con palabras, en este sentido creo que ayuda un poco a la lectura porque cuando las explicaciones están muy cargadas con notación se vuelven muy difíciles de seguir. No enontramos contradicciones ni errores.

2. Esteganografiar un mismo archivo en un .bmp con cada uno de los tres algoritmos, y
   comparar los resultados obtenidos. Hacer un cuadro comparativo de los tres algoritmos
   estableciendo ventajas y desventajas.

Para este punto aplicamos los tres algoritmos ocultando la imagen .png del logo del itba dentro del archivo .bmp llamado lado.bmp entregado por la cátedra.

Presentamos el siguiente cuadro comparativo:

            Calidad del                Tamaño necesario del                 Bits modificados
                ocultamiento                archivo portador

        Muy buena, indistinguible          Grande. 8 veces el

LSB1 para nuestro ojo al solo tamaño del payload
modificar el LS bit

        Sigue siendo indistinguible

LSB4 al ojo humano pero bastante Chico. 2 veces el
menor la calidad al modificar tamaño del payload
bastante el pixel (4 bits)

        Parecida a la calidad de LSB1,

LSBI es mejor al ocultar de forma Grande. Ídem LSB1
que los bits originales de la
imagen real sean el máximo posible

3. Explicar detalladamente el procedimiento realizado para descubrir qué se había
   ocultado en cada archivo y de qué modo. Indicar qué se encontró en cada archivo.

silence con lsbi dió pdf que nos da una password
madrid con lsb4 da un png con una imagen de un buscaminas

4. Algunos mensajes ocultos tenían, a su vez, otros mensajes ocultos. Indica cuál era ese
   mensaje y cómo se había ocultado.

5. Uno de los archivos ocultos era una porción de un video, donde se ve ejemplificado
   una manera de ocultar información ¿qué se ocultaba según el video y sobre qué
   portador?

6. ¿De qué se trató el método de estenografiado que no era LSB1 ni LSB4 ni LSBI? ¿Es un
   método eficaz? ¿Por qué?

7. ¿por qué la propuesta del documento de Akhtar, Khan y Johri es realmente una mejora
   respecto de LSB común?

La gran ventaja que tiene el algoritmo propuesto por estos autores es que se logra obtener una imagen con un payload oculto que se parece muchísimo a la imagen original. No se puede apreciar tanto esta mejora en archivos donde el número de bits invertidos en relación al .bmp original es parecida a la cantidad de bits no invertidos (ya presentes originalmente) en la imagen; pero si nos encontramos en un caso donde los invertidos superan ampliamente a los no invertidos, entonces la mejora es notoria al obtener un archivo con payload oculto muy parecido a la imagen original.
El hecho de poder ocultar la lógica no tan simple de los patrones en tan solo 4 bits es también un punto muy fuerte del algoritmo.
La desventaja clara que presenta la propuesta es tener que hacer 2 pasadas sobre el .bmp original y el archivo del payload para identificar si se debe invertir o no para cada patrón. De todas formas, una vez hecha esa lógica el algoritmo se convierte en un LSB1 común y corriente estando únicamente atentos a si se debe invertir o no al ocultar.

8. ¿de qué otra manera o en qué otro lugar podría guardarse el registro de los patrones
   invertidos?

Se nos ocurre que se podría guardar también en los 4 bits menos significativos pero del final del archivo para dificultarle el trabajo de revelar el secreto a alguien que tenga conocimiento de lsbi, aunque de todas formas no nos parece una mejora importante. Lo mismo si cambiamos el orden de guardado de los patrones en donde sea que guardemos.

9. Leer el Segundo esquema y analizar (sin implementar) cuáles serían las ventajas que
   pueden verse.

10. Leer el Segundo esquema e indicar qué desventajas o inconvenientes podría tener su
    implementación.

11. ¿Qué dificultades encontraron en la implementación del algoritmo del paper?

12. ¿Qué mejoras o futuras extensiones harías al programa stegobmp?

Algunas mejoras que se nos ocurran son:

- Soportar otros tipos de archivos como portadores. No trabajar solo con bmp, sino también otro formato de imágenes o ni siquiera imágenes. La dificultad está en atarse a los formatos de cada tipo de archivo (por ejemplo el parseo del header que hacemos sobre bmps)
- soportar otras versiones de imágenes BMP como portadores (en este trabajo
  sólo se trabajó con la versión 3).
- soportar otros tipos de imágenes como portadores, de manera que poder usar
  no sólo imágenes BMP para guardar datos ocultos.
- soportar otros tipos de archivos como portadores, como por ehemplo arhivos de
  audio o de video.
- soportar otros tipos de esteganografía fuera de los LSB implementados
- soportar otros tipos de encriptado. Por ejemplo se podría implementar cifrado
  asimétrico.
- soportar detección y extracción automática de un archivo portador
