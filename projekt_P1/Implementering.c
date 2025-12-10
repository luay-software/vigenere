#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "kryptering.h" 

// Menuen der vises til brugeren

// MENU 1: Startskærm
void print_menu_start(void) {
    printf("\n--- MENU 1: START ---\n");
    printf("1. Automatisk: Lad programmet vaelge en sikker, tilfaeldig noegle.\n");
    printf("2. Manuelt: Indtast selv en noegle.\n");
    printf("3. Beregn faelles hemmelighed (KRAEVER AT NOEGLEN ER PÅ MIN 10 CIFRA)\n");
    printf("4. Gem session (Backup)\n");
    printf("0. Afslut programmet\n");
    printf("Indtast valg: ");
}

// MENU 2: Efter man har sin egen nøgle
void print_menu_key(void) {
    printf("\n--- MENU 2: HAR EGEN NOEGLE ---\n");
    printf("3. Beregn faelles hemmelighed (Indtast partners offentlige noegle)\n");
    printf("4. Gem session (Backup)\n");
    printf("9. Gaa tilbage (Nulstil)\n");
    printf("0. Afslut programmet\n");
    printf("Indtast valg: ");
}

// MENU 3: Efter man har fælles hemmelighed (Klar til kryptering)
void print_menu_crypto(void) {
    printf("\n--- MENU 3: KLAR TIL KRYPTERING ---\n");
    printf("5. Krypter besked\n");
    printf("6. Dekrypter besked\n");
    printf("4. Gem session (Backup)\n");
    printf("9. Gaa tilbage\n");
    printf("0. Afslut programmet\n");
    printf("Indtast valg: ");
}


// Robust input-funktion
long long get_safe_long(long long min, long long max) {
    long long value;
    int status;
    char skip_ch;
    
    while (1) {
// Prøv at læse et tal
        status = scanf("%lld", &value);   // Læser long long

// Ryd buffer for resterende tegn hvis brugeren skrev hej
        do {
            scanf("%c", &skip_ch);
        } while (skip_ch != '\n');

// Tjek om det var et tal status==1 og om det er indenfor grænsen
        if (status == 1 && value >= min && value <= max) {
            return value; 
        } else {
            printf("Fejl: Indtast venligst et tal mellem %lld og %lld: ", min, max);
        }
    }
}

// Genererer et random 64-bit tal
long long generate_random_key(void) {
// Simpel random logic for at få et stort tal
// rand() giver kun op til 32.767, så vi ganger dem sammen
    return (long long)rand() * rand(); 
}

// Gemmer nøglen til en fil
void save_session(Session current_session) {
    FILE *fp = fopen("backup.txt", "w"); // "w" for write overskriver
    if (fp != NULL) {
        // Vi gemmer secret_key og status. Public key kan regnes ud igen.
        fprintf(fp, "%lld %d", current_session.secret_key, current_session.keys_generated);
        fclose(fp);  
        printf("--> Session gemt korrekt.\n");
    } else {
        printf("--> Fejl: Kunne ikke gemme filen.\n");
    }
}

// Indlæser session fra fil Call by reference via pointer
int load_session(Session *current_session) {
    FILE *fp = fopen("backup.txt", "r"); // "r" for read
    if (fp != NULL) {
// Vi bruger -> fordi current_session er en pointer
        fscanf(fp, "%lld %d", &current_session->secret_key, &current_session->keys_generated);
        fclose(fp);
        return 1; // Returner 1 for "Succes"
    }
    return 0; // Returner 0 for "Fejl/Ingen fil"
}

// logiken fra marius's fil keygen
// Binary exponentiation.
uint64_t modular_pow(uint64_t generator, uint64_t hidden_num, uint64_t prime_number) {
    uint64_t result = 1;
    generator %= prime_number;

    while (hidden_num > 0) {
        if (hidden_num & 1)
            result = (result * generator) % prime_number;   

        generator = (generator * generator) % prime_number; 
        hidden_num >>= 1; 
    }
    return result;
}

// Funktion der looper gennem tekst og kryptere den. Har tekst som input og keyword som input.
void encrypt_text(char plaintext[], int numbers[]) {
    int text_length = strlen(plaintext);
    int num_length = 5;
    int j = 0;

    for (size_t i = 0; i < text_length; i++)
    {
        if (j != num_length) {
            plaintext[i] += numbers[j];
            j++;
        } else {
            j = 0;
        }
    }
}

//Funktion der looper gennem tekst og dekryptere den. Har tekst som input og keyword som input.
void decrypt_text(char plaintext[], int numbers[]) {
    int text_length = strlen(plaintext);
    int num_length = 5;
    int j = 0;

    for (size_t i = 0; i < text_length; i++)
    {
        if (j != num_length) {
            plaintext[i] -= numbers[j];
            j++;
        } else {
            j = 0;
        }  
    }
}

// Funktion der konvertere long long til int array
void ll_to_intarr(long long ll, int numbers[], int length) { 
    char str[100];
    snprintf(str, sizeof(str), "%lld", ll);
    int j = 0;

    for(int i = 0; i < length; i++) {
 
        if (i == length - 1) {
            numbers[i] = (str[j] - '0'); 
        } else {
            numbers[i] = (str[j] - '0') * 10 + (str[j+1] - '0');  
        }
        
        j += 2;
        
    // - '0' er med til at osmkrive fra tegn til tal.
    // i er hvilket par vi arbejder med 0-4
    // key[i*2]-'0' = første tal i parret, *10 for tital-plads
    // key[i*2+1]-'0' = andet tal i parret, lægges til
    // gemmer resultatet som heltal i pairs[i] (fx 12, 34 osv.)
    }
    
}

// encryption funktion. tager en char og int som input og retuner en char.
char encrypt_char(char p, int k) {
    char c = p + k;
    return c;
}


// decryption funktion. tager en char og int som input og retuner en char.
char decrypt_char(char c, int k) {
    char p = c - k;
    return p;
}

// tager et tal og retunere hvor mange pladser der skal være i int-arrayet
int get_digits_for_array(long long ll) {
    char str[100];
    int digits = snprintf(str, sizeof(str), "%lld", ll);
    int arr_len = (int) ceil(digits/2.0);
    return arr_len;
}

void encryption(char plain_text[], long long secret_key, int numbers[]) {
    printf("Indtast tekst du oensker at kryptere: ");
    scanf(" %[^\n]", plain_text);
    int digits = get_digits_for_array(secret_key);
    ll_to_intarr(secret_key, numbers, digits);
    encrypt_text(plain_text, numbers);
    printf("Krypteret tekst: %s\n", plain_text);
}

void decryption(char plain_text[], long long secret_key, int numbers[]) {
    printf("Indtast tekst du oensker at dekryptere: ");
    scanf(" %[^\n]", plain_text);
    int digits = get_digits_for_array(secret_key);
    ll_to_intarr(secret_key, numbers, digits);
    decrypt_text(plain_text, numbers);
    printf("Dekrypteret tekst: %s\n", plain_text);
}