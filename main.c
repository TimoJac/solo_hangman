#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>


// Taille maximale d'un mot
const int MAX_WORD_SIZE = 20;

const char* FILEPATH = "mots.txt";


void pendu(int erreur);                                         //Dessin du pendu
char* wordChoicePgrm(/*int nb*/);
int randomNumber(int max);
char* wordChoiceFile(int rdmnb);
void viderBuffer();
int countWords();
int verifyPendu(int errors, int done, char* word, int lettersFound, int nbchar, char* guess, int letterAlreadyFound);





int main() {
    int errors = 0;                                             //Le nombre de fautes
    int done = 0;                                               //Booléen si il a trouvé le mot
    int nbchar = 0;                                             //Le nombre de lettres du mot
    char letter;
    int lettersFound = 0;
    int letterAlreadyFound = 0;


    // Nombre de mots dans le fichier (un mot par ligne)
    int wordCount = countWords();
    printf("DEBUG : Found %d words in %s\n", wordCount, FILEPATH);

    // Récupération d'un mot au hasard (en tenant compte du nombre de mots dans le fichier)
    char* word = wordChoiceFile(randomNumber(wordCount));
    nbchar = (int)strlen(word);

    printf("DEBUG : selected word is %s (%d characters)\n", word, nbchar);

    printf("Un mot a été choisi ! Proposez des lettres pour le deviner.\nAttention, 10 erreurs et vous avez perdu !\nBonne chance ;)\n\n");

    char guess[nbchar];                                         //On crée la variable guess qui sera l'affichage dispo au joueur, on le comparera avec word
    for (int j = 0; j < nbchar; j++) {
        guess[j] = '_';                                         //On l'initialise avec des _
    }

    verifyPendu(errors, done, word, lettersFound, nbchar, guess, letterAlreadyFound);

    printf("\nAppuyez sur <Entrée> pour quitter.");
    getchar();
    //getchar();

    if (word != NULL) free(word);

    return 0;
}



int verifyPendu(int errors, int done, char* word, int lettersFound, int nbchar, char* guess, int letterAlreadyFound){
    char letter;
    int compareWord = 0;
    letterAlreadyFound = 0;


    if (errors == 10){
        printf("Vous avez atteint le nombre maximal d'erreur... Vous êtes pendu !");
        return 0;
    } else if (done == 1){
        printf("Bravo, vous avez trouvé le mot avant d'être pendu ! Le mot était %s", word);
        return 0;
    } else {
        int foundLetter = 0;

        printf("Vous avez trouvé %d lettres sur %d.\n", lettersFound, nbchar);
        printf("\n");
        for (int i = 0; i < nbchar; i++) {
            putchar(guess[i]);
        }
        printf("\n\n Saisissez une lettre (sans accent)\n");
        saisie:
        scanf(" %c", &letter);

        while (((letter < 'A') || (letter > 'Z')) && ((letter < 'a') || (letter > 'z'))) {
            if (letter != EOF) viderBuffer();
            printf("Saisie invalide\nSaisissez une lettre (sans accent)\n");
            scanf("%c", &letter);
        }

        if (letter >= 'a' && letter <= 'z') {
            letter = toupper(letter);
        }

        for (int i = 0; i < nbchar - 1; i++) {
            if (letter == guess[i]) letterAlreadyFound = 1;
        }

        for (int i = 0; i < nbchar; i++) {
            if ((letter == word[i]) && (letterAlreadyFound == 0)) {
                guess[i] = letter;
                lettersFound++;
                foundLetter = 1;
                printf("La lettre %c est présente dans le mot, c'est la lettre n°%d !\n", letter, (i + 1));
            } else if (letterAlreadyFound == 1) {
                letterAlreadyFound = 0;
                printf("Vous avez déjà trouvé la lettre %c dans le mot !\n Rentrez une autre lettre.\n", letter);
                if (letter != EOF) viderBuffer();
                goto saisie;
            }
        }
        if (foundLetter == 0) {
            errors++;
            printf("La lettre %c n'est pas présente dans le mot... Vous êtes maintenant à %d erreurs sur 10 !\n",
                   letter, errors);
            if (errors != 0) pendu(errors);
            if (letter != EOF) viderBuffer();
        }


        for (int i = 0; i < nbchar; i++) {
            if (guess[i] == word[i])
                compareWord++;
        }
        if (compareWord == nbchar){
            done = 1;
            if (letter != EOF) viderBuffer();
        }

        verifyPendu(errors, done, word, lettersFound, nbchar, guess, letterAlreadyFound);
    }
}





int randomNumber(int max) {                                     //on utilise le temps pour faire un randomizer grâce au modulo
    srand(time(NULL));
    int rdmnb = rand() % max + 1;
    return rdmnb;                                               //Le numéro aléatoire
}




char* wordChoiceFile(int rdmnb) {                               //Pour chercher un mot dans le fichier mots.txt

    int compteur = 0;                                           //Pour compter le nombre de mots
    char *word = (char*) malloc(MAX_WORD_SIZE * sizeof (char));
    FILE *fp = fopen(FILEPATH, "r");

    if (fp == NULL) {                                           //Si problème d'ouverture du fichier : Fichier vide/inexistant
        printf("Error opening file\n");
        exit(1);
    }

    while (fgets(word, MAX_WORD_SIZE, fp) != NULL) {            //Tant qu'on atteint pas la fin du fichier (NULL)
        if (compteur == rdmnb) {                                //Si c'est le mot numéro (aléatoire)...
            // On a trouvé le mot, il contient le saut de ligne qu'il faut retirer
            // Le saut de ligne est le dernier caractère du mot
            word[strlen(word) - 1] = '\0';
            break;
        }

        compteur++;                                             //sinon on passe au mot suivant
    }

    fclose(fp);
    free(fp);

    return word;
}



// Compte le nombre de mots dans le fichier
int countWords() {
    int wordCount = 0;
    FILE *fp = fopen(FILEPATH, "r");

    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int c;
    do {
        c = fgetc(fp);
        if (feof(fp)) {
            break;
        }

        if (c == '\n') {
            wordCount++;
        }
    } while(1);

    fclose(fp);
    free(fp);

    return wordCount;
}




void viderBuffer(){
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}




void pendu(int erreur) {
    switch (erreur) {
// 0 //
        case 0:
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            break;
// 1 //
        case 1:
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("_____________\n");
            break;
// 2 //
        case 2:
            printf("\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|____________\n");
            break;
// 3 //
        case 3:
            printf("_____________\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|____________\n");
            break;
// 4 //
        case 4:
            printf("_____________\n");
            printf(" | /\n");
            printf(" |/\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|____________\n");
            break;
// 5 //
        case 5:
            printf("_____________\n");
            printf(" | /       |\n");
            printf(" |/\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|____________\n");
            break;
// 6 //
        case 6:
            printf("_____________\n");
            printf(" | /       |\n");
            printf(" |/        O\n");
            printf(" |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|____________\n");
            break;
// 7 //
        case 7:
            printf("_____________\n");
            printf(" | /       |\n");
            printf(" |/        O\n");
            printf(" |         |\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|____________\n");
            break;
// 8 //
        case 8:
            printf("_____________\n");
            printf(" | /       |\n");
            printf(" |/        O\n");
            printf(" |        -|-\n");
            printf(" |\n");
            printf(" |\n");
            printf("_|____________\n");
            break;
// 9 //
        case 9:
            printf("_____________\n");
            printf(" | /       |\n");
            printf(" |/        O\n");
            printf(" |        -|-\n");
            printf(" |         /\n");
            printf(" |\n");
            printf("_|____________\n");
            break;
// 10 //
        case 10:
            printf("_____________\n");
            printf(" | /       |\n");
            printf(" |/       (:c)\n");
            printf(" |        -|-\n");
            printf(" |         /\\\n");
            printf(" |\n");
            printf("_|____________\n");
            break;

        default:
            printf("Si vous voyez ça, vous avez bien planté le programme, bien joué :)");
            break;
    }
}