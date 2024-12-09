 #include <stdio.h>

#define MAX_FILAS 26
#define MAX_LUGARES 50
#define MAX_PISOS 5
#define MAX_MATRICULA_L 10
#define MAX_CODIGO 5

typedef struct lugar{
    char fila;
    int lugar;
    char estado;
    char motivo[20];
    char codigo[4];
    int num_piso;
} Lugar;

typedef struct piso{
    int numero;
    Lugar lugares[MAX_FILAS][MAX_LUGARES];
    int livres;
    int ocupados;
    int indisponiveis;
} Piso;

typedef struct parque{
    Piso pisos[MAX_PISOS];
    int num_pisos;
    char nome[10];
    char morada[30];
    int total_lugares;
    int lugares_ocupados;
    int lugares_livres;
    Tarifario tarifa_parque;
} Parque;

typedef struct tarifario{
    Tarifa *lista_tarifas;
} Tarifario;

typedef struct tarifa{
	char nome[10];
	char id[4];
	int periodo;
	float valor_hora;
	Horario inicio;
	Horario fim;
	char tp_tarifa; //H de horario ou D se for diario
	char cod_tarifa[3];
	int dias; //T1 e T2=0 T3=1 T4=-1(porque nao ha limite)
}Tarifa;

typedef struct horario{
	int hora;
	int min;
	int seg;
}Horario;

typedef struct data{
	int dia;
	int mes;
	int ano;
}Data;

typedef struct relatorio_diario{
    int dia;
    int mes;
    int ano;
    int num_veiculos;  // Número total de veículos
    float total_pago;  // Valor total pago no dia
} Relatorio_Diario;

typedef estacionamento{
	int numE;
	char *matricula;
	Data data_entrada;
	Horario hora_entrada;
	Lugar lugar;
	Data data_saida;
	Horario hora_saida;
	float valor_pago;
	char observacoes[30];
}Estacionamento;
