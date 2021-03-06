#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Commande* commande, Filtre* filtre, Menu* menu, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    commande_(commande), filtre_(filtre), menu_(menu)
{
    ui->setupUi(this);
    setUI();
    connecterSignauxAuxSlots();
    mettreAJourPlatsFiltres();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUI(){
    setWindowTitle(WINDOW_TITLE);
    creerWidgets();
    designLayout();
}

void MainWindow::creerWidgets(){
    creerWidgetTypeMenu();
    creerWidgetConditionsAlimentaires();
    creerWidgetPlatsFiltres();
    creerWidgetCommande();
    creerWidgetBoutonsAjouterRetirer();
    creerWidgetPrix();
    creerWidgetCommander();
}

void MainWindow::creerWidgetTypeMenu() {
    widgetTypeMenu_ = new QComboBox(this);
    widgetTypeMenu_->addItem(MATIN);
    widgetTypeMenu_->addItem(MIDI);
    widgetTypeMenu_->addItem(SOIR);
}

void MainWindow::creerWidgetConditionsAlimentaires(){
    widgetPlatsBios_ =  new QCheckBox(BIOLOGIQUE,this);
    widgetPlatsVeges_ = new QCheckBox(VEGETARIEN, this);
}

void MainWindow::creerWidgetPlatsFiltres(){
    widgetPlatsFiltres_ = new QListWidget(this);
    widgetPlatsFiltres_->setSortingEnabled(true);
}

//TODO //fait

void MainWindow::creerWidgetCommande() {
    widgetCommande_ = new QListWidget(this);
    widgetPlatsFiltres_->setSortingEnabled(true);
}

void MainWindow::creerWidgetBoutonsAjouterRetirer() {
    widgetAjouterPlat_ = new QPushButton(this);
    widgetAjouterPlat_->setText(AJOUTER_PLAT);
    widgetRetirerPlat_ = new QPushButton(this);
    widgetRetirerPlat_->setText(RETIRER_PLAT);
}

void MainWindow::creerWidgetPrix() {
    widgetPrix_ = new QLabel(this);
    widgetPrix_->setText(PRIX_STRING + "0" + DOLLAR_SIGN);
}

//TODO //FAIT
void MainWindow::creerWidgetCommander() {
    widgetCommander_ = new QPushButton(this);
    widgetCommander_->setText(COMMANDER);
}



//TODO //fait
void MainWindow::designLayout() {
   //Layout du Widget type menu et l'option de plats bio ou veges
    QVBoxLayout* boxCoinHautGauche = new QVBoxLayout;
    boxCoinHautGauche->addWidget(widgetTypeMenu_);
    boxCoinHautGauche->addWidget(widgetPlatsBios_);
    boxCoinHautGauche->addWidget(widgetPlatsVeges_);

    //Layout pour les deux bouton ajouter plat et retirer plat
    QHBoxLayout* ajouterRetirerLayout = new QHBoxLayout;
    ajouterRetirerLayout->addWidget(widgetAjouterPlat_);
    ajouterRetirerLayout->addWidget(widgetRetirerPlat_);

    //Layout avec la liste de plats filtres et les boutons ajouter et retirer plat
    QVBoxLayout* boxCoinHautDroite = new QVBoxLayout;
    boxCoinHautDroite->addWidget(widgetPlatsFiltres_);
    boxCoinHautDroite->addLayout(ajouterRetirerLayout);

    //layout de la setion haut du UI
    QHBoxLayout* sectionHautUi = new QHBoxLayout;
    sectionHautUi->addLayout(boxCoinHautGauche);
    sectionHautUi->addLayout(boxCoinHautDroite);

    //Layout pour le text de prix et le bouton commander
    QHBoxLayout* prixEtCommanderLayout = new QHBoxLayout;
    prixEtCommanderLayout->addWidget(widgetPrix_);
    prixEtCommanderLayout->addWidget(widgetCommander_);

    //layout de la setion bas du UI
    QVBoxLayout* sectionBasUI = new QVBoxLayout;
    sectionBasUI->addWidget(widgetCommande_);
    sectionBasUI->addLayout(prixEtCommanderLayout);


    QFrame* hLine = new QFrame();
    hLine->setFrameShape(QFrame::HLine);


    QVBoxLayout* mainBox = new QVBoxLayout(this);
    mainBox->addLayout(sectionHautUi);
    mainBox->addWidget(hLine);
    mainBox->addLayout(sectionBasUI);

    QWidget* widget = new QWidget;
    widget->setLayout(mainBox);
    setCentralWidget(widget);



}

/*
 * Connecte les actions de l'utilisateur sur l'interface aux modifications qui
 * doivent être faites sur le modèle
*/
void MainWindow::connecterSignauxAuxSlots() {
    // connecter checkboxes filtre bio et vege
    QObject::connect(widgetPlatsVeges_, SIGNAL(stateChanged(int)),
                     this, SLOT(clicOptionVege()));
    QObject::connect(widgetPlatsBios_, SIGNAL(stateChanged(int)),
                     this, SLOT(clicOptionBio()));

    //connecter menu defilant type plat
    QObject::connect(widgetTypeMenu_, SIGNAL(currentIndexChanged(QString)),
                     this, SLOT(choixTypeMenu(QString)));

    //TODO // FAIT
    //connecter boutons ajouterPlat et retirerPlat
   QObject::connect(widgetAjouterPlat_, SIGNAL(clicked()), this, SLOT(insererPlatsChoisisDansCommande()));
   QObject::connect(widgetRetirerPlat_, SIGNAL(clicked()), this, SLOT(retirerPlatsChoisisDeCommande()));


    //connecter bouton commander
    QObject::connect(widgetCommander_, SIGNAL(clicked()),
                     this, SLOT(commander()));
}
void MainWindow::mettreAJourPlatsFiltres(){
    QStringList plats = filtre_->getNomPlatsFiltres();
    widgetPlatsFiltres_->clear();
    widgetPlatsFiltres_->addItems(plats);
}

void MainWindow::clicOptionBio(){
    filtre_->clicOptionBio();
}

void MainWindow::clicOptionVege(){
    filtre_->clicOptionVege();
}

void MainWindow::choixTypeMenu(QString typeMenu){
    filtre_->choixTypeMenu(qstringToTypeMenu(typeMenu));
}

void MainWindow::mettreAJourPlatsCommande(){
    widgetCommande_->clear();
    foreach (auto nomPlat, commande_->getCommande()) {
        widgetCommande_->addItem(nomPlat);
    }
}

//TODO //FAIT
void MainWindow::insererPlatsChoisisDansCommande() {
    foreach (auto nomPlat, widgetPlatsFiltres_->selectedItems()) {
        commande_->ajouterPlat(nomPlat->text());
    }
}

//TODO//fait
void MainWindow::retirerPlatsChoisisDeCommande() {
    try{
      foreach (auto nomPlat, widgetPlatsFiltres_->selectedItems()) {
        commande_->retirerPlat(nomPlat->text());
      }
    }catch(ErreurPlatIntrouvable erreur){
            message(erreur.what());
        }

}

void MainWindow::mettreAJourPrix() {

    widgetPrix_->setText(PRIX_STRING +
                         QString::number(calculerPrixCommande()) +
                         DOLLAR_SIGN);
}

int MainWindow::calculerPrixCommande(){
    int prix = 0;
    try {
        foreach(auto nomPlatCommande, commande_->getCommande()){
            prix += menu_->getPlat(nomPlatCommande)->getPrix();
        }
    } catch (ErreurPlatIntrouvable erreur) {
        message(erreur.what());
    }
    return prix;
}

void MainWindow::message(QString erreur) {
    QMessageBox mbox;
    mbox.setText(erreur);
    mbox.exec();
}

void MainWindow::commander() {
    try {
        int prix = calculerPrixCommande();
        commande_->viderCommande();
        message(LABEL_COMMANDE_RECUE+ QString::number(prix) + DOLLAR_SIGN);
    } catch (ErreurCommandeVide erreur) {
        message(erreur.what());
    }
}
