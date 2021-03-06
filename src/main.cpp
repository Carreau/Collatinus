/*                           main.cpp
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
#include <QCoreApplication>
#include "ui_collatinus.h"
#include "main.h"
#include <assert.h>
#include <QtGui>
#include <QString>
#ifdef Q_OS_WIN32
#include <QDesktopServices>
#endif   
// fr�qences
#include "frequences.h"
// pour d�boguer
#include "libcollatinus.h"
/**
 * Bogues : 
 *
 *     lemmatisation pl�thorique de certaines formes (romains, f. suffix�es).
 *     le d�doublonnage de la lemmatisation d'une suite de formes, au lieu
 *     d'agir sur chaque lemme, agit sur l'ensemble de la lemmatisation
 *     de chaque forme. G�nant.
 *
 *     La forme canonique des pronoms ne donne pas sa morphologie
 *     � corriger progressivement
 *
 * � impl�menter
 *     Aide : appel du navigateur par d�faut ;
 *
 * Notes Mac OS X sp�cifiques :
 *      Q_OS_MAC est definit par Qt si la platforme cible est Mac Os
 */

// variables
string uia;
QString qsuia;
int minRaritas = 1; // 1 : traite tous les mots ; 4 : seulement les plus rares 
bool morphologia = true; // autorise/interdit l'affichage des morphologies.
bool syntaxis = true;

QString enTeteLaTeX (
    "\\documentclass[12pt]{article}"
    "\\usepackage[T1]{fontenc}"
    "\\usepackage[latin1]{inputenc}"
    "\\usepackage{pslatex}"
    "\\usepackage{geometry}"
    "\\usepackage[frenchb]{babel}"
    "\\geometry{a4paper, left=1in, right=1in, top=1in, bottom=1in}"
    "\\begin{document}");

QString queueLaTeX ( "\\end{document}");

QString prima (QString l)
{
    QRegExp re ("^\n?(\\w+)");
    int pos = re.indexIn(l);
    if (pos > -1)
       return re.cap (1);
    return "";
}

/**
 * La classe Editeur r�impl�mente mousePressEvent,
 * qui n'est pas captur� par d�faut dans QTextEdit.
 * Oblig� d'�tre malpropre : d�truire manuellement
 * l'�diteur d�fini dans le /designer/, et ins�rer 
 * manuellement au m�me emplacement une instance de 
 * cette classe.
 *
 */
Editeur::Editeur (QWidget *parent, const char *name)
: QTextEdit(tr(""), parent)
{
    setAccessibleName(QString::fromUtf8(name));
}

/**
 * Renvoie la lemmatisation de la s�lection, ou, si elle 
 * est nulle, tout le contenu de l'�diteur.
 *
 */

QString Editeur::lemmatiseTxt (bool alpha, bool cumVocibus)
{
    // qDebug () << "cumVocibus : " << cumVocibus << endl;
    QString txt = toPlainText ();
    QStringList formes = txt.split (QRegExp("\\W+"), QString::SkipEmptyParts);
    QStringList lemmes;
    QStringList echecs;
    QStringListIterator i (formes);
    QString Retour; 
    QString item;
    QString uox;
    while (i.hasNext ())
    {
       uox = i.next ();
       item = QString::fromStdString (lemmatise (uox.toStdString ()));
       //qDebug () << "item :" <<item << ":";
       QString lemma (prima (item));
       if (frequentia (lemma) < minRaritas)
           continue;
       if (cumVocibus) 
       {
           if (item.size () < uox.size () + 4)
              echecs << item;
           else 
           {
               item.replace ("\n", "\n    ");
               item.insert (1, uox + "\n"); 
               lemmes << item;
           }
       }
       else if (!lemmes.contains (item)) lemmes << item; 
    }
    if (alpha) lemmes.sort ();
    //if (cumVocibus && echecs.size () > 0)
    if (cumVocibus)
    {
        lemmes << "\nHAEC NON RECOGNITA:";
        lemmes << echecs; 
    }
    return lemmes.join ("");
}

QString Editeur::lemmatiseTxt_expr (bool alpha)
{
    QTextCursor C (document ());
    QTextCursor tc2 (document ());
    Phrase * P;
    //QString retour;
    QStringList lignes;
    while (!C.atEnd ())
    {
        tc2 = document ()->find (QRegExp ("[\\.\\;\\:\\?\\!]"), C);
        if (tc2.isNull ()) 
            C.movePosition (QTextCursor::End, QTextCursor::KeepAnchor); 
        else C.setPosition (tc2.position (), QTextCursor::KeepAnchor); 
        P = new Phrase (C.selectedText ());
        //emit (copie (P->analyse_et_lemmes ()));
        QStringList * sl = P->analyse_et_lemmes (minRaritas);
        QString l;
        for (int i=0;i<sl->size();i++) 
        {
            l = sl->at (i);
            if (!l.isEmpty () && !lignes.contains (l))
                lignes << sl->at (i);
        }
        delete sl;
        delete P;
        if (!C.atEnd ()) C.movePosition (QTextCursor::Right);
        qApp->processEvents ();
    }
    if (alpha) lignes.sort ();
    return lignes.join ("");
}

void Editeur::mousePressEvent (QMouseEvent *event)
{
    QTextEdit::mousePressEvent (event); 
}

void Editeur::mouseMoveEvent (QMouseEvent *event)
{
    QTextEdit::mouseMoveEvent (event); 
}

void Editeur::mouseReleaseEvent (QMouseEvent *event)
{
    /* impl�menter la lemmatisation de la s�lection 
     * si la touche Maj est enfonc�e, par exemple,
     * ou la proposer dans un contextuel.
     *
     */
    // poster un signal � l'autre �diteur 
    QPoint P = parentWidget()->mapFromGlobal(event->globalPos());
    QTextCursor C = cursorForPosition (P);
    QString mc = motCourant (C);
    //if (QString(uia).isEmpty ()) return;
    QString ligne = QString::fromStdString (lemmatise (mc.toStdString ()));
    ligne.remove (QRegExp ("^\n+"));
    if (frequentia (prima (ligne)) < minRaritas)
       return;
    // ajout �ventuel de l'expression
    if (syntaxis)
    {
        // calculer le num�ro du mot cliqu� 
        int pos = C.position ();
        int debut = 0;
        QTextCursor tc2 = document ()->find (QRegExp ("[\\.\\;\\:\\?\\!]"), C, QTextDocument::FindBackward);
        if (tc2.isNull ()) tc2 = QTextCursor (document ());
        debut = tc2.position ();
        tc2.setPosition (C.position (), QTextCursor::KeepAnchor); 
        QStringList lm = tc2.selectedText ().split (QRegExp ("\\W+"));
        int mot_num = -1;
        foreach (QString elm, lm)
           if (!elm.isEmpty ()) mot_num++;

        //int mot_num = lm.count () - 1; // -2 risque de faire planter
        //if (mot_num > 0) mot_num--; // pourquoi ? cf ligne pr�c.
        
        // si la phrase n'est pas encore analys�e, provoquer l'analyse
        if (!phrase_at_pos (pos))
        {
             tc2.setPosition (debut); 
             QTextCursor tc3 = document ()->find (QRegExp ("[\\.\\;\\:\\?\\!]"), tc2);
             tc2.setPosition (tc3.position (), QTextCursor::KeepAnchor);
             cree_phrase (tc2.selectedText (), debut, tc3.position ());
        }
        // appeler l'analyse syntaxique du mot
        QTextStream (&ligne) << analyse_syntaxique (pos, mot_num);
    }
    if (calepino_actif ())
    {
        QTextStream (&ligne) << QString::fromStdString (ambrogio (prima (ligne).toStdString ())); 
    }
    // la forme du texte latin demand�e ?
    emit (copie (ligne));
    QTextEdit::mouseReleaseEvent (event);
}

/**
 * renvoie le mot sous le curseur C
 *
 */
QString Editeur::motCourant (QTextCursor C)
{
    C.select (QTextCursor::WordUnderCursor);
    return C.selectedText ().trimmed ();
}

fenestra::fenestra(QString url)
{
    setupUi(this);
    actionAuxilium->setShortcut(QKeySequence::HelpContents);
    action_Noua->setShortcut(QKeySequence::New    );
    action_Onerare->setShortcut(QKeySequence::Open   );
    action_Exire->setShortcut(QKeySequence::Quit   );
    actionScribere_s_ub->setShortcut(QKeySequence::SaveAs );
    action_Scribere->setShortcut(QKeySequence::Save   );
    actionInuenire->setShortcut(QKeySequence::Find   );
    actionInuenire_etiam->setShortcut(QKeySequence::FindNext);
    actionMaiores_litteras->setShortcut(QKeySequence::ZoomIn );
    actionMinores_litteras->setShortcut(QKeySequence::ZoomOut);

	//ajout des pr�f�rences dans le menu

    #ifdef __APPLE__
    QWidget* stretchWidget = new QWidget;
    stretchWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QLineEdit* qle = new QLineEdit();
    QString style(
    "QListView, QLineEdit {"
        "selection-color: white; "
        "border: 2px groove gray;"
        "border-radius: 13px;"
        "padding: 2px 2px;"
        "background-position: top right;"
        "padding-right: 0px;"
    "}"
        "QLineEdit:focus {"
        "selection-color: white;     "
        "padding: 2px 2px;"
        "padding-right: 0px;"
    "}"
        "QLineEdit:edit-focus {"
        "selection-color: white;     "
        "padding: 2px 2px;"
        "padding-right: 0px;"
    "}"
    );
    qle->setStyleSheet(style);
    qle->setAttribute(Qt::WA_MacShowFocusRect, 0);
    toolBar->addWidget(stretchWidget);
    toolBar->addWidget(qle);
    connect(qle, SIGNAL(textChanged(QString)), this, SLOT(inuenire(QString)));
    #endif

    delete (EditLatin);
    Ed = new Editeur(splitter, "EditLatin");
    Ed->setObjectName(QString::fromUtf8("EditLatin"));
    // remont� dans la zone Apple
    // connect(qle, SIGNAL(textChanged(QString)), this, SLOT(inuenire(QString)));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(13), static_cast<QSizePolicy::Policy>(13));
    setSizePolicy(sizePolicy3);
    setMouseTracking(true);
    splitter->insertWidget(0,Ed);
    // pour win : police de Flexio
    #ifdef Q_OS_WIN32
    QFont font;
    font.setFamily(QString::fromUtf8("FixedSys"));
    font.setPointSize(9);
    font.setBold(false);
    font.setItalic(false);
    font.setUnderline(false);
    font.setWeight(50);
    font.setStrikeOut(false);
    EditFlexio->setFont(font);
    #endif
    createActions ();
    // chargement du lexique et du fichier d'exemple
    if (url.isEmpty ())
        capsamInLatinum ( qsuia + "lucretia.txt");
    else
        capsamInLatinum ( url);
    // chargement des lexiques fr et synt.
	lexicumLege ("fr", uia + "lemmata.fr");
    lexicumLege ("de", uia + "lemmata.de");
    lexicumLege ("uk", uia + "lemmata.uk");
    lexicumLege ("es", uia + "lemmata.es");
    lexicumLege ("ca", uia + "lemmata.ca");
    lexicumLege ("gl", uia + "lemmata.gl");

    lexicumDic ("fr");
    activeCalepin (false);
    //qDebug () << "uia :" << uia;
    lis_expr (qsuia + "expressions.fr");

    // param�trage de la recherche syntaxique ; � compl�ter
    cree_texte ();
    change_syntaxe ();
    repertoire = QDir (url).absolutePath ();
}


void fenestra::decollatino ()
{
    QMessageBox::about(this, tr("De Collatino"),
            tr("COLLATINVS\nLinguae latinae lemmatizatio \n"
               "Licenti� GPL, (c) Yves Ouvrard, annis 2009 - 2011 \n"
               "Versio "VERSION"\n"
               "Gratias illis habeo :\n"
               "William Whitaker,\n"
               "Jose Luis Redrejo."));
}

/**
 * cautio retourne Vrai si les �diteurs n'ont pas
 * �t� modifi�s, ou si l'utilisateur d�sire
 * les sauver.
 *
 */
bool fenestra::cautio ()
{
    if (Ed->document()->isModified()
       || EditTextus->document()->isModified() 
       || EditHtml->document()->isModified()
       || EditLaTeX->document()->isModified() ) 
       {
           int ret = QMessageBox::warning(this, tr("Collatinus"),
                   tr("Opus tuum scriptum non est.\n"
                      "Id facere uis nunc ?"),
                   QMessageBox::Yes | QMessageBox::Default,
                   QMessageBox::No,
                   QMessageBox::Cancel | QMessageBox::Escape);
           if (ret == QMessageBox::Yes)
               return scribere ();
           else if (ret == QMessageBox::Cancel)
               return false;
    }
    return true;
}

bool fenestra::capsaminDiscum (const QString &fileName)
{
    int ci = tabWidget->currentIndex ();
    if (ci > 2) return false;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Collatinus"),
                             tr("Capsam nequeo scribere %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // selon l'onglet
    if (ci == 0)
    {
        out << Ed->toPlainText();
        out << endl << endl;
        out << EditTextus->toPlainText ();
    }
    else if (ci == 1)
    {
       QString tampon = EditHtml->toHtml ();
       EditHtml->clear ();
       EditHtml->append (Ed->toPlainText ());
       EditHtml->append (tampon);
       out << EditHtml->toHtml ();
       EditHtml->clear ();
       EditHtml->insertHtml (tampon);
    }
    else if (ci == 2)
    {
       out << enTeteLaTeX;
       out << Ed->toPlainText ();
       out << EditLaTeX->toPlainText ();
       out << queueLaTeX;
    }
    QApplication::restoreOverrideCursor();

    daFichierCourant(fileName);
    statusBar()->showMessage(tr("Capsa scripta"), 2000);
    return true;
}

bool fenestra::scribere ()
{
    if (courant.isEmpty()) {
        return scribereVt ();
    } else {
        return capsaminDiscum (courant);
    }
}

bool fenestra::scribereVt ()
{
    QString format, ext;
    switch (tabWidget->currentIndex ())
    {
        case 0:
            format = "textus (*.txt);; quam libet (*)";
            ext = "txt";
            break;
        case 1:
            format = "html (*.html);; quam libet (*)";
            ext = "html";
            break;
        case 2:
            format = "LaTeX (*.tex);; quam libet (*)";
            ext = "tex";
            break;
        default:
            format = "";
    }
     
    QString capsaeNomen = QFileDialog::getSaveFileName (this,"Collatinus - quem capsam ?", "", format);
    if (capsaeNomen.isEmpty())
        return false;
    // s'assurer de l'extension
    QFileInfo fi (capsaeNomen); 
    if (fi.suffix () != ext)
       capsaeNomen = fi.filePath () + "." + ext;
    return capsaminDiscum (capsaeNomen);
}


/**
 * Charge le fichier nomm� fileName dans 
 * L'�diteur de texte latin (partie sup�rieure).
 *
 */
void fenestra::capsamInLatinum (const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) 
    {
        QMessageBox::warning(this, tr("Collatinus"),
                             tr("Capsam legere nequeo %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    Ed->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    daFichierCourant(""); // pour �viter d'�craser le fichier charg�.
    statusBar()->showMessage(tr("Capsa onerata"), 2000);
    // r�initialiser la syntaxe
    vide_phrases ();
}

QString fenestra::motCourant (QTextCursor C)
{
    C.select (QTextCursor::WordUnderCursor);
    return C.selectedText ().trimmed ();
}

bool fenestra::event (QEvent *event)
{
    if (event->type () == QEvent::ToolTip && Ed->underMouse ())
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        QPoint P = Ed->parentWidget()->mapFromGlobal(helpEvent->globalPos());
        QTextCursor muspos = Ed->cursorForPosition (P);
        QString mot = motCourant (muspos);
        // �ter crochets et guillemets
        mot = mot.replace (QRegExp ("['\"]"), "");
        QString bulla;
        if (morphologia) bulla = QString::fromStdString (lemmatiseM (mot.toStdString ()));
        else bulla = QString::fromStdString (lemmatise (mot.toStdString ()));
        if (frequentia (prima (bulla)) < minRaritas) 
            bulla = "id scire debes"; 
        QToolTip::showText (helpEvent->globalPos(), bulla, this);
    }
    return QWidget::event (event);
}

/**
 * Renvoie l format� en html pour que le 1er mot soit en gras
 */
QString crassaPrima (QString l, int latex=0)
{
    QStringList lineae = l.split ("\n");
    QRegExp re ("^(\\w+)");
    //QRegExp refin ("$");
    if (latex)
    {
        lineae.replaceInStrings (re, "\\textbf{\\1}");    
        return lineae.join ("\\\\\n");
    }
    else
    {
        lineae.replaceInStrings (re, "<b>\\1</b>");
        return lineae.join ("<br/>\n");
    }
}

void fenestra::recoisLemm (QString l)
{
    //editeurCourant ()->append (l);
    int ci = tabWidget->currentIndex ();
    if (ci == 0) 
        //editeurCourant ()->append (l);
        EditTextus->append (l);
    else if (ci == 1)
    {
        l = crassaPrima (l); 
        EditHtml->append (l);
    }
    else if (ci == 2)
    {
        l = crassaPrima (l, 1);
        EditLaTeX->append (l);
    }
    else if (ci == 3)
    {
        QRegExp re ("^(\\w+)");
        int pos = re.indexIn(l);
        if (pos > -1)
        {
            QString k (re.cap (1));
            Tentree *e = entree (k.toStdString ());
            EditFlexio->clear ();
            if (!e->est_vide ())
                EditFlexio->append (QString::fromStdString (flechis (e)));
            else EditFlexio->append ("id " + k + " non intellexi.");
        }
    }
}

QTextEdit* fenestra::editeurCourant ()
{
    switch (tabWidget->currentIndex ())
    {
        case 0: return EditTextus;
        case 1: return EditHtml;
        case 2: return EditLaTeX;
        default: return EditFlexio;
    }
}


void fenestra::daFichierCourant (const QString &capsaeNomen)
{
    courant = capsaeNomen;
    Ed->document()->setModified(false);
    EditTextus->document()->setModified(false);
    EditHtml->document()->setModified(false);
    EditLaTeX->document()->setModified(false);
    //setWindowModified(false);

    QString nomVisible;
    if (courant.isEmpty())
        nomVisible = "sine_nomine.txt";
    else
        nomVisible = nomSeul (courant);

    setWindowTitle(tr("%1[*] - %2").arg(nomVisible).arg(tr("Collatinus")));
}

QString fenestra::nomSeul (const QString &nomComplet)
{
    return QFileInfo(nomComplet).fileName();
}

/**
 * effacement de tous les �diteurs
 * apr�s appel �ventuel au dialogue
 * de pr�caution.
 */
void fenestra::noua ()
{
     if (cautio ())
     {
         Ed->clear ();
         EditTextus->clear ();
         EditHtml->clear ();
         EditLaTeX->clear ();
         vide_phrases ();
         Ed->setFocus ();
     }
}

void fenestra::legere()
{
    if (cautio ()) {
        QString capsaeNomen = QFileDialog::getOpenFileName(this, "Capsam legere", repertoire);
        if (!capsaeNomen.isEmpty())
        {
            capsamInLatinum (capsaeNomen);
            repertoire = QDir (capsaeNomen).absolutePath ();
        }
    }
}

void fenestra::lemmataRadere ()
{
    editeurCourant ()->clear (); 
}

void fenestra::closeEvent(QCloseEvent *event)
{
    if (cautio ()) {
        //writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void fenestra::lemmatiseTout ()
{
    QString (lem) (bool, bool); 
    editeurCourant ()->clear ();
    QString T;
    if (syntaxis) T = Ed->lemmatiseTxt_expr (actionAlphabetice->isChecked ());
    else T = Ed->lemmatiseTxt ( actionAlphabetice->isChecked (),
                                actionCum_textus_uocibus->isChecked ());
    int ci = tabWidget->currentIndex ();
    if (ci == 0)
        EditTextus->append (T);
    else if (ci == 1)
    {
        T = crassaPrima (T);
        EditHtml->insertHtml (T); 
    }
    else if (ci == 2)
    {
        T = crassaPrima (T, 1);
        EditLaTeX->insertPlainText (T); 
    }
    editeurCourant ()->moveCursor (QTextCursor::Start);
}

void fenestra::germanice ()
{
    lexicumDic ("de");
    actionGermanice->setChecked(true );
}

void fenestra::gallice ()
{
    lexicumDic ("fr");
    actionGallice->setChecked(true );
}

void fenestra::anglice ()
{
    lexicumDic ("uk");
    actionAnglice->setChecked(true );
}

void fenestra::hispanice ()
{
    lexicumDic ("es");
    actionHispanice->setChecked(true);
}

void fenestra::catalanice ()
{
    lexicumDic ("ca");
    actionCatal_n->setChecked(true);
}

void fenestra::gallaece ()
{
    lexicumDic ("gl");
    actionGallego->setChecked(true);
}

void fenestra::calepin ()
{
    activeCalepin (true);
}

void fenestra::inuenire (const QString & exp)
{
    QTextCursor find_result = Ed->document()->find(exp);
    if( find_result.hasSelection())
        Ed->setTextCursor(find_result);
    else
        Ed->setTextCursor(QTextCursor(Ed->document()));
}
void fenestra::inuenire ()
{
    bool ok;
    quaerendum  = QInputDialog::getText(this, tr("Quid quaeris ?"),
                                         tr("Quid quaeris ?"), QLineEdit::Normal,
                                         quaerendum, &ok);
    if (ok && !quaerendum.isEmpty())
        Ed->find (quaerendum);
}

void fenestra::inuenire_denuo ()
{
    if (quaerendum.isEmpty ()) inuenire ();
    else Ed->find (quaerendum);
}

void fenestra::auxilium ()
{
    #ifdef Q_OS_LINUX

    //QFileInfo info (qsuia + "../doc/index.html");
    QFileInfo info ("/doc/index.html");
    QString html;
    if (info.exists ())
        html = "mozilla \"file://" + info.absoluteFilePath () + "\""; 
    else html = "mozilla \"file:///usr/share/doc/collatinus/index.html\"";
    QProcess processus;
    processus.startDetached (html);
    #else
        #ifdef Q_OS_MAC
            QString qs = QApplication::applicationDirPath();
            qs.append("/doc/index.html");
            QDesktopServices::openUrl(QUrl::fromLocalFile(qs));
        #else
            QDesktopServices::openUrl(QUrl("index.html"));
        #endif
    #endif
}

/**************
 * Syntaxe 
 **************/

void fenestra::change_syntaxe ()
{
    syntaxis = actionSyntaxis->isChecked ();
    // d�sactiver s'il y a lieu l'option "cum textus uocibus"
    actionCum_textus_uocibus->setChecked(false);
    //actionCum_textus_uocibus->setCheckable(!syntaxis);
    actionCum_textus_uocibus->setEnabled(!syntaxis);
}

void fenestra::change_morpho (bool m)
{
    morphologia = m;
}

void fenestra::change_rarete ()
{
    if (action1->isChecked ()) minRaritas = 1;
    else if (action2->isChecked ()) minRaritas = 2;
    else if (action3->isChecked ()) minRaritas = 3;
    else if (action4->isChecked ()) minRaritas = 4;
    else minRaritas = 5;
}

void fenestra::vide_texte ()
{
    vide_phrases ();
}

QString fenestra::adHtml (QString t)
{
    t.insert (0, "<strong>");
    t.replace (QRegExp (":"), "</strong>");
    return t;
}

void fenestra::controleIcone (int o)
{
    actionOmnia_lemmatizare->setEnabled (o != 3);
}

void fenestra::createActions ()
{
    connect(action_Noua, SIGNAL(triggered()), Ed, SLOT(clear()));
    connect(action_Noua, SIGNAL(triggered()), this, SLOT(noua()));
    connect (action_Onerare, SIGNAL (triggered ()), this, SLOT (legere ()));
    connect (action_Scribere, SIGNAL (triggered ()), this, SLOT (scribere ()));
    connect (actionScribere_s_ub, SIGNAL (triggered ()), this, SLOT (scribereVt ()));
    connect(actionMaiores_litteras, SIGNAL(triggered()), Ed, SLOT(zoomIn()));
    connect(actionMinores_litteras, SIGNAL(triggered()), Ed, SLOT(zoomOut()));
    connect (actionDe_Collatino, SIGNAL (triggered ()), this, SLOT(decollatino ()));
    connect (actionOmnia_lemmatizare, SIGNAL (triggered ()), this, SLOT(lemmatiseTout ()));
    connect(Ed, SIGNAL(copie (QString)), this, SLOT(recoisLemm (QString)));
    connect(Ed, SIGNAL(textChanged()), actionVide_texte, SLOT(trigger()));
    connect(actionL_emmata_radere, SIGNAL(triggered()), this, SLOT(lemmataRadere()));
    //connect(action_Lineam_radere, SIGNAL(triggered()), this, SLOT(lineamRadere()));
    connect(actionGermanice, SIGNAL(triggered ()), this, SLOT(germanice ()));
    connect(actionAnglice, SIGNAL(triggered ()), this, SLOT(anglice ()));
    connect(actionGallice, SIGNAL(triggered ()), this, SLOT(gallice ()));
    connect(actionHispanice, SIGNAL(triggered ()), this, SLOT(hispanice ()));
    connect(actionCatal_n, SIGNAL(triggered ()), this, SLOT(catalanice ()));
    connect(actionGallego, SIGNAL(triggered ()), this, SLOT(gallaece ()));
    connect(actionCalepino, SIGNAL(triggered ()), this, SLOT (calepin ()));
    connect(actionInuenire, SIGNAL(triggered ()), this, SLOT(inuenire ()));
    connect(actionInuenire_etiam, SIGNAL(triggered ()), this, SLOT(inuenire_denuo ()));
    connect(actionAuxilium, SIGNAL(triggered ()), this, SLOT(auxilium ()));
    connect(actionVide_texte, SIGNAL(triggered ()), this, SLOT(vide_texte ()));
    connect(actionSyntaxis, SIGNAL(triggered ()), this, SLOT (change_syntaxe ()));
    connect(tabWidget, SIGNAL(currentChanged (int)), this, SLOT (controleIcone (int)));
    connect(actionMorphologia_in_bullis, SIGNAL(toggled (bool)), this, SLOT(change_morpho (bool)));

    QActionGroup * grLingua = new QActionGroup (this); 
    actionGermanice->setActionGroup (grLingua);
    actionAnglice->setActionGroup (grLingua);
    actionGallice->setActionGroup (grLingua);
    actionHispanice->setActionGroup (grLingua);
    actionCatal_n->setActionGroup (grLingua);
    actionGallego->setActionGroup (grLingua);

    actionGallice->setChecked (true);

    action1->setCheckable (true);
    action2->setCheckable (true);
    action3->setCheckable (true);
    action4->setCheckable (true);
    action5->setCheckable (true);

    action1->setChecked (true);

    QActionGroup * grRaritas = new QActionGroup (this);
    action1->setActionGroup (grRaritas);
    action2->setActionGroup (grRaritas);
    action3->setActionGroup (grRaritas);
    action4->setActionGroup (grRaritas);
    action5->setActionGroup (grRaritas);

    connect(action1, SIGNAL(triggered ()), this, SLOT(change_rarete ()));
    connect(action2, SIGNAL(triggered ()), this, SLOT(change_rarete ()));
    connect(action3, SIGNAL(triggered ()), this, SLOT(change_rarete ()));
    connect(action4, SIGNAL(triggered ()), this, SLOT(change_rarete ()));
    connect(action5, SIGNAL(triggered ()), this, SLOT(change_rarete ()));
}

int main( int argc, char **argv )
{
    uia = argv[0];
#ifdef Q_OS_WIN32
    uia.erase (uia.length () - 14);
#else
    uia.erase (uia.length () - 10);
#endif
    uia += "ressources/";
    qsuia = QString::fromStdString (uia);
    //qDebug () << qsuia; 
    QApplication app(argc, argv);

    //let's set a few variable use to get/load settings
    QCoreApplication::setOrganizationName("Collatinus");
    QCoreApplication::setOrganizationDomain("Collatinus.org");
    QCoreApplication::setApplicationName("Collatinus");

    QTranslator translator;
    translator.load("collatinus_la");
    app.installTranslator(&translator);
    
    if (!QFile::exists (qsuia + "lucretia.txt"))
    {
#ifdef Q_OS_WIN32
        uia = "C:\\Program Files\\collatinus\\";
#else
        //on evite les elifdef qui ne compilent pas sur tout les pr�processeurs
  #ifdef Q_OS_MAC
        QString tempqs = app.applicationDirPath();
        tempqs.append("/");
        uia = tempqs.toStdString();
  #else
        uia = "/usr/share/collatinus/";
  #endif
#endif
        qsuia = QString::fromStdString (uia);
    }
    fenestra f(argv[1]);
    f.show();
    return app.exec();
}
