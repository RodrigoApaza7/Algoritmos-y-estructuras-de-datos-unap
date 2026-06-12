import collections
import statistics
from dataclasses import dataclass
from enum import Enum
from typing import Optional, List, Dict, Any
import time
import random


# =====================================================================
# ACTIVIDAD 1: Estructuras de Datos Base (Expediente Académico UNAP)
# =====================================================================

class EstadoAcademico(Enum):
    REGULAR = "Regular"
    OBSERVADO = "Observado"
    RESERVADO = "Reservado"
    EGRESADO = "Egresado"


@dataclass
class Estudiante:
    codigo: int         # Código de 8 dígitos de la UNAP
    nombre: str
    escuela: str
    ppa: float          # Promedio Ponderado Acumulado (0.0 - 20.0)
    estado: EstadoAcademico

    def __post_init__(self):
        if not (10000000 <= self.codigo <= 99999999):
            raise ValueError("El código UNAP debe tener exactamente 8 dígitos.")
        if not (0.0 <= self.ppa <= 20.0):
            raise ValueError("El PPA debe estar en el rango de 0.0 a 20.0.")


@dataclass
class NodoBST:
    estudiante: Estudiante
    izquierdo: Optional['NodoBST'] = None
    derecho: Optional['NodoBST'] = None


# =====================================================================
# ACTIVIDADES 2, 3, 4 y 5: Clase ArbolAcademico Completa e Integrada
# =====================================================================

class ArbolAcademico:
    def __init__(self):
        self.raiz: Optional[NodoBST] = None

    # -----------------------------------------------------------------
    # MÉTODOS DE LA ACTIVIDAD 2: Inserción y Recorridos (DFS y BFS)
    # -----------------------------------------------------------------
    def insertar(self, estudiante: Estudiante) -> None:
        self.raiz = self._insertar(self.raiz, estudiante)

    def _insertar(self, nodo_actual: Optional[NodoBST], estudiante: Estudiante) -> NodoBST:
        if nodo_actual is None:
            return NodoBST(estudiante)

        if estudiante.codigo < nodo_actual.estudiante.codigo:
            nodo_actual.izquierdo = self._insertar(nodo_actual.izquierdo, estudiante)
        elif estudiante.codigo > nodo_actual.estudiante.codigo:
            nodo_actual.derecho = self._insertar(nodo_actual.derecho, estudiante)
        else:
            raise ValueError(f"El código {estudiante.codigo} ya está registrado en el árbol.")

        return nodo_actual

    def in_order(self) -> List[Estudiante]:
        resultados = []
        self._in_order(self.raiz, resultados)
        return resultados

    def _in_order(self, nodo: Optional[NodoBST], resultados: List[Estudiante]) -> None:
        if nodo is not None:
            self._in_order(nodo.izquierdo, resultados)
            resultados.append(nodo.estudiante)
            self._in_order(nodo.derecho, resultados)

    def pre_order(self) -> List[Estudiante]:
        resultados = []
        self._pre_order(self.raiz, resultados)
        return resultados

    def _pre_order(self, nodo: Optional[NodoBST], resultados: List[Estudiante]) -> None:
        if nodo is not None:
            resultados.append(nodo.estudiante)
            self._pre_order(nodo.izquierdo, resultados)
            self._pre_order(nodo.derecho, resultados)

    def bfs(self) -> List[Estudiante]:
        resultados = []
        if self.raiz is None:
            return resultados

        cola = collections.deque([self.raiz])
        while cola:
            nodo_actual = cola.popleft()
            resultados.append(nodo_actual.estudiante)

            if nodo_actual.izquierdo is not None:
                cola.append(nodo_actual.izquierdo)
            if nodo_actual.derecho is not None:
                cola.append(nodo_actual.derecho)

        return resultados

    # -----------------------------------------------------------------
    # MÉTODOS DE LA ACTIVIDAD 3: Búsqueda y Eliminación por Casos
    # -----------------------------------------------------------------
    def buscar(self, codigo: int) -> Optional[Estudiante]:
        nodo = self._buscar(self.raiz, codigo)
        return nodo.estudiante if nodo else None

    def _buscar(self, nodo_actual: Optional[NodoBST], codigo: int) -> Optional[NodoBST]:
        if nodo_actual is None or nodo_actual.estudiante.codigo == codigo:
            return nodo_actual

        if codigo < nodo_actual.estudiante.codigo:
            return self._buscar(nodo_actual.izquierdo, codigo)
        return self._buscar(nodo_actual.derecho, codigo)

    def eliminar(self, codigo: int) -> None:
        if self.buscar(codigo) is None:
            raise KeyError(f"El estudiante con código {codigo} no existe.")
        self.raiz = self._eliminar(self.raiz, codigo)

    def _eliminar(self, nodo_actual: Optional[NodoBST], codigo: int) -> Optional[NodoBST]:
        if nodo_actual is None:
            return None

        if codigo < nodo_actual.estudiante.codigo:
            nodo_actual.izquierdo = self._eliminar(nodo_actual.izquierdo, codigo)
        elif codigo > nodo_actual.estudiante.codigo:
            nodo_actual.derecho = self._eliminar(nodo_actual.derecho, codigo)
        else:
            # Caso 1 y 2: Cero o un hijo
            if nodo_actual.izquierdo is None:
                return nodo_actual.derecho
            elif nodo_actual.derecho is None:
                return nodo_actual.izquierdo

            # Caso 3: Dos hijos (Buscamos Sucesor In-Order)
            nodo_sucesor = self._minimo(nodo_actual.derecho)
            nodo_actual.estudiante = nodo_sucesor.estudiante
            nodo_actual.derecho = self._eliminar(nodo_actual.derecho, nodo_sucesor.estudiante.codigo)

        return nodo_actual

    def _minimo(self, nodo: NodoBST) -> NodoBST:
        actual = nodo
        while actual.izquierdo is not None:
            actual = actual.izquierdo
        return actual

    # -----------------------------------------------------------------
    # MÉTODOS DE LA ACTIVIDAD 4: Consultas y Filtros Estadísticos
    # -----------------------------------------------------------------
    def por_rango_ppa(self, min_ppa: float, max_ppa: float) -> List[Estudiante]:
        todos = self.in_order()
        return [e for e in todos if min_ppa <= e.ppa <= max_ppa]

    def por_escuela(self, escuela: str) -> List[Estudiante]:
        todos = self.in_order()
        return [e for e in todos if e.escuela.lower() == escuela.lower()]

    def por_estado(self, estado: EstadoAcademico) -> List[Estudiante]:
        todos = self.in_order()
        return [e for e in todos if e.estado == estado]

    def estadisticas(self) -> Dict[str, Any]:
        todos = self.in_order()
        if not todos:
            return {"total": 0, "media": 0.0, "max": 0.0, "min": 0.0, "stdev": 0.0}

        ppas = [e.ppa for e in todos]
        
        return {
            "total": len(todos),
            "media": round(statistics.mean(ppas), 2),
            "max": max(ppas),
            "min": min(ppas),
            "stdev": round(statistics.stdev(ppas), 2) if len(ppas) > 1 else 0.0
        }

    # -----------------------------------------------------------------
    # MÉTODOS DE LA ACTIVIDAD 5: Visualización de Estructura Jerárquica
    # -----------------------------------------------------------------
    def imprimir_arbol(self) -> None:
        if self.raiz is None:
            print("\n[!] El árbol está vacío actualmente.")
            return
        print("\n--- ESTRUCTURA VISUAL DEL ÁRBOL BINARIO (Rotado 90° CCW) ---")
        self._imprimir_arbol(self.raiz, 0)
        print("-" * 58)

    def _imprimir_arbol(self, nodo: Optional[NodoBST], espacio: int) -> None:
        DISTANCIA = 8
        if nodo is None:
            return

        espacio += DISTANCIA
        self._imprimir_arbol(nodo.derecho, espacio)

        print()
        for i in range(DISTANCIA, espacio):
            print(" ", end="")
        print(f"[{nodo.estudiante.codigo}]")

        self._imprimir_arbol(nodo.izquierdo, espacio)

def ejecutar_benchmark():
    # Asegúrate de tener las clases Estudiante y ArbolAcademico creadas antes
    tamanos = [100, 1000, 10000, 100000]
    
    print("Ejecutando Benchmark de Python...")
    for n in tamanos:
        # 1. Generar códigos aleatorios únicos para la UNAP
        codigos = random.sample(range(10000000, 99999999), n)
        estudiantes = [Estudiante(cod, f"Est-{cod}", "Sistemas", 15.0, EstadoAcademico.REGULAR) for cod in codigos]
        
        # --- BENCHMARK BST ---
        arbol_bench = ArbolAcademico()
        t0 = time.perf_counter()
        for est in estudiantes:
            arbol_bench.insertar(est)
        t_bst_ins = (time.perf_counter() - t0) * 1000 # Convertir a milisegundos
        
        # Medir búsqueda (buscamos el último elemento insertado para el peor de los casos promedio)
        codigo_buscar = codigos[-1]
        t0 = time.perf_counter()
        arbol_bench.buscar(codigo_buscar)
        t_bst_busq = (time.perf_counter() - t0) * 1000
        
        # Calcular altura del BST de forma iterativa o recursiva
        def calcular_altura(nodo):
            if nodo is None: return 0
            return 1 + max(calcular_altura(nodo.izquierdo), calcular_altura(nodo.derecho))
        altura_bst = calcular_altura(arbol_bench.raiz)
        
        # --- BENCHMARK DICCIONARIO ---
        dicc_bench = {}
        t0 = time.perf_counter()
        for est in estudiantes:
            dicc_bench[est.codigo] = est
        t_dict_ins = (time.perf_counter() - t0) * 1000
        
        t0 = time.perf_counter()
        _ = dicc_bench.get(codigo_buscar)
        t_dict_busq = (time.perf_counter() - t0) * 1000
        
        # Imprimir fila formateada lista para copiar a tu tabla
        print(f"N={n:6} | BST ins: {t_bst_ins:.2f}ms, busq: {t_bst_busq:.4f}ms | Dict ins: {t_dict_ins:.2f}ms, busq: {t_dict_busq:.4f}ms | Altura: {altura_bst}")

ejecutar_benchmark()


# =====================================================================
# INTERFAZ DE CONSOLA INTERACTIVA (Menú de Control)
# =====================================================================
def menu_interactivo():
    arbol = ArbolAcademico()

    # Pre-cargar los 7 estudiantes del laboratorio de forma automática para no empezar de cero
    estudiantes_iniciales = [
        Estudiante(20210300, "Apaza, Carlos", "Ingeniería de Sistemas", 14.5, EstadoAcademico.REGULAR),
        Estudiante(20200150, "Blanco, Ana", "Ingeniería de Sistemas", 11.2, EstadoAcademico.OBSERVADO),
        Estudiante(20220450, "Condori, Luis", "Ingeniería de Sistemas", 16.8, EstadoAcademico.REGULAR),
        Estudiante(20190050, "Mamani, Eva", "Ingeniería de Sistemas", 13.0, EstadoAcademico.RESERVADO),
        Estudiante(20210200, "Flores, Rene", "Ingeniería de Sistemas", 15.2, EstadoAcademico.REGULAR),
        Estudiante(20210400, "Quispe, Diana", "Ingeniería de Sistemas", 17.5, EstadoAcademico.REGULAR),
        Estudiante(20230600, "Zela, Hector", "Ingeniería de Sistemas", 12.1, EstadoAcademico.REGULAR)
    ]
    for est in estudiantes_iniciales:
        arbol.insertar(est)

    while True:
        print("\n=======================================================")
        print("    SISTEMA DE GESTIÓN DE EXPEDIENTES ACADÉMICOS UNAP  ")
        print("=======================================================")
        print(" 1. Insertar nuevo estudiante (Actividad 2)")
        print(" 2. Buscar estudiante por código (Actividad 3)")
        print(" 3. Eliminar estudiante por código (Actividad 3)")
        print(" 4. Ver consultas y filtros académicos (Actividad 4)")
        print(" 5. Ver reporte estadístico global (Actividad 4)")
        print(" 6. Mostrar estructura visual del BST (Actividad 5)")
        print(" 7. Listar todos los estudiantes (In-Order / BFS)")
        print(" 8. Salir de la aplicación")
        print("=======================================================")
        
        opcion = input("Seleccione una opción (1-8): ").strip()

        if opcion == "1":
            print("\n--- INSERTAR ESTUDIANTE ---")
            try:
                codigo = int(input("Código UNAP (8 dígitos): "))
                nombre = input("Nombre y Apellidos: ")
                escuela = input("Escuela Profesional: ")
                ppa = float(input("Promedio Ponderado Acumulado (0.0 - 20.0): "))
                print("Estados disponibles: 1.REGULAR, 2.OBSERVADO, 3.RESERVADO, 4.EGRESADO")
                est_opc = input("Seleccione número de estado: ").strip()
                estado_map = {"1": EstadoAcademico.REGULAR, "2": EstadoAcademico.OBSERVADO, 
                              "3": EstadoAcademico.RESERVADO, "4": EstadoAcademico.EGRESADO}
                estado = estado_map.get(est_opc, EstadoAcademico.REGULAR)

                nuevo_est = Estudiante(codigo, nombre, escuela, ppa, estado)
                arbol.insertar(nuevo_est)
                print("✓ Estudiante registrado exitosamente en el árbol.")
            except ValueError as ve:
                print(f"✗ Error de Validación: {ve}")

        elif opcion == "2":
            print("\n--- BUSCAR ESTUDIANTE ---")
            try:
                codigo = int(input("Ingrese código a buscar: "))
                est = arbol.buscar(codigo)
                if est:
                    print(f"\n[Registro Encontrado]")
                    print(f" Nombre   : {est.nombre}")
                    print(f" Escuela  : {est.escuela}")
                    print(f" PPA      : {est.ppa}")
                    print(f" Estado   : {est.estado.value}")
                else:
                    print("✗ No se encontró ningún estudiante con ese código.")
            except ValueError:
                print("✗ Entrada inválida. El código debe ser numérico.")

        elif opcion == "3":
            print("\n--- ELIMINAR ESTUDIANTE ---")
            try:
                codigo = int(input("Ingrese código a eliminar: "))
                arbol.eliminar(codigo)
                print(f"✓ El estudiante con código {codigo} fue removido del BST de forma correcta.")
            except KeyError as ke:
                print(ke)
            except ValueError:
                print("✗ Entrada inválida. El código debe ser numérico.")

        elif opcion == "4":
            print("\n--- CONSULTAS Y FILTROS ---")
            print("1. Filtrar por rango de PPA")
            print("2. Filtrar por Estado Académico")
            sub_opc = input("Seleccione filtro (1-2): ").strip()
            
            if sub_opc == "1":
                try:
                    min_p = float(input("PPA Mínimo: "))
                    max_p = float(input("PPA Máximo: "))
                    res = arbol.por_rango_ppa(min_p, max_p)
                    print(f"\nResultados ({len(res)} encontrados):")
                    for e in res: print(f"  - {e.nombre} | PPA: {e.ppa}")
                except ValueError: print("✗ Rango inválido.")
            elif sub_opc == "2":
                print("1.REGULAR, 2.OBSERVADO, 3.RESERVADO, 4.EGRESADO")
                est_opc = input("Seleccione número: ").strip()
                estado_map = {"1": EstadoAcademico.REGULAR, "2": EstadoAcademico.OBSERVADO, 
                              "3": EstadoAcademico.RESERVADO, "4": EstadoAcademico.EGRESADO}
                if est_opc in estado_map:
                    res = arbol.por_estado(estado_map[est_opc])
                    print(f"\nResultados ({len(res)} encontrados):")
                    for e in res: print(f"  - {e.nombre} | Código: {e.codigo}")
                else: print("✗ Opción inválida.")

        elif opcion == "5":
            print("\n--- REPORTE ESTADÍSTICO GENERAL ---")
            stats = arbol.estadisticas()
            print(f" Total de estudiantes matriculados : {stats['total']}")
            print(f" Media (Promedio general del PPA)  : {stats['media']}")
            print(f" PPA Máximo alcanzado              : {stats['max']}")
            print(f" PPA Mínimo registrado             : {stats['min']}")
            print(f" Desviación Estándar de la muestra : {stats['stdev']}")

        elif opcion == "6":
            arbol.imprimir_arbol()

        elif opcion == "7":
            print("\n--- LISTADO DE EXPEDIENTES ---")
            print("1. Ordenado ascendentemente (In-Order)")
            print("2. Por niveles jerárquicos (BFS)")
            sub_opc = input("Seleccione recorrido (1-2): ").strip()
            
            if sub_opc == "1":
                print("\n[Listado In-Order]")
                for e in arbol.in_order():
                    print(f"  Código: {e.codigo} | {e.nombre:<15} | PPA: {e.ppa} | {e.estado.value}")
            elif sub_opc == "2":
                print("\n[Listado BFS por Niveles]")
                for e in arbol.bfs():
                    print(f"  Código: {e.codigo} | {e.nombre}")

        elif opcion == "8":
            print("\nCerrando el sistema académico. ¡Hasta luego!")
            break
        else:
            print("[!] Opción no válida. Intente un número del 1 al 8.")


if __name__ == "__main__":
    menu_interactivo()