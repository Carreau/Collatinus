/*
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

#include "frequences.h"
#include "syntaxe.h"
#include <QFile>
#include <QMessageBox>
#include <map>
#include <list>

using namespace std;

QStringList k = QStringList () << "n" << "v" << "ac" << "g" << "d" << "ab";
QStringList g = QStringList () << "m" << "f" << "nt";
QStringList n = QStringList () << "s" << "p";
QStringList d = QStringList () << "" << "comp" << "sup";
QStringList p = QStringList () << "1" << "2" << "3";
QStringList t = QStringList () << "pr" << "fut" << "impf" << "pf" << "fa" << "pqp";
QStringList m = QStringList () << "ind" << "subj" << "imper" << "inf" << "part" << "ger" << "adjv";
// in libcollatinus: "","ind.","coniunct.","imper.","infin.","part.","gerund.","adiect. verb."
QStringList v = QStringList () << "act" << "pass";

//***************
// CLASSE MORPHOS 
//***************

Morphos::Morphos (QString mo)
{
    // éclater en sous-chaînes
    if (!mo.isEmpty ()) mo = mo.simplified ();
    if (mo.isEmpty ())
    {
        graphie = "";
        return;
    }

    elements = mo.split (QRegExp ("\\W+"));
    QStringList eclats = elements;
    QStringList gr;
    QString el;
    foreach (el, eclats)
       if (k.contains (el)) 
          {
              gr << el;
              eclats.replace(eclats.indexOf (el), "");
          }
    foreach (el, eclats)
       if (g.contains (el))
          {
              gr << el;
              eclats.replace(eclats.indexOf (el), "");
          }
    foreach (el, eclats)
       if (n.contains (el))
          {
              gr << el;
              eclats.replace(eclats.indexOf (el), "");
          }
    foreach (el, eclats)
       if (d.contains (el)) 
          {
              gr << el;
              eclats.replace(eclats.indexOf (el), "");
          }
    foreach (el, eclats)
       if (p.contains (el)) 
          {
              gr << el;
              eclats.replace(eclats.indexOf (el), "");
          }
    foreach (el, eclats)
       if (t.contains (el)) 
          {
              gr << el;
              eclats.replace(eclats.indexOf (el), "");
          }
    foreach (el, eclats)
       if (m.contains (el))
          {
              gr << el;
              eclats.replace(eclats.indexOf (el), "");
          }
    foreach (el, eclats)
       if (v.contains (el)) 
          {
              gr << el;
              eclats.replace(eclats.indexOf (el), "");
          }
    graphie = gr.join (" ").simplified ();
}

void Morphos::setCanon (QString c)
{
    canon = c;
}

QString Morphos::getCanon ()
{
    return canon;
}

QString Morphos::getGraphie ()
{
    return graphie;
}

void Morphos::setItem (int i)
{
    item = i;
}

int Morphos::getItem ()
{
    return item;
}

/**
 * Morphos::accepte (Morphos * mb)
 * Vérifie que la morpho d'expression peut utiliser 
 * la morpho de phrase mb 
 */
bool Morphos::accepte (Morphos * mb)
{
    // qDebug () << "graphie expr:" << graphie <<":";
    // qDebug () << "graphie Phr :" << mb->getGraphie ();
    QString mbK = mb->getCanon ();
    if (!canon.isEmpty ())
    {
        if (canon.contains (','))
        {
            QStringList eclatsC = canon.split (',');
            if (!eclatsC.contains (mbK)) return false;
        }
        else
        {
    
    
    
            if (canon != mbK) return false;
        }

    }
    if (graphie.isEmpty ()) return true;
    QStringList eclatsExpr = graphie.split (" ");
    QStringList eclatsP = mb->getGraphie ().split (" ");
    foreach (QString ee, eclatsExpr)
        if (eclatsP.indexOf (ee) < 0)
        {
            // qDebug () << mb->getGraphie () << " refuse " << ee;
            return false;
        }
    // qDebug () << "accepte"; 
    return true;
}

QString Morphos::cas ()
{
    foreach (QString el, elements)    
        if (k.contains (el)) return el;
    return "";
}

QString Morphos::genre ()
{
    foreach (QString el, elements)    
        if (g.contains (el)) return el;
    return "";
}

QString Morphos::nombre ()
{
    foreach (QString el, elements)    
        if (n.contains (el)) return el;
    return "";
}

QString Morphos::humain ()
{
    return QString ("%1.%2").arg (canon).arg (graphie);
}

// **********
// CLASSE MOT
// **********

Mot::Mot (QString g)
{
    // ramismes
    g.replace ("J","I");
    g.replace ("j","i");
    g.replace ("v","u");
    graphie = g;
    lemmatise ();
    adopte = false;
}

Mot::~Mot ()
{
    foreach (Morphos * morph, morphos)
       delete morph;
    morphos.clear ();
}

void Mot::lemmatise ()
{
    // TODO gestion des majuscules
    ListeAnalyses analyses = lanalyses (graphie.toStdString ());
    ListeAnalyses adde;
    if (graphie.at (0).isUpper ())
    {
        adde = lanalyses (graphie.toLower ().toStdString ());
        uniLanalyses (analyses, adde);
    }
	// ôter les éventuels suffxes
	QString sineSuff = graphie;
	sineSuff.remove (QRegExp ("(que|ue|ve|ne)$")); 
	if (sineSuff != graphie)
	{
		adde = lanalyses (sineSuff.toStdString ());
		uniLanalyses (analyses, adde);
	}
	// enregistrer la lemmatisation
    QString mo;
    QTextStream flux (&mo);
    Morphos * morph;
	std::string ancienL, nouvoL; 
    // convertir le retour des analyses en instances de Morphos
	if (analyses.empty ())
		QTextStream (&lemme) << "\n" << graphie << " ?";
    for (std::multimap<std::string, AnalyseMorpho>::iterator it = analyses.begin ();
         it != analyses.end (); it++)
    {
        // cas
        if (it->second.casus () > 0) flux << k[it->second.casus ()-1] << " ";
        // genre
        if (it->second.genus () > 0) flux << g[it->second.genus ()-1] << " ";
        // nombre
        if (it->second.numerus () > 0) flux << n[it->second.numerus ()-1] << " ";
        // degré
        if (it->second.gradus () > 0) flux << d[it->second.gradus ()-1] << " ";
        // personne
        if (it->second.persona () > 0) flux << p[it->second.persona ()-1] << " ";
        // temps
        if (it->second.tempus () > 0) flux << t[it->second.tempus ()-1] << " ";
        // mode
        if (it->second.modus () > 0) flux << m[it->second.modus ()-1] << " ";
        // voix
        if (it->second.uox () > 0) flux << v[it->second.uox ()-1] << " ";
        
        mo = mo.simplified ();
        morph = new Morphos (mo); 
        morph->setCanon (QString::fromStdString (it->second.lemma ()));
        morphos << morph;
        mo.clear ();
		nouvoL = entree (it->second.lemma ())->doc ();
		if (nouvoL != ancienL)
		    QTextStream (&lemme) << "\n" << QString::fromStdString (nouvoL);
		ancienL = nouvoL;
    }
}

QString Mot::getGraphie ()
{
    return graphie;
}

int Mot::count_morphos ()
{
    return morphos.count ();
}

QString Mot::getCanon (int i)
{
    if (i <  morphos.size ())
        return morphos[i]->getCanon ();
    return "";
}

Morphos * Mot::getMorpho (int i)
{
    return morphos[i];
}

QString Mot::humain (int i)
{
    return morphos[i]->humain ();
}

QString Mot::humain ()
{
    QString retour = graphie ;
    QTextStream flux (&retour);
    foreach (Morphos * morph, morphos)
       flux << "\n    - " << morph->humain ();
    return retour;
}

QString Mot::get_lemme ()
{
	return lemme;
}

void Mot::setAdopte (bool a)
{
    adopte = a;
}

bool Mot::estAdopte ()
{
    return adopte;
}

bool Mot::casCommun (Mot * m)
{
    foreach (Morphos * maMorpho, morphos)
        for (int i = 0;i<m->count_morphos();i++)
            if (maMorpho->cas () == m->getMorpho (i)->cas ()) return true;
    return false;
}

bool Mot::nombreCommun (Mot * m)
{
    foreach (Morphos * maMorpho, morphos)
        for (int i = 0;i<m->count_morphos();i++)
            if (maMorpho->nombre () == m->getMorpho (i)->nombre ()) return true;
    return false;
}

// *****************
// CLASSE EXPRESSION
// *****************

Expression::Expression (QString n)
{
    QStringList eclats = n.split ('|');
    nom = eclats[0];
    doc = eclats[1];
    noyo = eclats[2].toInt ();
    int ie = 3;
    while (ie < eclats.size ())
    {
        QStringList liste = eclats[ie].split ('.');
        Morphos * m = new Morphos (liste[1]);
        // if (liste[0] == "res") qDebug () << "res trouvé : " << n;
        m->setCanon (liste[0]); 
        if (!liste[2].isEmpty ()) 
        {
            de = ie;
            en = liste[2];
            avec = liste[3].toInt ();
        }
        else
        {
            avec = -1;
        }
        morphos.append (m);
        ie++;
    }
}

QString Expression::getNom ()
{
    return nom;
}

QString Expression::getDoc ()
{
    return doc;
}

QString Expression::getEn ()
{
    return en;
}

int Expression::countMorphos ()
{
    return morphos.size ();
}

Morphos * Expression::getMorpho (int im)
{
    if (im < morphos.size ())
        return morphos [im];
    return NULL;
}

QString Expression::humain ()
{
    QString retour;
    QTextStream (&retour) << nom << " : " << doc;
    return retour;
}
        
QString Expression::lemme_noyau ()
{
    return morphos[noyo]->getCanon ();
}

int Expression::getPosInP ()
{
    return posInP;
}

void Expression::setPosInP (int p)
{
    posInP = p;
}

bool Expression::accordVoulu ()
{
    return avec >= 0;
}

/*
 ************************
  classe Liste_expr
};
 ************************
*/

Liste_expr::Liste_expr (QString f)
{
    QFile file(f);
    if (!file.open(QFile::ReadOnly | QFile::Text)) 
    {
        //QMessageBox::warning(this, "Collatinus", "Capsam legere nequeo expressions.fr"); 
        return;
    }

    QTextStream in(&file);
    QString lin;
    lin = in.readLine ();
    //while (!file.atEnd ())
    while (!lin.isNull ())
    {
        Expression * E = new Expression (lin);
        // injection
        expressions.insert (E->lemme_noyau (), E); 
        //qDebug () << lin;
        lin = in.readLine ();
    }
}

Liste_expr::~Liste_expr ()
{

}

QList<Expression *> Liste_expr::expr_lemme (QString l)
{
    //qDebug () << "liste_expr (" << l << ")";
    return expressions.values (l);    
}

Liste_expr * LExpressions;

// *************
// CLASSE PHRASE
// *************

Phrase::Phrase (QString t)
{
    graphie = t.simplified ();
    Mot * nmot;
    QStringList l = graphie.split (QRegExp ("\\W+"));
    QString eclat;
    int j = 0;
    for (int i=0; i < l.count (); i++)
    {
        eclat = l[i];
        if (eclat.isEmpty ())
           continue;
        nmot = new Mot (eclat); 
        nmot->no = j;
        mots << nmot; 
        j++;
    }
    cherche_expressions ();
}

Phrase::~Phrase ()
{
    foreach (Mot * mot, mots)
        delete mot;
    mots.clear ();
}

bool Phrase::aiLeCanon (QString c)
{
    if (c.isEmpty ()) return true;
    if (c.contains (','))
    {
        QStringList lc = c.split (','); 
        foreach (QString cc, lc)
            foreach (Mot * mot, mots)
            {
                for (int i=0; i < mot->count_morphos (); i++)
                    if (mot->getCanon (i) == cc) 
                        return true;
            }
    }
    else
    {
        foreach (Mot * mot, mots)
            for (int i=0; i < mot->count_morphos (); i++)
                if (mot->getCanon (i) == c) return true;
    }
    return false;
}

// sert pour Expression::expressions ()
// évite les doublons dans la recherche d'expressions
bool contient (QList<Expression *> liste, Expression * expr)
{
    foreach (Expression * e, liste)
        if (e->getNom () == expr->getNom ()) return true;
    return false;
}

/**
 * Renvoie le premier mot de la phrase non adopté,
 * satisfaisant à la demande du membre d'expression m.
 * en profite pour marquer adopté le mot renvoyé.
 */
Mot * Phrase::motExpr (Morphos * m)
{
    foreach (Mot * imot, mots)
        for (int i = 0;i<imot->count_morphos ();i++)
            if ((!imot->estAdopte ()) && m->accepte (imot->getMorpho (i)))
            {
                imot->setAdopte (true);
                return imot;
            }
    return NULL;
}

bool Phrase::accord (int de, QString en, int avec)
{
    Mot * motDe = mot_no (de);
    Mot * motAvec = mot_no (avec);
    if (en.contains ('n') && !motDe->nombreCommun (motAvec)) return false;
    if (en.contains ('c') && !motDe->casCommun (motAvec)) return false; 
    return true; 
}

void Phrase::cherche_expressions ()
{
    // pour chaque mot de la phrase
    foreach (Mot * mot, mots)
    {
       // pour chaque canon du mot 
       for (int i=0;i<mot->count_morphos ();i++)
       {
            // s'il n'est pas noyo potentiel, mot suivant
            QList<Expression *> le = LExpressions->expr_lemme (mot->getCanon (i));
            //qDebug () << mot->getCanon (i) << " " << le.isEmpty ();
            if (le.isEmpty ()) continue;
            // ici, possibilité de renvoyer toute expression utilisant le mot
            // pour chaque expression potentielle
            foreach (Expression * expr, le)
            {
                initAdoptes ();
                // pour chaque mot requis de l'expression
                bool esuiv = false;
                for (int im = 0; im < expr->countMorphos (); im++)
                {
                    // mot absent de la phrase, expression suivante
                    if (!aiLeCanon (expr->getMorpho (im)->getCanon ()))
                    {
                        esuiv = true;        
                        continue;
                    }
                }
                if (esuiv) continue;
                // ici, possibilité de renvoyer les expressions sans vérif de morpho
                for (int im = 0; im < expr->countMorphos (); im++)
                {
                    // si aucun des mots repérés n'a la bonne morpho, expr. suivante
                    Mot * mota = motExpr (expr->getMorpho (im));
                    if (mota == NULL) 
                    {
                        //qDebug () << "jaipa";
                        esuiv = true;
                        continue;
                    }
                    if (expr->accordVoulu ()) 
                    {
                        de = mot->no;
                        avec = mota->no;
                        en = expr->getEn ();
                    }
                }
                if (esuiv) continue;
                //qDebug () << expr->humain () << " " << liste_expressions.size ();
                //vérifier l'accord
                if (avec >= 0 && !accord (de, en, avec)) continue;
                // ajouter le n° du mot + l'expression à la phrase
                if (!contient (liste_expressions, expr)) 
                {
                    expr->setPosInP (mot->no);
                    liste_expressions << expr;
                }
            }
        }
    }
}

QList<Expression *> Phrase::expressions ()
{
    return liste_expressions;
}

QString Phrase::analyse ()
{
   QString retour;
   QTextStream flux (&retour);
   QList<Expression *> liste = expressions ();
   foreach (Expression * e, liste)
      flux << e->getNom () << " : " << e->getDoc () << "\n"; 
   return retour;
}

Mot * Phrase::mot_no (int n)
{
	if (n < mots.size ()) return mots[n];
	return NULL;
}

Expression * Phrase::expression_no (int n)
{
	QString retour;
	foreach (Expression * expr, expressions ())
		if (expr->getPosInP () == n)
			return expr;
	return NULL;
}

QStringList * Phrase::analyse_et_lemmes (int minRaritas)
{
	QStringList * retour = new QStringList ();
	QString doc;
	Expression * e;
	//QTextStream ts (&retour);
	for (int i = 0; i < mots.count (); i++)
	{
		 //ts << "\n" <<  mots[i]->get_lemme ();	
         QString lemma (mots[i]->getCanon (0));
         //qDebug () << lemma << " " << frequentia (lemma);
         if (minRaritas <= frequentia (lemma))
		     retour->append (mots[i]->get_lemme ());
		 e = expression_no (i); 
		 if (e != NULL) 
             retour->append  ("\n   " + QString (0x00B6) + " " + e->getNom () + " : " + e->getDoc ());
	}
	return retour;
}

void Phrase::initAdoptes ()
{
    foreach (Mot * m, mots)
        m->setAdopte (false);
    de = -1;
    en = "";
    avec = -1;
}

// ************
// CLASSE TEXTE
// ************

Texte::Texte ()
{
}

Texte::~Texte ()
{
    foreach (Phrase * phr, phrases)
       delete phr;
    phrases.clear ();
}

void Texte::ajoute_phrase (Phrase * p)
{
    phrases << p;
}

void Texte::vide_phrases ()
{
    foreach (Phrase * phr, phrases)
        delete phr;
    phrases.clear ();
}

bool Texte::phrase_at_pos (int p)
{
    return phrase_pos (p) != NULL;
}

Phrase * Texte::phrase_pos (int p)
{
    foreach (Phrase * phr, phrases)
    if (p >= phr->debut && p <= phr->fin)
       return phr;
    return NULL;
}


/*
 ************************
  Fonctions hors classe
 ************************
*/

void lis_expr (QString ch)
{
    // QCoreApplication::dirPath ();
    //LExpressions = new Liste_expr ("expressions.fr");
    LExpressions = new Liste_expr (ch);
}

Texte * texte;

void cree_texte ()
{
    texte = new Texte ();
}

void vide_phrases ()
{
    if (texte == NULL) return;
    texte->vide_phrases ();
}

bool phrase_at_pos (int p)
{
    return texte->phrase_at_pos (p);    
}

void cree_phrase (QString p, int d, int f)
{
    Phrase * phr = new Phrase (p);
    phr->debut = d;
    phr->fin = f;
    texte->ajoute_phrase (phr);
}

// appel d'analyse syntaxique
QString analyse_syntaxique (int p, int mot_no)
{
    QString retour;
    Phrase * phr = texte->phrase_pos (p);
    if (phr == NULL) return "";
    foreach (Expression * expr, phr->expressions ())
    {
        if (expr->getPosInP () == mot_no)
            // retour précédé du signe unicode '|P' ou 00B6 (182) 
            QTextStream (&retour) << "\n   " << QString (0x00B6) << " " << expr->getNom () << " : " << expr->getDoc ();
    }
    return retour;
}
