#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#define MAX_FILAS 26
#define MAX_LUGARES 50
#define MAX_PISOS 5

// Estruturas principais
typedef struct {
    char fila;
    int lugar;
    char estado;  // 'L', 'X', 'I'
    char motivo;
} Lugar;

typedef struct {
    int numero;
    Lugar lugares[MAX_FILAS][MAX_LUGARES];
    int livres;
    int ocupados;
    int indisponiveis;
} Piso;

typedef struct {
    Piso pisos[MAX_PISOS];
    int num_pisos;
} Parque;

typedef struct {
    float tarifa_diurna;
    float tarifa_noturna;
    float tarifa_diaria;
    float tarifa_multidia;
} Tarifario;

// Prototipos
void configurar_parque(Parque *parque);
void configurar_tarifario(Tarifario *tarifario);
void gravar_configuracao_parque(const Parque *parque);
void gravar_tarifario(const Tarifario *tarifario);

#endif
