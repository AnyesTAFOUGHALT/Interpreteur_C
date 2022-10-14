#include <stdio.h>

#include "unbounded_int.h"

static void afficher_unbounded_int(unbounded_int a){
    if(a.signe=='*'){
        perror("Erreur lors de l'allocation d'un espace memoire");
        exit(EXIT_FAILURE);
    }
    chiffre *p=a.premier;
    printf("%c",a.signe);
    while(p!=NULL){
        printf("%c",p->c);
        p=p->suivant;
    }
}

int main()
{   
    unbounded_int  chiffre1 =string2unbounded_int("1230");
    unbounded_int  chiffre2 =ll2unbounded_int(520);

    int comparaison_unbounded_ll=unbounded_int_cmp_ll(chiffre2,520);
    printf("La comparaison entre unbounded et notre long est de : %d \n", comparaison_unbounded_ll);

    int comparasion_unb_unb=unbounded_int_cmp_unbounded_int(chiffre1,chiffre2);
    printf("La comparaison entre unbounded(1230) et undounded(520) est de : %d \n", comparasion_unb_unb);

    unbounded_int  somme=unbounded_int_somme(chiffre1,chiffre2);
    printf("\nSomme de 1230 et 520 est de : %s", unbounded_int2string(somme));
    printf("\n");

    
    unbounded_int  chiffre3 =string2unbounded_int("2022");
    unbounded_int  chiffre4 =string2unbounded_int("57");
    unbounded_int  diff=unbounded_int_difference(chiffre3,chiffre4);
    printf("\nDifference de 2022 et 57 est de : ");
    afficher_unbounded_int(diff);
    printf("\n");

    unbounded_int  chiffre5 =string2unbounded_int("27");
    unbounded_int  chiffre6 =string2unbounded_int("12");
    unbounded_int  produit=unbounded_int_produit(chiffre5,chiffre6);
    printf("\nProduit de 27 et 12 est de : ");
    afficher_unbounded_int(produit);
    printf("\n");

    unbounded_int  chiffre7 =string2unbounded_int("-1265465646666666666665444444444467665530");
    unbounded_int  chiffre8 =string2unbounded_int("-65468789868787987645555555555555411");
    somme=unbounded_int_somme(chiffre7,chiffre8);
    printf("\nSomme  de -1265465646666666666665444444444467665530 et -65468789868787987645555555555555411 est de : ");
    afficher_unbounded_int(somme);
    printf("\n");
    
    unbounded_int  chiffre9 =string2unbounded_int("-65465444444444444444445121325465");
    unbounded_int  chiffre10 =string2unbounded_int("6565423135476875465434");
    diff=unbounded_int_difference(chiffre9,chiffre10);
    printf("\nDifference de -65465444444444444444445121325465 et 6565423135476875465434 est de   : ");
    afficher_unbounded_int(diff);
    printf("\n");

    unbounded_int  chiffre11 =string2unbounded_int("-53435468654656354867641265787");
    unbounded_int  chiffre12 =string2unbounded_int("235468789421254658642568");
    produit=unbounded_int_produit(chiffre11,chiffre12);
    printf("\nProduit de -53435468654656354867641265787 et 235468789421254658642568 est de  : ");
    afficher_unbounded_int(produit);
    printf("\n");

    return 0;
}
