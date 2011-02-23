#include "frequences.h"
#include <QtCore>
//string ftab [4] [455] = {
// freq 1<< 405 items
QStringList fi = QStringList () <<
"a" << "ab" << "abs" << "absum" << "ac" << "accipio" << "acies" << "ad"
<< "addo" << "adsum" << "aetas" << "ager" << "agmen" << "ago" << "alienus" <<
"aliquis" << "alius" << "alter" << "altus" << "amicitia" << "amicus" << "amo"
<< "amor" << "an" << "animus" << "annus" << "ante" << "apud" << "aqua" <<
"arma" << "ars" << "at" << "atque" << "audeo" << "audio" << "aut" << "autem" <<
"bellum" << "bene" << "beneficium" << "bonum" << "bonus" << "cado" << "caelum"
<< "capio" << "caput" << "carmen" << "castra" << "causa" << "cedo" << "certus"
<< "ciuis" << "ciuitas" << "coepio" << "cogo" << "cognosco" << "coniux" <<
"consilium" << "constituo" << "consul" << "contra" << "copia" << "corpus" <<
"credo" << "cui" << "cum" << "cuncti" << "cur" << "cura" << "de" << "debeo" <<
"deinde" << "deus" << "dico" << "dies" << "dignus" << "diu" << "do" << "doceo"
<< "dolor" << "domus" << "duco" << "dum" << "duo" << "durus" << "dux" <<
"efficio" << "e" << "ego" << "enim" << "eo" << "eques" << "equus" << "ergo" <<
"et" << "etiam" << "ex" << "exercitus" << "existimo" << "facio" << "fama" <<
"fatum" << "fero" << "ferrum" << "fides" << "filius" << "finis" << "fio" <<
"flumen" << "fortis" << "fortuna" << "frater" << "fuga" << "fugio" << "gens" <<
"genus" << "gero" << "gloria" << "gratia" << "grauis" << "habeo" << "haud" <<
"hic" << "hinc" << "homo" << "honor" << "honos" << "hostis" << "humanus" <<
"ibi" << "idem" << "igitur" << "ignis" << "ille" << "imperium" << "impetus" <<
"in" << "inde" << "inquis" << "inquit" << "inferus" << "ingenium" << "ingens"
<< "iniuria" << "inquam" << "intellego" << "inter" << "inuenio" << "ipse" <<
"ira" << "is" << "iste" << "ita" << "itaque" << "item" << "iter" << "iaceo" <<
"iam" << "iubeo" << "iudico" << "ius" << "iuuenis" << "labor" << "laetus" <<
"legatus" << "legio" << "leuis" << "lex" << "liceo" << "licet" << "locus" <<
"longe" << "longus" << "loquor" << "lumen" << "magis" << "magnus" << "malum" <<
"malus" << "manus" << "mare" << "mater" << "medius" << "mens" << "metus" <<
"meus" << "miles" << "mille" << "miser" << "mitto" << "modo" << "modus" <<
"mons" << "morior" << "mors" << "mos" << "moueo" << "multum" << "multus" <<
"munus" << "muto" << "nam" << "nascor" << "natura" << "nauis" << "ne" << "nec"
<< "nego" << "nemo" << "nihil" << "nisi" << "nomen" << "non" << "nos" <<
"nosco" << "noster" << "nouus" << "nox" << "nullus" << "numerus" << "numquam"
<< "nunc" << "o" << "ob" << "oculus" << "omnis" << "oportet" << "ops" << "opus"
<< "ordo" << "os" << "ostendo" << "par" << "parens" << "paro" << "pars" <<
"parum" << "paruus" << "pater" << "patior" << "paucus" << "pax" << "pectus" <<
"pecunia" << "per" << "pereo" << "periculum" << "peruenio" << "pes" << "peto"
<< "placeo" << "placet" << "poena" << "pono" << "populus" << "possum" << "post"
<< "postquam" << "praesidium" << "praesto" << "premo" << "primum" << "primus"
<< "princeps" << "pro" << "proelium" << "propior" << "propter" << "prosum" <<
"prouincia" << "puella" << "puer" << "puto" << "quaero" << "qualis" << "quam"
<< "quamuis" << "quantus" << "quemadmodum" << "qui" << "quia" << "quicquis" <<
"quicumque" << "quid" << "quidam" << "quidem" << "quin" << "quis" << "quisquam"
<< "quisque" << "quisquis" << "quo" << "quod" << "quoniam" << "quoque" <<
"rapio" << "ratio" << "recipio" << "reddo" << "redeo" << "refero" << "regio" <<
"relinquo" << "reliquus" << "res" << "respublica" << "rex" << "saepe" <<
"sanguis" << "sapiens" << "satis" << "scelus" << "scio" << "scribo" << "se" <<
"sed" << "semper" << "senatus" << "sentio" << "sequor" << "seruo" << "seruus"
<< "si" << "sic" << "signum" << "silua" << "simul" << "sine" << "singuli" <<
"similis" << "siue" << "sol" << "soleo" << "solus" << "spatium" << "specto" <<
"spes" << "sto" << "studium" << "sub" << "sui" << "sum" << "sumo" << "superus"
<< "supra" << "suus" << "talis" << "tam" << "tamen" << "tamquam" << "tantum" <<
"tantus" << "telum" << "tempus" << "teneo" << "terra" << "timeo" << "tollo" <<
"tot" << "totus" << "trado" << "traho" << "transeo" << "tres" << "tu" << "tum"
<< "tunc" << "tuus" << "ubi" << "ullus" << "unda" << "unde" << "unus" << "urbs"
<< "usus" << "ut" << "uterque" << "utor" << "ue" << "uel" << "uenio" <<
"uentus" << "uerbum" << "uero" << "uerto" << "uerus" << "uester" << "uetus" <<
"uia" << "uideo" << "uideor" << "uinco" << "uir" << "uirtus" << "uis" << "uita"
<< "uitium" << "uiuo" << "uoco" << "uolo" << "uoluptas" << "uos" << "uox" <<
"uulnus" << "uultus";

//string freq2 455 items
QStringList fii = QStringList () << 
"abeo" << "accedo" << "accido" << "acer" << "acutus" << "adicio" << "adduco" <<
"adeo" << "adfero" << "adhuc" << "adiungo" << "adulescens" << "aduenio" <<
"aduersus" << "aequor" << "aequus" << "aer" << "affero" << "agito" << "aio" <<
"ait" << "aliquando" << "aliter" << "altitudo" << "amitto" << "amnis" <<
"anima" << "animal" << "antequam" << "antiquus" << "appareo" << "ara" <<
"arbor" << "ardor" << "arena" << "argumentum" << "armo" << "aspicio" << "atqui"
<< "auctor" << "auctoritas" << "aufero" << "augeo" << "aura" << "auris" <<
"aurum" << "auxilium" << "barbarus" << "Base" << "beatus" << "breuis" <<
"caedes" << "caedo" << "campus" << "cano" << "careo" << "carus" << "casus" <<
"cerno" << "certe" << "ceteri" << "cibus" << "circa" << "circum" << "cito" <<
"clamor" << "clarus" << "classis" << "claudo" << "coerceo" << "cogito" <<
"cohors" << "colloco" << "colo" << "color" << "columna" << "coma" << "comes" <<
"committo" << "communis" << "compono" << "concedo" << "concito" << "condo" <<
"confero" << "conficio" << "conloco" << "consisto" << "consto" << "consulo" <<
"contemno" << "contendo" << "contineo" << "contingit" << "contingo" <<
"conuenio" << "conuerto" << "cornu" << "cresco" << "crimen" << "cupio" <<
"curo" << "cursus" << "custos" << "deicio" << "dea" << "deduco" << "defendo" <<
"deficio" << "deleo" << "depello" << "desero" << "desino" << "desum" <<
"dexter" << "dextera" << "difficilis" << "dimitto" << "discedo" << "disco" <<
"dispono" << "diuersus" << "diues" << "diuido" << "diuitiae" << "diuus" <<
"dominus" << "donec" << "dono" << "donum" << "dubito" << "dubium" << "dubius"
<< "dulcis" << "edo" << "egeo" << "eligo" << "eloquor" << "equitatus" <<
"eripio" << "erro" << "etiamsi" << "excipio" << "exemplum" << "exeo" <<
"exerceo" << "exigo" << "experior" << "exterus" << "extra" << "facies" <<
"facile" << "facilis" << "factum" << "fallo" << "falsus" << "fateor" << "felix"
<< "femina" << "fera" << "fingo" << "flamma" << "fleo" << "fons" << "forma" <<
"forte" << "forum" << "frango" << "frons" << "frumentum" << "fundo" << "furor"
<< "gaudeo" << "gaudium" << "gigno" << "gradus" << "gratus" << "grauitas" <<
"grauiter" << "haereo" << "herba" << "hodie" << "honestus" << "hora" << "huc"
<< "humerus" << "humor" << "ideo" << "ignauus" << "illic" << "imago" <<
"imperator" << "impero" << "impleo" << "impono" << "incertus" << "incipio" <<
"indico" << "infero" << "ingratus" << "initium" << "intentio" << "interficio"
<< "interim" << "intersum" << "intra" << "inuidia" << "iacto" << "iudicium" <<
"iugum" << "iungo" << "iustus" << "iuuo" << "lacrima" << "laedo" << "latus" <<
"laudo" << "laus" << "lego" << "liber" << "liberi" << "libertas" << "libido" <<
"littera" << "litus" << "lucus" << "lux" << "magister" << "magnitudo" <<
"maiores" << "male" << "malo" << "maneo" << "materia" << "meditor" << "membrum"
<< "memini" << "memoria" << "metuo" << "minime" << "minor" << "miror" <<
"misceo" << "moenia" << "mollis" << "mora" << "morbus" << "moror" << "mortalis"
<< "motus" << "mox" << "multi" << "multitudo" << "multo" << "mundus" <<
"munitio" << "murus" << "namque" << "natus" << "necessarius" << "necesse" <<
"negotium" << "nescio" << "neue" << "nobilis" << "noceo" << "nolo" << "nondum"
<< "nonne" << "nostri" << "notus" << "nudus" << "num" << "numen" << "nusquam"
<< "obligo" << "occido" << "occupo" << "occurro" << "odium" << "officium" <<
"olim" << "onus" << "opera" << "opto" << "oratio" << "orbis" << "orior" <<
"otium" << "pactum" << "paratus" << "parco" << "pariter" << "pateo" << "patria"
<< "patrius" << "paulo" << "pello" << "perdo" << "perficio" << "permitto" <<
"persona" << "persuadeo" << "pertineo" << "plebs" << "plenus" << "plus" <<
"pondus" << "porta" << "portus" << "postea" << "posterus" << "potens" <<
"potestas" << "potius" << "praebeo" << "praeclarus" << "praeda" << "praemium"
<< "praesens" << "praesertim" << "praeter" << "praeterea" << "precor" <<
"pretium" << "prex" << "principium" << "prior" << "priuatus" << "probo" <<
"procedo" << "procul" << "proficiscor" << "prohibeo" << "promitto" << "prope"
<< "propero" << "prudens" << "prudentia" << "publicus" << "pudor" << "pugna" <<
"pugno" << "pulcher" << "qua" << "quamquam" << "quando" << "quanto" <<
"quantum" << "quare" << "quasi" << "quattuor" << "queror" << "quiesco" <<
"quinque" << "quippe" << "quisnam" << "quomodo" << "quotiens" << "regina" <<
"regius" << "rego" << "reperio" << "repeto" << "respondeo" << "retineo" <<
"rogo" << "rursus" << "sacer" << "saeuus" << "sapientia" << "saxum" <<
"secundus" << "sedeo" << "sedes" << "semen" << "senex" << "sensus" <<
"sententia" << "sermo" << "seruitium" << "sestertius" << "sicut" << "sinus" <<
"socius" << "soluo" << "somnus" << "soror" << "spargo" << "species" << "spero"
<< "spiritus" << "sponte" << "statim" << "statuo" << "subeo" << "summus" <<
"super" << "supero" << "supersum" << "supplicium" << "sustineo" << "tandem" <<
"tango" << "tectum" << "tego" << "tellus" << "tempestas" << "templum" <<
"tempto" << "tendo" << "tener" << "tenuis" << "tergum" << "tertius" << "timor"
<< "torqueo" << "tribunus" << "tristis" << "tueor" << "turba" << "turpis" <<
"tutus" << "ultimus" << "ultra" << "umbra" << "umerus" << "umor" << "umquam" <<
"undique" << "unquam" << "usque" << "utilis" << "uxor" << "ualeo" << "ualidus"
<< "uarius" << "uelut" << "uerum" << "uestis" << "uicinus" << "uictor" <<
"uictoria" << "uigeo" << "uinum" << "uirgo" << "uiscera" << "uix" << "uoluntas"
<< "uotum" << "uulgus";

//string freq3<< 450 items
QStringList fiii = QStringList () << 
"abstineo" << "accendo" << "aditus" << "admitto" << "admoneo" << "admoueo" <<
"aduentus" << "aedes" << "aedificium" << "aeger" << "aeque" << "aes" <<
"aestimo" << "aestus" << "aeternus" << "aeuum" << "affectus" << "afficio" <<
"alo" << "amplus" << "angustus" << "animaduerto" << "aperio" << "apis" <<
"aptus" << "arbitror" << "ardeo" << "artus" << "aruum" << "arx" << "asper" <<
"ater" << "attingo" << "auaritia" << "auerto" << "auidus" << "auis" << "aureus"
<< "bibo" << "bos" << "bracchium" << "caecus" << "canis" << "candidus" <<
"captiuus" << "capto" << "castellum" << "caueo" << "celer" << "censeo" <<
"centum" << "centurio" << "certamen" << "certo" << "ceruix" << "cesso" <<
"circiter" << "ciuilis" << "clamo" << "colligo" << "collis" << "collum" <<
"complures" << "condicio" << "coniicio" << "coniungo" << "conor" << "consequor"
<< "conspectus" << "conspicio" << "consuetudo" << "consulatus" << "consumo" <<
"contentus" << "continuus" << "contrarius" << "corona" << "corrumpo" <<
"crassitudo" << "creber" << "creo" << "cubile" << "culpa" << "cultus" <<
"cupiditas" << "curro" << "currus" << "decem" << "decet" << "decipio" <<
"decorus" << "decus" << "deffero" << "delecto" << "deligo" << "demitto" <<
"dens" << "depono" << "descendo" << "desidero" << "despicio" << "detraho" <<
"dictum" << "differo" << "difficultas" << "digitus" << "dignitas" << "dirus" <<
"discrimen" << "disputo" << "diuinus" << "doleo" << "dolus" << "domina" <<
"ecce" << "educo" << "effero" << "effugio" << "effundo" << "egredior" <<
"egregius" << "emineo" << "emitto" << "emo" << "error" << "euenio" << "excito"
<< "excutio" << "exiguus" << "eximius" << "exitus" << "expecto" << "exspecto"
<< "expedio" << "explico" << "expono" << "exilium" << "exsilium" << "exstinguo"
<< "fabula" << "facinus" << "familia" << "fax" << "felicitas" << "fere" <<
"ferus" << "fessus" << "figo" << "filia" << "finio" << "finitimus" << "flecto"
<< "fluo" << "foramen" << "foris" << "formosus" << "fraus" << "fretum" <<
"frigidus" << "frigus" << "fructus" << "fruor" << "frustra" << "fulgeo" <<
"fulmen" << "funus" << "gemo" << "genu" << "gladiator" << "gladius" << "Graeci"
<< "grandis" << "grex" << "habitus" << "hiems" << "hortor" << "hostilis" <<
"humus" << "ictus" << "ignarus" << "ignotus" << "illuc" << "imber" << "imitor"
<< "immo" << "immortalis" << "impedimentum" << "impedio" << "impello" <<
"impius" << "inanis" << "incendo" << "incido" << "inclino" << "incolumis" <<
"induco" << "ineo" << "infans" << "infelix" << "infestus" << "ingredior" <<
"inimicus" << "iniquus" << "inopia" << "insequor" << "insidiae" << "instituo"
<< "insto" << "instruo" << "insula" << "integer" << "interdum" << "interea" <<
"interuallum" << "intimus" << "intro" << "intueor" << "inuitus" << "irascor" <<
"iterum" << "iacio" << "iudex" << "iuro" << "iuuentus" << "iuuenta" << "laboro"
<< "lacertus" << "languidus" << "lapis" << "late" << "lateo" << "latitudo" <<
"lectus" << "lentus" << "libet" << "limen" << "lingua" << "ludo" << "lugeo" <<
"luna" << "maestus" << "magistratus" << "mano" << "maritus" << "maxime" <<
"mediocris" << "memoro" << "mensa" << "mereo" << "militaris" << "mirus" <<
"mobilis" << "moles" << "moneo" << "mutus" << "natio" << "nauigo" <<
"necessitas" << "nefas" << "nemus" << "neruus" << "niger" << "nimis" <<
"nimium" << "nimius" << "nubes" << "numero" << "obiicio" << "obsideo" <<
"obtineo" << "odi" << "offero" << "omitto" << "opinio" << "opprimo" << "ora" <<
"oro" << "paene" << "pareo" << "paries" << "pario" << "partim" << "passus" <<
"pauci" << "paulatim" << "paupertas" << "pecco" << "pecus" << "pedes" <<
"pendeo" << "perduco" << "perfero" << "pergo" << "peritus" << "permoueo" <<
"perpetuus" << "philosophia" << "philosophus" << "pietas" << "piscis" << "pius"
<< "plane" << "plerique" << "pomum" << "pons" << "pontus" << "porticus" <<
"possideo" << "posterius" << "postulo" << "potentia" << "praeceps" <<
"praeceptum" << "praecipio" << "praefectus" << "praetereo" << "praetor" <<
"primo" << "prius" << "priusquam" << "prodo" << "profecto" << "profero" <<
"profiteor" << "progredior" << "propono" << "proprius" << "prorsus" <<
"prosequor" << "puluis" << "purus" << "quartus" << "queo" << "quies" <<
"quondam" << "rarus" << "ratis" << "recedo" << "recens" << "recte" << "rectus"
<< "recuso" << "reduco" << "remedium" << "remitto" << "reor" << "repente" <<
"resisto" << "respicio" << "restituo" << "reus" << "reuertor" << "reuoco" <<
"rideo" << "ripa" << "robur" << "rubeo" << "rumpo" << "ruo" << "cacrum" <<
"saeculum" << "sanctus" << "sanus" << "scilicet" << "securus" << "semel" <<
"senectus" << "sepelio" << "sero" << "seuerus" << "sex" << "significo" << "sin"
<< "sino" << "sollicitus" << "solum" << "stultus" << "suadeo" << "subito" <<
"summa" << "superbus" << "supremus" << "surgo" << "suscipio" << "taceo" <<
"tacitus" << "tardus" << "taurus" << "tempero" << "tenebrae" << "terreo" <<
"terror" << "testis" << "tormentum" << "torus" << "tracto" << "transfero" <<
"tribuo" << "triginta" << "truncus" << "trubo" << "turris" << "ultro" << "una"
<< "uniuersus" << "urgeo" << "uro" << "utilitas" << "utrum" << "uaco" <<
"uacuus" << "uagus" << "uallum" << "uastus" << "uates" << "ueho" << "uelox" <<
"uelum" << "uena" << "uer" << "uereor" << "uerso" << "uersus" << "uestigium" <<
"ueto" << "uiginti" << "uinculum" << "uito" << "uiuus";

// frequence IV<< 408 items
QStringList fiiii = QStringList () <<
"acerrime" << "acriter" << "actio" << "adhibeo" << "adiuuo" << "aduersarius" <<
"aequo" << "aestas" << "aether" << "agger" << "aggredior" << "agnosco" << "ala"
<< "ales" << "albus" << "alte" << "altum" << "ambitio" << "ambo" << "ample" <<
"amplector" << "angustiae" << "antea" << "anus" << "apertus" << "arbitrium" <<
"arcus" << "aspectus" << "assiduus" << "assuesco" << "astrum" << "audacia" <<
"audax" << "auus" << "bis" << "blandus" << "boni" << "calidus" << "calor" <<
"cantus" << "carcer" << "carpo" << "castus" << "cauus" << "celebro" << "ceruus"
<< "cingo" << "cinis" << "circumdo" << "circumuenio" << "clades" << "clementia"
<< "coeo" << "cogitatio" << "commeatus" << "commouieo" << "comparo" <<
"comperio" << "complector" << "compleo" << "concipio" << "concutio" <<
"confido" << "confirmo" << "confiteor" << "conscientia" << "conscius" <<
"conscribo" << "consido" << "consuesco" << "contio" << "contraho" <<
"contumelia" << "conuiuium" << "cor" << "corripio" << "cottidie" << "cras" <<
"crinis" << "crudelis" << "cruentus" << "cruor" << "cupido" << "custodia" <<
"decedo" << "decimus" << "dedo" << "demo" << "demonstro" << "demum" << "densus"
<< "deprehendo" << "desiderium" << "diligenter" << "diligo" << "dimico" <<
"dirigo" << "disciplina" << "dissimilis" << "disto" << "doctus" << "domo" <<
"duro" << "eicio" << "en" << "epistula" << "erigo" << "etenim" << "etsi" <<
"euado" << "excedo" << "eximo" << "exitium" << "expeditus" << "expello" <<
"expeto" << "exprimo" << "externus" << "extollo" << "exuo" << "facultas" <<
"fames" << "fas" << "fastigium" << "fatigo" << "faueo" << "ferio" << "ferox" <<
"ferreus" << "ferueo" << "festus" << "fiducia" << "fidus" << "figura" <<
"firmo" << "firmus" << "fletus" << "flos" << "foedus" << "formido" <<
"forsitan" << "fortasse" << "fortiter" << "fossa" << "frequens" << "frux" <<
"fundus" << "furo" << "gelidus" << "geminus" << "gemitus" << "genitor" <<
"haurio" << "heu" << "honestum" << "horridus" << "hospes" << "humilis" <<
"idoneus" << "ignoro" << "ignosco" << "immanis" << "immensus" << "immineo" <<
"immitto" << "improbus" << "incedo" << "incendium" << "includo" << "indicium"
<< "indignus" << "indo" << "induo" << "iners" << "infinitus" << "iniicio" <<
"insanus" << "insignis" << "insum" << "intendo" << "intentus" << "interrogo" <<
"inuado" << "inuisus" << "iucundus" << "lacus" << "laetitia" << "lenis" <<
"leo" << "letum" << "leuo" << "libenter" << "libero" << "libertus" << "linea"
<< "linquo" << "liquidus" << "loco" << "longitudo" << "luctus" << "ludus" <<
"luxuria" << "maiestas" << "mando" << "manifestus" << "medicus" << "medium" <<
"memor" << "mensis" << "merces" << "mereor" << "meritum" << "militia" <<
"minae" << "mitis" << "modicus" << "molestia" << "monstro" << "monstrum" <<
"mulier" << "munio" << "mutuus" << "narro" << "naturalis" << "nauta" <<
"nauigium" << "neglego" << "nempe" << "nepos" << "nequeo" << "nex" << "ni" <<
"nitor" << "nix" << "nobilitas" << "nocturnus" << "nomino" << "nonnullus" <<
"nota" << "noto" << "numquid" << "nympha" << "obruo" << "obscurus" << "abses"
<< "obuius" << "occasio" << "occultus" << "oceanus" << "odor" << "offendo" <<
"oleum" << "omnino" << "oppono" << "orator" << "origo" << "orno" << "osculum"
<< "pabulum" << "palam" << "palus" << "pasco" << "passim" << "paternus" <<
"patientia" << "pauor" << "pauper" << "pelagus" << "penitus" << "perago" <<
"percutio" << "permaneo" << "persequor" << "perterreo" << "pinguis" <<
"placidus" << "plerumque" << "plerusque" << "poculum" << "poeta" << "polliceor"
<< "porrigo" << "porro" << "porto" << "potior" << "potis" << "praefero" <<
"praemitto" << "praesum" << "probus" << "produco" << "proficio" << "proinde" <<
"proicio" << "promptus" << "propinquus" << "propinqui" << "propositum" <<
"prouideo" << "pudeo" << "pudens" << "pudet" << "puppis" << "quatio" <<
"quietus" << "quilibet" << "quiuis" << "radix" << "ramus" << "rapidus" <<
"refert" << "regia" << "regno" << "religio" << "reliqui" << "remoueo" <<
"remus" << "repello" << "reprehendo" << "retro" << "rota" << "rudis" << "ruina"
<< "rumor" << "rupes" << "rus" << "saeuio" << "saeuitia" << "sagitta" <<
"saltus" << "saluus" << "sane" << "sapio" << "scientia" << "scutum" << "seco"
<< "secretus" << "secundum" << "seditio" << "septem" << "seruio" << "seruitus"
<< "siccus" << "silentium" << "simulacrum" << "simulo" << "sitis" << "solacium"
<< "solidus" << "sonitus" << "sonus" << "sordidus" << "spectaculum" << "sperno"
<< "spolium" << "status" << "sterno" << "struo" << "subduco" << "subigo" <<
"subitus" << "subicio" << "submitto" << "subsidium" << "succedo" << "sufficio"
<< "superbia" << "superi" << "tabula" << "tanto" << "ter" << "tero" <<
"thalamus" << "theatrum" << "timidus" << "tolero" << "totiens" << "trabs" <<
"tremo" << "trepido" << "trepidus" << "triumphus" << "tumultus" << "tumulus" <<
"utinam" << "utique" << "uadum" << "ualetudo" << "uallis" << "uanus" << "uapor"
<< "uendo" << "uenia" << "uerbera" << "uersor" << "uertex" << "uexo" << "uicis"
<< "uigilia" << "uilis" << "uilla" << "uincio" << "uindico" << "uiolo" <<
"uiridis" << "uiso" << "uitis" << "uoluo";

/*
bool habet(string liste[]<< int n<< string m)
{
    for (int i = 0; i < n; i++)
       if (liste[i] == m)
           return true;    
    return false;
}

int n[4] = {405<<455<<450<<407};

int frequentia (string requete)
{
    for (int i=0;i<4;i++)
       if (habet (ftab[i]<< n[i]<< requete)) return i;
    return 5;
}
*/

int frequentia (QString requete)
{
    if (fi.contains (requete, Qt::CaseInsensitive)) return 1;
    if (fii.contains (requete, Qt::CaseInsensitive)) return 2;
    if (fiii.contains (requete, Qt::CaseInsensitive)) return 3;
    if (fiiii.contains (requete, Qt::CaseInsensitive)) return 4;
    return 5;    
}

