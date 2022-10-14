#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include <stdbool.h>
#include "unbounded_int.h"

#define SIZEBUF 1024

typedef struct listes_des_var listes_des_var;
struct listes_des_var{
    listes_des_var *suivant;
    char *nom;
    unbounded_int val;
};

static void afficher_unbounded_int(FILE *f ,unbounded_int a){
    if(a.signe=='*'){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);
    }
    chiffre *p=a.premier;
    fprintf(f,"%c",a.signe);
    while(p!=NULL){
        fprintf(f,"%c",p->c);
        p=p->suivant;
    }
}

//Cette fonction permet de modifier la valeur d'un variable si elle existe déja, de la créer avec la valeur valeur_var sinon
static listes_des_var* modifier_ajouter_var(listes_des_var *l , char *nom_var , unbounded_int valeur_var){
    listes_des_var *suiv = l,
                   *prec = NULL;
    while (suiv!=NULL &&  strcmp(suiv->nom,nom_var)!=0){
        prec=suiv;
        suiv=suiv->suivant;
    }
    if(suiv==NULL){
        listes_des_var *new_var = malloc(sizeof(listes_des_var));
        if(new_var==NULL){
            perror("Erreur lors de l'allocation d'un espace memoire");
            exit(EXIT_FAILURE);
        }
        new_var->nom=nom_var;
        new_var->val=valeur_var;
        new_var->suivant=suiv;
        if(l==NULL){
            l=new_var;
        }
        else{
            prec->suivant=new_var;
        } 
    }
    else{
        suiv->val=valeur_var;
    }
    return l;
}
//cette fonction permet de retourner la valeur de la variable nom_var si elle existe dans la liste l , sinon elle crée une nouvelle variable et elle retourne 0
static unbounded_int valeur_var(listes_des_var *l , char *nom_var ){
     listes_des_var *suiv = l,
                   *prec = NULL;
    while (suiv!=NULL && strcmp(suiv->nom,nom_var)!=0){
        prec=suiv;
        suiv=suiv->suivant;
    }
    if(suiv==NULL){
        listes_des_var *new_var = malloc(sizeof(listes_des_var));
        if(new_var==NULL){
            perror("Erreur lors de l'allocation d'un espace memoire");
            exit(EXIT_FAILURE);
        }
        new_var->nom=nom_var;
        new_var->val=string2unbounded_int("0");
        new_var->suivant=suiv;
        if(l==NULL){
            l=new_var;
        }
        else{
            prec->suivant=new_var;
        }
        return new_var->val; 
    }
    else{
        return suiv->val;
    }
}

static listes_des_var* operation(char *ligne,listes_des_var *l){
    char op=' ';
    char *chiffre1=malloc(sizeof(char)*strlen(ligne));
    if(chiffre1==NULL){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);
    }

    char *chiffre2=malloc(sizeof(char)*strlen(ligne));
    if(chiffre2==NULL){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);
    }

    char *result=malloc(sizeof(char)*strlen(ligne));
    if(result==NULL){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);}

    int egaleTrouve=0;
    
    int i=0,chfr1=0,chfr2=0,rslt=0;
    while(ligne[i]==' ' || ligne[i]=='0'){
        i++;
    }
    while(ligne[i]!='\0'){
        if(ligne[i]!=' ' && ligne[i]!='\n'){
            if(ligne[i]=='='){
                egaleTrouve=1;
            }else if((ligne[i]=='-' || ligne[i]=='+' || ligne[i]=='*') &&  ((i+1)<strlen(ligne)) && (i-1>=0) && (ligne[i+1]==' ') && (ligne[i-1]==' ')){
                    op=ligne[i];
            }else{
                if(egaleTrouve==0){
                    result[rslt]=ligne[i];
                    rslt++;
                }else
                if(chfr1==0 || op==' '){
                    if(!(((chfr1==0 || (chfr1==1 && (chiffre1[0]=='+' || chiffre1[0]=='-'))) && ligne[i]=='0'))){
                        chiffre1[chfr1]=ligne[i];
                        chfr1++;

                    } 
                }else
                if(!(((chfr2==0 || (chfr2==1 && (chiffre2[0]=='+' || chiffre2[0]=='-')))  && ligne[i]=='0'))){
                    chiffre2[chfr2]=ligne[i];
                    chfr2++;
                }
            }
        }
        i++;
    }
    chiffre1[chfr1]='\0';
    chiffre2[chfr2]='\0';
    result[rslt]='\0';


    unbounded_int premierePartie;
    unbounded_int deuxiemePartie;
    unbounded_int resultat;

    if((isdigit(chiffre1[0]))|| chiffre1[0]=='+' || chiffre1[0]=='-'){
        premierePartie=string2unbounded_int(chiffre1);
    }else{
        premierePartie=valeur_var(l,chiffre1);
    }
    if((isdigit(chiffre2[0]))|| chiffre2[0]=='+' || chiffre2[0]=='-'){
        deuxiemePartie=string2unbounded_int(chiffre2);
    }else{
        deuxiemePartie=valeur_var(l,chiffre2);
    }
    switch(op){
        case '+' : resultat= unbounded_int_somme(premierePartie,deuxiemePartie);break;
        case '-' : resultat= unbounded_int_difference (premierePartie,deuxiemePartie);break;
        case '*' : resultat= unbounded_int_produit(premierePartie,deuxiemePartie);break;
        default : resultat= string2unbounded_int("0");
    }
    return modifier_ajouter_var(l , result, resultat);
}

int main(int arc , char *argv[]) {
 
    FILE *source=NULL,
         *dest=NULL;
    if(arc<=5){
        int i=1;
        if(i<arc && strcmp("-i",argv[i])==0){
            i++;
            source = fopen(argv[i],"r");
            if(source==NULL){
                perror("Erreur lors de l'ouverture du fichier source");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        else{
            source=stdin;
        }
        if(i<arc && strcmp("-o",argv[i])==0){
            i++;
            dest = fopen(argv[i],"w");
            if(dest==NULL){
                perror("Erreur lors de l'ouverture du fichier dest");
                exit(EXIT_FAILURE);
            }
        }
        else{
            dest=stdout;
        }
        char *print="print";
        listes_des_var *l=NULL;

        char buf[SIZEBUF];
        size_t len=SIZEBUF;
        char *ligne=malloc(len);
        bool first=true;
        if(ligne ==NULL){
            perror("Erreur lors de l'allocation d'un espace memoire pour le buffer");
            exit(EXIT_FAILURE);
        }

        ligne[0]='\0';

        while(fgets(buf,sizeof(buf),source)!=NULL){
            size_t tailleRecupere=strlen(buf);
            size_t tailleOcuppe=strlen(ligne);

            if(len-tailleOcuppe<tailleRecupere){
                len*=2;
                char *new =realloc(ligne,len);
                if(new==NULL){
                    perror("Erreur lors de l'allocation d'un espace memoire pour le buffer");
                    free(ligne);
                    exit(EXIT_FAILURE);
                }
                ligne=new;
            }
            strncpy(ligne+tailleOcuppe,buf,len-tailleOcuppe);
            tailleOcuppe+=tailleRecupere;

            if(ligne[tailleOcuppe-1]=='\n' || feof(source)){ // pour la derniere ligne si jamais elle n'a pas de \n a la fin
                int i=0;
                while( i<strlen(ligne) && (ligne[i]==' ' || ligne[i]=='0')){
                    i++;
                }
                int j=0;
                while( (i+j)<strlen(ligne) && ligne[i+j]==print[j] ){
                    j++;
                }
                if(j==5){ //on a trouvé un print
                    i+=j;
                    if(ligne[i]==' '){
                        i++;
                        char *variable=malloc(sizeof(char)*(strlen(ligne)-5));
                        if(variable==NULL){
                            perror("Erreur lors de l'allocation d'un espace memoire");
                            exit(EXIT_FAILURE);
                        }
                        int curseur=0;
                        while(ligne[i]!='\0' && ligne[i]!='\n'){
                            variable[curseur]=ligne[i];
                            i++;
                            curseur++;
                        }
                        variable[curseur]='\0';
                        unbounded_int n = valeur_var(l,variable);
                        //Juste pour ne pas avoir un saut de ligne inutile au debut du fichier
                        if(!first){
                            fprintf(dest,"\n");
                        }else{
                            first=false;
                        }
                        fprintf(dest,"%s = ",variable);
                        afficher_unbounded_int(dest,n);
                    }
                    else{
                        perror("\nErreur d'une ligne de commande");
                        exit(EXIT_FAILURE);
                    }
                }
                else{
                    char *variable=malloc(sizeof(char)*(strlen(ligne)));
                    if(variable==NULL){
                        perror("Erreur lors de l'allocation d'un espace memoire");
                        exit(EXIT_FAILURE);
                    }
                    int curseur=0;
                    while( i<strlen(ligne) && (ligne[i]==' ' || ligne[i]=='0')){
                        i++;
                    }
                    while(i<strlen(ligne) && ligne[i]!='=' && ligne[i]!=' ' && ligne[i]!='\n'){
                        variable[curseur]=ligne[i];
                        i++;
                        curseur++;
                    }
                    variable[curseur]='\0';
                    if(i<strlen(ligne)){
                        char *op1=malloc(sizeof(char)*(strlen(ligne)-strlen(variable)));
                        if(op1==NULL){
                            perror("Erreur lors de l'allocation d'un espace memoire");
                            exit(EXIT_FAILURE);
                        }
                        while( i<strlen(ligne) && (ligne[i]==' ' || ligne[i]=='0' ||ligne[i]=='=' )){
                            i++;
                        }
                        int j=0;
                        while( (i+j)<strlen(ligne) && ligne[i+j]!=' ' && ligne[i+j]!='\n'){
                            op1[j] = ligne[i+j];
                            j++;
                        }
                        op1[j]='\0';
                        if( (i+j)==strlen(ligne)  || ligne[i+j]=='\n'){//on est dans le cas de l'initialisation
                            unbounded_int n = string2unbounded_int(op1);
                            l = modifier_ajouter_var(l,variable,n);
                        }
                        else{
                            l = operation(ligne,l);
                        }
                    }
                }
                ligne[0]='\0';//Liberer le contenu de la ligne pour en lire une autre
            }
        }

        fclose(source);
        fclose(dest);
        return EXIT_SUCCESS;
    }
    else{
        perror("\nErreur d'une ligne de commande");
        exit(EXIT_FAILURE);
    }
    
}

