2019-09-21

		+ LE JEU DE L'ADDITION +

######################################################################
######################################################################

		DESCRIPTION

C'est un jeu où il faut faire des additions.
Deux nombres aléatoires sont donnés et il faut donner leur somme.
Quand la réponse donnée est bonne, on passe au niveau suivant.
Quand la réponse donnée est mauvaise, on recule d'un niveau.
Quand aucune réponse n'est donnée après 15 secondes on recule d'un niveau.
Le plafond des deux nombres aléatoires varie selon le cube du niveau.

Jeu en couleurs.
Fonctionne aussi en plein écran.

Codé en juin 2017.
Déposé sur GitHub en septembre 2019.

######################################################################
######################################################################

		COMMANDES

• 0123...9 : ajouter des nombres à la réponse qu'on veut donner

• Arrière : effacer la réponse
• Entrer : soumettre la réponse
• Escape : quitter

######################################################################
######################################################################

		PRÉREQUIS

Le code est pour macOS, mais ça devrait pouvoir se porter à d'autres OS assez aisément.

Le code est en C++.
Il utilise SDL2 pour l'interface graphique.

Après avoir installé Brew et les Command Line Tools, il faut installer SDL2 et certaines librairies :
	brew install sdl2
	brew install sdl2_image
	brew install sdl2_ttf

######################################################################
######################################################################

		COMPILATION

Le code devrait compiler avec :

g++ $(sdl2-config --cflags) -Wall -o main  main.cpp $(sdl2-config --libs) -lSDL2_ttf -lSDL2_image

######################################################################
######################################################################

Noé Aubin-Cadot, 2019.