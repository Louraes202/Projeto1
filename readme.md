# Verificação de requisitos

### **Verificação dos Requisitos da Alínea 1b**

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

---

### **Verificação dos Requisitos da Alínea 2**

1. **Leitura Automática de Ficheiros ao Iniciar o Programa:**
    - **Estado:** ✅ Implementado.
    - **Descrição:**
        - O programa lê automaticamente os ficheiros `config_parque.txt` e `estado_parque.bin` ao iniciar.
        - **Validações:**
            - Verifica se o ficheiro `config_parque.txt` existe, não está vazio e segue o formato esperado.
            - Caso o ficheiro esteja ausente ou vazio, solicita a configuração do parque.
            - Verifica a integridade dos dados no ficheiro binário e inicializa os dados em memória.
2. **Carregamento de Configuração do Parque:**
    - **Estado:** ✅ Implementado.
    - **Função:** `carregar_configuracao_parque`.
    - **Descrição:**
        - Carrega o nome, morada, número de pisos, filas, lugares por fila e total de lugares.
        - Inicializa a ocupação e os estados dos lugares.
3. **Carregamento de Estado do Parque:**
    - **Estado:** ✅ Implementado.
    - **Função:** `carregar_estado_binario`.
    - **Descrição:**
        - Lê o ficheiro binário `estado_parque.bin` para carregar:
            - Ocupação atual dos lugares (livres, ocupados, indisponíveis).
            - Registos de estacionamento ainda válidos.
        - Atualiza o estado de ocupação no programa.
        - Caso o ficheiro binário não exista ou esteja corrompido, inicializa com valores padrão.
4. **Integração de Dados Carregados com a Ocupação:**
    - **Estado:** ✅ Implementado.
    - **Descrição:**
        - Ao carregar o estado do ficheiro binário:
            - Atualiza os lugares ocupados e livres no parque.
            - Recalcula estatísticas para garantir consistência.
5. **Persistência dos Dados Carregados:**
    - **Estado:** ✅ Implementado.
    - **Função:** `guardar_estado_binario`.
    - **Descrição:**
        - Após carregar dados e realizar alterações, permite guardar o estado atualizado em `estado_parque.bin`.

---

### **Validações e Tratamento de Erros**

- **Estado:** ✅ Implementado.
- **Descrição:**
    - Caso os ficheiros não estejam no formato esperado ou contenham dados inválidos:
        - O utilizador é notificado com mensagens claras.
        - O programa continua a execução sem interrupções.
    - Registos incompatíveis com a configuração do parque são descartados e reportados.

### **Verificação dos Requisitos da Alínea 3**

1. **Estrutura de Menus Adequada:**
    - **Estado:** ✅ Implementado.
    - **Descrição:**
        - O programa organiza os menus de forma modular:
            - **Menu Principal** com as opções de configuração e registos.
            - **Submenu de Configuração** para configurar o parque, visualizar dados e limpar memória.
            - **Submenu de Registos** com funcionalidades como carregar registos de ficheiros `.txt` e ver estatísticas.
        - Cada menu permite navegar entre submenus e voltar ao menu anterior.
2. **Exibição de Estatísticas nos Menus:**
    - **Estado:** ✅ Implementado.
    - **Função:** `exibir_lugares_por_piso`.
    - **Descrição:**
        - O número de lugares livres por piso é exibido no canto superior direito de todos os menus.
        - Caso o parque não esteja configurado, exibe: `"N.º de lugares por pisos: Sem pisos configurados"`.
3. **Recalcular Estatísticas de Ocupação:**
    - **Estado:** ✅ Implementado.
    - **Função:** `recalcular_estatisticas_parque`.
    - **Descrição:**
        - Recalcula os números de lugares livres, ocupados e indisponíveis por piso, além dos valores globais do parque.
        - Atualiza automaticamente após carregar dados do ficheiro binário ou ao alterar a configuração do parque.
4. **Validação e Tratamento de Erros:**
    - **Estado:** ✅ Implementado.
    - **Descrição:**
        - Ao carregar registos ou estado do parque, valida:
            - Compatibilidade com a configuração do parque (pisos, filas, lugares).
            - Registos com dados incompatíveis são ignorados e reportados.
        - Registos de lugares já ocupados ou indisponíveis também são descartados.

### **Verificação dos Requisitos da Alínea 4a**

1. **Registo de entrada de veículo:**
    - **Estado:** ✅ Implementado.
    - **Função:** `registar_entrada_veiculo`.
    - **Descrição:** Permite registar a entrada de um veículo, atribuindo automaticamente um lugar disponível no parque.
        - O utilizador pode especificar ou deixar que o sistema determine a data e hora de entrada.
        - A matrícula do veículo é verificada para evitar duplicados.
        - O lugar atribuído é marcado como ocupado no parque e associado ao veículo.
2. **Atualização da ocupação do parque:**
    - **Estado:** ✅ Implementado.
    - **Função:** `recalcular_estatisticas_parque`.
    - **Descrição:** Recalcula automaticamente os lugares livres, ocupados e indisponíveis após o registo de entrada de um veículo.
3. **Persistência de dados:**
    - **Estado:** ✅ Implementado.
    - **Função:** `guardar_estado_binario`.
    - **Descrição:** Após a entrada de um veículo, o estado atualizado do parque é gravado no ficheiro binário.
4. **Validações:**
    - **Estado:** ✅ Implementado.
    - **Descrição:**
        - Matrículas duplicadas são rejeitadas.
        - Caso não haja lugares disponíveis, o utilizador é informado.
        - Atribuição de lugares é garantida de forma eficiente, respeitando a configuração do parque.

---

### **Verificação dos Requisitos da Alínea 4b**

1. **Consulta de registos de estacionamento:**
    - **Estado:** ✅ Implementado.
    - **Função:** `consultar_registo`.
    - **Descrição:** Permite visualizar os detalhes de um registo específico, identificado pelo número de entrada, incluindo a matrícula, lugar atribuído e horário de entrada.
2. **Alteração de registo de estacionamento:**
    - **Estado:** ✅ Implementado.
    - **Função:** `alterar_registo`.
    - **Descrição:**
        - Permite alterar o lugar atribuído a um registo existente, garantindo que o novo lugar esteja livre.
        - Atualiza a ocupação do parque para refletir a mudança.
3. **Eliminação de registo de estacionamento:**
    - **Estado:** ✅ Implementado.
    - **Função:** `eliminar_registo`.
    - **Descrição:** Remove um registo de estacionamento, liberando o lugar associado no parque e atualizando a ocupação.
4. **Atualização da ocupação do parque:**
    - **Estado:** ✅ Implementado.
    - **Função:** `recalcular_estatisticas_parque`.
    - **Descrição:** Após alterações ou remoções de registos, as estatísticas de ocupação do parque são recalculadas para refletir o estado atual.
5. **Validações e tratamento de erros:**
    - **Estado:** ✅ Implementado.
    - **Descrição:**
        - Garantia de que apenas registos existentes podem ser consultados, alterados ou eliminados.
        - Verificação de que o novo lugar atribuído está livre antes de efetuar alterações.
        - Mensagens claras ao utilizador em caso de erros ou tentativas inválidas.