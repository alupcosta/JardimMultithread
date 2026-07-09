# Projeto Jardim Multithread

Este projeto usa threads e semáforos para simular um jardim onde as flores crescem sozinhas e os jardineiros precisam tentar colher tudo a tempo.

## Como o Problema Funciona

- **Flores:** Cada canteiro roda em uma thread separada. A flor nasce, cresce e floresce em tempos aleatórios.
- **Jardineiros:** Olham o jardim para colher as flores prontas.
- **Flores Murchas:** Se o jardineiro demorar mais de 3 segundos para chegar em uma flor pronta, ela murcha. O jardineiro perde a flor e só limpa o canteiro.
- **Cesta Cheia:** Cada jardineiro só consegue carregar 3 flores por vez. Quando a cesta enche, ele gasta um tempo guardando tudo no depósito, deixando o jardim sozinho.

## Sincronização

-  `mutex_estado`: Garante que a tela do terminal e a lista de canteiros não sejam atualizadas por duas threads ao mesmo tempo.
- `precisa_atencao`: Conta quantas flores estão prontas ou murchas aguardando um jardineiro.
- `mutex_tesoura`: Só existe 1 tesoura no jardim. Os jardineiros disputam por ela, impedindo que os dois tentem ir na mesma flor.

## Estrutura do Projeto

- `jardim.h`: Guarda as configurações de tempo, tamanho do jardim e o nome das funções.
- `jardim.c`: Tem o código que faz as flores crescerem e a lógica dos jardineiros trabalharem.
- `main.c`: Cria os semáforos, liga as threads e inicia o programa.

## Como Compilar e Executar
No terminal, estando no diretório com os arquivos do projeto, execute:
`gcc main.c jardim.c -o jardim -pthread`
`./jardim` 
Para finalizar a simulação basta pressionar Ctrl-Z