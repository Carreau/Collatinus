/*
 *                          libcollatinus.h
 *
 *  This file is part of COLLATINVS.
 *                                                                            
 *  COLLATINVS is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *                                                                            
 *  COLLATINVS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *                                                                            
 *  You should have received a copy of the GNU General Public License
 *  along with COLLATINVS; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

// AGENDA : toutes les defs de libcollatinus.cc à placer dans le header !!

/*
 *
 *  . flexions, synthèse de formes.
 *
 *  . renvois "cf." trop vagues.
 *      - mettre en place un renvoi exclusif : 
 *        utiliser la balise "pl. " pour les formes
 *        sans singulier.
 *        Le même résultat peut être obtenu en
 *          - détectant -ae, -i, -es (ambigu) ;
 *          - éliminant les lemmatisations au singulier.
 *
 *       D'autres renvois exclusifs sont sans doute à mettre en place.
 *
 */

#ifndef LIBCOLLATINUS_H
#define LIBCOLLATINUS_H

#include <map>
#include <list>
#include <string>

/*   Fonctions de conversion  */
std::string ini (std::string c, int n); // ini  donne la chaine c tronquée de n caractères à droite
std::string jviu (std::string ch);      // jviu transforme la graphie ramiste en graphie ancienne
std::string basDeCasse (std::string s);
std::string indente (std::string s);
std::string sinesuffixo (std::string e, std::string s); // utile pour les formes en -que ou -ue.
int aRomano (std::string R); // Fonction de conversion des chiffres romains en chiffres arabes.

/* classes */
class Tdes
{
    private:
        std::string gr; // graphie
        int c;     // cas
        int g;     // genre
        int n;     // nombre
        int d;     // degré (gradus)
        int p;     // personne
        int t;     // temps
        int m;     // mode
        int v;     // voix
        int mo;    // modèle (paradigma)
        int rn;    // numéro de radical (radix_num)
    public:
        /*
         * Format d'une ligne de lemmata.fr :
         * graph|casus|genus|numerus|gradus|persona|tempus|modus|uox|paradigma|radix_num
         */
        Tdes (std::string linea);
        Tdes ( std::string l, int k, int ge, int num, int deg, 
                int per, int tem, int mod, int vo, 
                int  mdl, int radn);
        // affectations
        void da_c (int x);
        void da_g (int x);
        void da_n (int x);
        void da_d  (int x);
        void da_p  (int x);
        void da_t  (int x);
        void da_m  (int x);
        void da_v  (int x);
        void da_mo (int x);
        void da_rn (int x);
        // consultation string
        std::string graphie ();
        int mdl ();
        std::string modele ();
        int casus ();
        std::string cas ();
        int genus ();
        std::string genre ();
        int persona ();
        std::string personne ();
        int numerus ();
        std::string nombre ();
        int gradus ();
        std::string degre ();
        int tempus ();
        std::string temps ();
        // consultation integer
        int modus ();
        std::string mode ();
        int uox ();
        std::string voix ();
        int nr ();
        std::string morpho ();
        std::string doc ();
        void affiche ();
};

class AnalyseMorpho : public Tdes
{
    private:
        std::string lemme;
    public:
        std::string lemma ();
        AnalyseMorpho ( 
                std::string l, int k, int ge, int num, int deg, 
                int per, int tem, int mod, int vo, 
                int  mdl, int radn);
};

typedef std::multimap<std::string, AnalyseMorpho> ListeAnalyses;

/* classe des irréguliers */
class Tirr
{
    private:
        std::string gr;
        std::string k;
        int c;
        int g;
        int n;
        int d;
        int p;
        int t;
        int m;
        int v;
    public:
        int casus ();
        int genus ();
        int gradus ();
        int persona ();
        int numerus ();
        int tempus ();
        int modus ();
        int uox ();
        Tirr (std::string linea);
        std::string canon ();
        std::string graphie ();
        std::string morpho ();
        void affiche ();
};

/* classe des radicaux */

class Tradix
{
    private:
        std::string can;
        int mo;
        int num;
    public:
        Tradix (std::string c, int m, int n);
        int mdl ();
        int nr ();
        std::string canon ();
        std::string doc ();
        void affiche ();
};   

/* classe des entrées */
class Tentree
{
    private:
        std::string k;
        int mdl;
        std::string radp;
        std::string rads;
        std::string def;
        bool vide;
        std::string ote2 (std::string k);
    public:
        Tentree (std::string linea);
        Tentree ();
        std::string canon ();
        int modele ();
        char pars ();
        std::string radix ();
        std::string radical (int n);
        std::string  grmodele ();
        std::string rperfectum ();
        std::string rsupinum () ;
        std::string definition () ;
        bool est_neutre (); // vrai si la définition inclut la chaîne ", .n ".
        bool est_pluriel ();
        bool est_deponent ();
        std::string morphoK ();
        AnalyseMorpho amMorphoK ();
        AnalyseMorpho amMorphoKN ();
        std::string doc ();
        bool est_vide ();
        void affiche ();
};


/* classe lexique */
class TLexicum
{
    private:
        bool uacuum;
        // liste d'entrées
        typedef std::map<std::string, Tentree*> c2e;
        // liste des désinences
        typedef std::multimap<std::string, Tdes> c2d; 
        // liste des irréguliers
        typedef std::multimap<std::string, Tirr> c2i;
        // liste des radicaux
        typedef std::multimap<std::string, Tradix> c2r;
        // std::multimap des désinences
        c2d desinentiae;
        // std::multimap des irréguliers
        c2i irregulares;
        // std::multimap des entrées
        c2e entrees;
        // std::multimap des radicaux
        c2r radices;
        // type liste de radicaux
        typedef std::list<Tradix> lr;
        void radices_adde (std::string k, std::string r, int m, int n);
        std::string ote2 (std::string k);
    public:
        typedef std::map<std::string, std::string> ee;
        typedef ee::iterator e2m;
        ee liste_e;
        TLexicum (std::string url);
        TLexicum ();
        Tentree *renvoi (Tentree *e);  // renvoi d'entrée si la déf. commence par cf.
        Tentree * entree (std::string k);
        void uniLanalyses (ListeAnalyses& l1, ListeAnalyses& l2);
        std::string irregulier (std::string f);
        std::string radical (std::string r);
        std::string desinence (std::string d);
        void ajoute (std::string e, std::string ch);
        std::string analyses (std::string f);
        void lajoute (std::string l, ListeAnalyses& la, AnalyseMorpho am);
        void lajoute (std::string l, ListeAnalyses& la, Tdes d);
        ListeAnalyses lanalyses (std::string f);
        std::string lemmatiseM (std::string f); // analyse morphologique
        std::string lemmatise (std::string f); // simple lemmatisation
        std::string desN (Tentree* e, int c, int n);
        std::string desA (Tentree* e, int c, int n, int g, int d);
        std::string synthA (Tentree* e, int c, int n, int g, int d);
        std::string synthPart (Tentree* e, int c, int g, int n, int t, int v);
        std::string synthN (Tentree *e, int c, int n);
        std::string synthP (Tentree* e, int c, int n, int g);
        std::string synthV (Tentree* e, int p, int n, int t, int m, int v, int rn=1);
        std::string flechis (Tentree* e);
};

// fonctions globales qui évitent une 
// double déclaration de lexicum

void lexicumLege (std::string lang, std::string c);
void lexicumDic (std::string lang);
void activeCalepin (bool a);
bool calepino_actif ();
std::string ambrogio (std::string m);
std::string lemmatise (std::string f);
std::string lemmatiseM (std::string f);
Tentree * entree (std::string linea);
std::string flechis (Tentree* e);
ListeAnalyses lanalyses (std::string f);
void uniLanalyses (ListeAnalyses& l1, ListeAnalyses& l2);

#endif
