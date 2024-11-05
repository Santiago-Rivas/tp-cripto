# Trabajo Práctico De Implementación: Esteganografía

Este proyecto implementa una herramienta de esteganografía que permite ocultar información dentro de imágenes BMP.
La aplicación facilita la inserción de archivos en imágenes portadoras sin alterar su apariencia visual, permitiendo almacenar datos de forma discreta y recuperar la información oculta cuando se requiera.
Además, incluye opciones de encriptación para aumentar la seguridad del contenido oculto, haciéndolo accesible solo para quienes tengan la clave de acceso.

## Requisitos

- make
- gcc
- bash

## Compilacion

Para compilar el programa utilizar el siguiente comando:

```bash
make
```

Para elminar el ejecutable y codigo objeto se puede utilizar el comando:

```
make clean
```

## Ejecución

El comando `make` creara un directorio `bin` donde se encontrara el ejecutable `stegobmp`.

```bash
./stegobmp -MODE [-in filename] -p filename -out filename -steg <LSB1|LSB4|LSBI> [-a <aes128|aes192|aes256|des>] [-m <ecb|cfb|ofb|cbc>] [-pass password] [-help]
```

### Parametros

El primer parametro a ingresar indica el modo en que operara el programa.

| Modos    | Descripcion                             |
|----------|-----------------------------------------|
| -embed   | El programa ocultara un archivo         |
| -extract | El programa obtendra un archivo oculto  |

A partir de el modo de operacion se podran pasar los siguientes parametros para especificar la operacion del programa.

| Parametro                        | Descripción                                                                         |
|----------------------------------|-------------------------------------------------------------------------------------|
| -in filename                     | Archivo a ocultar (solo para el modo embed)                                         |
| -p filename                      | Archivo bmp portador                                                                |
| -out filename                    | Nombre del archivo de salida                                                        |
| -steg <LSB1\|LSB4\|LSBI>         | Algoritmo de esteganografiado: LSB de 1 bit, LSB de 4 bits, LSB Improved            |
| -a <aes128\|aes192\|aes256\|des> | Algoritmo de encripción: aes128, aes192, aes256, 3des. Default: aes128              |
| -m <ecb\|cfb\|ofb\|cbc>          | Modo de encripción: ecb, cfb, ofb, cbc. Default: cbc                                |
| -pass password                   | Contraseña de encripción. Si no se pasa este parametro no se realiza esteganografía |
| -help                            | Imprime ayuda del programa                                                          |

### Ejemplo de Uso

Para oculatar un archivo (sin encripcion):

```bash
./bin/stegobmp\
    -embed\
    -in "img/files_to_embed/image.jpg"\
    -p "img/samples/ejemplo.bmp" \
    -out "ejemplo_image_LSB1.bmp"\
    -steg "LSB1"
```

Para extraer un archivo (sin encripcion):

```bash
./bin/stegobmp\
    -extract\
    -p "ejemplo_image_LSB1.bmp"\
    -out "ejemplo_image_LSB1_extracted"\
    -steg "LSB1"
```

Para oculatar un archivo (con encripcion):

```bash
./bin/stegobmp\
    -embed\
    -in "img/files_to_embed/image.jpg"\
    -p "img/samples/ejemplo.bmp"\
    -out "ejemplo_image_LSBI_enc.bmp"\
    -pass "hola"\
    -steg "LSBI"\
    -a "aes192"\
    -m "cfb"
```

Para extraer un archivo (con encripcion):

```bash
./bin/stegobmp\
    -extract\
    -p "ejemplo_image_LSBI_enc.bmp"\
    -out "ejemplo_image_LSBI_enc_extracted"\
    -pass "hola"\
    -steg "LSBI"\
    -a "aes192"\
    -m "cfb"
```
