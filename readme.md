# Verificação de requisitos

## **Verificação dos Requisitos da Alínea 1b**

1. **Leitura do ficheiro `Tarifas.txt` e armazenamento em memória:**
    - **Estado:** ✅ Implementado.
    - **Função:** `carregar_tarifas`.
    - **Descrição:** Lê as tarifas do ficheiro `Tarifas.txt` e armazena-as na estrutura `Tarifario`.
2. **Leitura do ficheiro `Estacionamentos.txt` e processamento dos registos:**
    - **Estado:** ✅ Implementado.
    - **Função:** `carregar_registos`.
    - **Descrição:** Lê os registos do ficheiro `Estacionamentos.txt`, identifica se têm hora de saída definida, e:
        - Calcula o valor pago para registos completos.
        - Marca registos incompletos como "veículos ainda no parque".
3. **Integração com os dados em memória (ocupação do parque):**
    - **Estado:** ✅ Implementado.
    - **Descrição:** Veículos sem hora de saída são considerados na ocupação do parque, garantindo que refletem o estado atual do estacionamento.

---

### **Verificação dos Requisitos da Alínea 1c**

1. **Estruturas de Dados em Memória:**
    - **Estado:** ✅ Implementado.
    - **Descrição:** A estrutura `Estacionamento` e o array `estacionamentos[]` armazenam todos os registos. A numeração dos registos e o estado de ocupação do parque estão devidamente integrados.
2. **Persistência dos Dados:**
    - **Estado:** ✅ Garantido no final da fase.
    - **Descrição:** Os dados em memória (tarifas e registos) podem ser persistidos no ficheiro através de uma função dedicada.
- **Detalhes necessários**:
    - Aplicar as tarifas de acordo com os horários especificados no ficheiro `Tarifas.txt`.
    - Calcular o valor com base nos períodos de 15 minutos e ajustar ao valor máximo diário.
    - Implementar suporte para estadias de múltiplos dias.
