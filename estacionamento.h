#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILAS 26
#define MAX_LUGARES 50
#define MAX_PISOS 5
#define MAX_TARIFAS 10

typedef struct horario {
    int hora;
    int min;
    int seg;
} Horario;

typedef struct tarifa {
    char nome[10];
    char cod_tarifa[4];
    float valor_hora;
    Horario inicio;
    Horario fim;
    char tp_tarifa;
    int dias;
} Tarifa;

typedef struct tarifario {
    Tarifa *lista_tarifas;
    int num_tarifas;
} Tarifario;

typedef struct lugar {
    char fila;
    int lugar;
    char estado;
    char codigo[10];
    int num_piso;
} Lugar;

typedef struct piso {
    int numero;
    Lugar lugares[MAX_FILAS][MAX_LUGARES];
    int livres;
    int ocupados;
    int indisponiveis;
} Piso;

typedef struct parque {
    Piso pisos[MAX_PISOS];
    int num_pisos;
    char nome[10];
    char morada[30];
    int total_lugares;
    int lugares_ocupados;
    int lugares_livres;
} Parque;

typedef struct estacionamento {
    int numE;
    char matricula[MAX_MATRICULA_L];
    Data data_entrada;
    Horario hora_entrada;
    Lugar lugar;
    Data data_saida;
    Horario hora_saida;
    float valor_pago;
    char observacoes[30];
} Estacionamento;

void configurar_parque(Parque *parque);
void configurar_piso(Piso *piso, int numero, int *total_lugares);
void configurar_tarifario(Tarifario *tarifa_parque);
void gravar_configuracao_parque(const Parque *parque);
int carregar_configuracao_parque(Parque *parque);
void gravar_tarifario(const Tarifario *tarifa_parque);
int carregar_tarifario(Tarifario *tarifa_parque);

#endif
