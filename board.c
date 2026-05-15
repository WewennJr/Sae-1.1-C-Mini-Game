#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"

/**
 * \file board.c
 *
 * \brief Source code associated with \ref board.h
 *
 * \author Implementation for SAE 1.2
 */

// STRUCTURES

/**
 * @brief Structure pour stocker l'historique des mouvements (pour cancel_step)
 */
typedef struct move_history_s {
    int line;
    int column;
    int moves_left;
} move_history;

/**
 * @brief The board of the game.
 */
struct board_s {
    size board[DIMENSION][DIMENSION];           // Le plateau de jeu
    player winner;                              // Le gagnant (NO_PLAYER si pas de gagnant)
    
    // Phase de placement
    int pieces_to_place[NB_PLAYERS][NB_SIZE];   // Nombre de pièces à placer pour chaque joueur
    
    // Phase de jeu / pièce en mouvement
    player picked_player;                        // Joueur qui a pris une pièce
    size picked_size;                            // Taille de la pièce prise
    int picked_line;                             // Ligne de la pièce en mouvement
    int picked_col;                              // Colonne de la pièce en mouvement
    int moves_left;                              // Mouvements restants
    
    // Position initiale (pour cancel_movement)
    int initial_line;
    int initial_col;
    
    // Pour savoir si on est sur une autre pièce (pour swap)
    bool on_piece;
    size piece_below_size;                       // Taille de la pièce en dessous
    
    // Historique (pour cancel_step)
    move_history history[20];                    // Max 20 mouvements
    int history_count;
    
    // Pour éviter de prendre 2 fois la même arrête dans le même sens
    int last_direction;                          // Dernière direction prise
};


// FONCTIONS DE CREATION/DESTRUCTION

board new_game() {
    board new_board = malloc(sizeof(struct board_s));
    
    // Initialiser le plateau vide
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            new_board->board[i][j] = NONE;
        }
    }
    
    // Pas de gagnant au début
    new_board->winner = NO_PLAYER;
    
    // Initialiser les pièces à placer
    for (int p = 0; p < NB_PLAYERS; p++) {
        for (int s = 0; s < NB_SIZE; s++) {
            new_board->pieces_to_place[p][s] = NB_INITIAL_PIECES;
        }
    }
    
    // Pas de pièce en mouvement
    new_board->picked_player = NO_PLAYER;
    new_board->picked_size = NONE;
    new_board->picked_line = -1;
    new_board->picked_col = -1;
    new_board->moves_left = -1;
    new_board->initial_line = -1;
    new_board->initial_col = -1;
    new_board->on_piece = false;
    new_board->piece_below_size = NONE;
    new_board->history_count = 0;
    new_board->last_direction = -1;
    
    return new_board;
}

board copy_game(board original_game) {
    board new_board = malloc(sizeof(struct board_s));
    
    // Copier le plateau
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            new_board->board[i][j] = original_game->board[i][j];
        }
    }
    
    // Copier le gagnant
    new_board->winner = original_game->winner;
    
    // Copier les pièces à placer
    for (int p = 0; p < NB_PLAYERS; p++) {
        for (int s = 0; s < NB_SIZE; s++) {
            new_board->pieces_to_place[p][s] = original_game->pieces_to_place[p][s];
        }
    }
    
    // Copier l'état de la pièce en mouvement
    new_board->picked_player = original_game->picked_player;
    new_board->picked_size = original_game->picked_size;
    new_board->picked_line = original_game->picked_line;
    new_board->picked_col = original_game->picked_col;
    new_board->moves_left = original_game->moves_left;
    new_board->initial_line = original_game->initial_line;
    new_board->initial_col = original_game->initial_col;
    new_board->on_piece = original_game->on_piece;
    new_board->piece_below_size = original_game->piece_below_size;
    new_board->history_count = original_game->history_count;
    new_board->last_direction = original_game->last_direction;
    
    // Copier l'historique
    for (int i = 0; i < original_game->history_count; i++) {
        new_board->history[i] = original_game->history[i];
    }
    
    return new_board;
}

void destroy_game(board game) {
    free(game);
}


// FONCTIONS D'ACCES AUX DONNEES

size get_piece_size(board game, int line, int column) {
    if (line < 0 || line >= DIMENSION || column < 0 || column >= DIMENSION) {
        return NONE;
    }
    return game->board[line][column];
}

player get_winner(board game) {
    return game->winner;
}

int southmost_occupied_line(board game) {
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            if (game->board[i][j] != NONE) {
                return i;
            }
        }
    }
    return -1;
}

int northmost_occupied_line(board game) {
    for (int i = DIMENSION - 1; i >= 0; i--) {
        for (int j = 0; j < DIMENSION; j++) {
            if (game->board[i][j] != NONE) {
                return i;
            }
        }
    }
    return -1;
}

player picked_piece_owner(board game) {
    return game->picked_player;
}

size picked_piece_size(board game) {
    return game->picked_size;
}

int picked_piece_line(board game) {
    return game->picked_line;
}

int picked_piece_column(board game) {
    return game->picked_col;
}

int movement_left(board game) {
    if (game->picked_player == NO_PLAYER) {
        return -1;
    }
    if (game->on_piece) {
        return 0;
    }
    return game->moves_left;
}


// FONCTIONS UTILITAIRES

player next_player(player current_player) {
    if (current_player == SOUTH_P) {
        return NORTH_P;
    } else {
        return SOUTH_P;
    }
}


// PHASE DE PLACEMENT

int nb_pieces_available(board game, size piece, player p) {
    if (p != SOUTH_P && p != NORTH_P) {
        return -1;
    }
    if (piece < ONE || piece > THREE) {
        return -1;
    }
    
    int player_index;
    if (p == SOUTH_P) {
        player_index = 0;
    } else {
        player_index = 1;
    }
    
    int size_index = piece - 1;
    
    return game->pieces_to_place[player_index][size_index];
}

return_code place_piece(board game, size piece, player p, int column) {
    // Vérifier les paramètres
    if (p != SOUTH_P && p != NORTH_P) {
        return PARAM;
    }
    if (piece < ONE || piece > THREE) {
        return PARAM;
    }
    if (column < 0 || column >= DIMENSION) {
        return PARAM;
    }
    
    // Déterminer la ligne selon le joueur
    int line;
    if (p == SOUTH_P) {
        line = 0;
    } else {
        line = DIMENSION - 1;
    }
    
    // Vérifier que la case est vide
    if (game->board[line][column] != NONE) {
        return EMPTY;
    }
    
    // Vérifier que le joueur a encore des pièces de cette taille
    if (nb_pieces_available(game, piece, p) <= 0) {
        return FORBIDDEN;
    }
    
    // Placer la pièce
    game->board[line][column] = piece;
    
    // Décrémenter le nombre de pièces disponibles
    int player_index;
    if (p == SOUTH_P) {
        player_index = 0;
    } else {
        player_index = 1;
    }
    int size_index = piece - 1;
    game->pieces_to_place[player_index][size_index]--;
    
    return OK;
}


// PHASE DE JEU

return_code pick_piece(board game, player current_player, int line, int column) {
    // Vérifier que la phase de placement est terminée
    for (int p = 0; p < NB_PLAYERS; p++) {
        for (int s = 0; s < NB_SIZE; s++) {
            if (game->pieces_to_place[p][s] > 0) {
                return FORBIDDEN;
            }
        }
    }
    
    // Vérifier qu'il n'y a pas déjà de gagnant
    if (game->winner != NO_PLAYER) {
        return FORBIDDEN;
    }
    
    // Vérifier les paramètres
    if (current_player != SOUTH_P && current_player != NORTH_P) {
        return PARAM;
    }
    if (line < 0 || line >= DIMENSION || column < 0 || column >= DIMENSION) {
        return PARAM;
    }
    
    // Vérifier que la case n'est pas vide
    if (game->board[line][column] == NONE) {
        return EMPTY;
    }
    
    // Vérifier que la pièce est sur la bonne ligne
    if (current_player == SOUTH_P) {
        if (line != southmost_occupied_line(game)) {
            return FORBIDDEN;
        }
    } else {
        if (line != northmost_occupied_line(game)) {
            return FORBIDDEN;
        }
    }
    
    // Prendre la pièce
    game->picked_player = current_player;
    game->picked_size = game->board[line][column];
    game->picked_line = line;
    game->picked_col = column;
    game->moves_left = game->picked_size;
    game->initial_line = line;
    game->initial_col = column;
    game->on_piece = false;
    game->piece_below_size = NONE;
    game->history_count = 0;
    game->last_direction = -1;
    
    // Retirer la pièce du plateau
    game->board[line][column] = NONE;
    
    // Sauvegarder dans l'historique
    game->history[game->history_count].line = line;
    game->history[game->history_count].column = column;
    game->history[game->history_count].moves_left = game->moves_left;
    game->history_count++;
    
    return OK;
}

bool is_move_possible(board game, direction dir) {
    // Pas de pièce en mouvement
    if (game->picked_player == NO_PLAYER) {
        return false;
    }
    
    // Si on est sur une pièce ET qu'on a 0 mouvements, on peut bouncer
    if (game->on_piece && game->moves_left == 0) {
        // On peut bouncer dans n'importe quelle direction valide
        int line = game->picked_line;
        int col = game->picked_col;
        
        int new_line = line;
        int new_col = col;
        
        switch (dir) {
            case NORTH:
                new_line++;
                break;
            case SOUTH:
                new_line--;
                break;
            case EAST:
                new_col++;
                break;
            case WEST:
                new_col--;
                break;
            case GOAL:
                return false; // Pas de GOAL depuis un bounce
            default:
                return false;
        }
        
        // Vérifier que c'est dans le plateau
        if (new_line < 0 || new_line >= DIMENSION || new_col < 0 || new_col >= DIMENSION) {
            return false;
        }
        
        // La case doit être vide pour le premier pas du bounce
        return (game->board[new_line][new_col] == NONE);
    }
    
    // Si on est sur une pièce mais pas en état de bounce, pas de mouvement
    if (game->on_piece) {
        return false;
    }
    
    int line = game->picked_line;
    int col = game->picked_col;
    
    // Direction GOAL
    if (dir == GOAL) {
        if (game->picked_player == SOUTH_P && line == DIMENSION - 1) {
            return true;
        }
        if (game->picked_player == NORTH_P && line == 0) {
            return true;
        }
        return false;
    }
    
    // Calculer la nouvelle position
    int new_line = line;
    int new_col = col;
    
    switch (dir) {
        case NORTH:
            new_line++;
            break;
        case SOUTH:
            new_line--;
            break;
        case EAST:
            new_col++;
            break;
        case WEST:
            new_col--;
            break;
        default:
            return false;
    }
    
    // Vérifier que c'est dans le plateau
    if (new_line < 0 || new_line >= DIMENSION || new_col < 0 || new_col >= DIMENSION) {
        return false;
    }
    
    // Si c'est le dernier mouvement, on peut aller sur une pièce
    if (game->moves_left == 1) {
        return true;
    }
    
    // Sinon, la case doit être vide
    return (game->board[new_line][new_col] == NONE);
}

return_code move_piece(board game, direction dir) {
    // Vérifier qu'il y a une pièce en mouvement
    if (game->picked_player == NO_PLAYER) {
        return EMPTY;
    }
    
    // Si on est sur une pièce, on bounce
    if (game->on_piece) {
        game->moves_left = game->piece_below_size;
        game->on_piece = false;
        game->piece_below_size = NONE;
        // Ne pas incrémenter history_count car c'est un bounce
    }
    
    int line = game->picked_line;
    int col = game->picked_col;
    
    // Direction GOAL
    if (dir == GOAL) {
        if (game->picked_player == SOUTH_P && line == DIMENSION - 1) {
            game->winner = game->picked_player;
            game->picked_player = NO_PLAYER;
            game->picked_size = NONE;
            game->picked_line = -1;
            game->picked_col = -1;
            game->moves_left = -1;
            return OK;
        }
        if (game->picked_player == NORTH_P && line == 0) {
            game->winner = game->picked_player;
            game->picked_player = NO_PLAYER;
            game->picked_size = NONE;
            game->picked_line = -1;
            game->picked_col = -1;
            game->moves_left = -1;
            return OK;
        }
        return FORBIDDEN;
    }
    
    // Calculer la nouvelle position
    int new_line = line;
    int new_col = col;
    
    switch (dir) {
        case NORTH:
            new_line++;
            break;
        case SOUTH:
            new_line--;
            break;
        case EAST:
            new_col++;
            break;
        case WEST:
            new_col--;
            break;
        default:
            return PARAM;
    }
    
    // Vérifier que c'est dans le plateau
    if (new_line < 0 || new_line >= DIMENSION || new_col < 0 || new_col >= DIMENSION) {
        return PARAM;
    }
    
    // Vérifier que le mouvement est possible
    if (!is_move_possible(game, dir)) {
        return FORBIDDEN;
    }
    
    // Effectuer le mouvement
    game->picked_line = new_line;
    game->picked_col = new_col;
    game->moves_left--;
    game->last_direction = dir;
    
    // Sauvegarder dans l'historique
    game->history[game->history_count].line = new_line;
    game->history[game->history_count].column = new_col;
    game->history[game->history_count].moves_left = game->moves_left;
    game->history_count++;
    
    // Vérifier si on arrive sur une pièce
    if (game->board[new_line][new_col] != NONE) {
        if (game->moves_left == 0) {
            // On est sur une pièce et on a fini nos mouvements
            game->on_piece = true;
            game->piece_below_size = game->board[new_line][new_col];
        } else {
            // On ne devrait pas arriver ici (mouvement interdit)
            return FORBIDDEN;
        }
    }
    
    // Si on a fini nos mouvements et qu'on n'est pas sur une pièce
    if (game->moves_left == 0 && !game->on_piece) {
        // Poser la pièce
        game->board[new_line][new_col] = game->picked_size;
        game->picked_player = NO_PLAYER;
        game->picked_size = NONE;
        game->picked_line = -1;
        game->picked_col = -1;
        game->moves_left = -1;
    }
    
    return OK;
}

return_code swap_piece(board game, int target_line, int target_column) {
    // Vérifier qu'on est bien sur une pièce
    if (!game->on_piece) {
        return EMPTY;
    }
    
    // Vérifier les paramètres
    if (target_line < 0 || target_line >= DIMENSION || 
        target_column < 0 || target_column >= DIMENSION) {
        return PARAM;
    }
    
    // Vérifier que la case cible est vide
    if (game->board[target_line][target_column] != NONE) {
        return FORBIDDEN;
    }
    
    // Effectuer le swap
    int current_line = game->picked_line;
    int current_col = game->picked_col;
    
    // Placer la pièce en mouvement à sa position actuelle
    game->board[current_line][current_col] = game->picked_size;
    
    // Placer la pièce du dessous à la position cible
    game->board[target_line][target_column] = game->piece_below_size;
    
    // Réinitialiser l'état
    game->picked_player = NO_PLAYER;
    game->picked_size = NONE;
    game->picked_line = -1;
    game->picked_col = -1;
    game->moves_left = -1;
    game->on_piece = false;
    game->piece_below_size = NONE;
    
    return OK;
}

return_code cancel_movement(board game) {
    // Vérifier qu'il y a une pièce en mouvement
    if (game->picked_player == NO_PLAYER) {
        return EMPTY;
    }
    
    // Remettre la pièce à sa position initiale
    game->board[game->initial_line][game->initial_col] = game->picked_size;
    
    // Réinitialiser l'état
    game->picked_player = NO_PLAYER;
    game->picked_size = NONE;
    game->picked_line = -1;
    game->picked_col = -1;
    game->moves_left = -1;
    game->on_piece = false;
    game->piece_below_size = NONE;
    game->history_count = 0;
    
    return OK;
}

return_code cancel_step(board game) {
    // Vérifier qu'il y a une pièce en mouvement
    if (game->picked_player == NO_PLAYER) {
        return EMPTY;
    }
    
    // Vérifier qu'il y a au moins 2 étapes dans l'historique
    if (game->history_count < 2) {
        return cancel_movement(game);
    }
    
    // Revenir à l'étape précédente
    game->history_count--;
    game->picked_line = game->history[game->history_count - 1].line;
    game->picked_col = game->history[game->history_count - 1].column;
    game->moves_left = game->history[game->history_count - 1].moves_left;
    game->on_piece = false;
    game->piece_below_size = NONE;
    
    return OK;
}