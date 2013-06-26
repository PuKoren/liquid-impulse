#ifndef _HERO_H
#define _HERO_H
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "../../Config.h"
#include "../Effects/Effect.h"
#include "../Effects/Projectile.h"
#include "../../Maths/Rectangle.h"
#include "../../Maths/Vector2.h"
#include "../../Tools/Surface.h"
using namespace std;

#define HERO_WIDTH 175*RATIO

enum HERO_ANIMATION_STATE {
    HERO_RUN_LEFT,
	HERO_STAND_LEFT,
	HERO_PUNCH_LEFT,
	HERO_JUMP_LEFT_UP,
	HERO_JUMP_LEFT_DOWN,
	HERO_PUNCH_AIR_LEFT,
	HERO_HIT_LEFT,
	HERO_HARD_HIT_LEFT,
    HERO_IMPULSE_LEFT,
    HERO_REPULSE_LEFT,
    HERO_UPPERCUT_LEFT,
    HERO_TAKEDOWN_AIR_LEFT,
    HERO_BOMB_AIR_LEFT,

	HERO_RUN_RIGHT,
	HERO_STAND_RIGHT,
	HERO_PUNCH_RIGHT,
	HERO_JUMP_RIGHT_UP,
	HERO_JUMP_RIGHT_DOWN,
	HERO_PUNCH_AIR_RIGHT,
	HERO_HIT_RIGHT,
	HERO_HARD_HIT_RIGHT,
    HERO_IMPULSE_RIGHT,
    HERO_REPULSE_RIGHT,
    HERO_UPPERCUT_RIGHT,
    HERO_TAKEDOWN_AIR_RIGHT,
    HERO_BOMB_AIR_RIGHT,

	HERO_DIE_LEFT,
	HERO_DIE_RIGHT
};

class Hero {
    private:
        int life;
        int health;
        int lastMoveDirection;

        float velocity;
        float jump_velocity_current;
        float jump_velocity;
        float gravity;

        bool jumping;
        bool attacking;
        bool specialAttack;    
        bool hit;
        bool alive;
		bool blockingAnimationModifier;

        Surface ** heroAnimationList;
        HERO_ANIMATION_STATE heroAnimationState;
		HERO_ANIMATION_STATE animationModifier;

        Vector2 position;
        Vector2 limitTopLeftPoint;
        Vector2 limitBottomRightPoint;
        Vector2 jump_position;
        Vector2 direction;
        Vector2 directionSpecial;

        Projectile * lastProjectile;
        Projectile * CreateProjectile();
        Projectile* CreateProjectile(Vector2, Vector2, int, float, float, float, int);
        std::vector<Projectile*> * heroProjectiles;
        void Init(int, int, float, Vector2, Vector2);
        void RefreshAnimation();
        void ChangeAnimation(HERO_ANIMATION_STATE);

		void HandleMove(int);
        void HandleAxis(int);
        void HandleJump();
        void HandleAttack();
        void HandleImpulseAttack();
        void HandleAirBombAttack();
        void HandleUppercutAttack();
        void HandleAirTakedownAttack();
        void HandleRepulseAttack();
        bool HandleSpecialAttack(HERO_ANIMATION_STATE state);

        Mix_Chunk * hitSound;
		Mix_Chunk * impulseSound;
		Mix_Chunk * repulseSound;
		Mix_Chunk * jumpSound;

        Effect * impulseEffectRight;
        Effect * impulseEffectLeft;
        Effect * repulseEffect;
        Effect * airBombEffect;
		Effect * jumpDustEffect;

    public:
        Hero(std::vector<Projectile*>*);
        Hero(std::string, int, int, float, Vector2, Vector2, std::vector<Projectile*>*);
        ~Hero();

        Vector2 GetPosition();
		Vector2 GetFootPosition();
        Rectangle GetCollisionBox();
        int GetLife();
        int GetHealth();		
		float GetPositionZ();
		int GetMovingDirection();
        float GetVelocity();

        void Hit(Projectile*);
        bool IsAlive();
		bool IsJumping();
		bool IsJumpingUp();
		bool IsAttacking();

        void Load(std::string);
		void Event(SDL_Event *);
        void Update(Uint32, std::vector<Projectile*>&);
        void UpdateEffect(Uint32);
        void DrawEffect(SDL_Surface *);

        void Draw(SDL_Surface *);
		void SetPositionX(float);
        void ControlHeroLimitsX();
        bool ControlHeroLimitsY();

		
		Surface * heroShadow;
        Surface ** GetAnimationList();
};

#endif


//#ifndef _HERO_H
//#define _HERO_H
//#include <SDL.h>
//#include <SDL_mixer.h>
//#include <vector>
//#include "../../Config.h"
//#include "../Effects/Effect.h"
//#include "../Effects/Projectile.h"
//#include "../../Maths/Rectangle.h"
//#include "../../Tools/Surface.h"
//
//#define HERO_WIDTH 175
//#define HERO_HEIGHT 175
//
//#define HERO_IMPULSE_WIDTH 225
//#define HERO_IMPULSE_HEIGHT 225
//
//#define JUMP_THRESHOLD 2
//#define JUMP_MAX_VELOCITY 25
//
//#define NB_SPRITES_STAND 8
//#define NB_SPRITES_MOVE 10
//#define NB_SPRITES_ATTACK 8
//#define NB_SPRITES_IMPULSE_ATTACK 24
//#define NB_SPRITES_REPULSE_ATTACK 7
//#define NB_SPRITES_UPPERCUT_ATTACK 8
//#define NB_SPRITES_AIR_ATTACK 7
//#define NB_SPRITES_AIR_TAKE_DOWN_ATTACK 24
//#define NB_SPRITES_AIR_BOMB_ATTACK 15
//#define NB_SPRITES_GET_HURTED 5
//#define NB_SPRITES_GET_HURTED_HARD 5
//
//#define ANIMATION_TIMER_STAND 80
//#define ANIMATION_TIMER_MOVE 50
//#define ANIMATION_TIMER_JUMP 40
//#define ANIMATION_TIMER_ATTACK 50
//#define ANIMATION_TIMER_IMPULSE_ATTACK 20
//#define ANIMATION_TIMER_REPULSE_ATTACK 130
//#define ANIMATION_TIMER_UPPERCUT_ATTACK 50
//#define ANIMATION_TIMER_AIR_ATTACK 50
//#define ANIMATION_TIMER_AIR_TAKE_DOWN_ATTACK 30
//#define ANIMATION_TIMER_AIR_BOMB_ATTACK 30
//#define ANIMATION_TIMER_GET_HURTED 50
//#define ANIMATION_TIMER_GET_HURTED_HARD 50
//
//#define HERO_SPEED_X 15
//#define HERO_SPEED_Z 15
//
//#define AIR_BOMB_ATTACK_SPEED 30
//
//using namespace std;
//
//enum HeroStatus{
//	STAND,
//	ATTACK,
//	IMPULSE_ATTACK,
//	REPULSE_ATTACK,
//	UPPERCUT_ATTACK,
//    AIR_TAKE_DOWN_ATTACK,
//    AIR_BOMB_ATTACK,
//	MOVE_H,
//	MOVE_V,
//	JUMP,
//	GET_HURTED,	
//	GET_HURTED_HARD
//};
//
//enum AttackStatus{
//	FINISHED,
//	ATTACK_STARTED,
//	AIR_ATTACK_STARTED,
//	IMPULSE_ATTACK_STARTED,
//	REPULSE_ATTACK_STARTED,
//	UPPERCUT_ATTACK_STARTED,
//    AIR_TAKE_DOWN_ATTACK_STARTED,    
//    AIR_BOMB_ATTACK_STARTED
//};
//
//enum DirectionStatus{
//	RIGHT,
//	LEFT,
//	UP,
//	DOWN
//};
//
//class Hero {
//	public:
//		Hero();
//		~Hero();
//
//		void Event(SDL_Event *);
//		void Update(Uint32, std::vector<Projectile*>&);
//		void Draw(SDL_Surface *);
//		void SetMoveClips();
//        void SetClipsLoop(bool rightDirection, SDL_Rect * rect, int nbSprites, int width, int height);
//
//		void Move();
//		void Jump();
//		void Attack(std::vector<Projectile*>&);
//		void ImpulseAttack();		
//		void RepulseAttack();
//		void UppercutAttack();
//		void GetHurted(int type);
//		void Hit(Projectile*);
//        
//		void AirAttack(std::vector<Projectile*>&);
//		void AirTakeDownAttack();
//		void AirBombAttack();
//
//		void HandleMove(int direction);
//		void HandleAxis(int direction);
//		void HandleJump();
//		void HandleAttack();
//		void HandleImpulseAttack();
//		void HandleRepulseAttack();
//		void HandleUppercutAttack();
//		void HandleGetHurted(int type);
//    
//		void HandleAirAttack();
//		void HandleAirTakeDownAttack();
//		void HandleAirBombAttack();
//
//		bool HeroIsInAction();
//
//		int GetPositionX();
//		int GetPositionZ();
//		int GetPositionY();
//		void SetPositionX(int newX);
//		int GetVelocityX();
//		bool IsJumping();
//		bool IsJumpingUp();
//
//		int GetMovingDirection();
//		Rectangle GetCollisionBox();
//		Projectile* CreateProjectile();
//
//		int GetLife();
//		int GetHealth();
//
//		void SetZLimit(int, int);
//
//	private:
//		int posX;
//		int posY;
//		int health;
//		int life;
//		int velocityX;
//		int velocityY;
//		int velocityZ;
//		int jumpStartAtPosY;
//		bool canJump;
//		bool jumping;
//		int frame;
//		int status;
//		int attackStatus;
//		int directionStatus;
//		int animationStatus;
//		int animationTimer;
//
//		int maxZ;
//		int minZ;
//
//		Surface * surfStand;
//		Surface * surfStandLeft;
//		Surface * surfMove;
//		Surface * surfMoveLeft;
//		Surface * surfJumpUp;
//		Surface * surfJumpDown;
//		Surface * surfJumpUpLeft;
//		Surface * surfJumpDownLeft;
//
//		Surface * surfShadow;
//
//		Surface * surfAttack;
//		Surface * surfAttackLeft;
//		Surface * surfImpulseAttack;
//		Surface * surfImpulseAttackLeft;
//		Surface * surfRepulseAttack;
//		Surface * surfRepulseAttackLeft;
//		Surface * surfUppercutAttack;
//		Surface * surfUppercutAttackLeft;
//        Surface * surfAirAttack;
//		Surface * surfAirAttackLeft;
//		Surface * surfAirTakeDownAttack;
//		Surface * surfAirTakeDownAttackLeft;        
//		Surface * surfAirBombAttack;
//		Surface * surfAirBombAttackLeft;
//		Surface * surfGetHurted;
//		Surface * surfGetHurtedLeft;
//		Surface * surfGetHurtedHard;
//		Surface * surfGetHurtedHardLeft;
//
//		Mix_Chunk *hitSound;
//		Mix_Chunk *impulseSound;
//		Mix_Chunk *repulseSound;
//		Mix_Chunk *jumpSound;
//
//		SDL_Rect clipStand[NB_SPRITES_STAND];
//		SDL_Rect clipStandLeft[NB_SPRITES_STAND];
//		SDL_Rect clipMove[NB_SPRITES_MOVE];
//		SDL_Rect clipMoveLeft[NB_SPRITES_MOVE];
//
//		SDL_Rect clipAttack[NB_SPRITES_ATTACK];
//		SDL_Rect clipAttackLeft[NB_SPRITES_ATTACK];
//		SDL_Rect clipImpulseAttack[NB_SPRITES_IMPULSE_ATTACK];
//		SDL_Rect clipImpulseAttackLeft[NB_SPRITES_IMPULSE_ATTACK];
//		SDL_Rect clipRepulseAttack[NB_SPRITES_REPULSE_ATTACK];
//		SDL_Rect clipRepulseAttackLeft[NB_SPRITES_REPULSE_ATTACK];
//		SDL_Rect clipUppercutAttack[NB_SPRITES_UPPERCUT_ATTACK];
//		SDL_Rect clipUppercutAttackLeft[NB_SPRITES_UPPERCUT_ATTACK];
//        SDL_Rect clipAirAttack[NB_SPRITES_AIR_ATTACK];
//		SDL_Rect clipAirAttackLeft[NB_SPRITES_AIR_ATTACK];
//        SDL_Rect clipAirTakeDownAttack[NB_SPRITES_AIR_TAKE_DOWN_ATTACK];
//		SDL_Rect clipAirTakeDownAttackLeft[NB_SPRITES_AIR_TAKE_DOWN_ATTACK];
//        SDL_Rect clipAirBombAttack[NB_SPRITES_AIR_BOMB_ATTACK];
//		SDL_Rect clipAirBombAttackLeft[NB_SPRITES_AIR_BOMB_ATTACK];
//		SDL_Rect clipGetHurted[NB_SPRITES_GET_HURTED];
//		SDL_Rect clipGetHurtedLeft[NB_SPRITES_GET_HURTED];
//		SDL_Rect clipGetHurtedHard[NB_SPRITES_GET_HURTED];
//		SDL_Rect clipGetHurtedHardLeft[NB_SPRITES_GET_HURTED];
//};
//
//#endif
