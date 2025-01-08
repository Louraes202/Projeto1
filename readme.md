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

### **2. Validar a leitura e gravação de dados.**

- Garantir que o programa:
    - Lê corretamente o ficheiro `Estacionamentos.txt`, considerando registros incompletos (sem saída) como veículos ainda no parque.
    - Atualiza o ficheiro ao registrar saídas ou entradas.

### **3. Apresentar mapa de ocupação de pisos.**

- Mostrar uma visualização gráfica simples no terminal:
    - **'-'** para lugares livres.
    - **'X'** para lugares ocupados.
    - Motivo (ex.: **'I'**, **'O'**) para lugares indisponíveis.

### **4. Listagem de veículos que saíram num dia.**

- Permitir que o utilizador:
    - Insira uma data específica (AAAA-MM-DD).
    - Veja todos os registos de veículos que saíram nesse dia, com os valores pagos.
    - Ordene os resultados por valor pago, decrescentemente.

### **5. Persistência e sincronização de dados.**

- Ao sair do programa, todos os dados em memória devem ser salvos:
    - Configuração do parque.
    - Tarifário.
    - Registos de estacionamento.

---

### **Sugestão de Ordem de Implementação:**

1. Ajustar a lógica de cálculo na função `registar_saida`.
2. Criar a funcionalidade de listar veículos por dia.
3. Implementar o mapa de ocupação de pisos.
4. Testar e validar com dados de exemplo (`Tarifas.txt` e `Estacionamentos.txt`).