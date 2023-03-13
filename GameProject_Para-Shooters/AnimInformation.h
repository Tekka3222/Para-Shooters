#pragma once
/////////////////////////////////////////////
/////////////////////////////////////////////
//描画内容のマクロをひとまとめにするヘッダ///
/////////////////////////////////////////////
/////////////////////////////////////////////
#include <utility>
using namespace std;

#define BASIC_TEX_SIZE 512
#define TEX_SMALL 16
#define TEX_BIG 32
//tilemap
#define BLACK_AIR 0
#define WHITE_AIR 1
#define BOUND 2
#define WL_CORNER 3
#define BL_CORNER 4
#define WR_CORNER 5
#define BR_CORNER 6
#define WL_WALL 7
#define BL_WALL 8
#define SUN pair<int,int>(9,10)
#define MOON pair<int,int>(11,12)
#define WHITE_DOOR 13
#define BLACK_DOOR 14
#define RESULT 15


//litsmap
//1～10までの数字はそのまま使用
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
#define PARTICLE_DAMAGE 45
#define PARTICLE_HEAL 46
#define PARTICLE_RELOAD 47
#define PARTICLE_REVERSE 48
#define PARTICLE_DAMAGE_REVERSE 49
#define PARTICLE_HEAL_REVERSE 50
#define PARTICLE_RELOAD_REVERSE 51
#define PARTICLE_REVERSE_REVERSE 52

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
#define PLAYER_REV_NOR pair<int,int>(52,55)
#define PLAYER_REV_REV pair<int,int>(56,59)

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
#define ENEMY_REV_NOR pair<int,int>(60,63)
#define ENEMY_REV_NOR_SLEEP pair<int,int>(64,67)
#define ENEMY_REV_REV pair<int,int>(68,71)
#define ENEMY_REV_REV_SLEEP pair<int,int>(72,75)
//enemy_gun
#define ENEMY_GUN_RIGHT 36
#define ENEMY_GUN_LEFT 37
#define ENEMY_GUN_RIGHT_REV 38
#define ENEMY_GUN_LEFT_REV 39
//gun-None
#define GUN_NONE 76


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