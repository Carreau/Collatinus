/*                 main.h
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
 *  You should have received a copy of the GNU General Public License along
 *  with COLLATINVS; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/**
 *  BOGUES & AGENDA  
 *
 
    - base d'expression : ajouter ferro et igni 

    - beaucoup de formes attribuées à 'sum'

    - conserver les réglages dans le home de l'utilisateur.

    - fichiers récents ?

    - esse + dat. omniprésent

    - conspectus : vérifier le Jeanneau, qui semble confondre in et e
      conspectu(m)

    - implémenter la lemmatisation de la sélection si la touche Maj est
      enfoncée, par exemple, ou la proposer dans un contextuel.
     
 *  
 */

#ifndef MAIN_H
#define MAIN_H

#include "ui_collatinus.h"
#include <QSettings>
#include <QApplication>
#include <QtGui/QTextEdit>
#include "syntaxe.h"

class Editeur : public QTextEdit
{
    Q_OBJECT

    private:
         QString motCourant (QTextCursor C);
    public:
         Editeur (QWidget *parent, const char *name);
         QString lemmatiseTxt (bool alpha=0, bool cumVocibus = false);
		 QString lemmatiseTxt_expr (bool alpha = false);
    protected:
         void mousePressEvent(QMouseEvent *event);
         void mouseMoveEvent (QMouseEvent *event);
         void mouseReleaseEvent (QMouseEvent *event);
    signals:
         void copie (QString);
};

class fenestra : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        fenestra (QString url);
        Editeur *Ed;

    private:
        void createActions();
        void capsamInLatinum (const QString &fileName);
        bool capsaminDiscum (const QString &fileName);
        QString motCourant (QTextCursor C);
        QTextEdit* editeurCourant ();
        bool cautio ();
        bool capsaminDiscum ();
        void daFichierCourant (const QString &capsaeNomen);
        QString nomSeul (const QString &nomComplet);
        QString courant;
        QString quaerendum;
        bool licetMorpho;
        QString adHtml (QString t);
        QString repertoire;
        QString lang;

    private slots:
        void anglice ();
        void auxilium ();
        void decollatino ();
        void gallice ();
        void germanice ();
        void hispanice ();
        void catalanice ();
        void gallaece ();
        void calepin ();
        void inuenire ();
        void inuenire(const QString & exp);
        void inuenire_denuo ();
        void legere ();
        void lemmatiseTout ();
        void noua ();
        bool scribere ();
        bool scribereVt ();
        void vide_texte ();
        void change_syntaxe ();
        void change_morpho (bool m);
        void change_rarete ();

    public slots:
        void recoisLemm (QString);
        void lemmataRadere ();
        void controleIcone (int o);

    protected:
        bool event (QEvent *event);
        void closeEvent(QCloseEvent *event);
};

#endif
