#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void encrypt_text(char plaintext[], int numbers[]);
void decrypt_text(char plaintext[], int numbers[]);
void ll_to_intarr(long long ll, int numbers[]);

int main(void) {
    int number[5] = {13, 65, 23, 12, 89}; 
    char plaintext[15] = "Secret message";
    
    encrypt_text(plaintext, number);

    printf("%s\n", plaintext);

    decrypt_text(plaintext, number);

    printf("%s\n", plaintext);

    int numbers[5];
    long long ll = 12345672;
    ll_to_intarr(ll, numbers);
    int length = sizeof(numbers) / sizeof(numbers[0]);
    printf("%d\n", length);

    for (size_t i = 0; i < length; i++)
    {
        printf("%d\n", numbers[i]);
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


// Get-value funktion der tager en char som input og spytter en value ud som int.
// I ascii-kode har a har værdien 97 værdien og A har værdien 65, men de er begge indeks nul og skal retunere 0. b og B skal retunere 1 osv.
// Lav evt. en switch case.
int get_value(char c) {
    return 0;
}


// Get-char funktion der tager en int og spytter en char ud.
// Tager et tal ind fra 0-25 og spytter en char ud alt efter dets index. a / A = 0, og z / Z = 25.
char get_char(int n) {
    return ' ';
}


// Funktion der looper gennem tekst og kryptere den. Har tekst som input og keyword som input.
void encrypt_text(char plaintext[], int numbers[]) {
    int text_length = strlen(plaintext);
    int num_length = 5;
    int j = 0;

    for (size_t i = 0; i < text_length; i++)
    {
        if (i != num_length) {
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
        if (i != num_length) {
            plaintext[i] -= numbers[j];
            j++;
        } else {
            j = 0;
        }  
    }
}

// Funktion der konvertere long long til int array
void ll_to_intarr(long long ll, int numbers[]) {
    char str[10+1];
    snprintf(str, sizeof(str), "%lld", ll);
    int string_length = strlen(str);

    int length = (int) ceil(string_length/2.0);
    int j = 0;

    for(int i = 0; i < length; i++) {
 
        if (i == length - 1) {
            numbers[i] = (str[j] - '0');
        } else
        {
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


// Evt. flere funktioner