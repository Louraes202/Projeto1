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
typedef struct Hora {
    int hora;  // Hora do dia (0 a 23)
    int min;   // Minutos (0 a 59)
    int seg;   // Segundos (0 a 59)
} Hora;

// Representa uma tarifa do parque de estacionamento.
typedef struct tarifa {
    char nome[10];       // Nome descritivo da tarifa (ex: "Diurna")
    char cod_tarifa[4];  // Código identificador da tarifa (ex: "CT1")
    float valor_hora;    // Valor por hora da tarifa
    Hora inicio;      // Hora de início da tarifa
    Hora fim;         // Hora de fim da tarifa
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
    char fila;               // Identificador da fila
    int lugar;               // Número do lugar
    char estado;             // 'L' para livre, 'O' para ocupado
    char codigo[MAX_CODIGO]; // Código único do estacionamento (se ocupado)
    int num_piso;            // Piso onde o lugar está localizado
} Lugar;

// Representa um piso do parque de estacionamento.
typedef struct piso {
    int livres;                    // Lugares livres
    int ocupados;                  // Lugares ocupados
    int indisponiveis;             // Lugares indisponíveis
    int num_filas;                 // Número de filas no piso
    int lugares_por_fila[MAX_FILAS]; // Lugares por fila (tamanho variável por fila)
    Lugar lugares[MAX_FILAS][MAX_LUGARES]; // Matriz de lugares (baseada no número de filas e lugares por fila)
} Piso;

// Representa o parque de estacionamento como um todo.
typedef struct parque {
    char nome[30];                 // Nome do parque
    char morada[50];               // Morada do parque
    int num_pisos;                 // Total de pisos
    Piso pisos[MAX_PISOS];         // Array de pisos
    int total_lugares;             // Total de lugares no parque
    int lugares_ocupados;          // Total de lugares ocupados
    int lugares_livres;            // Total de lugares livres
} Parque;


// Representa um registo de estacionamento de um veículo.
typedef struct estacionamento {
    int numE;                      // Número do registo
    char matricula[MAX_MATRICULA]; // Matrícula do veículo
    Data data_entrada;             // Data de entrada
    Data data_saida;               // Data de saída (se aplicável)
    Hora entrada;               // Hora de entrada
    Hora saida;                 // Hora de saída (se aplicável)
    Lugar lugar;                   // Lugar ocupado
    float valor_pago;              // Valor pago (se aplicável)
    char observacoes[50];          // Observações adicionais
} Estacionamento;

// Declaração de funções
void configurar_parque(Parque *parque);
void gravar_configuracao_parque(const Parque *parque);
int carregar_configuracao_parque(Parque *parque);
void visualizar_dados_parque(const Parque *parque);
int carregar_tarifas(Tarifario *tarifario);
int carregar_registos(Estacionamento estacionamentos[], int *total_estacionamentos, Tarifario *tarifario);
float calcular_valor_pago(const Estacionamento *estacionamento, const Tarifario *tarifario);

// Funções para persistência e carregamento
int carregar_estado_binario(Parque *parque, Estacionamento estacionamentos[], int *total_estacionamentos, int *ultimo_id);
void guardar_estado_binario(const Parque *parque, Estacionamento estacionamentos[], int total_estacionamentos);


// Função para carregar registos de ficheiro .txt
int carregar_registos_txt(const char *nome_ficheiro, Estacionamento estacionamentos[], int *total_estacionamentos, Parque *parque);

// Função para limpar memória e ficheiros
void limpar_memoria(Parque *parque, Estacionamento estacionamentos[], int *total_estacionamentos);

// Função para exibir lugares por pisos no menu
void exibir_lugares_por_piso(const Parque *parque);

//
void exibir_estatisticas_ocupacao(const Parque *parque);

//
void recalcular_estatisticas_parque(Parque *parque);

//
void registar_entrada_veiculo(Parque *parque, Estacionamento estacionamentos[], int *total_estacionamentos, int *ultimo_id);

//
void configurar_pisos(Parque *parque);

#endif
