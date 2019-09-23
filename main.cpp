/*
Noé Aubin-Cadot.
Codé en juin 2017.
Déposé sur GitHub en septembre 2019.
C'est un jeu où il faut faire des additions.
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compiler avec :
// g++ $(sdl2-config --cflags) -Wall -o main  main.cpp $(sdl2-config --libs) -lSDL2_ttf -lSDL2_image
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pour C++
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>

// Pour SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
// #include <SDL2_image/SDL_image.h>
// #include <SDL2_ttf/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void ViderTexture(SDL_Renderer* &renderer, SDL_Texture* &texture_textbox)
{
    SDL_SetRenderTarget(renderer, texture_textbox); // on modifie la texture_textbox_input et non plus le renderer
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);  // on vide le renderer (i.e. la texture). Ça remplit de noir car je viens de mettre la couleur de dessin noir (dernière ligne)
    SDL_RenderDrawRect(renderer,NULL);
    SDL_SetRenderTarget(renderer, NULL);// Dorénavent, on modifie à nouveau le renderer et non plus la texture target
}

void AjouterRectangleDansTexture(SDL_Renderer* &renderer, SDL_Texture* &texture_textbox, SDL_Rect rectangle,SDL_Color couleur_texte_box)	// cette fonction n'efface pas ce qui est dans la texture
{
    SDL_SetRenderTarget(renderer, texture_textbox); // on modifie la texture_textbox_input et non plus le renderer
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  // on met texture_textbox_input en mode blend
    SDL_SetRenderDrawColor(renderer,couleur_texte_box.r,couleur_texte_box.g,couleur_texte_box.b,couleur_texte_box.a);    // les 3 premiers param sont la couleur et le dernier param. est l'opacité
    SDL_RenderFillRect(renderer, &rectangle); // on met le rectangle dans texture_textbox_input
    SDL_SetRenderDrawColor(renderer,0,0,0,0);    // les 3 premiers param sont la couleur et le dernier param. est l'opacité
    SDL_SetRenderTarget(renderer, NULL);// Dorénavent, on modifie à nouveau le renderer et non plus la texture target
}

// Cette fonction charge la texture texture_textbox d'un rectangle coloré et d'un texte
void CreerBoutonAvecTexte(SDL_Renderer* &renderer, SDL_Texture* &texture_textbox,std::string Texte, int w, int h,TTF_Font* fonte,SDL_Color couleur_texte,SDL_Color couleur_texte_box)
{
	// On réinitialise le textbox
	ViderTexture(renderer,texture_textbox);

    // On initialise le bouton
    SDL_SetRenderTarget(renderer, texture_textbox); // on modifie la texture_textbox_input et non plus le renderer
    SDL_RenderClear(renderer);  // on vide le renderer (i.e. la texture)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  // on met texture_textbox_input en mode blend
    // On met un rectangle en texture_textbox_input :
    SDL_SetRenderDrawColor(renderer,couleur_texte_box.r,couleur_texte_box.g,couleur_texte_box.b,couleur_texte_box.a);    // les 3 premiers param sont la couleur et le dernier param. est l'opacité
    SDL_RenderFillRect(renderer, NULL); // on met le rectangle dans texture_textbox_input
    SDL_SetRenderDrawColor(renderer,0,0,0,0);    // les 3 premiers param sont la couleur et le dernier param. est l'opacité
    // On met du texte en texture_textbox_input :
    if (Texte != "")
    {
    	SDL_Surface* surface_texte_temporaire = nullptr;
    	SDL_Texture* texture_text_temporaire = nullptr;
        surface_texte_temporaire = TTF_RenderText_Solid(fonte,Texte.c_str(),couleur_texte);
        if (surface_texte_temporaire == NULL){std::cout<<"\n\nERREUR : création de la surface_texte_temporaire a échoué"<<std::endl;} // un message d'erreur si jamais
        SDL_SetSurfaceAlphaMod(surface_texte_temporaire,couleur_texte.a);
        texture_text_temporaire = SDL_CreateTextureFromSurface(renderer, surface_texte_temporaire);
        SDL_RenderCopy(renderer, texture_text_temporaire, NULL, NULL);  
        // On vide la mémoire
    	SDL_FreeSurface(surface_texte_temporaire);
    	surface_texte_temporaire = nullptr;
    	SDL_DestroyTexture(texture_text_temporaire);
    	texture_text_temporaire = nullptr;
    }    
    // On rend le renderer indépendant du texture_textbox_input
    SDL_SetRenderTarget(renderer, NULL);
}

void MettreImageDansTexture(SDL_Renderer* &renderer, SDL_Texture* &texture_image,std::string path_image)
{
    if (path_image!="")
    {
	    SDL_Surface* surface_image_temporaire = IMG_Load(path_image.c_str());	// ajouter un message d'erreur si ça load pas
	    ViderTexture(renderer,texture_image);
	    texture_image = SDL_CreateTextureFromSurface(renderer, surface_image_temporaire);
	    SDL_FreeSurface(surface_image_temporaire);
	    surface_image_temporaire = nullptr;
    }
}

char ToucheVersCharNumOnly(int InputKey)
{
    char Character = '\0';
    switch(InputKey)
    {
        case SDL_SCANCODE_0:
            Character = '0';
            break;
        case SDL_SCANCODE_1:
            Character = '1';
            break;
        case SDL_SCANCODE_2:
            Character = '2';
            break;
        case SDL_SCANCODE_3:
            Character = '3';
            break;
        case SDL_SCANCODE_4:
            Character = '4';
            break;
        case SDL_SCANCODE_5:
            Character = '5';
            break;
        case SDL_SCANCODE_6:
            Character = '6';
            break;
        case SDL_SCANCODE_7:
            Character = '7';
            break;
        case SDL_SCANCODE_8:
            Character = '8';
            break;
        case SDL_SCANCODE_9:
            Character = '9';
            break;
    }
    return Character;
}

int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////       Initialisation de diverses choses      //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // INITIALISATION SDL2
    SDL_Init(SDL_INIT_VIDEO);
    // INITIALISATION SDL2_ttf
    TTF_Init();
    if(TTF_Init() != 0){std::cout<<"Erreur d'initialisation de TTF_Init :\n"<<TTF_GetError();exit(EXIT_FAILURE);}
    // CRÉATION D'UNE FENÊTRE
    SDL_Window *window;
    window = SDL_CreateWindow("Fenêtre SDL",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,500,SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    // Si la fenêtre n'a pas été créée on fait un message d'erreur
    if (window == NULL){std::cout<<"La création de la fenêtre SDL a échoué : "<<SDL_GetError()<<"\n";return 1;}
    // ON PREND LA TAILLE DE L'ÉCRAN
    SDL_DisplayMode current_display_mode;
    // message d'erreur si SDL échoue
    if (SDL_GetDesktopDisplayMode(0, &current_display_mode) != 0){SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());return 1;}
    // CRÉATION D'UN RENDERER
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	// Création de deux integers qui ont pour valeur le nombre de pixels en largeur et en hauteur de la fenêtre
    int gl_w=1; // on met des valeurs inutiles qui sont remplacées juste après
    int gl_h=1; // on met des valeurs inutiles qui sont remplacées juste après
    SDL_GL_GetDrawableSize(window,&gl_w,&gl_h); // ici on met les valeurs en gl_w et en gl_h
    // remarque : je dois mettre à jour gl_w et gl_h si la fenêtre est redimensionnée

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////       Création de textures etc.      //////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // DÉCLARATION D'UNE FONTE (choix de fonte + taille en pixels)
    // On doit télécharger et mettre les fontes dans le directory du main.cpp.
    std::string fonte_string = "fontes/josefin-sans/JosefinSans-Regular.ttf";
    TTF_Font* fonte = TTF_OpenFont(fonte_string.c_str(), 100); // avec 100 c'est pixelisé vintage mais c'est super rapide et le processeur roule à 11% au lieu de 99%
    // On vérifie si la fonte a été chargée
    if(fonte == NULL){std::cout<<"\n\nERREUR : fonte non chargée\n"<<std::endl;return 1;}
    // remarque : ici je prend la taille de la fonte = gl_w. C'est énorme. Possibilité de rendre ça dynamique pour accélérer le logiciel (ou encore déclarer une fonte par texture)

    // On crée une boîte pour le texte en haut
    SDL_Rect position_textbox_titre = {gl_w/4.0,gl_h/16.0,gl_w/2.0,gl_h/16.0}; 
    SDL_Color couleur_texte_titre = {0,255,255,100};
    SDL_Color couleur_texte_box_titre = {0,100,100,150};
    std::string Texte_titre = "Le jeu de l'addition";

    // Pour les nombres aléatoires
    int level = 1;
    srand ( time(NULL) );
    int x_max = level;
    int y_max = level;
    int x = rand() % x_max; std::string x_string = std::to_string(x); // deux nombres entiers qui seront dans la question
    int y = rand() % y_max; std::string y_string = std::to_string(y);
    int reponse_voulue = x+y;
    int reponse_donnee;

    // On crée une boîte pour les deux nombres
    SDL_Rect position_textbox_nombres = {gl_w/4.0,gl_h*3.0/16.0,gl_w/2.0,gl_h/16.0}; 
    SDL_Color couleur_texte_nombres = {0,255,0,100};
    SDL_Color couleur_texte_box_nombres = {0,100,0,150};
    std::string Texte_nombres = x_string + " + " + y_string + " = ?";

    // On crée le bouton d'input
    SDL_Rect position_textbox_input = {gl_w/4.0,gl_h*5.0/16.0,gl_w/2.0,gl_h*7.0/16.0}; 
    SDL_Color couleur_texte_input = {255,255,0,100};
    SDL_Color couleur_texte_box_input = {100,0,0,150};
    std::string Texte_input = "";

    // On crée une boîte pour dire si la réponse est bonne ou mauvaise
    SDL_Rect position_textbox_reponse = {gl_w/4.0,gl_h*13.0/16.0,gl_w/2.0,gl_h*2.0/16.0}; 
    SDL_Color couleur_texte_reponse = {0,255,0,100};
    SDL_Color couleur_texte_box_reponse = {0,0,255,100};
    std::string Texte_reponse = "";

    // On crée quatre boîtes qui indique le temps qui reste (les 4 barres autour de l'écran)
    SDL_Rect position_textbox_BarreDeVie_haut = {0,gl_h/48.0,gl_w,gl_h/48}; 
    SDL_Rect position_textbox_BarreDeVie_bas = {0,gl_h - gl_h*2.0/48.0,gl_w,gl_h/48}; 
    SDL_Rect position_textbox_BarreDeVie_gauche = {gl_w/48.0,0,gl_w/48,gl_h};
    SDL_Rect position_textbox_BarreDeVie_droite = {gl_w - gl_w*2.0/48.0,0,gl_w/48,gl_h};
    SDL_Color couleur_texte_box_BarreDeVie = {0,255,0,100};

    // On crée une boîte qui dit le level auquel on est rendu
    SDL_Rect position_textbox_level = {gl_w/12.0,gl_h/12.0,gl_w/12.0,gl_h/12.0}; 
    SDL_Color couleur_texte_level = {100,100,100,200};
    SDL_Color couleur_texte_box_level = {100,100,100,100};
    std::string Texte_level = "Lvl. ";
    Texte_level = Texte_level + std::to_string(level);

    // On crée une boîte qui dit le temps qu'il reste
    SDL_Rect position_textbox_temps = {gl_w - gl_w*2.0/12.0,gl_h - gl_h*2.0/12.0,gl_w/12.0,gl_h/12.0}; 
    SDL_Color couleur_texte_temps = {100,100,100,200};
    SDL_Color couleur_texte_box_temps = {100,100,100,100};
    std::string Texte_temps = "";

    // Pour l'image
    std::string texte_image = "";

    // CRÉATION DE TEXTURES
    SDL_Texture* texture_textbox_titre = NULL;      // un premier texte permanent
    SDL_Texture* texture_textbox_nombres = NULL;    // c'est la texture où on montre deux nombres
    SDL_Texture* texture_textbox_input = NULL;	    // c'est la texture où on écrit du texte
    SDL_Texture* texture_textbox_reponse = NULL;    // c'est la texture où on dit si la réponse est vraie ou fausse
    SDL_Texture* texture_textbox_BarreDeVie = NULL; // c'est la texture où on met la vie qui reste    
    SDL_Texture* texture_textbox_level = NULL;      // c'est la texture où on met le level 
    SDL_Texture* texture_textbox_temps = NULL;      // c'est la texture où on met le temps restant  
    // CRÉATION D'UNE TEXTURE POUR LES IMAGES
    SDL_Texture* texture_image_win = NULL;          // c'est la texture où on met une image quand on gagne   
    SDL_Texture* texture_image_fail = NULL;         // c'est la texture où on met une image quand on perd
    SDL_Texture* texture_image_temps = NULL;        // c'est la texture où on met une image quand le temps achève

    // INITIALISATION DES TEXTURES BOÎTES AVEC OU SANS TEXTE
    texture_textbox_titre = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_titre.w,position_textbox_titre.h);
    texture_textbox_nombres = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_nombres.w,position_textbox_nombres.h);
    texture_textbox_input = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_input.w,position_textbox_input.h);
    texture_textbox_reponse = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_reponse.w,position_textbox_reponse.h);
    texture_textbox_BarreDeVie = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,gl_w,gl_h);
    texture_textbox_level = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_level.w,position_textbox_level.h);
    texture_textbox_temps = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_temps.w,position_textbox_temps.h);
    // INITIALISATION DE LA TEXTURE D'IMAGE
	MettreImageDansTexture(renderer,texture_image_win,"images/win.png");
	MettreImageDansTexture(renderer,texture_image_fail,"images/fail.png");
	MettreImageDansTexture(renderer,texture_image_temps,"images/temps.png");

    char Texte_touche = '\0';

    int integer_red = 0;
    int integer_green = 0;
    int integer_w = 0;
    int integer_h = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////       Déclaration de variables utiles      //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Déclaration de variables utiles dans la boucle

    int currentTime;
    int lastTimeBoutonCree = 0;

    bool win = false;
    bool fail = false;
    bool temps_acheve = false;
    bool image_active = false;

    bool affichage_images = false; // pour avoir l'image d'une face quand on a une bonne ou mauvaises réponse.

    int delay = 16;
	double temps_restant_max_default = 15000; // 10 000 ms = 15 secondes
	double temps_restant_max = temps_restant_max_default;
	double temps_restant = temps_restant_max;
	double temps_restant_fraction1; 
	double temps_restant_fraction2; 
	double temps_image = 500.0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////       Début de la boucle principale      //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // On commence les événements
    // Pour plus de détails : voir SDL_PollEvent()
    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        SDL_Delay(delay);
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) // si on change la taille de la fenêtre
                {
                    // On met les nouvelles dimension de la fenêtre en gl_w et gl_h
                    SDL_GL_GetDrawableSize(window,&gl_w,&gl_h);
                    // On recalcule les dimensions des boîtes
                    position_textbox_titre = (SDL_Rect){gl_w/4.0,gl_h/16.0,gl_w/2.0,gl_h/16.0}; 
                    position_textbox_nombres = (SDL_Rect){gl_w/4.0,gl_h*3.0/16.0,gl_w/2.0,gl_h/16.0}; 
                    position_textbox_input = (SDL_Rect){gl_w/4.0,gl_h*5.0/16.0,gl_w/2.0,gl_h*7.0/16.0}; 
                    position_textbox_reponse = (SDL_Rect){gl_w/4.0,gl_h*13.0/16.0,gl_w/2.0,gl_h*2.0/16.0}; 
                    position_textbox_BarreDeVie_haut = (SDL_Rect){0,gl_h/48.0,gl_w,gl_h/48};
                    position_textbox_BarreDeVie_bas = (SDL_Rect){0,gl_h - gl_h*4.0/96.0,gl_w,gl_h/48};
                    position_textbox_BarreDeVie_gauche = (SDL_Rect){gl_w/48.0,0,gl_w/48,gl_h};
                    position_textbox_BarreDeVie_droite = (SDL_Rect){gl_w - gl_w*2.0/48.0,0,gl_w/48,gl_h};      				
                    position_textbox_level = (SDL_Rect){gl_w/12.0,gl_w/12.0,gl_w/12.0,gl_w/12.0}; 
                    position_textbox_temps = (SDL_Rect) {gl_w - gl_w*2.0/12.0,gl_h - gl_h*2.0/12.0,gl_w/12.0,gl_h/12.0};
                    // on détruit les textures
                    SDL_DestroyTexture(texture_textbox_titre);
    				SDL_DestroyTexture(texture_textbox_nombres);
    				SDL_DestroyTexture(texture_textbox_input);
    				SDL_DestroyTexture(texture_textbox_reponse);
    				SDL_DestroyTexture(texture_textbox_BarreDeVie);
    				SDL_DestroyTexture(texture_textbox_level);
    				SDL_DestroyTexture(texture_textbox_temps);
    				// on recrée les textures
    				texture_textbox_titre = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_titre.w,position_textbox_titre.h);
    				texture_textbox_nombres = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_nombres.w,position_textbox_nombres.h);
    				texture_textbox_input = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_input.w,position_textbox_input.h);
    				texture_textbox_reponse = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_reponse.w,position_textbox_reponse.h);
    				texture_textbox_BarreDeVie = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,gl_w,gl_h);
    				texture_textbox_level = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_level.w,position_textbox_level.h);
    				texture_textbox_temps = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,position_textbox_temps.w,position_textbox_temps.h);   				
                }
            }

            if (event.type == SDL_KEYDOWN)	// si on enfonce une touche
            {   
                if (event.key.keysym.sym == SDLK_ESCAPE)	// si on enfonce ESCAPE ça quitte
                {
                    quit = true;
                }
                Texte_touche = '\0';
                Texte_touche = ToucheVersCharNumOnly(event.key.keysym.scancode);
                if (Texte_touche != '\0')
                {
                    Texte_input = Texte_input + Texte_touche;
                    CreerBoutonAvecTexte(renderer,texture_textbox_input,Texte_input,position_textbox_input.w,position_textbox_input.h,fonte,couleur_texte_input,couleur_texte_box_input);
                }
                
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) // Si on fait backspace ça efface notre texte
                {
                    Texte_input = "";
                    CreerBoutonAvecTexte(renderer,texture_textbox_input,Texte_input,position_textbox_input.w,position_textbox_input.h,fonte,couleur_texte_input,couleur_texte_box_input);
                }
                reponse_donnee = atoi(Texte_input.c_str());
                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {
                    if (reponse_donnee == reponse_voulue)
                    {
                        Texte_reponse = "VRAI"; 
                        couleur_texte_reponse = (SDL_Color){0,255,0,250};
                        Texte_input = "";
                        level = level+1;
                        Texte_level = "Lvl. ";
    					Texte_level = Texte_level + std::to_string(level);
    					
    					win = true;
    					fail = false;
    					temps_acheve = false;
    					temps_restant_max = temps_restant_max_default;
    					temps_restant = temps_restant_max;
                    }
                    if (reponse_donnee != reponse_voulue)
                    {
                        Texte_reponse = "FAUX"; 
                        couleur_texte_reponse = (SDL_Color){255,0,0,250};
                        Texte_input = "";
                        if (level>1)
                        {
                        	level = level - 1;
                        }
                        Texte_level = "Lvl. ";
    					Texte_level = Texte_level + std::to_string(level);

    					win = false;
    					fail = true;
    					temps_acheve = false;
    					temps_restant = temps_restant_max;
                    }
                    x_max = level*level*level;
            		y_max = level*level*level;
                    x = rand() % x_max + 1; x_string = std::to_string(x); // deux nombres entiers qui seront dans la question
                    y = rand() % y_max + 1; y_string = std::to_string(y);
                    reponse_voulue = x+y;
                    Texte_nombres = x_string + " + " + y_string + " = ?";
                    CreerBoutonAvecTexte(renderer,texture_textbox_nombres,Texte_nombres,position_textbox_nombres.w,position_textbox_nombres.h,fonte,couleur_texte_nombres,couleur_texte_box_nombres);
                    CreerBoutonAvecTexte(renderer,texture_textbox_input,Texte_input,position_textbox_input.w,position_textbox_input.h,fonte,couleur_texte_input,couleur_texte_box_input);
                    CreerBoutonAvecTexte(renderer,texture_textbox_reponse,Texte_reponse,position_textbox_reponse.w,position_textbox_reponse.h,fonte,couleur_texte_reponse,couleur_texte_box_reponse);
                    ViderTexture(renderer,texture_textbox_BarreDeVie);
                    AjouterRectangleDansTexture(renderer,texture_textbox_BarreDeVie, position_textbox_BarreDeVie_haut, couleur_texte_box_BarreDeVie);
                    AjouterRectangleDansTexture(renderer,texture_textbox_BarreDeVie, position_textbox_BarreDeVie_bas, couleur_texte_box_BarreDeVie);
                    AjouterRectangleDansTexture(renderer,texture_textbox_BarreDeVie, position_textbox_BarreDeVie_gauche, couleur_texte_box_BarreDeVie);
                    AjouterRectangleDansTexture(renderer,texture_textbox_BarreDeVie, position_textbox_BarreDeVie_droite, couleur_texte_box_BarreDeVie);
                    CreerBoutonAvecTexte(renderer,texture_textbox_level,Texte_level,position_textbox_level.w,position_textbox_level.h,fonte,couleur_texte_level,couleur_texte_box_level);
                    CreerBoutonAvecTexte(renderer,texture_textbox_temps,Texte_temps,position_textbox_temps.w,position_textbox_temps.h,fonte,couleur_texte_temps,couleur_texte_box_temps);
                }
            }
        }

        temps_restant = temps_restant - delay;
        if (!(temps_restant > 0.0))	// s'il ne reste plus de temps
        {

			temps_restant_max = temps_restant_max_default;
			if (level > 1)
			{
				level = level - 1;
			}

        	Texte_temps = "0";
        	temps_restant = temps_restant_max;	// on remet le temps restant au temps restant maximal
        	x_max = level*level*level;
        	y_max = level*level*level;
        	couleur_texte_box_BarreDeVie.r = 0;
        	couleur_texte_box_BarreDeVie.g = 255;
        	position_textbox_BarreDeVie_haut.w = gl_w;
        	position_textbox_BarreDeVie_bas.w = gl_w;
        	position_textbox_BarreDeVie_bas.x = 0;
        	position_textbox_BarreDeVie_gauche.y = 0;
        	position_textbox_BarreDeVie_gauche.h = gl_h;
        	position_textbox_BarreDeVie_droite.h = gl_h;
        	x = rand() % x_max + 1; x_string = std::to_string(x);
            y = rand() % y_max + 1; y_string = std::to_string(y);
            reponse_voulue = x+y;
            Texte_nombres = x_string + " + " + y_string + " = ?";
            Texte_level = "Lvl. ";
    		Texte_level = Texte_level + std::to_string(level);
    		Texte_reponse = "RECOMMENCEZ";
    		couleur_texte_reponse = (SDL_Color){255,255,0,200};
    		win = false;
    		fail = false;
    		temps_acheve = true;
        }
        else	// s'il reste du temps
        {
        	temps_restant_fraction1 = (temps_restant_max - temps_restant)/temps_restant_max;
        	integer_red = (int)( temps_restant_fraction1 * 255.0 );
        	if (integer_red>0)
        	{
        		couleur_texte_box_BarreDeVie.r = rand() % integer_red;
        	}

        	temps_restant_fraction2 = temps_restant/temps_restant_max;
        	integer_green = (int)( temps_restant_fraction2 * 255.0 );
        	if (integer_green>0)
        	{
        		couleur_texte_box_BarreDeVie.g = rand() % integer_green;
        	}
        	integer_w = (int)( temps_restant_fraction2 * ((double)gl_w) );
        	integer_h = (int)( temps_restant_fraction2 * ((double)gl_h) );
        	if (integer_w > 0)
        	{
        		position_textbox_BarreDeVie_haut.w = integer_w;
        		position_textbox_BarreDeVie_bas.w = integer_w;
        		position_textbox_BarreDeVie_bas.x = gl_w - integer_w;
        	}
        	if (integer_h > 0)
        	{
        		position_textbox_BarreDeVie_gauche.h = integer_h;
        		position_textbox_BarreDeVie_gauche.y = gl_h - integer_h;
        		position_textbox_BarreDeVie_droite.h = integer_h;
        	}
        	Texte_temps = std::to_string((int)(temps_restant/1000));
       	}

    	if ((temps_restant>(temps_restant_max-temps_image)) && (win == true)) // si on vient de gagner
    	{
    		fail = false;
    		temps_acheve = false;
    	}
    	if ((temps_restant>(temps_restant_max-temps_image)) && (fail == true)) // si on vient de perdre
    	{
    		win = false;
    		temps_acheve = false;
    	}
    	if (temps_restant<temps_image)	// s'il ne reste presque plus de temps
    	{
    		win = false;
    		fail = false;
    		temps_acheve = true;
    	}
    	if ((temps_restant<(temps_restant_max-temps_image)) && (temps_restant>temps_image))	// période où il n'y a pas d'image
    	{
    		win = false;
    		fail = false;
    		temps_acheve = false;
    		image_active = false;
    	}

        // On recréé les textures à chaque 100 ms
        currentTime = SDL_GetTicks(); // on prend l'heure pour le taux d'affichage
        if(currentTime > lastTimeBoutonCree + 100) // pause avant l'affichage en millisecondes (16 = optimal car écran à 60Hz, sinon 100 donne truc intéressant)
        {
            // On fait évoluer aléatoirement la couleur et la position de certains objets
            // titre
            couleur_texte_titre = (SDL_Color){0,rand() % 256,0,100};
            // nombres
            position_textbox_nombres = (SDL_Rect){gl_w/4.0 + rand() % 20-10,gl_h*3.0/16.0,gl_w/2.0,gl_h/16.0}; 
            couleur_texte_nombres = (SDL_Color){rand() % 256,rand() % 256,rand() % 256,rand() % 256};
            // input
            position_textbox_input = (SDL_Rect){gl_w/4.0+pow(rand()%level,2),gl_h*5.0/16.0+pow(rand()%level,2),gl_w/2.0+pow(rand()%level,2),gl_h*7.0/16.0+pow(rand()%level,2)}; 
            couleur_texte_input.a = rand() % 256;
            couleur_texte_box_input.a = 150 + (rand() % 20-10);
            // réponse
            position_textbox_reponse = (SDL_Rect){gl_w/4.0 + (rand() % 20-10),gl_h*13.0/16.0 + (rand() % 20-10),gl_w/2.0 + (rand() % 20-10),gl_h*2.0/16.0 + (rand() % 20-10)}; 
            couleur_texte_reponse.a = rand() % 256;
            // level
            position_textbox_level = (SDL_Rect){gl_w/12.0 + pow(rand()%level,2),gl_h/12.0+ pow(rand()%level,2),gl_w/12.0+ pow(rand()%level,2),gl_w/12.0+ pow(rand()%level,2)};
            // temps
            position_textbox_temps = (SDL_Rect){gl_w - gl_w*2.0/12.0 - pow(rand()%level,2),gl_h - gl_h*2.0/12.0 - pow(rand()%level,2),gl_w/12.0 + pow(rand()%level,2),gl_h/12.0 + pow(rand()%level,2)}; 

            // On crée les textures
            CreerBoutonAvecTexte(renderer,texture_textbox_titre,Texte_titre,position_textbox_titre.w,position_textbox_titre.h,fonte,couleur_texte_titre,couleur_texte_box_titre);
            CreerBoutonAvecTexte(renderer,texture_textbox_nombres,Texte_nombres,position_textbox_nombres.w,position_textbox_nombres.h,fonte,couleur_texte_nombres,couleur_texte_box_nombres);
            CreerBoutonAvecTexte(renderer,texture_textbox_input,Texte_input,position_textbox_input.w,position_textbox_input.h,fonte,couleur_texte_input,couleur_texte_box_input);
            CreerBoutonAvecTexte(renderer,texture_textbox_reponse,Texte_reponse,position_textbox_reponse.w,position_textbox_reponse.h,fonte,couleur_texte_reponse,couleur_texte_box_reponse);
            CreerBoutonAvecTexte(renderer,texture_textbox_level,Texte_level,position_textbox_level.w,position_textbox_level.h,fonte,couleur_texte_level,couleur_texte_box_level);
            CreerBoutonAvecTexte(renderer,texture_textbox_temps,Texte_temps,position_textbox_temps.w,position_textbox_temps.h,fonte,couleur_texte_temps,couleur_texte_box_temps);
            ViderTexture(renderer,texture_textbox_BarreDeVie);
            AjouterRectangleDansTexture(renderer,texture_textbox_BarreDeVie, position_textbox_BarreDeVie_haut, couleur_texte_box_BarreDeVie);
            AjouterRectangleDansTexture(renderer,texture_textbox_BarreDeVie, position_textbox_BarreDeVie_bas, couleur_texte_box_BarreDeVie);
            AjouterRectangleDansTexture(renderer,texture_textbox_BarreDeVie, position_textbox_BarreDeVie_gauche, couleur_texte_box_BarreDeVie);
            AjouterRectangleDansTexture(renderer,texture_textbox_BarreDeVie, position_textbox_BarreDeVie_droite, couleur_texte_box_BarreDeVie);
            lastTimeBoutonCree = currentTime;
        }

        //std::cout<<"\nVrai = "<<win<<",\nFaux = "<<fail<<",\ntemps_acheve = "<<temps_acheve<<"\nimage_active = "<<image_active<<std::endl;
        //std::cout<<"Horloge : "<<currentTime<<std::endl;
        
        // On met les textures dans le renderer
        SDL_RenderClear(renderer);  // on claire le renderer
        SDL_SetRenderDrawColor(renderer,0,0,0,0);    // on remet la couleur du renderer à noir
        SDL_RenderCopy(renderer, texture_textbox_BarreDeVie, NULL, NULL); 	// je met la barre de vie en premier car c'est une texture grosse comme la fenêtre et c'est plein de noir (trouver comment mettre ça transparent)
        SDL_RenderCopy(renderer, texture_textbox_titre, NULL, &position_textbox_titre);
        SDL_RenderCopy(renderer, texture_textbox_nombres, NULL, &position_textbox_nombres);
        SDL_RenderCopy(renderer, texture_textbox_input, NULL, &position_textbox_input);
        SDL_RenderCopy(renderer, texture_textbox_reponse, NULL, &position_textbox_reponse);
        SDL_RenderCopy(renderer, texture_textbox_level, NULL, &position_textbox_level);
        SDL_RenderCopy(renderer, texture_textbox_temps, NULL, &position_textbox_temps);
        if (((win == true)||(fail == true)||(temps_acheve == true)) && affichage_images)
        {
        	if (win == true)
        	{
        		SDL_RenderCopy(renderer, texture_image_win, NULL, NULL);
        	}
        	if (fail == true)
        	{
        		SDL_RenderCopy(renderer, texture_image_fail, NULL, NULL);
        	}
        	if (temps_acheve == true)
        	{
        		SDL_RenderCopy(renderer, texture_image_temps, NULL, NULL);
        	}
        }
        SDL_RenderPresent(renderer); // on affiche le contenu du renderer à l'écran
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////       On termine le programme      ///////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Bye bye
    std::cout<<"\n\nAu revoir !\n\n";
    // On termine le programme (vider la mémoire vive utilisée)
    SDL_DestroyRenderer(renderer); // on détruit le renderer_background
    renderer = NULL;
    SDL_DestroyWindow(window);     // on ferme la fenêtre
    window = NULL;
    SDL_DestroyTexture(texture_textbox_titre);      // on détruit la texture_textbox_titre
    SDL_DestroyTexture(texture_textbox_nombres);    // on détruit la texture_textbox_nombres
    SDL_DestroyTexture(texture_textbox_input);      // on détruit la texture_textbox_input
    SDL_DestroyTexture(texture_textbox_reponse);    // on détruit la texture_textbox_reponse
    SDL_DestroyTexture(texture_textbox_BarreDeVie); // on détruit la texture_textbox_BarreDeVie
    SDL_DestroyTexture(texture_textbox_level);      // on détruit la texture_textbox_level
    SDL_DestroyTexture(texture_textbox_temps);      // on détruit la texture_textbox_level
    SDL_DestroyTexture(texture_image_win);          // on détruit la texture_image_win
    SDL_DestroyTexture(texture_image_fail);         // on détruit la texture_image_fail
    SDL_DestroyTexture(texture_image_temps);        // on détruit la texture_image_temps

    TTF_CloseFont(fonte); // on vide la mémoire de la fonte
    fonte=NULL;           // on vide la mémoire de la fonte
    SDL_Quit();           // on quitte SDL
    TTF_Quit();           // on quitte TTF
    return 0;
}