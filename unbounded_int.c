#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "unbounded_int.h"

//cette fonction return true si s est un nombre, false sinon
static bool isNumber(const char *s){
    int i=0;
    if( !isdigit(s[i]) ){
        if(!((s[i] != '-') || (s[i] != '+'))){
            return false;
        }
    }
    i++;
    while(s[i]!='\0'){
        if (!isdigit(s[i]))
            {
                return false;
            }
        i++;
    }
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------
static chiffre *createChiffre(char c){
    chiffre *maillon = malloc(sizeof(chiffre));
    if(maillon!=NULL){
        maillon->c=c;
        maillon->suivant=NULL;
        maillon->precedent=NULL;
        return maillon;
    }
    return NULL;
}
//-----------------------------------------------------------------------------------------------------------------------------------
// cette fonction supprime les zéros au début d'un unbounded_int (par exemple si on a 00010 elle retourne 10)
static void free_0(unbounded_int *a){
    chiffre *p;
    while((a->premier)->suivant!=NULL && (a->premier)->c=='0'){
        p=a->premier;
        a->premier=(a->premier)->suivant;
        free(p);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------
unbounded_int string2unbounded_int(const char *e){
    unbounded_int entier;
    entier.signe='*';
    entier.premier=NULL;
    entier.dernier=NULL;
    entier.len=0;
    unbounded_int sauv_entier=entier;
    if(isNumber(e)){
         int i=0;
         int len = strlen(e);
        while(e[i]!='\0'){
            if(e[i]=='-' || e[i]=='+'){
                entier.signe=e[i];
                len--;
            }
            else{
                chiffre *maillon = createChiffre(e[i]);
                if(maillon==NULL){
                    return sauv_entier;
                }
                if(entier.premier==NULL){
                    entier.premier=maillon;
                }
                else{
                    (entier.dernier)->suivant=maillon;
                    maillon->precedent=entier.dernier;
                }
                entier.dernier=maillon;
            } 
            i++;
        }
        entier.len=len;
        if(entier.signe=='*'){
            entier.signe='+';
        }
    }
    return entier;
}
//--------------------------------------------------------------------------------------------------------------------
unbounded_int ll2unbounded_int(long long i){
    unbounded_int representation={.signe='*',.len=0,.premier=NULL,.dernier=NULL};
    if(i<0){
        representation.signe='-';
        i=i*(-1);
    }else{
        representation.signe='+';
    }

    int quotient=i/10,reste=i%10,len=0;
    chiffre *courant=NULL;

    while(!(quotient==0 && reste==0) || (quotient==0 && reste==0 && representation.dernier==NULL)){//POUR 0
        chiffre *new=createChiffre((char)(reste+'0'));
        if(new==NULL){
            representation.signe='*';
            return representation;
        }else {
            if(representation.dernier==NULL){
                representation.dernier=new;
                courant=representation.dernier;
            }else{
                courant->precedent=new;
                courant->precedent->suivant=courant;
                courant=courant->precedent;
            }
            reste=quotient%10;
            quotient=quotient/10;
            len++;
        }
       
    }
    representation.premier=courant;
    representation.len=len;
    return representation;
}
//--------------------------------------------------------------------------------------------------------------------
char *unbounded_int2string(unbounded_int i) {
    char *s = malloc((i.len+2)*sizeof(char));
    int x=0;
    if(i.signe=='-'){
        s[x]='-';
        x++;
    }
    chiffre *current;
    current=i.premier;
    while(current!=NULL){
        s[x]=current->c;
        x++;
        current=current->suivant;
    }
    s[x]='\0';
    return s;
}
//--------------------------------------------------------------------------------------------------------------------
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b){
    if(a.signe=='*' || b.signe=='*'){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);
    }
    if(a.signe=='+' && b.signe=='-'){
        return 1;
    }
    else{
        if(a.signe=='-' && b.signe=='+'){
            return -1;
        }
        else{
            chiffre *p,*q;
            if(a.signe=='+'){
                if(a.len>b.len){
                    return 1;
                }
                if(a.len<b.len){
                    return -1;
                }
                p=a.premier;
                q=b.premier;
            }
            else{
                if(a.len>b.len){
                    return -1;
                }
                if(a.len<b.len){
                    return 1;
                }
                q=a.premier;
                p=b.premier;
            }
            while(p!=NULL){
                if(p->c > q->c){
                    return 1;
                }
                if(p->c < q->c){
                    return -1;
                }
                p=p->suivant;
                q=q->suivant;
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------------------------------------------------------
int unbounded_int_cmp_ll(unbounded_int a, long long b){
    char *a2string=unbounded_int2string(a);//todo fonction de anyes
    int len=strlen(a2string)-1,
        i=0,
        atolong=0;

    while(a2string[i]!='\0'){
        if(a2string[i]=='-' ||  a2string[i]=='+'){
            len--;
        }
        else{
            atolong+=(a2string[i]-'0')*(pow(10,len));
            len--;
        }
        i++;

    }
    if(a.signe=='-'){
        atolong*=-1;
    }
    if(atolong>b)return 1;
    if(atolong==b)return 0;
    return -1;
}
//---------------------------------------------------------------------------------------------------------------------------
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
    if(a.signe=='*' || b.signe=='*'){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);
    }
    unbounded_int somme={.signe='*',.len=0,.premier=NULL,.dernier=NULL};
    if(a.signe==b.signe){
        somme.signe=a.signe;
    }else{
        if(a.signe=='-'){
            a.signe='+';
            return unbounded_int_difference(b,a);
        }else{
            b.signe='+';
            return unbounded_int_difference(a,b);
        }
    }
    chiffre *curseurA=a.dernier,
            *curseurB=b.dernier,
            *curseurS=NULL;
    int retenue=0,len=0;
    while((curseurA!=NULL || curseurB!=NULL) || retenue!=0){
        int resultat=0;
        if(curseurA!=NULL && curseurB!=NULL){
            resultat=(curseurA->c-'0')+(curseurB->c-'0')+retenue;
            curseurA=curseurA->precedent;
            curseurB=curseurB->precedent;
        } else if(curseurA!=NULL){
            resultat=(curseurA->c-'0')+retenue;
            curseurA=curseurA->precedent;
        }else if(curseurB!=NULL){
            resultat=(curseurB->c-'0')+retenue; 
            curseurB=curseurB->precedent;           
        }else{
            resultat=retenue;//Cela veut dire que la retenue n'est pas nulle on doit donc la rajouter
        }
        retenue=resultat/10;
        resultat=resultat%10;
        chiffre *new=createChiffre((char)(resultat+'0'));
        if(new==NULL){
            somme.signe='*';
            return somme;
        }else{
            if(somme.dernier==NULL){
                somme.dernier=new;
                curseurS=somme.dernier;
            }else{
                curseurS->precedent=new;
                curseurS->precedent->suivant=curseurS;
                curseurS=curseurS->precedent;
            }
            len++;
        }
    }
    somme.premier=curseurS;
    somme.len=len;
    return somme;
}
//---------------------------------------------------------------------------------------------------------------------------
unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b){
    if(a.signe=='*' || b.signe=='*'){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);
    }
    unbounded_int res;
    res.dernier=NULL;
    res.premier=NULL;
    res.len=0;
    res.signe='*';
    unbounded_int sauv_res=res;
    int retenue=0;
    if((a.signe=='+' && b.signe=='+') || (a.signe=='-' && b.signe=='-')){
        if(unbounded_int_cmp_unbounded_int(a,b)==0){
            res.signe='+';
            res.premier = createChiffre('0');
            if(res.premier==NULL){
                return sauv_res;
            }
            res.dernier=res.premier;
            res.len++;
            return res;
        }
        chiffre *p,
                *q,
                *maillon;
        if((unbounded_int_cmp_unbounded_int(a,b)==1 && a.signe=='+') || (unbounded_int_cmp_unbounded_int(a,b)==-1 &&a.signe=='-')){
            p=a.dernier;
            q=b.dernier;
            res.signe=a.signe;
        }
        else{
            p=b.dernier;
            q=a.dernier;
            if(a.signe=='+'){
                res.signe='-';
            }
            else{
                res.signe='+';
            }
        }
        while(p!=NULL && q!=NULL){
                char c;
                if((p->c - '0')-(q->c - '0')+retenue>=0){
                    c = (p->c- '0')-(q->c- '0')+retenue+'0';
                    retenue = 0;
                }
                else{
                    c =(p->c- '0')-(q->c- '0')+retenue+10+'0';
                    retenue = -1;
                }
                maillon = createChiffre(c);
                if(maillon==NULL){
                    return sauv_res;
                }
                if(res.dernier==NULL){
                    res.dernier=maillon;
                    res.premier=maillon;
                }
                else{
                    (res.premier)->precedent=maillon;
                    maillon->suivant=res.premier;
                }
                res.premier=maillon;
                p=p->precedent;
                q=q->precedent;
                res.len++;
        }
        while(p!=NULL){
            char c;
            if((p->c - '0')+retenue>=0){
                c = (p->c- '0')+retenue+'0';
                retenue = 0;
            }
            else{
                c =(p->c- '0')+retenue+10+'0';
                retenue = -1;
            }
            maillon = createChiffre(c);
            if(maillon==NULL){
                return sauv_res;
            }
            (res.premier)->precedent=maillon;
            maillon->suivant=res.premier;
            res.premier=maillon;
            p=p->precedent;
            res.len++;
        }
        free_0(&res);
        return res;
    }
    if(b.signe=='+'){
        b.signe='-';
    }
    else{
        b.signe='+';
    }
    return unbounded_int_somme(a,b);

}
//-------------------------------------------------------------------------------------------------------------------------------------
unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b){
    if(a.signe=='*' || b.signe=='*'){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);
    }
    unbounded_int produit={.signe='*',.len=0,.premier=NULL,.dernier=NULL};
    if(a.signe==b.signe){
        produit.signe='+';
    }else{
        produit.signe='-';
    }
    if(unbounded_int_cmp_ll(a,0)==0 || unbounded_int_cmp_ll(b,0)==0){
        produit.signe='+';
        chiffre *zero=createChiffre('0');
        if(zero==NULL){
            produit.signe='*';
            return produit;
        }
        produit.premier=zero;
        produit.dernier=produit.premier;
        produit.len++;
        return produit;
    }
    chiffre *curseurA=a.dernier,
            *curseurB=b.dernier,
            *curseurP1=NULL,
            *curseurP2=NULL;
    int retenue=0,len=0;
    while(curseurB!=NULL){
        int resultat=0;
        curseurA=a.dernier;
        while(curseurA!=NULL || retenue!=0){
            if(curseurA!=NULL){
                resultat=(curseurA->c-'0')*(curseurB->c-'0')+retenue;  
                curseurA=curseurA->precedent;      
            }else{
                resultat=retenue;//Cela veut dire que la retenue n'est pas nulle on doit donc la rajouter
            }
            if(curseurP1!=NULL && curseurP1->precedent!=NULL ){
                resultat+=curseurP1->precedent->c-'0';
                retenue=resultat/10;
                resultat=resultat%10;
                curseurP1->precedent->c=(char)(resultat+'0');
                curseurP1=curseurP1->precedent;
            }else{
                retenue=resultat/10;
                resultat=resultat%10;
            
                chiffre *new=createChiffre((char)(resultat+'0'));
                if(new==NULL){
                    produit.signe='*';
                    return produit;
                }else{
                    if(produit.dernier==NULL){
                        produit.dernier=new;
                        curseurP2=produit.dernier;
                        curseurP1=curseurP2;
                    }else{
                        curseurP1->precedent=new;
                        curseurP1->precedent->suivant=curseurP1;
                        curseurP1=curseurP1->precedent;
                    }   
                }
                len++;
            }
        }
        if(curseurB->precedent!=NULL){
            curseurP1=curseurP2;
            curseurP2=curseurP2->precedent;
        }
        curseurB=curseurB->precedent;

    }

    produit.premier=curseurP1;
    produit.len=len;
    return produit;

}
//-------------------------------------------------------------------------------------------------------------------------------------
