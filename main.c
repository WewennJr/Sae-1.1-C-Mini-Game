/* ----- Importation --------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "board.h"

/* --------------------------------------------------------------------------------------------- */










/* ----- Affichage ----------------------------------------------------------------------------- */

/* ----- Color ----- */
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define DIM "\033[2m"

// Text colors
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Background colors
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"
#define BG_GRAY "\033[100m"

// Bright colors
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"


/* ----- Function to display the board ----- */
void show_plato(board game) {
    printf("\n");
    
    // Header
    printf("     %s%s╔═══════════════════════════╗%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("     %s%s║           NORTH           ║%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("     %s%s╚═══════════════════════════╝%s\n", BOLD, BRIGHT_CYAN, RESET);
    
    // Top border of the board
    printf("      %s╔═══╤═══╤═══╤═══╤═══╤═══╗%s\n", CYAN, RESET);
    
    // Display from row 5 to 0 (North at the top, South at the bottom)
    for (int i = DIMENSION - 1; i >= 0; i--) {
        printf("   %s%s%d%s  %s║%s", BOLD, YELLOW, i+1, RESET, CYAN, RESET);
        
        for (int j = 0; j < DIMENSION; j++) {
            size p = get_piece_size(game, i, j);
            
            // Color based on the size of the piece
            char* color;
            char* bg_color = "";
            char symbol[10];
            
            switch (p) {
                case NONE:
                    color = DIM;
                    sprintf(symbol, " · ");
                    break;
                case ONE:
                    color = BRIGHT_GREEN;
                    sprintf(symbol, " 1 ");
                    break;
                case TWO:
                    color = BRIGHT_YELLOW;
                    sprintf(symbol, " 2 ");
                    break;
                case THREE:
                    color = BRIGHT_RED;
                    sprintf(symbol, " 3 ");
                    break;
                default:
                    color = WHITE;
                    sprintf(symbol, " ? ");
                    break;
            }

            if (picked_piece_line(game) == i && picked_piece_column(game) == j) {
                bg_color = BG_MAGENTA;
                printf("%s%s%s%s%s", bg_color, BOLD, WHITE, symbol, RESET);
            } else {
                printf("%s%s%s%s", BOLD, color, symbol, RESET);
            }
            
            if (j < DIMENSION - 1) {
                printf("%s│%s", CYAN, RESET);
            }
        }
        
        printf("%s║%s\n", CYAN, RESET);
        
        // Separator line between rows
        if (i > 0) {
            printf("      %s╟───┼───┼───┼───┼───┼───╢%s\n", CYAN, RESET);
        }
    }
    
    // Bottom border of the board
    printf("      %s╚═══╧═══╧═══╧═══╧═══╧═══╝%s\n", CYAN, RESET);
    printf("        %s%s1   2   3   4   5   6%s\n", BOLD, YELLOW, RESET);
    
    // Footer
    printf("     %s%s╔═══════════════════════════╗%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("     %s%s║           SOUTH           ║%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("     %s%s╚═══════════════════════════╝%s\n", BOLD, BRIGHT_CYAN, RESET);
    
    printf("\n");
}

/* ----- Function to display the game state ----- */

void show_game_state(board game, player current) {
    printf("\n%s════════════════════════════════════════%s\n", BOLD, RESET);
    
    // Display the current player
    if (current == SOUTH_P) {
        printf("  🎮 Tour du joueur : %s%s %s%s%s%s\n", BOLD, BRIGHT_MAGENTA, BOLD, "SOUTH", BRIGHT_MAGENTA, RESET);
    } else {
        printf("  🎮 Tour du joueur : %s%s %s%s%s%s\n", BOLD, BRIGHT_CYAN, BOLD, "NORTH", BRIGHT_CYAN, RESET);
    }
    
    // If a piece is in motion
    if (picked_piece_owner(game) != NO_PLAYER) {
        printf("  📍 Pièce en mouvement : ");
        
        size piece_size = picked_piece_size(game);
        char* color;
        char* symbol;
        
        switch(piece_size) {
            case ONE:
                color = BRIGHT_GREEN;
                symbol = "1";
                break;
            case TWO:
                color = BRIGHT_YELLOW;
                symbol = "2";
                break;
            case THREE:
                color = BRIGHT_RED;
                symbol = "3";
                break;
            default:
                color = WHITE;
                symbol = "?";
                break;
        }
        
        printf("%s%s%s%s", BOLD, color, symbol, RESET);
        printf(" à [%d,%d]\n", picked_piece_line(game), picked_piece_column(game));
        printf("  ⚡ Mouvements restants : %s%s%d%s\n", BOLD, YELLOW, movement_left(game), RESET);
    }
    
    printf("%s════════════════════════════════════════%s\n\n", BOLD, RESET);
}


/* ----- Function to display the win ----- */

void show_winner(player winner) {
    printf("\n\n");
    printf("%s╔═══════════════════════════════════════════╗%s\n", BOLD, RESET);
    printf("%s║                                           ║%s\n", BOLD, RESET);
    if (winner == SOUTH_P) {
        printf("║%s  🎉🎉  LE JOUEUR SUD A GAGNÉ ! 🎉🎉      %s║\n", BRIGHT_CYAN, RESET);
    } else {
        printf("║%s  🎉🎉  LE JOUEUR NORTH A GAGNÉ ! 🎉🎉     %s║\n", BRIGHT_CYAN, RESET);
    }
    printf("%s║                                           ║%s\n", BOLD, RESET);
    printf("%s╚═══════════════════════════════════════════╝%s\n", BOLD, RESET);
    printf("\n\n");
}

/* ----- Function for clear the screen ----- */
void clear_screen() {
    printf("\033[2J\033[H"); // ANSI escape codes to clear screen and move cursor to home position
}


/* --------------------------------------------------------------------------------------------- */










/* ----- Init and End ----------------------------------------------------------------- */

/* ----- Init function and End ----- */

board init_game() {
    clear_screen();

    // Welcome panel
    printf("\n\n");
    printf("%s%s", BOLD, BRIGHT_CYAN);
    printf("  ╔═══════════════════════════════╗\n");
    printf("  ║                               ║\n");
    printf("  ║    BIENVENUE DANS LE JEU !    ║\n");
    printf("  ║                               ║\n");
    printf("  ╚═══════════════════════════════╝\n");
    printf("%s\n", RESET);
    
    printf("%s  Un jeu de stratégie a 2 joueurs%s\n", YELLOW, RESET);

    // Fake loading because why not
    printf("  %sChargement du plateau ", WHITE);
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
        usleep(300000); 
        printf(".");
        fflush(stdout);
    }
    printf("%s\n\n", RESET);

    board game = new_game();
    
    printf("%s  Plateau pret !%s\n\n", BRIGHT_GREEN, RESET);
    sleep(1);

    // Display the rules
    clear_screen();
    printf("\n\n");
    printf("%s╔═══════════════════ RÈGLES RAPIDES ═══════════════════╗%s\n", CYAN, RESET);
    printf("%s║%s  - Chaque joueur place 6 pieces (2 de chaque taille) %s║%s\n", CYAN, RESET, CYAN, RESET);
    printf("%s║%s  - %s1%s se déplace de 1 cases                           %s║%s\n", CYAN, RESET, BRIGHT_GREEN, RESET, CYAN, RESET);
    printf("%s║%s  - %s2%s se déplace de 2 cases                           %s║%s\n", CYAN, RESET, BRIGHT_YELLOW, RESET, CYAN, RESET);
    printf("%s║%s  - %s3%s se déplace de 3 cases                           %s║%s\n", CYAN, RESET, BRIGHT_RED, RESET, CYAN, RESET);
    printf("%s║%s  - Objectif : Atteindre le but adverse !             %s║%s\n", CYAN, RESET, CYAN, RESET);
    printf("%s╚══════════════════════════════════════════════════════╝%s\n", CYAN, RESET);
    printf("\n\n");
    // Display the legend
    printf("%s        ╔══════════════ LÉGENDE ══════════════╗%s\n", CYAN, RESET);
    printf("%s        ║%s  %s1 %s= Pièce taille 1 (1 pas)         %s║%s\n", CYAN, RESET, BRIGHT_GREEN, RESET, CYAN, RESET);
    printf("%s        ║%s  %s2 %s= Pièce taille 2 (2 pas)         %s║%s\n", CYAN, RESET, BRIGHT_YELLOW, RESET, CYAN, RESET);
    printf("%s        ║%s  %s3 %s= Pièce taille 3 (3 pas)         %s║%s\n", CYAN, RESET, BRIGHT_RED, RESET, CYAN, RESET);
    printf("%s        ║%s  %s· %s= Case vide                      %s║%s\n", CYAN, RESET, DIM, RESET, CYAN, RESET);
    printf("%s        ║%s  %s%s* %s= Pièce en mouvement             %s║%s\n", CYAN, RESET, MAGENTA, WHITE, RESET, CYAN, RESET);
    printf("%s        ╚═════════════════════════════════════╝%s\n\n", CYAN, RESET);
    printf("\n\n");
    printf("%sAppuyez sur ENTREE pour commencer...%s", CYAN, RESET);
    getchar(); // Wait for the user to press enter
    
    return game;
}

/* --------------------------------------------------------------------------------------------- */










/* ---- Game ----------------------------------------------------------------------------------- */

board put_piece_players(board game) {
    player current = SOUTH_P;
    int tours = 12;

    for (int i = 0; i < tours; i++) {
        clear_screen();
        printf("=== Mise en place des pieces ===\n\n");

        // Display the game state
        show_game_state(game, current);
        show_plato(game);

        size piece = NONE;
        int col = -1;

        // ----- Size Choice -----
        while (piece == NONE) {
            printf("Choisissez la taille de la piece (1, 2 ou 3) : ");
            int input;
            if (scanf("%d", &input) != 1) {
                printf("❌ %sEntree invalide.%s\n", RED, RESET);
                while (getchar() != '\n');
                continue;
            }

            if (input == 1) {
                piece = ONE;
            }
            else if (input == 2) {
                piece = TWO;
            }
            else if (input == 3) {
                piece = THREE;
            }
            else {
                printf("Taille invalide.\n");
                continue;
            }

            // Check availability
            if (nb_pieces_available(game, piece, current) == 0) {
                printf("❌ %sVous n'avez plus de pieces de cette taille.%s\n", RED, RESET);
                piece = NONE;
            }
        }

        // ----- Column choice -----
        while (col < 0 || col >= DIMENSION) {
            printf("Colonne de placement (1 à 6) : ");
            if (scanf("%d", &col) != 1) {
                printf("❌ %sEntree invalide.%s\n", RED, RESET);
                while (getchar() != '\n');
                col = -1;
                continue;
            }
            col--; // convert input 1-6 to index 0-5
            
            if (col < 0 || col >= DIMENSION) {
                printf("❌ %sColonne invalide (doit être entre 1 et 6).%s\n", RED, RESET);
            }
        }

        // ----- Placement -----
        return_code rc = place_piece(game, piece, current, col);
        
        while (rc != OK) {
            if (rc == EMPTY) {
                printf("Cette colonne est deja occupee. Nouvelle colonne (1 à 6) : ");
                if (scanf("%d", &col) != 1) {
                    printf("❌ %sEntree invalide.%s\n", RED, RESET);
                    while (getchar() != '\n');
                    col = -1;
                    continue;
                }
                col--;
                if (col < 0 || col >= DIMENSION) {
                    printf("❌ %sColonne invalide.%s\n", RED, RESET);
                    continue;
                }
                rc = place_piece(game, piece, current, col);
            } 
            else if (rc == PARAM) {
                printf("❌ %sParametre invalide.%s\n", RED, RESET);
                break;
            }
            else if (rc == FORBIDDEN) {
                printf("❌ %sPlacement interdit.%s\n", RED, RESET);
                break;
            }
        }

        if (rc == OK) {
            printf("✅ %sPièce placée avec succès !%s\n", GREEN, RESET);
            sleep(1);
        }

        // ----- Swap player -----
        current = next_player(current);
    }

    return game;
}





/* ----- Movement functions and play ----- */

/* ----- Function to display possible directions ----- */
void show_possible_direction(board game) {
    printf("\n%s=== DIRECTIONS POSSIBLES ===%s\n", YELLOW, RESET);
    
    if (is_move_possible(game, GOAL)) {
        printf("  %s[G]%s - GOAL (GAGNER) %s✓%s\n", BRIGHT_GREEN, RESET, BRIGHT_GREEN, RESET);
    }
    if (is_move_possible(game, NORTH)) {
        printf("  %s[N]%s - NORD (↑) %s✓%s\n", BRIGHT_GREEN, RESET, BRIGHT_GREEN, RESET);
    }
    if (is_move_possible(game, SOUTH)) {
        printf("  %s[S]%s - SUD (↓) %s✓%s\n", BRIGHT_GREEN, RESET, BRIGHT_GREEN, RESET);
    }
    if (is_move_possible(game, EAST)) {
        printf("  %s[E]%s - EST (→) %s✓%s\n", BRIGHT_GREEN, RESET, BRIGHT_GREEN, RESET);
    }
    if (is_move_possible(game, WEST)) {
        printf("  %s[O]%s - OUEST (←) %s✓%s\n", BRIGHT_GREEN, RESET, BRIGHT_GREEN, RESET);
    }
    
    printf("\n  %s[A]%s - Annuler le mouvement\n", CYAN, RESET);
    printf("  %s[R]%s - Annuler le dernier pas\n", CYAN, RESET);
    printf("\n");
}

/* ----- Function to request directions ----- */
direction ask_direction(board game, int *annuler, int *retour) {
    char input[10];
    *annuler = 0;
    *retour = 0;
    
    show_possible_direction(game);
    printf("Choisissez une direction : ");
    
    if (scanf("%s", input) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    
    // Convert into uppercase
    if (input[0] >= 'a' && input[0] <= 'z') {
        input[0] = input[0] - 'a' + 'A';
    }
    
    switch (input[0]) {
        case 'G':
            return GOAL;
        case 'N':
            return NORTH;
        case 'S':
            return SOUTH;
        case 'E':
            return EAST;
        case 'O':
        case 'W':
            return WEST;
        case 'A':
            *annuler = 1;
            return -1;
        case 'R':
            *retour = 1;
            return -1;
        default:
            printf("❌ %sDirection invalide.%s\n", RED, RESET);
            sleep(1);
            return -1;
    }
}

/* ----- Function for swap ----- */
void manage_swap(board game) {
    printf("\n%s=== COUP SPECIAL ===%s\n", BRIGHT_MAGENTA, RESET);
    printf("Voulez-vous échanger les pièces ? (O/N) : ");
    
    char choix;
    if (scanf(" %c", &choix) != 1) {
        while (getchar() != '\n');
        return;
    }
    
    if (choix == 'O' || choix == 'o') {
        int target_line, target_col;
        
        printf("Position de la pièce éjectée (ligne 1-6) : ");
        if (scanf("%d", &target_line) != 1) {
            printf("❌ %sEntrée invalide.%s\n", RED, RESET);
            while (getchar() != '\n');
            return;
        }
        target_line--;
        
        printf("Position de la pièce éjectée (colonne 1-6) : ");
        if (scanf("%d", &target_col) != 1) {
            printf("❌ %sEntrée invalide.%s\n", RED, RESET);
            while (getchar() != '\n');
            return;
        }
        target_col--;
        
        return_code rc = swap_piece(game, target_line, target_col);
        
        if (rc == OK) {
            printf("✅ %sÉchange réussi !%s\n", GREEN, RESET);
            sleep(1);
        } else if (rc == EMPTY) {
            printf("❌ %sÉchange impossible (pas de pièce sous).%s\n", RED, RESET);
            sleep(1);
        } else if (rc == PARAM) {
            printf("❌ %sPosition invalide.%s\n", RED, RESET);
            sleep(1);
        } else if (rc == FORBIDDEN) {
            printf("❌ %sCase occupée ou échange interdit.%s\n", RED, RESET);
            sleep(1);
        }
    }
}

/* ----- Function to display playable parts ----- */
void show_playable_piece(board game, player current) {
    int line_to_check;
    
    if (current == SOUTH_P) {
        line_to_check = southmost_occupied_line(game);
    } else {
        line_to_check = northmost_occupied_line(game);
    }
    
    if (line_to_check == -1) {
        printf("%s⚠️  Aucune pièce disponible !%s\n", YELLOW, RESET);
        return;
    }
    
    printf("\n%s=== PIÈCES JOUABLES (ligne %d) ===%s\n", CYAN, line_to_check + 1, RESET);
    
    int has_pieces = 0;
    for (int j = 0; j < DIMENSION; j++) {
        size p = get_piece_size(game, line_to_check, j);
        if (p != NONE) {
            char* color;
            switch (p) {
                case ONE:
                    color = BRIGHT_GREEN;
                    break;
                case TWO:
                    color = BRIGHT_YELLOW;
                    break;
                case THREE:
                    color = BRIGHT_RED;
                    break;
                default:
                    color = WHITE;
                    break;
            }
            printf("  Colonne %d : %s%d%s\n", j + 1, color, p, RESET);
            has_pieces = 1;
        }
    }
    
    if (!has_pieces) {
        printf("  %sAucune pièce sur cette ligne.%s\n", DIM, RESET);
    }
    printf("\n");
}

/* ----- Function to move pieces ----- */
void slide_piece(board game, player current) {
    int line = -1;
    int column = -1;
    
    // ----- Selection of the piece -----
    while (picked_piece_owner(game) == NO_PLAYER) {
        clear_screen();
        show_game_state(game, current);
        show_plato(game);
        show_playable_piece(game, current);
        
        printf("Sélectionnez une pièce :\n");
        printf("  Ligne (1-6) : ");
        if (scanf("%d", &line) != 1) {
            printf("❌ %sEntrée invalide.%s\n", RED, RESET);
            while (getchar() != '\n');
            sleep(1);
            continue;
        }
        line--;
        
        printf("  Colonne (1-6) : ");
        if (scanf("%d", &column) != 1) {
            printf("❌ %sEntrée invalide.%s\n", RED, RESET);
            while (getchar() != '\n');
            sleep(1);
            continue;
        }
        column--;
        
        return_code rc = pick_piece(game, current, line, column);
        
        if (rc == OK) {
            printf("✅ %sPièce sélectionnée !%s\n", GREEN, RESET);
            sleep(1);
        } else if (rc == EMPTY) {
            printf("❌ %sCase vide.%s\n", RED, RESET);
            sleep(1);
        } else if (rc == FORBIDDEN) {
            printf("❌ %sPièce non jouable (pas sur la bonne ligne).%s\n", RED, RESET);
            sleep(1);
        } else if (rc == PARAM) {
            printf("❌ %sPosition invalide.%s\n", RED, RESET);
            sleep(1);
        }
    }
    
    // ----- Part displacement -----
    while (picked_piece_owner(game) != NO_PLAYER) {
        clear_screen();
        show_game_state(game, current);
        show_playable_piece(game, current);
        show_plato(game);
        
        // Check if a swap is possible
        if (movement_left(game) == 0) {
            manage_swap(game);
            if (picked_piece_owner(game) == NO_PLAYER) {
                // Swap successful or not, exit movement
                break;
            }
        }
        
        int annuler = 0, retour = 0;
        direction dir = ask_direction(game, &annuler, &retour);
        
        if (annuler) {
            return_code rc = cancel_movement(game);
            if (rc == OK) {
                printf("✅ %sMouvement annulé.%s\n", GREEN, RESET);
                sleep(1);
                return; // Start the tour again
            }
        }
        
        else if (retour) {
            return_code rc = cancel_step(game);
            if (rc == OK) {
                printf("✅ %sDernier pas annulé.%s\n", GREEN, RESET);
                sleep(1);
                continue;
            } else {
                printf("❌ %sImpossible d'annuler.%s\n", RED, RESET);
                sleep(1);
                continue;
            }
        }
        
        if (dir == -1) {
            continue;
        }
        
        if (!is_move_possible(game, dir)) {
            printf("❌ %sMouvement impossible dans cette direction.%s\n", RED, RESET);
            sleep(1);
            continue;
        }
        
        return_code rc = move_piece(game, dir);
        
        if (rc == OK) {
            if (dir == GOAL) {
                return; // We're going out, the victory will be managed in play_game.
            }
            
            printf("✅ %sDéplacement effectué !%s\n", GREEN, RESET);
            sleep(1);
            
            // If the movement is finished and there is no possible swap 
            if (movement_left(game) == -1) {
                break; // End of Turn
            }
        } else if (rc == FORBIDDEN) {
            printf("❌ %sMouvement interdit.%s\n", RED, RESET);
            sleep(1);
        } else if (rc == PARAM) {
            printf("❌ %sPosition invalide.%s\n", RED, RESET);
            sleep(1);
        } else if (rc == EMPTY) {
            printf("❌ %sAucune pièce en mouvement.%s\n", RED, RESET);
            sleep(1);
        }
    }
}

/* ----- Main game function ----- */
void play_game(board game) {
    player current = SOUTH_P;
    
    while (1) {
        // Check victory conditions
        player winner = get_winner(game);
        if (winner != NO_PLAYER) {
            clear_screen();
            show_plato(game);
            show_winner(winner);
            break;
        }
        
        // Player's turn
        slide_piece(game, current);
        
        // Check again after the movement
        winner = get_winner(game);
        if (winner != NO_PLAYER) {
            clear_screen();
            show_plato(game);
            show_winner(winner);
            break;
        }
        
        // Pass to next player
        current = next_player(current);
    }
}





void finish_game (board game) {
    printf("\nMerci d'avoir joué !\n");
    destroy_game(game);
}


/* --------------------------------------------------------------------------------------------- */




int main () {
    board game = init_game();   // Initialize the game
    put_piece_players(game);    // Players place their pieces
    play_game(game);            // Start the main game loop
    finish_game(game);          // Clean up and exit
    return 0;
}