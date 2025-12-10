#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "kryptering.h" 

int main(void) {
    srand(time(NULL)); 

    Session my_session = {0, 0, 0, 0, {0}, {0}}; //Vi nulstiller alt med det samme. secret_key=0, public_key=0, shared_secret=0, keys_generated=0

    // Variabel til at styre hvilken menu vi er i 0, 1 eller 2

    int state = 0;


    if (load_session(&my_session)) {
        printf("Velkommen tilbage! Gammel session indlaest.\n");
        printf("Nuværende hemmelig noegle: %lld\n", my_session.secret_key);
        
// Forsøger at læse fra backup.txt. Hvis det lykkes returnerer 1, går vi ind i if-blokken.
        if (my_session.keys_generated) {
//Da vi kun gemte den hemmelige nøgle i filen, bruger vi Marius matematik funktion til at regne den offentlige nøgle ud igen med det samme. Så er vi klar til brug.
            my_session.public_key = modular_pow(GENERATOR, my_session.secret_key, PRIME_MODULUS);
            printf("Din OFFENTLIGE noegle: %lld\n", my_session.public_key);
        }
    } else {
        printf("Velkommen. Ingen gemt session fundet.\n"); 
    }

    int choice;
    do {
       // --- VIS MENU BASERET PÅ HVOR MAN ER HEN 
        if (state == 0) {
            print_menu_start(); // Vis Menu 1
            choice = (int)get_safe_long(0, 4); // Tillad 0-4 valgmuligheder 
        } 
        else if (state == 1) {
            printf("\nDin offentlige noegle er: %lld\n", my_session.public_key); // Husk at vise denne!
            print_menu_key();   // Vis Menu 2
            choice = (int)get_safe_long(0, 9); // Tillad op til 9
        } 
        else if (state == 2) {
            printf("\nFaelles hemmelighed er aktiv.\n");
            print_menu_crypto(); // Vis Menu 3
            choice = (int)get_safe_long(0, 9);
        }
        switch (choice) {
            case 1: // Random key
                if (state == 0){
                my_session.secret_key = generate_random_key(); //Laver et tilfældigt tal vores private hemmelighed
                
//Diffie-Hellman logikken. Vi tager generator og opløfter den i vores secretkey og tager modulus PRIME_MODULUS. Resultatet er vores publickey, som vi må vise til andre.
                my_session.public_key = modular_pow(GENERATOR, my_session.secret_key, PRIME_MODULUS);
                my_session.keys_generated = 1;
                    save_session(my_session);
                    state = 1; // Opgrader til næste menu
                }
                break;

            case 2: // Manuel key
                if (state == 0)
                printf("Indtast din hemmelige noegle: ");
                // Vi sikrer at input er mindre end primtallet for at matematikken virker
                my_session.secret_key = get_safe_long(999999999, 9999999999);
                
                // Beregn Public Key
                my_session.public_key = modular_pow(GENERATOR, my_session.secret_key, PRIME_MODULUS);
                
                my_session.keys_generated = 1;
                save_session(my_session); 
                state = 1; //Opgrader til menu 2
                break;

                case 3: // Beregn fælles (I menu 1 og 2)
                if (my_session.keys_generated == 0) {
                    printf("\nFEJL: Du skal lave dine egne noegler (Valg 1 eller 2) foer du kan lave denne!\n");
                } else {
                    printf("Indtast partners OFFENTLIGE noegle: ");
                    long long partner_pub = get_safe_long(0, 9223372036854775800LL);
                    my_session.shared_secret = modular_pow(partner_pub, my_session.secret_key, PRIME_MODULUS);
                    printf("\n*** FAELLES NOEGLE BEREGNET! ***\n");
                    state = 2; // Opgrader til Menu 3
                }
                break;

            case 4: // Gem i alle menuer
                save_session(my_session);
                break;
            
            case 5: // Krypter Kun menu 3  PLADSHOLDER
                if (state == 2) {
                    encryption(my_session.plain_text, my_session.secret_key, my_session.numbers);
                }
                break;

            case 6: // Dekrypter (Kun menu 3) - PLADSHOLDER
                if (state == 2) {
                    decryption(my_session.plain_text, my_session.secret_key, my_session.numbers);
                }
                break;

            case 9: // Gå tilbage
                if (state > 0) {
                    state--; // Gå et niveau ned ved -- dekrement-operatoren betyder træk 1 fra værdien af state.


                    printf("\nGaar tilbage...\n");
                }
                break;

            case 0:
                printf("Afslutter...\n");
                break;
                
            default:
                printf("Ugyldigt valg i denne menu.\n");
                break;
        }

    } while (choice != 0);

    return 0;
}  