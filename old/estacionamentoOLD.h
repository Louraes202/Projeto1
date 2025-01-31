#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILAS 26
#define MAX_LUGARES 50
#define MAX_PISOS 5
#define MAX_TARIFAS 10
#define MAX_MATRICULA 10
#define MAX_CODIGO 10
#define MAX_ESTACIONAMENTOS 100
#define MAX_PARQUES 10


// Definição da estrutura Data
typedef struct data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    Parque parques[MAX_PARQUES];
    int total_parques;
} GestaoParques;

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
    char estado; // 'L' para livre, 'O' para ocupado, 'I' para indisponível
    char codigo[MAX_CODIGO];
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

typedef struct estacionamento {
    int numE;
    char matricula[MAX_MATRICULA];
    Data data_entrada;
    Data data_saida;
    Horario entrada;
    Horario saida;
    Lugar lugar;
    float valor_pago;
    char observacoes[50];
} Estacionamento;

// Funções relacionadas ao parque
void configurar_parque(Parque *parque);
void configurar_piso(Piso *piso, int numero, int *total_lugares);
void gravar_configuracao_parque(const Parque *parque);
int carregar_configuracao_parque(Parque *parque);

// Funções relacionadas ao tarifário
void configurar_tarifario(Tarifario *tarifa_parque);
void gravar_tarifario(const Tarifario *tarifa_parque);
int carregar_tarifario(Tarifario *tarifa_parque);

// Funções relacionadas aos registos de estacionamento
int carregar_estacionamentos(const char *filename, Estacionamento *estacionamentos, int *total_estacionamentos, Parque *parque);
void gravar_estacionamentos(const char *filename, Estacionamento *estacionamentos, int total_estacionamentos);

// Funções relacionadas à gestão de entradas e saídas do parque
void registar_entrada(Parque *parque, const char *matricula, int piso, char fila, int lugar, Horario entrada);
void registar_saida(Parque *parque, const char *matricula, Horario saida, const Tarifario *tarifario);

// Função para mostrar o mapa de ocupação de um piso
void mostrar_ocupacao_piso(const Piso *piso);

// Função para alterar o lugar ocupado por um veículo
void alterar_lugar(Parque *parque, const char *matricula, int novo_piso, char nova_fila, int novo_lugar);

// Função para especificar lugares indisponíveis
void especificar_lugar_indisponivel(Parque *parque, int piso, char fila, int lugar, char motivo);



#endif
