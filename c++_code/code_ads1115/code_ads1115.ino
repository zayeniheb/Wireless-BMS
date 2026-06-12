/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier: ads1115_mesure_differentielle.ino
  Description: fichier test de mesure de tensions différentielles
  Page projet : https://passionelectronique.fr/
  Auteur: Jérôme TOMSKI

  Créé le 21.12.2020
*/
#include "ADS1X15.h"

ADS1115 ADS(0x48);  // ADS1115 physiquement défini à l'adresse 0x48, avec sa broche ADDR reliée à la masse

void setup() {
  // Initialisation de port série
  Serial.begin(9600);
  Serial.println("Lecture de tensions différentielles / ADS1115");
  Serial.println("---------------------------------------------");
  Serial.println("Mesure de différence de potentiel entre les entrées A0 et A1 de l'ADC");
  Serial.println("Nota : ADC Range: +/- 6.144V (soit 1 bit = 0.1875 mV)");
  Serial.println("");
  
  // ADS1115
  Wire.begin();
  ADS.begin();         // Initialisation du module ADS1115
  ADS.setGain(0);      // On prend le gain le plus bas (index 0), pour avoir la plus grande plage de mesure (6.144 volt)
  ADS.setMode(1);      // On indique à l'ADC qu'on fera des mesures à la demande, et non en continu (0 = CONTINUOUS, 1 = SINGLE)
  ADS.setDataRate(7);  // On spécifie la vitesse de mesure de tension qu'on souhaite, allant de 0 à 7 (7 étant le plus rapide, soit 860 échantillons par seconde)
  ADS.readADC(0);      // Et on fait une lecture à vide, pour envoyer tous ces paramètres
}

void loop() {
  // Mesure de la tension différentielle entre A0 et A1 (le résultat se situera entre -32768 et +32767, comme nous sommes sur 16 bits)
  int16_t difference_potentiel_A0_A1 = ADS.readADC_Differential_0_1();
  Serial.print("Lecture de l'ADC (A0-A1) : ");
  Serial.println(difference_potentiel_A0_A1);

  // Conversion de cette valeur en volts (le résultat sera fonction de la plage sélectionnée, fonction du gain ; ici, le résultat sera entre -6.144 et +6.144 volts)
  float tension_volts_A0_A1 = ADS.toVoltage(difference_potentiel_A0_A1);
  Serial.print("Tension différentielle = ");
  Serial.print(tension_volts_A0_A1,3);    // On limite l'affichage à 3 chiffres après la virgule
  Serial.println(" volts");

  // Puis on boucle, après une pause de 2 secondes !
  Serial.println("");
  delay(2000);
}
