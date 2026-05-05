# Instrucciones para ejecutar el programa

1. Asegúrate de que la estructura de carpetas sea la siguiente:

```
PRACTICAS/PROGRAMAS/C++/
├── codigo_hash_2.cpp
├── DATOS/
│   └── kaggle/
│       └── kaggle_datos.csv
```

2. Compila el programa desde la carpeta `C++`:

```
g++ codigo_hash_2.cpp -o output/codigo_hash_2.exe
```

3. Ejecuta el programa desde la carpeta `C++` (no desde `output`):

```
output/codigo_hash_2.exe
```

Así, el programa podrá encontrar el archivo CSV usando la ruta relativa `DATOS/kaggle/kaggle_datos.csv`.

Si cambias la ubicación del archivo CSV, actualiza la ruta en el código o mantén la estructura indicada.
