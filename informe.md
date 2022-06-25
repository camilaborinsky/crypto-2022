# Trabajo Práctico: Esteganografía

1. Discutir los aspectos relativos al documento
   a. Organización formal del documento
   b. La descripción del algoritmo.
   c. La notación utilizada, ¿es clara? ¿hay algún error o contradicción?

La organización formal del documento es académica y apropiada. Primero introduce el tema, brindando la idea básica de la técnica y un poco de historia y los conceptos, luego sigue la explicación de cómo se realizó la implementación y por último la sección con los resultados empíricos y análisis de los mismos.

En cuanto a la claridad en la descripción del algoritmo, creemos que no se logró plasmar la idea del mismo de forma simple. Necesitamos varias leídas del mismo para entender verdaderamente cómo era la implementación. Hubiera servido tener alguna figura que tal vez ayudaba a la comprensión y no solo la explicación en prosa.

La notación utilizada es escasa, se explica bastante con palabras, en este sentido creo que ayuda un poco a la lectura porque cuando las explicaciones están muy cargadas con notación se vuelven muy difíciles de seguir. 

No encontramos contradicciones pero encontramos un error pequeño en la primera línea del 3er párrafo en la explicación del First Scheme en la página 3 del pdf publicado por la cátedra (página 2 del paper). El error es simplemente un typo en la enumeración de los patrones pues dice "(00, 10, 10, 11)" donde entendemos debería decir (00, 01, 10, 11).

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
Otra opción sería enviar los patrones por otro canal o de otra forma y que actúe como una suerte de key, aunque no sería muy segura al solo haber 2^4 posibilidades. Es fácilemente atacable por fuerza bruta.

9. Leer el Segundo esquema y analizar (sin implementar) cuáles serían las ventajas que
   pueden verse.

La ventaja que presenta este nuevo esquema es que ahora la imagen con payload oculto es aún más parecida a la imagen original que para el primer esquema. Esto ocurre porque no solo se trabaja con los 4 patrones que forma el 2do y 3er LSB sino que también se abre cada patrón para considerar si se debería invertir los casos donde había un 0 originalemente en la imagen o un 1. Tenemos ahora 8 patrones, lo que nos da mayor calidad de ocultamiento.

No estamos seguros por qué en el 3er párrafo de esta sección en su 4ta línea habla de tener que guardar un máximo de 8 patrones, cuando nos parece que siempre se deberían guardar 8 patrones al tener que guardar un 0 o un 1 siempre. De otra forma no sabríamos si hay que invertir o no al no tener valor alguno, ni sabríamos cuál es el patrón que se saltearon si no tenemos 8 valores.

10. Leer el Segundo esquema e indicar qué desventajas o inconvenientes podría tener su
    implementación.

La primera que encontramos no es tanto de implementación sino de su uso por el hecho de la necesidad de que el destinatario de nuestro archivo con payload oculto tenga en su posesión también la imagen original. Sería un problema grande en casos en los que no sepamos la identidad con seguridad del destinatario y enviarle 2 veces una misma imagen por un canal público (pues nos parece no tendría mucha gracia ocultar información que se le enviaría directamente a un destinatario de manera segura) llamaría la atención y daría una oportunidad fácil de aprovechar de comparar los dos archivos lado a lado.

11. ¿Qué dificultades encontraron en la implementación del algoritmo del paper?

12. ¿Qué mejoras o futuras extensiones harías al programa stegobmp?

Algunas mejoras que se nos ocurran son:

- Soportar otros tipos de archivos como portadores. No trabajar solo con bmp, sino también otro formato de imágenes o ni siquiera imágenes. La dificultad está en atarse a los formatos de cada tipo de archivo (por ejemplo el parseo del header que hacemos sobre bmps)
- Soportar otras versiones de imágenes BMP como portadores y no solo la 3. Deberíamos estar atentos a las diferencias del formato.
- Soportar archivos BMP comprimidos como portadores. Deberíamos estar atentos a las diferencias del formato.
- Implementar detección automática del LSB usado y extracción automática del payload.
- Soportar otras técnicas de esteganografía además de los LSB implementados.
- Soportar encriptación de a bloques, actualmente requiere guardar todo el archivo en memoria (el no encriptado).

