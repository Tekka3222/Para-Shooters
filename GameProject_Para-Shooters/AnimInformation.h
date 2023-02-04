#pragma once
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//This Header initialize AnimationRange for every textures.//
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
#include <utility>
using namespace std;

#define BASIC_TEX_SIZE 512
#define TEX_SMALL 16
#define TEX_BIG 32
//tilemap
#define BLACK_AIR 0
#define WHITE_AIR 1
#define BW_WALL 2
#define WB_WALL 3
#define SUN pair<int,int>(4,5)
#define MOON pair<int,int>(6,7)
#define BOUND 8
#define WHITE_DOOR 9
#define BLACK_DOOR 10
#define RESULT 11


//litsmap
//1Å`10Ç‹Ç≈ÇÃêîéöÇÕÇªÇÃÇ‹Ç‹égóp
#define ALPHA_A 10
#define ALPHA_B 11
#define ALPHA_C 12
#define ALPHA_D 13
#define ALPHA_E 14
#define ALPHA_F 15
#define ALPHA_G 16
#define ALPHA_H 17
#define ALPHA_I 18
#define ALPHA_K 19
#define ALPHA_L 20
#define ALPHA_M 21
#define ALPHA_N 22
#define ALPHA_O 23
#define ALPHA_P 24
#define ALPHA_R 25
#define ALPHA_S 26
#define ALPHA_T 27
#define ALPHA_U 28
#define ALPHA_V 29
#define SLASH 30
#define NULL_PIC 31
#define COLON 32
#define RETICLE_NOR 33
#define RETICLE_RELOAD 34
#define FULL_NOR_HEART 35
#define HALF_NOR_HEART 36
#define FULL_DANGER_HEART pair<int,int>(37,38)
#define HALF_DANGER_HEART pair<int,int>(39,40)
#define PLAYER_BULLET 41
#define ENEMY_BULLET 42
#define PLAYER_BULLET_HIT 43
#define ENEMY_BULLET_HIT 44

//character
//player
#define PLAYER_STAND_RIGHT pair<int,int>(0,0)
#define PLAYER_STAND_LEFT pair<int,int>(1,1)
#define PLAYER_STAND_RIGHT_REV pair<int,int>(2,2)
#define PLAYER_STAND_LEFT_REV pair<int,int>(3,3)
#define PLAYER_MOVE_RIGHT pair<int,int>(8,13)
#define PLAYER_MOVE_LEFT pair<int,int>(14,19)
#define PLAYER_MOVE_RIGHT_REV pair<int,int>(20,25)
#define PLAYER_MOVE_LEFT_REV pair<int,int>(26,31)
//player_gun
#define PLAYER_GUN_RIGHT 4
#define PLAYER_GUN_LEFT 5
#define PLAYER_GUN_RIGHT_REV 6
#define PLAYER_GUN_LEFT_REV 7
//enemy
#define ENEMY_SLEEP_STAND_RIGHT pair<int,int>(48,48)
#define ENEMY_SLEEP_STAND_LEFT pair<int,int>(49,49)
#define ENEMY_SLEEP_STAND_RIGHT_REV pair<int,int>(50,50)
#define ENEMY_SLEEP_STAND_LEFT_REV pair<int,int>(51,51)
#define ENEMY_ACTIVE_STAND_RIGHT pair<int,int>(32,32)
#define ENEMY_ACTIVE_STAND_LEFT pair<int,int>(33,33)
#define ENEMY_ACTIVE_STAND_RIGHT_REV pair<int,int>(34,34)
#define ENEMY_ACTIVE_STAND_LEFT_REV pair<int,int>(35,35)
#define ENEMY_ACTIVE_MOVE_RIGHT pair<int,int>(40,41)
#define ENEMY_ACTIVE_MOVE_LEFT pair<int,int>(42,43)
#define ENEMY_ACTIVE_MOVE_RIGHT_REV pair<int,int>(44,45)
#define ENEMY_ACTIVE_MOVE_LEFT_REV pair<int,int>(46,47)
//enemy_gun
#define ENEMY_GUN_RIGHT 36
#define ENEMY_GUN_LEFT 37
#define ENEMY_GUN_RIGHT_REV 38
#define ENEMY_GUN_LEFT_REV 39

//Upgrade
#define CURSOR 0
#define CURSOR_BOUGHT 1
#define HEAL_TEX 2
#define HEAL_COMP_TEX 3
#define DAMAGE_TEX 4
#define DAMAGE_COMP_TEX 5
#define INTERVAL_TEX 6
#define INTERVAL_COMP_TEX 7
#define NUM_OF_BULLET_TEX 8
#define NUM_OF_BULLET_COMP_TEX 9
#define RELOAD_TEX 10
#define RELOAD_COMP_TEX 11
#define SHIELD_TEX 12
#define SHIELD_COMP_TEX 13