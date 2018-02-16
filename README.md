# MatrixVoice

Pas vraiment de projet, mais commencer à jouer avec cette carte couplée avec un PI2 du tiroir.

La partie mic array étant prévue pour remplacer le kinect utilisé par le bloc Listen de mon Home Butler (aka SARAH @JpEncausse).

LEDS
- prise en main du everloop

MICS
- utilisation du beam channel (qualité d'enregistrement bluffante !)
- snowboy pour la détection d'un mot clé
- algo basé sur l'énergie moyenne pour détecter la fin de la parole
 > étonnamment solution simple mais assez robuste même avec du bruit ambiant voir de la musique (HP diffusant du Hysteria@Muse à même pas 50 cm et niveau sonore quasiment identique à celui de ma voix lors des tests) !
 

Update 2018 02 10

LISTEN - brique remplacant le module Listen C# (ou python) de SARAH v5
- clean du code (basé sur leds & mics)
- ajustement de la détection de fin de phrase
- leds rouges quand le mote clé est détecté
- leds vertes à la fin de la phrase
- curl pour l'envoi du buffer encodé en base 64 vers le coeur de SARAH v5 (node-red)

SPEAK (python) - brique remplacant le module Speak C# de SARAH v5
- utilisation de svox pico tts (offline)
- speak.py pour un petit serveur http écoutant les requêtes GET avec un param speak - chaine envoyée directement à tts.sh
- tts.sh pour encapsuler la ligne de commande pico


Update 2018 02 13

Démarrage du client SARAH complet en C++
- ajout de la base d'un serveur http pour répondre aux requêtes GET /speak/.....


Update 2018 02 16

Client SARAH
- http serveur pour la partie speak
- http client pour envoyer l'enregistrement coté node-red (aka SARAH v5)
- animation minimaliste mais sympa des leds du Matrix Voice sur détection du hotword, pendant la restitution de la synthèse
- nettoyage du code / makefile

