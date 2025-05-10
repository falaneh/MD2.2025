#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Function to calculate GCD (Greatest Common Divisor) using Euclidean Algorithm
long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        long long temp = a;
        a = b;
        b = temp;
    }
    return a;
}

// Function to check if a number is prime
bool is_prime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Function to calculate modular inverse using Extended Euclidean Algorithm
// Returns x such that (a*x) % m == 1
long long mod_inverse(long long a, long long m) {
    long long m0 = m;
    long long y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        long long q = a / m;
        long long t = m;

        m = a % m, a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0)
        x += m0;

    return x;
}

// Function to calculate Euler's Totient function (phi)
long long phi(long long n) {
    long long result = n;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            result -= result / p;
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

// Function for modular exponentiation (a^b % m)
long long power(long long base, long long exp, long long mod) {
    long long res = 1;

    if (mod <= 0) {
        // Problem implies positive n1. This is defensive.
        // Handle error appropriately if this can occur.
        // For now, assume mod >= 1 based on problem context.
        if (mod == 0) {
            printf("Erro critico: Modulo zero na exponenciacao modular.\n");
            exit(1); // Or return an error code
        }
        // Negative modulo is not standard for this problem.
    }

    if (mod == 1) {
        return 0; // For any base and exp, result mod 1 is 0.
    }

    // Now mod > 1
    base %= mod;
    if (base < 0) { // Ensure base is in [0, mod-1]
        base += mod;
    }

    if (exp == 0) {
        return 1; // base^0 = 1. (1 % mod) is 1 for mod > 1.
    }

    if (exp < 0) {
        // Negative exponents require modular inverse of the base,
        // which is beyond the current problem's scope.
        printf("Erro: Expoente negativo nao e suportado diretamente.\n");
        // Consider returning an error indicator or exiting.
        // For now, assuming exp >= 0 as per typical problem context.
        exit(1); 
    }

    // Standard exponentiation by squaring, using multiply_mod for safety
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = multiply_mod(res, base, mod);
        }
        base = multiply_mod(base, base, mod); // base * base can also overflow
        exp /= 2;
    }
    return res;
}

// Helper for Step 3 if H or G_inv can be very large
long long multiply_mod(long long val_a, long long val_b, long long mod_m) {
    long long res = 0;
    val_a %= mod_m;
    if (val_a < 0) {
        val_a += mod_m;
    }
    
    while (val_b > 0) {
        if (val_b % 2 == 1) {
            res = (res + val_a);
            if (res >= mod_m) {
                res -= mod_m;
            }
        }
        val_a = (val_a + val_a); 
        if (val_a >= mod_m) { 
            val_a -= mod_m;
        }
        val_b /= 2;
    }
    return res;
}

int main() {
    long long H, G, n, x, n1;

    printf("Digite o valor de H: ");
    if (scanf("%lld", &H) != 1 || H <= 0) {
        printf("Entrada invalida para H.\n");
        return 1;
    }
    printf("Digite o valor de G: ");
    if (scanf("%lld", &G) != 1 || G <= 0) {
        printf("Entrada invalida para G.\n");
        return 1;
    }
    printf("Digite o valor de n (para Z_n): ");
    if (scanf("%lld", &n) != 1 || n <= 0) {
        printf("Entrada invalida para n.\n");
        return 1;
    }
    printf("Digite o valor do expoente x: ");
    if (scanf("%lld", &x) != 1 || x < 0) {
        printf("Entrada invalida para x (deve ser >= 0).\n");
        return 1;
    }
    printf("Digite o valor do modulo n1: ");
    if (scanf("%lld", &n1) != 1 || n1 <= 0) {
        printf("Entrada invalida para n1.\n");
        return 1;
    }

    printf("\n--- Etapa 1: Verificar se G e n sao primos entre si ---\n");
    long long common_divisor_G_n = gcd(G, n);
    if (common_divisor_G_n != 1) {
        printf("G (%lld) e n (%lld) nao sao primos entre si (MDC = %lld).\n", G, n, common_divisor_G_n);
        printf("A divisao modular H / G em Z_n nao e possivel diretamente pois G nao possui inverso em Z_n.\n");
        return 1;
    }
    printf("G (%lld) e n (%lld) sao primos entre si (MDC = 1).\n", G, n);

    printf("\n--- Etapa 2: Calcular o inverso de G em Z_n ---\n");
    long long G_inv = mod_inverse(G, n);
    printf("O inverso de G (%lld) em Z_%lld e G^-1 = %lld.\n", G, n, G_inv);
    
    long long verification_product = multiply_mod(G, G_inv, n);
    if (verification_product != 1) {
        printf("Verificacao: (G * G^-1) mod n = (%lld * %lld) mod %lld = %lld != 1. O calculo do inverso pode ter um problema ou G nao tem inverso (MDC(G,n) != 1).\n", G, G_inv, n, verification_product);
    } else {
         printf("Verificacao: (G * G^-1) mod n = (%lld * %lld) mod %lld = %lld.\n", G, G_inv, n, verification_product);
    }

    printf("\n--- Etapa 3: Dividir H por G em Z_n para encontrar a ---\n");
    long long a = multiply_mod(H, G_inv, n);
    printf("a = (H * G^-1) mod n = (%lld * %lld) mod %lld = %lld.\n", H, G_inv, n, a);

    printf("\n--- Etapa 4: Verificar se a e n1 sao coprimos ---\n");
    long long common_divisor_a_n1 = gcd(a, n1);
    if (common_divisor_a_n1 != 1) {
        printf("a (%lld) e n1 (%lld) nao sao coprimos (MDC = %lld).\n", a, n1, common_divisor_a_n1);
        printf("Isso pode afetar a aplicabilidade direta do Teorema de Euler/Fermat para simplificacao do expoente.\n");
    } else {
        printf("a (%lld) e n1 (%lld) sao coprimos (MDC = 1).\n", a, n1);
    }

    printf("\n--- Etapa 5: Verificar se n1 e primo ---\n");
    bool n1_is_prime = is_prime(n1);
    if (n1_is_prime) {
        printf("n1 (%lld) e um numero primo.\n", n1);
    } else {
        printf("n1 (%lld) nao e um numero primo.\n", n1);
    }

    long long x1;
    if (n1_is_prime) {
        printf("\n--- Etapa 6: Aplicar o Pequeno Teorema de Fermat ---\n");
        x1 = n1 - 1;
        printf("Como n1 (%lld) e primo, pelo Pequeno Teorema de Fermat, a^(n1-1) === 1 (mod n1) (se n1 nao divide a).\n", n1);
        printf("Definimos x1 = n1 - 1 = %lld - 1 = %lld.\n", n1, x1);
    } else {
        printf("\n--- Etapa 7: Aplicar o Teorema de Euler ---\n");
        printf("Como n1 (%lld) nao e primo, aplicaremos o Teorema de Euler.\n", n1);
        if (common_divisor_a_n1 != 1) {
             printf("Alerta: a (%lld) e n1 (%lld) nao sao coprimos. O Teorema de Euler (a^phi(n1) === 1 mod n1) requer que a e n1 sejam coprimos.\n", a, n1);
             printf("A simplificacao do expoente pode nao ser valida.\n");
        }
        x1 = phi(n1);
        printf("Calculando phi(n1) (Funcao Totiente de Euler para n1 = %lld):\n", n1);
        long long temp_n1_for_phi_display = n1;
        long long phi_val_display = n1;
        printf("phi(%lld) = %lld", temp_n1_for_phi_display, temp_n1_for_phi_display);
        for (long long p = 2; p * p <= temp_n1_for_phi_display; ++p) {
            if (temp_n1_for_phi_display % p == 0) {
                printf(" * (1 - 1/%lld)", p);
                phi_val_display -= phi_val_display / p;
                while (temp_n1_for_phi_display % p == 0)
                    temp_n1_for_phi_display /= p;
            }
        }
        if (temp_n1_for_phi_display > 1) { 
            printf(" * (1 - 1/%lld)", temp_n1_for_phi_display);
            phi_val_display -= phi_val_display / temp_n1_for_phi_display;
        }
        printf(" = %lld.\n", phi_val_display); 
        printf("Pelo Teorema de Euler, a^phi(n1) === 1 (mod n1), desde que a e n1 sejam coprimos.\n");
        printf("Definimos x1 = phi(n1) = %lld.\n", x1);
    }
    
    if (x1 == 0 && n1 > 1) { // phi(1)=1, phi(2)=1. x1 should not be 0 if n1 > 1.
         printf("Erro: x1 (phi(n1) ou n1-1) resultou em 0 para n1 > 1. Isso nao deveria acontecer.\n");
         return 1;
    }
    if (x1 == 0 && n1 == 1) { // phi(1) = 1, so x1 should be 1.
        // This case is fine as x1 will be 1.
    }


    printf("\n--- Etapa 8: Decompor o expoente x usando o teorema da divisao ---\n");
    long long q = x / x1;
    long long r = x % x1;
    printf("x = %lld, x1 = %lld\n", x, x1);
    printf("x = x1 * q + r\n");
    printf("%lld = %lld * %lld + %lld\n", x, x1, q, r);

    printf("\n--- Etapa 9: Reescrever a expressao a^x mod n1 ---\n");
    printf("a^x mod n1 = a^(x1*q + r) mod n1\n");
    printf("           = (a^(x1*q) * a^r) mod n1\n");
    printf("           = ((a^x1)^q * a^r) mod n1\n");
    if (common_divisor_a_n1 == 1 || (n1_is_prime && (a % n1 != 0)) ) { 
        printf("Como a^x1 === 1 (mod n1) (por Fermat ou Euler, se condicoes satisfeitas),\n");
        printf("           = ((1)^q * a^r) mod n1\n");
        printf("           = (1 * a^r) mod n1\n");
        printf("           = a^r mod n1\n");
        printf("Calcularemos a^x mod n1 como a^r mod n1.\n");
    } else {
        printf("Como a e n1 nao sao coprimos (MDC != 1), ou n1 e primo mas a e multiplo de n1,\n");
        printf("a simplificacao a^x1 === 1 (mod n1) pode nao ser valida.\n");
        printf("Prosseguiremos com o calculo ((a^x1)^q * a^r) mod n1, onde a^x1 mod n1 pode nao ser 1.\n");
    }

    printf("\n--- Etapa 10: Calcular os valores intermediarios ---\n");
    long long ax1_mod_n1;
    if (common_divisor_a_n1 == 1 || (n1_is_prime && (a % n1 != 0))) {
        ax1_mod_n1 = 1;
        printf("a^x1 mod n1 = x2 = 1 (pela propriedade de Fermat/Euler).\n");
    } else {
        ax1_mod_n1 = power(a, x1, n1);
        printf("a^x1 mod n1 = %lld^%lld mod %lld = %lld. (Denotado como x2)\n", a, x1, n1, ax1_mod_n1);
        if (n1_is_prime && (a % n1 == 0)) {
             printf("Nota: n1 e primo e 'a' (%lld) e um multiplo de n1 (%lld), entao a^x1 mod n1 (%lld^%lld mod %lld) e 0 (para x1 >=1).\n", a, n1, a, x1, n1);
        } else {
             printf("Nota: Como a e n1 nao sao coprimos, ou outra condicao de Fermat/Euler nao foi plenamente satisfeita, a^x1 mod n1 nao e necessariamente 1.\n");
        }
    }
    long long x2 = ax1_mod_n1;

    long long x2_q_mod_n1 = power(x2, q, n1);
    printf("x2^q mod n1 = %lld^%lld mod %lld = %lld.\n", x2, q, n1, x2_q_mod_n1);

    long long ar_mod_n1 = power(a, r, n1);
    printf("a^r mod n1 = %lld^%lld mod %lld = %lld.\n", a, r, n1, ar_mod_n1);

    printf("\n--- Etapa 11: Combinar os resultados e imprimir o valor final ---\n");
    long long final_result;
    if (common_divisor_a_n1 == 1 || (n1_is_prime && (a % n1 != 0))) {
        final_result = ar_mod_n1;
        printf("Resultado final: a^x mod n1 = a^r mod n1 = %lld.\n", final_result);
    } else {
        final_result = multiply_mod(x2_q_mod_n1, ar_mod_n1, n1);
        printf("Resultado final: ((x2^q) * (a^r)) mod n1 = (%lld * %lld) mod %lld = %lld.\n",
               x2_q_mod_n1, ar_mod_n1, n1, final_result);
    }

    printf("\nCalculo direto para verificacao: a^x mod n1 = %lld^%lld mod %lld = %lld\n", a, x, n1, power(a,x,n1));


    return 0;
}