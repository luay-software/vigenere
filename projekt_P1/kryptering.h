#ifndef CRYPTO_UI_H
#define CRYPTO_UI_H

#include <stdio.h>
#include <stdint.h> // Nødvendig for uint64_t
#include <string.h>
#include <math.h>

// Konstanter fra marius's fil 
#define PRIME_MODULUS 2147483647ULL  
#define GENERATOR 7

#define TEXT_LENGTH 100


// her kalder vi hele struct for session så vi angår at skrive struuct
typedef struct session {
    long long secret_key;   // Det hemmelige tal, vi har skrevet ned.
    long long public_key;   // Det tal, vi må vise til andre.
    long long shared_secret;
    int keys_generated; // Flag: 0 = nej, 1 = ja Et grønt stempel, der viser, om mappen er udfyldt og klar til brug.
    char plain_text[TEXT_LENGTH];
    int numbers[5];
} Session;


//         Funktionsprototyper 

// UI og Input Validering
void print_menu(void);
void print_menu_start(void);   // Menu 1
void print_menu_key(void);     // Menu 2
void print_menu_crypto(void);  // Menu 3
long long get_safe_long(long long min, long long max);

// Logik og Filhåndtering
long long generate_random_key(void);
void save_session(Session current_session);
int load_session(Session *current_session);

// Matematik fnktion fra marius's fil 
uint64_t modular_pow(uint64_t base, uint64_t exponent, uint64_t modulus);

// Krypteringsfunktioner
void encrypt_text(char plaintext[], int numbers[]);
void decrypt_text(char plaintext[], int numbers[]);
void ll_to_intarr(long long ll, int numbers[], int length);
char encrypt_char(char p, int k);
char decrypt_char(char c, int k);

void encryption(char plain_text[], long long secret_key, int numbers[]);
void decryption(char plain_text[], long long secret_key, int numbers[]);

#endif