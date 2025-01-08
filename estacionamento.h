#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Valores máximos para determinados parâmetros
#define MAX_FILAS 26
#define MAX_LUGARES 50
#define MAX_PISOS 5
#define MAX_TARIFAS 10
#define MAX_MATRICULA 10
#define MAX_CODIGO 10
#define MAX_ESTACIONAMENTOS 100

// Representa uma data no formato dia, mês e ano.
typedef struct data {
    int dia;  // Dia do mês (1 a 31)
    int mes;  // Mês do ano (1 a 12)
    int ano;  // Ano (4 dígitos)
} Data;

// Representa um horário no formato hora, minuto e segundo.
typedef struct horario {
    int hora;  // Hora do dia (0 a 23)
    int min;   // Minutos (0 a 59)
    int seg;   // Segundos (0 a 59)
} Horario;

// Representa uma tarifa do parque de estacionamento.
typedef struct tarifa {
    char nome[10];       // Nome descritivo da tarifa (ex: "Diurna")
    char cod_tarifa[4];  // Código identificador da tarifa (ex: "CT1")
    float valor_hora;    // Valor por hora da tarifa
    Horario inicio;      // Hora de início da tarifa
    Horario fim;         // Hora de fim da tarifa
    char tp_tarifa;      // Tipo da tarifa ('H' para horária, 'D' para diária)
    int dias;            // Número de dias aplicável (usado apenas para tarifas diárias)
} Tarifa;

// Representa o tarifário completo com todas as tarifas disponíveis no parque.
typedef struct tarifario {
    Tarifa *lista_tarifas;  // Array dinâmico de tarifas
    int num_tarifas;        // Número total de tarifas na lista
} Tarifario;

// Representa um lugar de estacionamento individual.
typedef struct lugar {
    char fila;             // Identificador da fila (A a Z)
    int lugar;             // Número do lugar na fila (1 a MAX_LUGARES)
    char estado;           // Estado do lugar: 'L' (livre), 'O' (ocupado), 'I' (indisponível)
    char codigo[MAX_CODIGO];  // Código único do lugar (ex: "1A01")
    int num_piso;          // Número do piso onde o lugar está localizado (1 a MAX_PISOS)
} Lugar;

// Representa um piso do parque de estacionamento.
typedef struct piso {
    int numero;                          // Número do piso (1 a MAX_PISOS)
    Lugar lugares[MAX_FILAS][MAX_LUGARES]; // Array bidimensional de lugares organizados por fila
    int livres;                          // Número total de lugares livres no piso
    int ocupados;                        // Número total de lugares ocupados no piso
    int indisponiveis;                   // Número total de lugares indisponíveis no piso
} Piso;

// Representa o parque de estacionamento como um todo.
typedef struct parque {
    Piso pisos[MAX_PISOS];               // Array de pisos do parque
    int num_pisos;                       // Número total de pisos no parque
    char nome[10];                       // Nome do parque
    char morada[30];                     // Morada do parque
    int total_lugares;                   // Número total de lugares no parque
    int lugares_ocupados;                // Número total de lugares ocupados
    int lugares_livres;                  // Número total de lugares livres
    int filas;                           // Número de filas por piso
    int lugares_por_fila;                // Número de lugares por fila
    Tarifario tarifa_parque;             // Tarifário associado ao parque
} Parque;

// Representa um registo de estacionamento de um veículo.
typedef struct estacionamento {
    int numE;                     // Número sequencial do registo de estacionamento
    char matricula[MAX_MATRICULA]; // Matrícula do veículo (ex: "AA-00-BB")
    Data data_entrada;            // Data de entrada no parque
    Data data_saida;              // Data de saída do parque (se aplicável)
    Horario entrada;              // Horário de entrada no parque
    Horario saida;                // Horário de saída do parque (se aplicável)
    Lugar lugar;                  // Lugar de estacionamento atribuído ao veículo
    float valor_pago;             // Valor pago pelo estacionamento
    char observacoes[50];         // Observações ou notas sobre o registo
} Estacionamento;

// Declarações de funções
void configurar_parque(Parque *parque);
void gravar_configuracao_parque(const Parque *parque);
int carregar_configuracao_parque(Parque *parque);
void visualizar_dados_parque(const Parque *parque);

#endif
