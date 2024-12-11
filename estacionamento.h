#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILAS 26
#define MAX_LUGARES 50
#define MAX_PISOS 5
#define MAX_MATRICULA_L 10
#define MAX_CODIGO 5

typedef struct horario {
    int hora;
    int min;
    int seg;
} Horario;

typedef struct tarifa {
    char nome[10];
    char id[4];
    int periodo;
    float valor_hora;
    Horario inicio;
    Horario fim;
    char tp_tarifa;
    char cod_tarifa[3];
    int dias;
} Tarifa;

typedef struct tarifario {
    Tarifa *lista_tarifas;
} Tarifario;

typedef struct lugar {
    char fila;
    int lugar;
    char estado;
    char motivo[20];
    char codigo[4];
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
    Tarifario tarifa_parque;
} Parque;

typedef struct data {
    int dia;
    int mes;
    int ano;
} Data;

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

#endif
