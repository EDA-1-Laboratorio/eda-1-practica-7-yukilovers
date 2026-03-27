/*
 * PRACTICA: Lista Simple - Sistema de Logs de Errores
 * ====================================================
 *
 * Descripcion:
 *   Se simula la recepcion de logs de errores durante un periodo de tiempo.
 *   Cada error tiene un ID que comienza con una letra mayuscula (A-Z):
 *     - Letras A-E: error CRITICO
 *     - Letras F-Z: error NO CRITICO
 *   Cada error tiene ademas una prioridad numerica (float de 0.0 a 10.0).
 *
 * Objetivo:
 *   Implementar las funciones marcadas con TODO para que los errores se
 *   almacenen en una lista simple ordenada por:
 *     1. Letra inicial del ID (orden alfabetico ascendente)
 *     2. Prioridad numerica (de mayor a menor) cuando la letra es la misma
 *
 * Compilacion:
 *   gcc -o practica_logs practica_logs.c -lm
 *
 * Ejecucion:
 *   ./practica_logs
 */

#include "error_log.h"

/* =====================================================================
 * FUNCIONES PROPORCIONADAS (no modificar)
 * =====================================================================*/

ErrorLog *generar_error_aleatorio(void)
{
    ErrorLog *nuevo = (ErrorLog *)malloc(sizeof(ErrorLog));
    if (!nuevo) {
        fprintf(stderr, "Error: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }

    char letra = 'A' + (rand() % 26);
    int numero = rand() % 10000;
    sprintf(nuevo->id, "%c%04d", letra, numero);

    nuevo->prioridad = (float)(rand() % 1001) / 100.0f;  /* 0.00 - 10.00 */
    nuevo->es_critico = (letra <= UMBRAL_CRITICO) ? 1 : 0;
    nuevo->sig = NULL;

    return nuevo;
}

void imprimir_lista(ErrorLog *cabeza)
{
    ErrorLog *actual = cabeza;
    int i = 1;

    printf("\n%-4s %-10s %-10s %-10s\n", "#", "ID", "Prioridad", "Tipo");
    printf("--------------------------------------\n");

    while (actual != NULL) {
        printf("%-4d %-10s %-10.2f %-10s\n",
               i++,
               actual->id,
               actual->prioridad,
               actual->es_critico ? "CRITICO" : "normal");
        actual = actual->sig;
    }
    printf("--------------------------------------\n");
}

void liberar_lista(ErrorLog *cabeza)
{
    ErrorLog *tmp;
    while (cabeza != NULL) {
        tmp = cabeza;
        cabeza = cabeza->sig;
        free(tmp);
    }
}

/* =====================================================================
 * FUNCIONES A IMPLEMENTAR
 * =====================================================================*/

/*
 * TODO: Implementar insertar_ordenado
 *
 * Inserta 'nuevo' en la lista apuntada por *cabeza de forma que la lista
 * se mantenga ordenada:
 *   - Primero por la letra inicial del ID (A < B < C < ...)
 *   - Si dos errores comparten la misma letra, el de MAYOR prioridad
 *     va primero.
 *
 * Pista: recorre la lista buscando la posicion correcta. Necesitaras un
 * puntero al nodo anterior para reconectar los enlaces.
 */
void insertar_ordenado(ErrorLog **cabeza, ErrorLog *nuevo)
{
    /* ESCRIBE TU CODIGO AQUI */
    ErrorLog *actual = *cabeza;
    ErrorLog *anterior = NULL;

    // Recorrer hasta encontrar la posición correcta
    while (actual != NULL) {
        // Criterio 1: Letra del ID (A < B < C...)
        if (nuevo->id[0] < actual->id[0]) break;

        // Criterio 2: Misma letra, mayor prioridad primero
        if (nuevo->id[0] == actual->id[0] && nuevo->prioridad > actual->prioridad) break;

        anterior = actual;
        actual = actual->sig;
    }

    // Insertar el nodo
    nuevo->sig = actual;
    if (anterior == NULL) {
        *cabeza = nuevo; // Insertar al inicio o lista vacía
    } else {
        anterior->sig = nuevo; // Insertar en medio o al final
    }
}

/*
 * TODO: Implementar contar_criticos
 *
 * Recorre la lista y cuenta los nodos cuyo campo es_critico sea 1.
 */
int contar_criticos(ErrorLog *cabeza)
{
    /* ESCRIBE TU CODIGO AQUI */
    int contador = 0;
    while (cabeza != NULL) {
        if (cabeza->es_critico) contador++;
        cabeza = cabeza->sig;
    }
    return contador;
}


/*
 * TODO: Implementar contar_no_criticos
 *
 * Recorre la lista y cuenta los nodos cuyo campo es_critico sea 0.
 */
int contar_no_criticos(ErrorLog *cabeza)
{
    /* ESCRIBE TU CODIGO AQUI */
    int contador = 0;
    while (cabeza != NULL) {
        if (!cabeza->es_critico) contador++;
        cabeza = cabeza->sig;
    }
    return contador;
    return 0;
}

/*
 * TODO: Implementar eliminar_por_prioridad
 *
 * Elimina de la lista todos los nodos cuya prioridad sea estrictamente
 * menor que 'umbral'. Libera la memoria de cada nodo eliminado.
 * Retorna la nueva cabeza de la lista (puede cambiar si se eliminan
 * nodos al inicio).
 */
ErrorLog *eliminar_por_prioridad(ErrorLog *cabeza, float umbral)
{
    /* ESCRIBE TU CODIGO AQUI */
    ErrorLog *actual = cabeza;
    ErrorLog *anterior = NULL;

    while (actual != NULL) {
        if (actual->prioridad < umbral) {
            ErrorLog *a_eliminar = actual;
            if (anterior == NULL) {
                cabeza = actual->sig; // Elminar el primero
            } else {
                anterior->sig = actual->sig; // Reconectar
            }
            actual = actual->sig;
            free(a_eliminar);
        } else {
            anterior = actual;
            actual = actual->sig;
        }
    }
    return cabeza;
}

/*
 * TODO: Implementar conservar_mayor_no_critico
 *
 * Conserva todos los errores criticos (A-E) y unicamente el error
 * no critico (F-Z) que tenga la mayor prioridad numerica.
 * Elimina y libera todos los demas errores no criticos.
 * Retorna la nueva cabeza de la lista.
 *
 * Pista: primero encuentra cual es el nodo no critico con mayor
 * prioridad, y luego recorre la lista eliminando los demas no criticos.
 */
ErrorLog *conservar_mayor_no_critico(ErrorLog *cabeza)
{
    /* ESCRIBE TU CODIGO AQUI */
    if (cabeza == NULL) return NULL;

    // Fase 1: Encontrar el no crítico con mayor prioridad
    ErrorLog *max_nc = NULL;
    ErrorLog *aux = cabeza;
    while (aux != NULL) {
        if (!aux->es_critico) {
            if (max_nc == NULL || aux->prioridad > max_nc->prioridad) {
                max_nc = aux;
            }
        }
        aux = aux->sig;
    }

    // Si no hay errores no críticos, no hay nada que borrar
    if (max_nc == NULL) return cabeza;

    // Fase 2: Eliminar todos los no críticos excepto max_nc
    ErrorLog *actual = cabeza;
    ErrorLog *anterior = NULL;
    while (actual != NULL) {
        // Si es no crítico pero NO es el máximo encontrado
        if (!actual->es_critico && actual != max_nc) {
            ErrorLog *a_eliminar = actual;
            if (anterior == NULL) {
                cabeza = actual->sig;
            } else {
                anterior->sig = actual->sig;
            }
            actual = actual->sig;
            free(a_eliminar);
        } else {
            anterior = actual;
            actual = actual->sig;
        }
    }
    return cabeza;
}

/* =====================================================================
 * PROGRAMA PRINCIPAL
 * =====================================================================*/

int main(void)
{
    ErrorLog *lista = NULL;
    int total_errores;
    int i;

    srand((unsigned)time(NULL));

    /* Numero aleatorio de errores entre 10 y 25 */
    total_errores = 10 + rand() % 16;

    printf("=== SISTEMA DE LOGS DE ERRORES ===\n");
    printf("Simulando recepcion de %d errores...\n\n", total_errores);

    /* Simular la llegada de errores uno por uno */
    for (i = 0; i < total_errores; i++) {
        ErrorLog *nuevo = generar_error_aleatorio();
        printf("  Recibido: [%s] prioridad=%.2f (%s)\n",
               nuevo->id, nuevo->prioridad,
               nuevo->es_critico ? "CRITICO" : "normal");

        insertar_ordenado(&lista, nuevo);
    }

    /* Mostrar la lista ordenada */
    printf("\n=== LISTA ORDENADA DE ERRORES ===");
    imprimir_lista(lista);

    /* Estadisticas */
    printf("\nEstadisticas:\n");
    printf("  Errores criticos    (A-E): %d\n", contar_criticos(lista));
    printf("  Errores no criticos (F-Z): %d\n", contar_no_criticos(lista));
    printf("  Total:                     %d\n", total_errores);

    /* Filtrar errores de baja prioridad */
    float umbral = 3.0f;
    printf("\n=== ELIMINANDO ERRORES CON PRIORIDAD < %.1f ===\n", umbral);
    lista = eliminar_por_prioridad(lista, umbral);

    printf("\n=== LISTA DESPUES DEL FILTRADO ===");
    imprimir_lista(lista);

    printf("\nEstadisticas post-filtrado:\n");
    printf("  Errores criticos    (A-E): %d\n", contar_criticos(lista));
    printf("  Errores no criticos (F-Z): %d\n", contar_no_criticos(lista));

    /* Conservar solo el error no critico de mayor prioridad */
    printf("\n=== CONSERVANDO SOLO EL MAYOR ERROR NO CRITICO ===\n");
    lista = conservar_mayor_no_critico(lista);

    printf("\n=== LISTA FINAL ===");
    imprimir_lista(lista);

    printf("\nEstadisticas finales:\n");
    printf("  Errores criticos    (A-E): %d\n", contar_criticos(lista));
    printf("  Errores no criticos (F-Z): %d\n", contar_no_criticos(lista));

    liberar_lista(lista);

    return 0;
}
