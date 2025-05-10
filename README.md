# Verificação Modular em C - Matemática Discreta 2

Este projeto implementa uma função de segurança baseada em verificação modular. O programa calcula `a^x mod n1`, onde a base `a` é definida de forma especial como o resultado da divisão modular `(H * G^-1) mod n`.

## Funcionalidades e Etapas Implementadas

O programa executa as seguintes etapas para calcular `a^x mod n1`:

1.  **Verificação de Coprimalidade (G, n):** Utiliza o Algoritmo de Euclides para verificar se `G` e `n` são primos entre si. Se não forem, a divisão modular para encontrar `a` não é possível.
2.  **Cálculo do Inverso Modular (G<sup>-1</sup>):** Calcula o inverso de `G` em `Z_n` utilizando o Algoritmo Estendido de Euclides (baseado em divisões sucessivas).
3.  **Cálculo da Base `a`:** Determina `a = (H * G^-1) mod n`.
4.  **Verificação de Coprimalidade (a, n1):** Verifica se `a` e `n1` são coprimos.
5.  **Verificação de Primalidade (n1):** Verifica se o módulo `n1` é um número primo.
6.  **Aplicação do Pequeno Teorema de Fermat:** Se `n1` for primo, define `x₁ = n1 - 1`.
7.  **Aplicação do Teorema de Euler:** Se `n1` não for primo, define `x₁ = φ(n1)` (função totiente de Euler). A função `φ(n1)` é implementada com base nos fatores primos de `n1`, sem uso de bibliotecas prontas.
8.  **Decomposição do Expoente:** Utiliza o teorema da divisão para decompor `x` como `x = x₁ * q + r`.
9.  **Reescrita da Expressão:** Demonstra como `a^x mod n1` pode ser reescrito como `(((a^x₁)^q mod n1) * (a^r mod n1)) mod n1`.
10. **Cálculo de Valores Intermediários:**
    *   `a^x₁ mod n1 = x₂`
    *   `x₂^q mod n1`
    *   `a^r mod n1`
11. **Combinação e Resultado Final:** Combina os resultados intermediários para obter o valor final da congruência.

## Como Compilar e Executar

### Pré-requisitos
*   Um compilador C (por exemplo, GCC).

### Compilação
No terminal, navegue até o diretório do projeto e execute:
```bash
gcc main.c -o verificacao_modular