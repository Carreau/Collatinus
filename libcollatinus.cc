/*
 *                           libcollatinus.cc
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

/*
 *                       AGENDA
 *
 *  . vérifier les désinences des adjectif en -er -ra -rum (noster)
 *
 *  . dans la partie flexion : 
 *     
 *     - vérifier la présence dans lemmata.fr de toutes les formes pronominales
 *     - affichage indésiré de formes défectives (dat. de uis)
 *     - affichage indésiré du comparatif et superlatif de
 *       certains irréguliers
 *     - traiter le cas des verbes sans passif à cause de leur sens
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <libcollatinus.h>

using namespace std;

string cass [7] =
    {"", "nom.", "uoc.", "acc.", "gen.", "dat.", "abl."};
string genres [4] = {"", "masc.", "fem.", "neut."}; 
string nombres [3] = { "", "sing.", "plur."};
string personnes [4] = { "", "prim.", "sec.", "tert."};
string degres [4] = { "", "posit.", "compar.", "superl."};
string tempss [7] = { "", "praes.", "fut.", "imperf.", "perf.",
                      "fut. ant.", "plus-quam-perf."};
string modes [8] = 
{
    "", "ind.", "coniunct.", "imper.", "infin.", // 0 - 4
    "part.", "gerund.", "adiect. verb."
};             // 5 - 7

string voixs [3] = { "", "act.", "pass."};

string modeles [31] = 
{
    "uita", "amicus", "puer", "ager", "templum",        //  0 - 4  
    "miles", "ciuis", "corpus", "mare", "manus",        //  5 - 9  
    "res", "bonus", "miser", "pulcher", "fortis",       // 10 - 14 
    "uetus", "acer", "amo", "moneo", "lego",            // 15 - 19 
    "capio", "audio", "sum", "eo", "imitor",            // 20 - 24 
    "uereor", "sequor", "patior", "potior", "pronoms",  // 25 - 29  
    "invaria"
};


/**
 * ini  donne la chaine c tronquée de n caractères à droite
 *
 */
string ini (string c, int n)
{
    return c.replace (c.length () - n, n, "");
}

/** 
 * jviu transforme la graphie ramiste en graphie ancienne
 *
 */
string jviu (string ch)
{
    size_t i;
    for (i=0; i < ch.length(); i++)
    {
        if (ch[i] == 'v') ch[i] = 'u';
        else if (ch[i] == 'j') ch[i] = 'i';
        else if (ch[i] == 'J') ch[i] = 'I'; // pour Jovis et Junonis
    }
    return ch;
}

string basDeCasse (string s)
{
    size_t i;
    for (i=0; i<s.length();i++)
        s[i] = tolower (s[i]);
    return s;
}

string indente (string s)
{
    return "    " + s;
}

/**
 * Débarrasse éventuellement e de son suffixe s)
 *
 */
string sinesuffixo (string e, string s)
{
    size_t u = e.rfind (s);
    size_t ls = s.length ();
    if (u != string::npos && u == e.length () - ls)
        return e.replace (u, ls, ""); 
    return e;
}

/**
 * Fonction de conversion 
 * des chiffres romains en chiffres arabes.
 *
 */

bool est_romain (string R)
{
	string rr = "IVXLCDM";
	for (size_t i=0;i<R.size();i++)
		if (rr.find (R[i]) == string::npos) return false;
	return true;
}

int aRomano (string R)
{
    if (R.size () == 0
      || R.find ("IX") != string::npos 
      || R.find ("IL") != string::npos 
      || R.find ("IC") != string::npos 
      || R.find ("ID") != string::npos 
      || R.find ("IM") != string::npos
      || R.find ("VC") != string::npos
      || R.find ("VL") != string::npos
      || R.find ("VD") != string::npos
      || R.find ("VM") != string::npos
      || R.find ("XD") != string::npos
      || R.find ("XM") != string::npos
      || R.find ("CM") != string::npos
      || R.find ("DM") != string::npos
    ) return 0;
	
    if (R.size () >= 2)
    {
        // traitement des soustractions
        if (R[0] == 'C' && R[1] == 'D') 
           return 400 + aRomano (R.substr (2)); 
        if (R[0] == 'X' && R[1] == 'L') 
            return 40  + aRomano (R.substr (2));
        if (R[0] == 'I' && R[1] == 'V') 
            return 4   + aRomano (R.substr (2));
    }
    if (R[0] == 'M')
        return 1000 + aRomano (R.substr (1));
    if (R[0] == 'D')
        return 500 + aRomano (R.substr (1));
    if (R[0] == 'C')
        return 100 + aRomano (R.substr (1));
    if (R[0] == 'L')
        return 50 + aRomano (R.substr (1));
    if (R[0] == 'X')
        return 10 + aRomano (R.substr (1));
    if (R[0] == 'V')
        return 5 + aRomano (R.substr (1));
    return 1 + aRomano (R.substr (1));
}


/*
int aRomano (string R)
{
    //return romain_vers_decimal (R );
    //R = basDeCasse (R);
    string L[13] = { "IV", "IX", "XL", "XC", "CD", "CM",
                     "I",  "V",  "X",  "L",  "C",  "D", "M" };
    int N [13]   = {  4,    9,   40,   90,   400,  900,
                      1,    5,   10,   50,   100,  500, 1000 };
    int i;
    int retour = 0;
    while (R.length () != 0)
    {
        i = 0;
        while (i < 13)
        {
            if (R.find (L[i]) == 0)
            {
                retour += N[i];
                R.erase (0, L[i].length ());
                i = 13;
            }
            else if (i < 12) i++;
            else return 0;
        }
    }
    return retour;
}
*/

/********************\
 * Les désinences   *
\********************/
Tdes::Tdes (string linea)
{
    //gr|c|g|n|d|p|t|m|v|mdl|rn
    string eclat;
    istringstream lin (linea);
    getline (lin, eclat, '|');
    gr = eclat;
    getline (lin, eclat, '|');
    istringstream ic (eclat);
    ic >> c;
    getline (lin, eclat, '|');
    istringstream ig (eclat);
    ig >> g;
    getline (lin, eclat, '|');
    istringstream in (eclat);
    in >> n;
    getline (lin, eclat, '|');
    istringstream id (eclat);
    id >> d; 
    getline (lin, eclat, '|');
    istringstream ip (eclat);
    ip >> p; 
    getline (lin, eclat, '|');
    istringstream it (eclat);
    it >> t; 
    getline (lin, eclat, '|');
    istringstream im (eclat);
    im >> m; 
    getline (lin, eclat, '|');
    istringstream iv (eclat);
    iv >> v; 
    getline (lin, eclat, '|');
    istringstream imdl (eclat);
    imdl >> mo; 
    getline (lin, eclat, '|');
    istringstream irn (eclat);
    irn >> rn; 
}

Tdes::Tdes ( std::string l, int k, int ge, int num, int deg, 
                int per, int tem, int mod, int vo, 
                int  mdl, int radn)
{
    gr = l;
    c = k;
    g = ge;
    n = num;
    d = deg;
    p = per;
    t = tem;
    m = mod;
    v = vo;
    mo = mdl;
    rn = radn;
}

string Tdes::graphie ()
{
    return gr;
}

int Tdes::mdl ()
{
    return mo;
}

string Tdes::modele ()
{
    return modeles[mo];
}

int Tdes::casus ()
{
    return c;
}

string Tdes::cas ()
{
    return cass[c];
}

int Tdes::genus ()
{
    return g;
}

string Tdes::genre () 
{
    return genres[g];
}

int Tdes::persona ()
{
    return p;
}

string Tdes::personne ()
{
    return personnes [p];
}

int Tdes::numerus ()
{
    return n;
}

string Tdes::nombre ()
{
    return nombres [n];
}

int Tdes::gradus ()
{
    return d;
}

string Tdes::degre () 
{
    return degres [d];
}

int Tdes::tempus ()
{
    return t;
}

string Tdes::temps () 
{
    return tempss [t];
}

int Tdes::modus ()
{
    return m;
}

string Tdes::mode () 
{
    return modes [m];
}

int Tdes::uox () 
{
    return v;
}

string Tdes::voix () 
{
    return voixs [v];
}

int Tdes::nr ()
{
    return rn;
}

string Tdes::morpho ()
{
    const string s (" ");
    stringstream sortie;
    sortie << cas () << s << genre () << s << personne ()
        << s << nombre () << s << degre () << s << temps () << s << mode () << s << voix ();
    return sortie.str ();
}

string Tdes::doc ()
{
    stringstream ss;
    ss << "R" << rn << ", modèle " << mo << " " << morpho ();
    return ss.str ();
}


void Tdes::affiche ()
{
    cout << gr << " " << cass[c] << " " << genres[g] << " " << modeles[mo] << endl;
}

// affectations
void Tdes::da_c (int x)
{
    c = x;
}

void Tdes::da_g (int x)
{
    g = x;
}

void Tdes::da_n (int x)
{
    n = x;
}

void Tdes::da_d  (int x)
{
    d = x;
}


void Tdes::da_p  (int x)
{
    p = x;
}


void Tdes::da_t  (int x)
{
    t = x;
}


void Tdes::da_m  (int x)
{
    m = x;
}


void Tdes::da_v  (int x)
{
    v = x;
}


void Tdes::da_mo (int x)
{
    n = x;
}


void Tdes::da_rn (int x)
{
   rn = x;
}

/*
Tdes::Tdes (
    string l, int k, int ge, int num, int deg, 
    int per, int tem, int mod, int vo, 
    int  mdl, int radn)
{
    gr = l;
    c = k;     // cas
    g = ge;     // genre
    n = num;     // nombre
    d = deg;     // degré (gradus)
    p = per;     // personne
    t = tem;     // temps
    m = mod;     // mode
    v = vo;     // voix
    mo = mdl;    // modèle (paradigma)
    rn = radn;    // numéro de radical (radix_num)
}
*/

/********************/
/*   AnalyseMorpho  */
/********************/

AnalyseMorpho::AnalyseMorpho ( 
    std::string l, int k, int ge, int num, int deg, 
    int per, int tem, int mod, int vo, 
    int  mdl, int radn) : Tdes (l,k,ge,num,deg,per,tem,mod,vo,mdl,radn)
{
   lemme = l; 
}

string AnalyseMorpho::lemma ()
{
    return lemme;
}

/********************/
/* Les irréguliers  */
/********************/

Tirr::Tirr (string linea)
{
    // maximis|magnus|5|1|2|0|0|0|0|0
    // gr        k    c g n d p t m v 
    string eclat; 
    istringstream lin (linea);
    getline (lin, eclat, '|');
    gr = eclat;
    getline (lin, eclat, '|');
    k = eclat;
    getline (lin, eclat, '|');
    istringstream ic (eclat);
    ic >> c;
    getline (lin, eclat, '|');
    istringstream ig (eclat);
    ig >> g;
    getline (lin, eclat, '|');
    istringstream in (eclat);
    in >> n;
    getline (lin, eclat, '|');
    istringstream id (eclat);
    id >> d;
    getline (lin, eclat, '|');
    istringstream ip (eclat);
    ip >> p;
    getline (lin, eclat, '|');
    istringstream it (eclat);
    it >> t;
    getline (lin, eclat, '|');
    istringstream im (eclat);
    im >> m;
    getline (lin, eclat, '|');
    istringstream iv (eclat);
    iv >> v;
}

int Tirr::casus ()
{
    return c;
}

int Tirr::genus ()
{
    return g;
}

int Tirr::gradus ()
{
    return d;
}

int Tirr::persona ()
{
    return p;
}

int Tirr::numerus ()
{
    return n;
}

int Tirr::tempus ()
{
    return t;
}

int Tirr::modus ()
{
    return m;
}

int Tirr::uox ()
{
    return v;
}

string Tirr::canon ()
{
    return k;
}

string Tirr::graphie ()
{
    return gr;
}

string Tirr::morpho ()
    /*
       string gr ; // graphie
       string k;   // canon
       int c;      // cas
       int g;      // genre
       int n;      // nombre
       int d;      // degré
       int p;      // personne
       int t;      // temps
       int m;      // mode
       int v;      // voix     
       */
{
    stringstream ss;
    string s (" ");
    ss << cass [c] << s;
    ss << genres [g] << s;
    ss << nombres [n] << s;
    ss << degres [d] << s;
    ss << personnes [p] << s;
    ss << tempss [t] << s;
    ss << modes [m] << s;
    ss << voixs [v] ;
    return ss.str ();
}

void Tirr::affiche ()
{
    cout << gr << " " << k << endl; 
}


/* classe des radicaux */

Tradix::Tradix (string c, int m, int n)
{
    can = c;
    mo = m;
    num = n;
}

int Tradix::mdl ()
{
    return mo;
}

int Tradix::nr ()
{
    return num;
}

string Tradix::canon ()
{
    return can;
}

string Tradix::doc ()
{
    stringstream ss;
    ss << "rad. " << num << " de " << can << " (" << modeles[mo] << ")";
    return ss.str ();
}

void Tradix::affiche ()
{
    cout << "rad. " << num << " de " << can << " (" << modeles[mo] << ")" <<  endl; 
}


/* classe des entrées */

string Tentree::ote2 (string k)
{
    if (k.rfind (" (2)") != string::npos || k.rfind (" (3)") != string::npos)
        return k.replace (k.length () - 4, 4, "");
    else return k;
}

Tentree::Tentree (string linea)
{
    string eclat; 
    istringstream lin (linea);
    getline (lin, eclat, '|');
    k =  eclat;
    getline (lin, eclat, '|');
    istringstream m (eclat);
    m >> mdl;
    getline (lin, eclat, '|');
    radp = eclat;
    getline (lin, eclat, '|');
    rads = eclat;
    getline (lin, eclat);
    def = eclat;
    vide = false;
}

Tentree::Tentree ()
{
    vide = true;
}

string Tentree::canon ()
{
    return k;
}

int Tentree::modele ()
{
    return mdl;
}

char Tentree::pars ()
{
    if (mdl < 11) return 'n';
    if (mdl < 17) return 'a';
    if (mdl < 29) return 'v';
    if (mdl == 29) return 'p';
    return 'i';
}

string Tentree::radix ()
    /* radix représente pour les noms et 
     * adjectifs le radical de génitif,
     * et pour les verbes le radical d'infectum.
     */
{
    // ôter le " (2)" qui suit les entrées doublons
    string ka = ote2 (k);
    if (k == "sum" || k == "eo")
        return ".";
    switch (mdl)
    {
        case 5:
        case 6:
        case 7:
        case 8:
        case 14:
        case 15:
            if (radp > "") return radp;
            else return ""; 
            break; 
        case 0:
            if (radp > "") return radp;
        case 17:
            //if (radp > "") return radp;
            return ini (ka, 1);
            break;
        case 19:
            return ini (ka, 1);
            break;
        case 2:
            if (radp > "") return radp;
            return k;
        case 1:
        case 3:
        case 4:
        case 9:
        case 10:
        case 11:
        case 12:
            if (radp > "") return radp;
            else return ini (ka, 2);
            break;
        case 18:
        case 20:
        case 21:
        case 23:
        case 24:
        case 26:
            return ini (ka, 2);
            break;
        case 13:
        case 16:
            return ini (ka, 2) + "r";
            break;
        case 22:
        case 25:
        case 27:
        case 28:
            return ini (ka, 3);
            break;
        default:
            return string ("");
    }
    return string ("");
}

string Tentree::radical (int n)
{
    if (n == 1) return radix ();
    else if (n == 2) 
    { 
        // pour le superl. en -errimus
        if (mdl == 16 || mdl == 13) return ini (k, 2);
        // calculer le radical de parfait des
        // verbes de modèle amo, en respectant
        // celui qui est donné pour do, sto et
        // leurs composés.
        if (!radp.empty () || mdl != 17) return radp;
        return radix () + "au";
    }
    //else if (n == 3) return rads;
    else
    {
        // même méthode pour le radical de supin
        if (!rads.empty () /*&& mdl != 17 && mdl != 24*/ ) return rads;
        else if (mdl == 17 || mdl == 24) return radix () + "at";
        else return "";
    }
}

/*
// string renvoi_cf () //canon renvoyé par le cf. ... en tête de définition 

*/
string Tentree::grmodele ()
{
	if (mdl < 0 || mdl > 30) return "error";
    return modeles [mdl];
}

string Tentree::rperfectum ()
{
    return radp;
}

string Tentree::rsupinum () 
{
    return rads;
}

string Tentree::definition () 
{
    return def; 
}

/**
 * vrai si la définition inclut la chaîne ", .n ".
 */
bool Tentree::est_neutre ()
{
    return def.find (", n. ") < def.length ();
}

bool Tentree::est_pluriel ()
{
    return def.find (" pl. ") < def.length ();
}

bool Tentree::est_deponent ()
{
    return (mdl > 23 && mdl < 29);
}

string Tentree::morphoK ()
{
    string nbr;
    if (est_pluriel ())
        nbr = "plur. ";
    else nbr = "sing. ";
    if (mdl < 11) 
    {
        stringstream ss;
        ss << "nominatif " + nbr ;
        if (est_neutre ()) 
            ss << endl << indente ("acc " + nbr); 
        return ss.str ();
    }
    else if (mdl < 17) 
	{
		stringstream ss;
		ss << "nomin. masc. " + nbr;
		if (mdl > 11)
			ss << endl << "voc. masc. " + nbr;
		return ss.str ();
	}
    else if (mdl < 29)
        return "prim. sing. praes. indic. act.";
    else return "";
}

AnalyseMorpho Tentree::amMorphoK ()
{

    int c=0, n=0, g=0, p=0, d=0, m=0, t=0, v=0;
    int nbr = 1;
    if (est_pluriel ())
        nbr = 2;
    if (mdl < 11)
    {
        n = nbr;
        c = 1;
    }
    else if (mdl < 17 || mdl == 29)
    {
        c = 1;
        g = 1;
        n = nbr;
    }
    else if (mdl < 29)
    {
        p = 1;
		n = 1;
        t = 1;
        m = 1;
        v = 1;
    }
    AnalyseMorpho am = AnalyseMorpho (k, c, g, n, d, p, t, m, v, mdl, 1);
    return am;
}

AnalyseMorpho Tentree::amMorphoKN ()
// ajoute automatiquement la morpho acc. neutre.
{
    AnalyseMorpho am = AnalyseMorpho (k, 3, 0, 1, 0, 0, 0, 0, 0, mdl, 1);
    return am;
}

string Tentree::doc ()
{
    stringstream ss;
    ss << k << ", " << def; 
    return ss.str ();
}

bool Tentree::est_vide ()
{
    return vide;
}

void Tentree::affiche ()
{
    cout << k << " " << modeles[mdl] << " radical " << radix () << " : " << def << endl;
}
 
/*******************
** classe lexique **
********************/

void TLexicum::radices_adde (string k, string r, int m, int n)
{
    /*
     *    Insère le radical de graphie r, de modèle m  et de numéro n
     *    dans la table des radicaux. S'il y a une ou plusieurs 
     *    virgules dans la chaîne r, elle est éclatée et chaque
     *    éclat est inséré, avec les mêmes modèle et numéro.
     *
     */
    if (r == "") return;
    string eclat;
    size_t pos = r.find(',');
    while (pos != string::npos) 
    {
        eclat = r.substr (0, pos);
        // insérer le radical
        radices.insert (c2r::value_type (eclat, Tradix (k, m, n)));
        r.erase (0, pos+1);
        pos = r.find_first_of (",", 1);
    } 
    radices.insert (c2r::value_type (r, Tradix (k, m, n)));
}

string TLexicum::ote2 (string k)
{
    if (k.rfind (" (2)") != string::npos || k.rfind (" (3)") != string::npos)
        return k.replace (k.length () - 4, 4, "");
    else return k;
}

TLexicum::TLexicum ()
{
   uacuum = true;
}

TLexicum::TLexicum (string url)
{
    ifstream Capsa;
    string linea;
    int pars = 0;
    Capsa.open (url.c_str (), ios::in); 
    while (getline (Capsa, linea))
    {
<<<<<<< HEAD
        if (linea.empty () || linea[0] == '!') continue;
=======
        if (linea.empty () or linea[0] == '!') continue;
>>>>>>> 14e07471d012cf0906eeadb57bd6f59ca0ff0cf6
        // analyse d'un éventuel en-tête de partie
        else if (linea == "---desinentiae---")
        {
            pars = 1;
            continue;
        }
        else if (linea == "---irregulares---")
        {
            pars = 2;
            continue;
        }
        // peuplement des lexiques en fonction de pars
        else if (pars == 0)
        {
            Tentree *E = new Tentree (linea);
            entrees.insert (c2e::value_type (E->canon (), E));
            string r1 = E->radical (1);
            string r2 = E->radical (2);
            string r3 = E->radical (3);
            if (r1.length () != string::npos)
                radices_adde (E->canon (), r1, E->modele (), 1);
            if (r2.length () != string::npos)
                radices_adde (E->canon (), r2, E->modele (), 2);
            if (r3.length () != string::npos)
                radices_adde (E->canon (), r3, E->modele (), 3);
        }
        else if (pars == 1)
        {
            Tdes D = Tdes (linea);
            desinentiae.insert (c2d::value_type (D.graphie (), D));
        }
        else
        {
            Tirr I = Tirr (linea);
            irregulares.insert (c2i::value_type (I.graphie (), I)); 
        }
    }
    Capsa.close();
    //cerr << ":" << linea << ":" << endl;
}

/**
 * Renvoi d'entrée.
 * Détecte si l'entrée pointe sur une autre entrée,
 * et dans ce cas renvoie cette autre entrée.
 * sinon, renvoie la même entrée
 */
Tentree *TLexicum::renvoi (Tentree *e)
{
    string k4 (e->definition (), 0, 4);
    if (k4 == "cf. ")
        return entree (string (e->definition (), 4, 100));
    else return e;
}

Tentree * TLexicum::entree (string k)
{
    k = jviu (k);
    c2e::iterator i;
    i = entrees.find (k); 
    if (i != entrees.end ())
        return renvoi (i->second);
    else return new Tentree ();
}

string TLexicum::irregulier (string f)
{
    f = jviu (f);
    c2i::iterator i;
    stringstream ss;
    i = irregulares.lower_bound (f);
    while (i != irregulares.upper_bound (f))
    {
        ss << endl << i->second.morpho ();
        ++i;
    }
    return ss.str ();
}

string TLexicum::radical (string r)
{
    c2r::iterator ir;
    stringstream ss;
    ir = radices.lower_bound (r);
    while (ir != radices.upper_bound (r))
    {
        ss << ir->second.doc () << endl;
        ++ir;
    }
    return ss.str ();
}

string TLexicum::desinence (string d)
{
    d = jviu (d);
    stringstream ss;
    c2d::iterator i;
    i = desinentiae.lower_bound (d); 
    while (i != desinentiae.upper_bound (d))
    {
        ss << i->second.doc () << endl;
        ++i;
    }
    return ss.str ();
}

void TLexicum::ajoute (string e, string ch)
{
    e2m ie = liste_e.find (e); 
    if ((ie) == liste_e.end ())
    {
        liste_e.insert (ee::value_type (e, ch));
    }
    else if (ch.length () > 0) 
    {
        string txt = ie->second; 
        txt +=  ("\n    " + ch); 
        ie->second = txt;
    }
}


// simple lemmatisation
string TLexicum::analyses (string f)
{
    ostringstream ss;

    // ramismes
    f = jviu (f);

    // variables
    lr liste_r;
    c2d::iterator id;
    c2r::iterator ir;
    c2i::iterator ii;
    lr::iterator ilr;

    // initialisation de la liste de retour
    liste_e.clear ();

    // recherche dans les formes canoniques ;
    Tentree *e = entree (f);
    if (!e->est_vide () && !e->est_pluriel ())
        ajoute (e->doc (), "");

    // et recherche du doublon
    e = entree (f + " (2)");
    if (!e->est_vide ())
        ajoute (e->doc (), "");

    e = entree (f + " (3)");
    if (!e->est_vide ())
        ajoute (e->doc (), "");

    // recherche de toutes les possibilités radical.désinence
    // initialisation des variables de la boucle
    string r, d;
    int l = f.length ();
    int i;
    // début de boucle
    for (i = 0; i < l; i++)
    {
        // calcul du radical et de la désinence       
        r = f.substr (0, i);
        d = f.substr (i, l-i);

        // pour les radicaux nuls :
        if (r.length () == 0)
            r = ".";

        // recherche et mise en liste des radicaux
        ir = radices.lower_bound (r);
        liste_r.clear ();
        while (ir != radices.upper_bound (r))
        {
            liste_r.push_back (ir->second);
            ir++;
        }

        // recherche des désinences
        id = desinentiae.lower_bound (d); 
        while (id != desinentiae.upper_bound (d))
        {
            // boucle de recherche dans les radicaux
            ilr = liste_r.begin ();
            while (ilr != liste_r.end ())
            {
                // comparaison du modèle et du n° de radical
                if ((ilr->mdl () == id->second.mdl ()) && (ilr->nr () == id->second.nr ()))
                {
                    e = entree (ilr->canon ()); 
                    if (!(e->est_pluriel () && id->second.numerus () == 1))
                        ajoute (e->doc (), "");
                }
                ++ilr;
            }
            ++id;
        }
    } // fin de boucle de recherce r.d 

    // recherche dans les irréguliers
    ii = irregulares.lower_bound (f); 
    while (ii != irregulares.upper_bound (f))
    {
        e = entree (ii->second.canon ());
        ajoute (e->doc (), "");
        ++ii;
    }

    // dépouillement de la liste & mise en forme de la sortie 
    //string proxima;          // une recherche sur la liste serait plus judicieuse
    for(e2m ee = liste_e.begin(); ee != liste_e.end(); ee++)
    {
        ss << endl << ee->first;    
        //proxima = ee->first;
    }

    // conversion du flux et retour
    return ss.str (); 
}

void TLexicum::lajoute (string l, ListeAnalyses& la, AnalyseMorpho am)
{
    la.insert (ListeAnalyses::value_type (l, am));
    //la.insert (pair<string, AnalyseMorpho>(l, am));
}

void TLexicum::lajoute (string l, ListeAnalyses& la, Tdes d)
{
    AnalyseMorpho am = AnalyseMorpho (l, d.casus (), d.genus (), d.numerus (),
         d.gradus (), d.persona (), d.tempus (), d.modus (), d.uox (), 
         0, d.nr ());
    la.insert (ListeAnalyses::value_type (l, am));
    //la.insert (ListeAnalyses::value_type ("debog lajoute tdes", am));
}

/**
 * Réécriture de TLexicum::analyses, 
 * en utilisant une sortie sous
 * forme de liste d'analyses, dont tous
 * les items sont des instances de la
 * classe AnalyseMorpho, ce qui permet
 * une bonne récupération de la sortie.
 *
 * Cette méthode, sera sans doute celle
 * du Collatinus de base, mais il faudra
 * procéder à un classement des analyses
 * par lemme, de façon à ne pas avoir
 * une répétition du lemme.
 */
ListeAnalyses TLexicum::lanalyses (string f)
{
    // ramismes
    f = jviu (f);

    // variables
    ListeAnalyses la;
    lr liste_r;
    c2d::iterator id;
    c2r::iterator ir;
    c2i::iterator ii;
    lr::iterator ilr;

    // initialisation de la liste de retour
    liste_e.clear ();

    // recherche dans les formes canoniques ;
    Tentree *e = entree (f);
    if (!e->est_vide () && !e->est_pluriel ())
    {
        lajoute (e->canon (), la, e->amMorphoK ()); 
        if (e->est_neutre ())
            lajoute (e->canon (), la, e->amMorphoKN ());
		// pour fortis, uetus
		if (e->modele () == 14 || e->modele () == 15)
		{
			AnalyseMorpho am = AnalyseMorpho (e->canon (), 1, 2, 1, 0, 0, 0, 0, 0, e->modele (), 1);
			lajoute (e->canon (), la, am); 
			AnalyseMorpho av = AnalyseMorpho (e->canon (), 2, 1, 1, 0, 0, 0, 0, 0, e->modele (), 1);
			lajoute (e->canon (), la, av); 
            // neutre de uetus, et de fortis si = masc.
            if (e->canon ().rfind ("is") != e->canon ().size () - 2)
            {
                AnalyseMorpho an = AnalyseMorpho (e->canon (), 1, 3, 1, 0, 0, 0, 0, 0, e->modele (), 1);
                lajoute (e->canon (), la, an); 
                AnalyseMorpho ac = AnalyseMorpho (e->canon (), 3, 3, 1, 0, 0, 0, 0, 0, e->modele (), 1);
                lajoute (e->canon (), la, ac); 

            }
		}
    }

    // et recherche du doublon
    e = entree (f + " (2)");
    if (!e->est_vide ())
        lajoute (e->canon (), la, e->amMorphoK ()); 

    e = entree (f + " (3)");
    if (!e->est_vide ())
        lajoute (e->canon (), la, e->amMorphoK ()); 

    // recherche de toutes les possibilités radical.désinence
    // initialisation des variables de la boucle
    string r, d;
    int l = f.length ();
    int i;
    // début de boucle
    for (i = 0; i < l; i++)
    {
        // calcul du radical et de la désinence       
        r = f.substr (0, i);
        d = f.substr (i, l-i);

        // pour les radicaux nuls :
        if (r.length () == 0)
            r = ".";

        // recherche et mise en liste des radicaux
        ir = radices.lower_bound (r);
        liste_r.clear ();
        while (ir != radices.upper_bound (r))
        {
            liste_r.push_back (ir->second);
            ir++;
        }

        // recherche des désinences
        id = desinentiae.lower_bound (d); 
        while (id != desinentiae.upper_bound (d))
        {
            // boucle de recherche dans les radicaux
            ilr = liste_r.begin ();
            while (ilr != liste_r.end ())
            {
                // comparaison du modèle et du n° de radical
                if ((ilr->mdl () == id->second.mdl ()) && (ilr->nr () == id->second.nr ()))
                {
                    e = entree (ilr->canon ()); 
                    if (!(e->est_pluriel () && id->second.numerus () == 1))
                        lajoute (e->canon (), la, id->second);
                }
                ++ilr;
            }
            ++id;
        }
    } // fin de boucle de recherce r.d 

    // recherche dans les irréguliers
    ii = irregulares.lower_bound (f); 
    while (ii != irregulares.upper_bound (f))
    {
        e = entree (ii->second.canon ());
        AnalyseMorpho am = AnalyseMorpho (
             ii->second.canon (), ii->second.casus (), ii->second.genus (), ii->second.numerus (), 
             ii->second.gradus (), ii->second.persona (), ii->second.tempus (),
             ii->second.modus (), ii->second.uox (), 0, 0);
        //cout << "debug " << e->canon () << endl;
        lajoute (e->canon (), la, am); 
        ++ii;
    }

    return la; 
}

void TLexicum::uniLanalyses (ListeAnalyses& l1, ListeAnalyses& l2)
// ajoute l2 à l1
{
    if (l2.size () == 0) return; 
    for (multimap<string, AnalyseMorpho>::iterator it = l2.begin (); it != l2.end (); it++)
        lajoute (it->first, l1, it->second);
}

void minuscules (string &f)
{
   if (f.size () == 1) return;
   for (unsigned int i = 1; i < f.size (); i++)
      f[i] = tolower (f[i]);
}

string TLexicum::lemmatise (string f)
{
    ostringstream oss;
	if (est_romain (f))
		oss << f << " : " << aRomano (f);
    //int aRom = aRomano (f);
    //if (aRom > 0)
    // 	oss << f << " : " << aRom;
    string adde ("");
    // majuscules
    // minuscules pour toutes les lettres sauf la première
    minuscules (f); 
    oss << analyses (f);
    if (toupper (f[0]) == f[0])
    {
        adde = analyses (basDeCasse (f));
        if (!adde.empty ())
        oss << adde; 
    }
    // suffixes
    string fq = sinesuffixo (f, "que");
    string fu = sinesuffixo (f, "ue");
    string fv = sinesuffixo (f, "ve");
    string fn = sinesuffixo (f, "ne");
    if (fq != f)
    {
        oss << analyses (fq);
        if (basDeCasse (fq) != fq)
            oss << analyses (basDeCasse (fq));
    }
    else if (fu != f)
    {
        oss << analyses (fu);
    }
    else if (fv != f)
    {
        oss << analyses (fv);
    }
    else if (fn != f && oss.str ().find (fn) == string::npos)
    {
        oss << analyses (fn);
    }

    string retour = oss.str ();
    if (retour.empty ()) oss << endl << f << " ?"; 
    return oss.str ();
}

string TLexicum::lemmatiseM (string f)
{
    ListeAnalyses r = lanalyses (f);
    ListeAnalyses adde;
    ostringstream oss;

    // les romains, en amont parce que les romains ne sont
    // pas dans le lexique.
	if (est_romain (f))
		oss << f << " : " << aRomano (f);
    //int aRom = aRomano (f);
    //if (aRom > 0)
    //    oss << f << " : " << aRom << endl;

    // minuscules pour toutes les lettres sauf la première
    minuscules (f); 

    // majuscule initiale (début de phrase)
    if (toupper (f[0]) == f[0])
    {
        adde = lanalyses (basDeCasse (f));
        uniLanalyses (r, adde);
    }
    // suffixes
    string fq = sinesuffixo (f, "que");
    string fu = sinesuffixo (f, "ue");
    string fv = sinesuffixo (f, "ve");
    string fn = sinesuffixo (f, "ne");
    if (fq != f)
    {
        adde = lanalyses (fq);
        uniLanalyses (r, adde);
        if (basDeCasse (fq) != fq)
        {
            adde = lanalyses (basDeCasse (fq));
            uniLanalyses (r, adde);
        }
    }
    else if (fu != f)
    {
        adde = lanalyses (fu);
        uniLanalyses (r, adde);
    }
    else if (fv != f)
    {
        adde = lanalyses (fv);
        uniLanalyses (r, adde);
    }
    else if (fn != f)
    {
        adde = lanalyses (fn);
        uniLanalyses (r, adde);
    }

    // récup. des analyses
    if (r.empty () && oss.str ().empty ()) 
    {
        oss << endl << f << " : ?"; 
        return oss.str ();
    }

    string lemme;
    for (multimap<string, AnalyseMorpho>::iterator it = r.begin ();
         it != r.end (); it++)
    {
        if (it->second.lemma () != lemme)
        {
            oss << entree (it->second.lemma ())->doc () << endl; 
            lemme = it->second.lemma ();
        }
        oss << "    " << it->second.morpho () << endl; 
    }

    // retour
    return oss.str ();
}

string TLexicum::desN (Tentree* e, int c, int n)
{
    string retour;
    c2d::iterator id;
    for (id = desinentiae.begin (); id != desinentiae.end (); id++)
        if (e->modele () == id->second.mdl () &&  id->second.casus () == c && id->second.numerus () == n)
        {
            if (!retour.empty ()) retour += "/";
            retour += id->first;
        }
    return retour;
}

string TLexicum::desA (Tentree* e, int c, int n, int g, int d)
{
    string retour;
    c2d::iterator id;
    for (id = desinentiae.begin (); id != desinentiae.end (); id++)
        if (e->modele () == id->second.mdl () 
            &&  id->second.casus () == c 
            && id->second.numerus () == n
            && id->second.genus () == g
            && id->second.gradus () == d)
        {
            if (!retour.empty ()) retour += "/";
            retour += id->first;
        }
    return "." + retour;
}

/**
 * Synthèse des formes adjectivales
 */
string TLexicum::synthA (Tentree* e, int c, int n, int g, int d)
{
    // chercher dans les irréguliers
    c2i::iterator ii; 
    for (ii = irregulares.begin (); ii !=irregulares.end (); ii++)
        if (ii->second.canon () == e->canon ()
                && ii->second.casus () == c
                && ii->second.numerus () == n
                && ii->second.genus () == g
                && ii->second.gradus () == d)
            return ii->first;
    string retour;
    c2d::iterator id;
    for (id = desinentiae.begin (); id != desinentiae.end (); id++)
        if (e->modele () == id->second.mdl () 
            && id->second.casus () == c 
            && id->second.numerus () == n
            && id->second.genus () == g
            && id->second.gradus () == d)
        {
            if (!retour.empty ()) retour += "/";
            retour += id->first;
        }
    return e->radical (1) + "." + retour;
}

/**
 * Synthèse des formes nominales
 */
string TLexicum::synthN (Tentree *e, int c, int n)
{
    // chercher dans les irréguliers
    c2i::iterator ii; 
    for (ii = irregulares.begin (); ii !=irregulares.end (); ii++)
        if (ii->second.canon () == e->canon ()
                && ii->second.casus () == c
                && ii->second.numerus () == n)
            return ii->first;
    if (c == 1 && n == 1) return e->canon ();
    return e->radical (1) + "." + desN (e, c, n);
}

/**
 * Synthèse des formes pronominales (uniquement irrégulières)
 */
string TLexicum::synthP (Tentree* e, int c, int n, int g)
{
    // renvoi du canon
    if (c == 1 && n == 1 && g== 1) return e->canon ();
    // chercher dans les irréguliers
    c2i::iterator ii; 
    for (ii = irregulares.begin (); ii !=irregulares.end (); ii++)
        if (ii->second.canon () == e->canon ()
                && ii->second.casus () == c
                && ii->second.numerus () == n
                && ii->second.genus () == g)
            return ii->first;
    return "-";
}            

/**
 * Synthèse des formes verbales
 */
string TLexicum::synthV (Tentree* e, int p, int n, int t, int m, int v, int rn)
{
    // chercher dans les irréguliers
    c2i::iterator ii; 
    for (ii = irregulares.begin (); ii !=irregulares.end (); ii++)
        if (ii->second.canon () == e->canon ()
                && ii->second.persona () == p
                && ii->second.numerus () == n
                && ii->second.tempus () == t
                && ii->second.modus () == m
                && ii->second.uox () == v)
            return ii->first;

    // chercher dans les désinences
    string retour;
    int nr;
    c2d::iterator id;
    if ((v == 2 || e->est_deponent ()) && t > 3)
    {
        string pp;
        // si inf. passif, accusatif
        if (m == 4) pp = synthPart (e, 3,1,1,4,2);
        // sinon, nominatif
        else pp = synthPart (e, 1,1,n,4,v);
        t = t - 3;
        for (id = desinentiae.begin (); id != desinentiae.end (); id++)
        {
            if (
                    id->second.mdl () == 22
                    && id->second.persona () == p
                    && id->second.numerus () == n
                    && id->second.tempus () == t
                    && id->second.modus () == m
               )
            {
                if (!retour.empty ()) retour += "/";
                retour += id->first;
            }
        }
        return pp + " " + retour;
    }
    else
    {
        for (id = desinentiae.begin (); id != desinentiae.end (); id++)
            if (
                    e->modele () == id->second.mdl () 
                    && id->second.persona () == p 
                    && id->second.numerus () == n
                    && id->second.tempus () == t
                    && id->second.modus () == m
                    && id->second.uox () == v
                    && id->second.nr () == rn
               )
            {
                if (!retour.empty ()) retour += "/";
                retour += id->first;
            }
        // forme canonique, sans désinence dans la table !
        if (retour.empty () && v==1 && m==1 && t==1 && p==1 && n==1)
            return e->canon ();
        if (t < 4) nr = 1;
        else nr = 2;
        string r = e->radical (nr);
        string k = e->canon ();
        if ((r.empty () || retour.empty ()) 
                && k != "sum" && k != "eo")
            return "-";
        return r + "." + retour;
    }
}

/**
 * Synthèse des formes participiales
 */
string TLexicum::synthPart (Tentree* e, int c, int g, int n, int t, int v)
{
    string retour;
    int nr;
    c2d::iterator id;
    for (id = desinentiae.begin (); id != desinentiae.end (); id++)
        if (e->modele () == id->second.mdl () 
            && id->second.casus () == c 
            && id->second.numerus () == n
            && id->second.genus () == g
            && id->second.tempus () == t
            && id->second.modus () == 5
            && (id->second.uox () == v)
            )
        {
            if (!retour.empty ()) retour += "/";
            retour += id->first;
        }
    if ((v == 1) && t == 1) nr = 1;
    else nr = 3;
    string r = e->radical (nr);
    string k = e->canon ();
    if ((r.empty () || retour.empty ())
            && k != "sum" && k != "eo")
        return "-";
    return r + "." + retour;
}

string TLexicum::flechis (Tentree* e)
{
    stringstream ss;
    string ind ("    ");
    switch (e->pars ())
    {
        case 'n':
        {
            int ic=3;
            ss << endl << "sing." << endl << ind << e->canon ();
            if (e->modele () == 1) ic = 2; 
            else ss << endl << ind << e->canon ();
            for (;ic < 7; ic ++)
                ss << endl << ind << synthN (e, ic, 1);
            ss << endl << "plur.";
            for (ic=1;ic < 7; ic ++)
                ss << endl << ind << synthN (e, ic, 2);
            return ss.str ();
        }
        case 'a':
        {
            string chM, chF, chN;
            ss << endl << "POSIT." << endl;
            ss << endl << "sing." << endl; 
            int ic = 1;
            for (;ic < 7; ic ++)
            {
                if (ic == 1) chM = e->canon ();
                else if (ic == 2 && e->modele () != 11) chM = e->canon ();
                else chM = synthA (e, ic,1,1,0);
                string tabM (20-chM.size(),' ');
                chF = synthA (e, ic,1,2,0);
                string tabF (20-chF.size(),' ');
                chN = synthA (e, ic,1,3,0);

                ss << endl << ind  << chM << tabM << chF << tabF << chN; 
            }
            ss << endl << "plur.";
            for (ic=1;ic < 7; ic ++)
            {
                chM = synthA (e, ic,2,1,0);
                string tabM (20-chM.size(),' ');
                chF = synthA (e, ic,2,2,0);
                string tabF (20-chF.size(),' ');
                chN = synthA (e, ic,2,3,0);

                ss << endl << ind  << chM << tabM << chF << tabF << chN; 
            }
            ss << endl << "COMPAR.";
            ic = 1;
            for (;ic < 7; ic ++)
            {
                chM = synthA (e, ic,1,1,2);
                string tabM (20-chM.size(),' ');
                chF = synthA (e, ic,1,2,2);
                string tabF (20-chF.size(),' ');
                chN = synthA (e, ic,1,3,2);

                ss << endl << ind  << chM << tabM << chF << tabF << chN; 
            }
            ss << endl << "plur.";
            for (ic=1;ic < 7; ic ++)
            {
                chM = synthA (e, ic,2,1,2);
                string tabM (20-chM.size(),' ');
                chF = synthA (e, ic,2,2,2);
                string tabF (20-chF.size(),' ');
                chN = synthA (e, ic,2,3,2);

                ss << endl << ind  << chM << tabM << chF << tabF << chN; 
            }
            ss << endl << "SVPERL.";
            ic = 1;
            for (;ic < 7; ic ++)
            {
                chM = synthA (e, ic,1,1,3);
                string tabM (20-chM.size(),' ');
                chF = synthA (e, ic,1,2,3);
                string tabF (20-chF.size(),' ');
                chN = synthA (e, ic,1,3,3);

                ss << endl << ind  << chM << tabM << chF << tabF << chN; 
            }
            ss << endl << "plur.";
            for (ic=1;ic < 7; ic ++)
            {
                chM = synthA (e, ic,2,1,3);
                string tabM (20-chM.size(),' ');
                chF = synthA (e, ic,2,2,3);
                string tabF (20-chF.size(),' ');
                chN = synthA (e, ic,2,3,3);

                ss << endl << ind  << chM << tabM << chF << tabF << chN; 
            }
            return ss.str ();
        }
        case 'p':
        {
            string chM, chF, chN;
            ss << endl << "sing.";
            int ic = 1;
            for (;ic<7;ic++)
            {
                chM = synthP (e, ic, 1, 1);
                string tabM (20-chM.size(),' ');
                chF = synthP (e, ic, 1, 2);
                string tabF (20-chF.size(),' ');
                chN = synthP (e, ic, 1, 3);
                ss << endl << ind  << chM << tabM << chF << tabF << chN; 
            }
            ss << endl << "plur.";
            for (ic=1;ic<7;ic++)
            {
                chM = synthP (e, ic, 2, 1);
                string tabM (20-chM.size(),' ');
                chF = synthP (e, ic, 2, 2);
                string tabF (20-chF.size(),' ');
                chN = synthP (e, ic, 2, 3);
                ss << endl << ind  << chM << tabM << chF << tabF << chN; 
            }
            return ss.str ();
        }
        case 'v':
        {
            int ip, in;
            string col1, col2, col3;
            ss << "ACTIVA VOX" << endl;
            ss << "indicatiuus" << endl;
            ss << "      praes.              fut.                impf." << endl;
            for (in=1;in<3;in++)
                for (ip=1;ip<4;ip++)
                {
                    col1 =  synthV (e, ip,in,1,1,1);
                    string tab1 (20-col1.size(),' ');
                    col2 =  synthV (e, ip,in,2,1,1);
                    string tab2 (20-col2.size(),' ');
                    col3 =  synthV (e, ip,in,3,1,1);
                    ss << "   " << col1 << tab1 << col2 << tab2 << col3 << endl;
                }
            ss << "      pf.                 fut. ant.           pqp" << endl;
            for (in=1;in<3;in++)
                for (ip=1;ip<4;ip++)
                {
                    col1 =  synthV (e, ip,in,4,1,1,2);
                    string tab1 (25-col1.size(),' ');
                    col2 =  synthV (e, ip,in,5,1,1,2);
                    string tab2 (20-col2.size(),' ');
                    col3 =  synthV (e, ip,in,6,1,1,2);
                    ss << "   " << col1 << tab1 << col2 << tab2 << col3 << endl;
                }
            ss << "coniunctiuus" << endl;
            ss << "      praes.              impf." << endl;
            for (in=1;in<3;in++)
                for (ip=1;ip<4;ip++)
                {
                    col1 =  synthV (e, ip,in,1,2,1);
                    string tab1 (20-col1.size(),' ');
                    col2 =  synthV (e, ip,in,3,2,1);
                    ss << "   " << col1 << tab1 << col2 << endl;
                }
            ss << "      pf.                 pqp" << endl;
            for (in=1;in<3;in++)
                for (ip=1;ip<4;ip++)
                {
                    col1 =  synthV (e, ip,in,4,2,1,2);
                    string tab1 (20-col1.size(),' ');
                    col2 =  synthV (e, ip,in,6,2,1,2);
                    ss << "   " << col1 << tab1 << col2 << endl;
                }
            ss << "imperatiuus" << endl;
            ss << "      praes.              fut." << endl;
                col1 = synthV (e,2,1,1,3,1);
                string tab1 (20-col1.size(),' ');
                col2 = synthV (e,2,1,2,3,1);
                ss << "   " << col1 << tab1 << col2 << endl;
                col1 = synthV (e,2,2,1,3,1);
                string tab2 (20-col1.size(),' ');
                col2 = synthV (e,2,2,2,3,1);
                ss << "   " << col1 << tab1 << col2 << endl;
            ss << "infinitiuus" << endl;
            ss << "      praes.              pf." << endl;
                col1 = synthV (e,0,0,1,4,1);
                string tabinf (20-col1.size(),' ');
                col2 = synthV (e,0,0,4,4,1,2);
                ss << "   " << col1 << tabinf << col2 << endl;
            ss << "part. pr." << endl;
            ss << "    sing." << endl;
                // ans|1|3|1|0|0|1|5|1|17|1
                int ic;
                for (in=1;in<3;in++)
                {
                    for (ic=1;ic<7;ic++) 
                    {
                        col1 = synthPart (e,ic,1,in,1,1);
                        string tab1 (30-col1.size (),' '); 
                        col2 = synthPart (e,ic,2,in,1,1);
                        string tab2 (30-col2.size (),' '); 
                        col3 = synthPart (e,ic,3,in,1,1);
                        ss << "   " << col1 << tab1 << col2 << tab2 << col3 << endl;
                    }
                    if (in == 1) ss << "    plur." << endl;
                }
            ss << "part. fut." << endl;
            ss << "    sing." << endl;
                // ans|1|3|1|0|0|1|5|1|17|1
                for (in=1;in<3;in++)
                {
                    for (ic=1;ic<7;ic++) 
                    {
                        col1 = synthPart (e,ic,1,in,2,1);
                        string tab1 (30-col1.size (),' '); 
                        col2 = synthPart (e,ic,2,in,2,1);
                        string tab2 (30-col2.size (),' '); 
                        col3 = synthPart (e,ic,3,in,2,1);
                        ss << "   " << col1 << tab1 << col2 << tab2 << col3 << endl;
                    }
                    if (in == 1) ss << "    plur." << endl;
                }
            if (e->est_deponent ())
                ss << "deponentibus non passiua uox." << endl;
            else 
            {
                ss << "PASSIVA VOX" << endl;
                ss << "indicatiuus" << endl;
                ss << "      praes.              fut.                impf." << endl;
                for (in=1;in<3;in++)
                    for (ip=1;ip<4;ip++)
                    {
                        col1 =  synthV (e, ip,in,1,1,2);
                        string tab1 (20-col1.size(),' ');
                        col2 =  synthV (e, ip,in,2,1,2);
                        string tab2 (20-col2.size(),' ');
                        col3 =  synthV (e, ip,in,3,1,2);
                        ss << "   " << col1 << tab1 << col2 << tab2 << col3 << endl;
                    }
                ss << "      pf.                 fut. ant.           pqp" << endl;
                for (in=1;in<3;in++)
                    for (ip=1;ip<4;ip++)
                    {
                        col1 =  synthV (e, ip,in,4,1,2);
                        string tab1 (20-col1.size(),' ');
                        col2 =  synthV (e, ip,in,5,1,2);
                        string tab2 (20-col2.size(),' ');
                        col3 =  synthV (e, ip,in,6,1,2);
                        ss << "   " << col1 << tab1 << col2 << tab2 << col3 << endl;
                    }
                ss << "coniunctiuus" << endl;
                ss << "      praes.              impf." << endl;
                for (in=1;in<3;in++)
                    for (ip=1;ip<4;ip++)
                    {
                        col1 =  synthV (e, ip,in,1,2,2);
                        string tab1 (20-col1.size(),' ');
                        col2 =  synthV (e, ip,in,3,2,2);
                        ss << "   " << col1 << tab1 << col2 << endl;
                    }
                ss << "      pf.                 pqp" << endl;
                for (in=1;in<3;in++)
                    for (ip=1;ip<4;ip++)
                    {
                        col1 =  synthV (e, ip,in,4,2,2);
                        string tab1 (20-col1.size(),' ');
                        col2 =  synthV (e, ip,in,6,2,2);
                        ss << "   " << col1 << tab1 << col2 << endl;
                    }
                ss << "imperatiuus praes." << endl;
                col1 = synthV (e,2,1,1,3,2);
                ss << "   " << col1 << endl;
                col1 = synthV (e,2,2,1,3,2);
                ss << "   " << col1 << endl;
                ss << "infinitiuus" << endl;
                ss << "      praes.              pf." << endl;
                col1 = synthV (e,0,0,1,4,2);
                string tabinfp (20-col1.size(),' ');
                col2 = synthV (e,0,0,4,4,2);
                ss << "   " << col1 << tabinfp << col2 << endl;
                ss << "part. pf." << endl;
                for (in=1;in<3;in++)
                {
                    for (ic=1;ic<7;ic++) 
                    {
                        col1 = synthPart (e,ic,1,in,4,2);
                        string tab1 (30-col1.size (),' '); 
                        col2 = synthPart (e,ic,2,in,4,2);
                        string tab2 (30-col2.size (),' '); 
                        col3 = synthPart (e,ic,3,in,4,2);
                        ss << "   " << col1 << tab1 << col2 << tab2 << col3 << endl;
                    }
                    if (in == 1) ss << "    plur." << endl;
                }
            }
            return ss.str ();
        }
        case 'i':
        {
            return "inuar.";
        }
        default:
           return "nequeo";
    }
}

TLexicum * lexicum;

void lexicumLege (string c)
{
    delete lexicum;
    lexicum = new TLexicum (c);
}

string lemmatise (string f)
{
    return lexicum->lemmatise (f);
}

string lemmatiseM (string f)
{
    return lexicum->lemmatiseM (f);
}

Tentree * entree (string linea)
{
    //return new Tentree (linea);
	return lexicum->entree (linea);
}

string flechis (Tentree* e)
{
    return lexicum->flechis (e);
}

ListeAnalyses lanalyses (std::string f)
{
    return lexicum->lanalyses (f);
}    

void uniLanalyses (ListeAnalyses& l1, ListeAnalyses& l2)
{
    return lexicum->uniLanalyses (l1, l2);
}
