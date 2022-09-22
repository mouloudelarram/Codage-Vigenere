#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int estLettre(char c){ return ((c>='a' && c <= 'z') || (c>='A' && c<= 'Z'));}
int estMinuscule(char c) { return (c >= 'a' && c <= 'z'); }
int estMajuscule(char c) { return (c >= 'A' && c <= 'Z'); }

char prochaineLettre(FILE* in){
    int eof;
    char c;
    eof = fscanf(in, "%c", &c);

    if (in != NULL){
        if (eof != EOF){    
            if (estLettre(c)){
                return c;
            } else {
                return '2'; // caractaire speciaux
            }
            eof = fscanf(in, "%c", &c); // on passe au caractaire suivant;
        } else {
            fprintf(stderr, "Erreur : la fin de fichier\n");
            return '1'; // la fin de la fichier
        }
    } else {
        fprintf(stderr, "Erreur : fichier mon_fichier.txt introuvable dans le repertoire courant\n");
    }
    return '0'; // error
    
}

char lettreLaPlusFrequente(FILE *in){
    rewind(in);
    int tab[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    char c = prochaineLettre(in) ;
    int max=0;
    int indice=0;
    int i=0;
   
    if (c!='0'){
        while(c!='1'){
            if (c != '2')
                tab[c-'a']++;
            c=prochaineLettre(in);
        }
    }
    max = tab[0];
    for ( i=0; i<26; i++)
        if (max<tab[i]){
            max = tab[i];
            indice = i;
        }
    return indice+'a';
}

char majToMin(char l) {
    return (l-'A')+'a';
} 

void normaliserFichier(FILE* in, FILE* out)
{
    char c;
    int status = fscanf(in, "%c", &c);
    do 
    {
        if(estMajuscule(c)) 
            fprintf(out, "%c", majToMin(c));
        else if (estMinuscule(c))
            fprintf(out, "%c", c);
        status = fscanf(in, "%c", &c);
    } while (status != EOF);
}


void decouperFichier(FILE *in, FILE *out, int longueur, int debut)
{
    //
    rewind(in);
    char c;
    int i = 0;
    int status;
    do
    {
        i++;
        status = fscanf(in, "%c", &c);
    } while (status != EOF && i<debut);
    fprintf(out,"%c", c);
    i=0;
    do
    {
        if (i==longueur)
        {
            fprintf(out,"%c", c);
            i=0;
        }
        i++;
        status = fscanf(in, "%c", &c); 
    } while (status != EOF);
}



void decrypte(FILE* in, FILE* out, char *password, int taille) {
    rewind(in);

    char c;
    int eof = fscanf(in,"%c", &c) ;
    int i=0;
   do {
       if (estLettre(c)){
        
        int decalage =password[i%taille]-'a';
        fprintf(out,"%c", (c-'a'+26-decalage)%26+'a');
        i++;
       }else {
           fprintf(out,"%c",c);
       }       
    eof = fscanf(in,"%c", &c) ;
   }while(eof !=EOF);
}

int main(int argc, char *argv[])
{
    FILE* in;
    FILE* out;
    FILE* textDecoupe;
    FILE* textNormalise;
    int taille = atoi(argv[2]);
    char pwd[taille];
    in = fopen(argv[1], "rb");
    out = fopen(argv[3], "wb");
    textDecoupe = fopen("fileDecouper.txt", "w+");
    textNormalise = fopen("textNormalise.txt", "wb");

    normaliserFichier(in,textNormalise);
    fclose(textNormalise);

    textNormalise = fopen("textNormalise.txt", "rb");

    if (textNormalise == NULL){
        return 0;
    }

    rewind(textDecoupe);


    //touver la lettre la plus freq dans le text decouper.
    for (int i =1; i<= taille;i++){
        rewind(textNormalise);
        rewind(textDecoupe);
        decouperFichier(textNormalise, textDecoupe, taille, i); 
        rewind(textDecoupe);
        pwd[i-1]= lettreLaPlusFrequente(textDecoupe);
    }

    int decalage = 0 ;

    //touver le mot de pass a 'aide de les valeurs de pwd.
    for (int i=0;i<taille;i++){
        decalage = 'e' -pwd[i];
        printf("\n*>%c", pwd[i]);
        printf("\n=>%d",decalage);
        if (decalage <= 0) 
            pwd[i] =   -1*decalage +'a';
        else 
            pwd[i] = 'z'-decalage +1 ;
    }

    printf("\nLe mot de pass est : ");
     for (int i=0;i<taille;i++){
        printf("%c", pwd[i]);
     }

    decrypte(in, out, pwd, taille);

    return EXIT_SUCCESS;
}