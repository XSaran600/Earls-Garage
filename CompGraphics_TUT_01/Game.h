#pragma once
#include "Timer.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "GameObject.h"
#include "Light.h"
#include "PowerUp.h"
#include "ParticleEmitterSoA.h"
#include "Sound.h"

#include <vector>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FRAMES_PER_SECOND 60

#include "Timer.h"

#include "queueType.h"

enum States {
	MENU,
	PAUSE,
	GAME,
	TIME_DONE,
	WIN
};

enum Pause_States {
	PAUSE_DEFAULT,
	PAUSE_CONTINUE,
	PAUSE_OPTIONS,
	PAUSE_QUIT,
	PAUSE_OPTION_DEFAULT,
	PAUSE_OPTION_SOUND,
	PAUSE_OPTION_MUSIC,
	PAUSE_OPTION_CONTROL,
	PAUSE_OPTION_BACK,
	PAUSE_CONTROL_DEFAULT,
	PAUSE_CONTROL_BACK
};

enum Menu_States {
	MENU_DEFAULT,
	MENU_PLAY,
	MENU_LEVEL,
	MENU_OPTIONS,
	MENU_QUIT,
	MENU_LEVEL_1_DEFAULT,
	MENU_LEVEL_1_LEFT,
	MENU_LEVEL_1_RIGHT,
	MENU_LEVEL_1_PLAY,
	MENU_LEVEL_1_BACK,
	MENU_LEVEL_2_DEFAULT,
	MENU_LEVEL_2_LEFT,
	MENU_LEVEL_2_RIGHT,
	MENU_LEVEL_2_PLAY,
	MENU_LEVEL_2_BACK,
	MENU_OPTION_DEFAULT,
	MENU_OPTION_SOUND,
	MENU_OPTION_MUSIC,
	MENU_OPTION_CONTROL,
	MENU_OPTION_BACK,
	MENU_CONTROL_DEFAULT,
	MENU_CONTROL_BACK
};

enum Win_States {
	WIN_DEFAULT_0,
	WIN_MAIN_0,
	WIN_NEXT_0,
	WIN_DEFAULT_1,
	WIN_MAIN_1,
	WIN_NEXT_1, 
	WIN_DEFAULT_2,
	WIN_MAIN_2,
	WIN_NEXT_2, 
	WIN_DEFAULT_3,
	WIN_MAIN_3,
	WIN_NEXT_3,
	
};

enum Star_States {
	STAR_0,
	STAR_1,
	STAR_2,
	STAR_3,
};

enum Level_States {
	LEVEL_1_1,
	LEVEL_1_2,
};

enum Car_Move_States {
	MOVE_IN_CAR,
	MOVE_OUT_CAR,
	MOVE_IN_CUSTOMER,
	MOVE_OUT_CUSTOMER
};

enum Customer_States {
	CUSTOMER_IN_CAR,
	CUSTOMER_TO_COUCH,
	CUSTOMER_AT_COUCH,
	CUSTOMER_TO_CAR
};

class Game {
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	//Input callback fuctions
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	//Date Memmber
	Timer* updateTime = nullptr;

	GameObject map;
	GameObject workbench;
	GameObject palmtree1;
	GameObject palmtree2;
	GameObject couch;
	GameObject tireRack;
	GameObject conveyor;
	GameObject garbage;
	GameObject computer;
	GameObject paintGun;
	GameObject paintGunShow;
	GameObject carTire;
	GameObject engine;
	GameObject engineShow;
	GameObject quad;

	GameObject character;

	GameObject customer;
	GameObject bubble_Tire;
	GameObject bubble_Engine;
	GameObject bubble_Paint;
	GameObject bubble_Angry;

	// Clock
	GameObject clock;
	GameObject clock1;
	GameObject clock2;
	GameObject clock3;

	// Main Menu
	GameObject mainMenu;
	GameObject mainMenu1;
	GameObject mainMenu2;
	GameObject mainMenu3;
	GameObject mainMenu4;

	// Main Menu Level Select
	GameObject mainMenu_Level_1_1;
	GameObject mainMenu_Level_1_2;
	GameObject mainMenu_Level_1_3;
	GameObject mainMenu_Level_1_4;
	GameObject mainMenu_Level_1_5;
	GameObject mainMenu_Level_2_1;
	GameObject mainMenu_Level_2_2;
	GameObject mainMenu_Level_2_3;
	GameObject mainMenu_Level_2_4;
	GameObject mainMenu_Level_2_5;

	// Main Menu Option Screen
	GameObject mainMenuOption_Sound_ON_Music_ON;
	GameObject mainMenuOption_Sound_ON_Music_ON1;
	GameObject mainMenuOption_Sound_ON_Music_ON2;
	GameObject mainMenuOption_Sound_ON_Music_ON3;
	GameObject mainMenuOption_Sound_ON_Music_ON4;
			   
	GameObject mainMenuOption_Sound_ON_Music_OFF;
	GameObject mainMenuOption_Sound_ON_Music_OFF1;
	GameObject mainMenuOption_Sound_ON_Music_OFF2;
	GameObject mainMenuOption_Sound_ON_Music_OFF3;
	GameObject mainMenuOption_Sound_ON_Music_OFF4;
			   
	GameObject mainMenuOption_Sound_OFF_Music_ON;
	GameObject mainMenuOption_Sound_OFF_Music_ON1;
	GameObject mainMenuOption_Sound_OFF_Music_ON2;
	GameObject mainMenuOption_Sound_OFF_Music_ON3;
	GameObject mainMenuOption_Sound_OFF_Music_ON4;
			   
	GameObject mainMenuOption_Sound_OFF_Music_OFF;
	GameObject mainMenuOption_Sound_OFF_Music_OFF1;
	GameObject mainMenuOption_Sound_OFF_Music_OFF2;
	GameObject mainMenuOption_Sound_OFF_Music_OFF3;
	GameObject mainMenuOption_Sound_OFF_Music_OFF4;

	// Main Menu Control Screen
	GameObject mainMenuControl;
	GameObject mainMenuControl1;

	// Pause Screen
	GameObject pauseScreen;
	GameObject pauseScreen1;
	GameObject pauseScreen2;
	GameObject pauseScreen3;

	// Pause Option Screen
	GameObject pauseScreenOption_Sound_ON_Music_ON;
	GameObject pauseScreenOption_Sound_ON_Music_ON1;
	GameObject pauseScreenOption_Sound_ON_Music_ON2;
	GameObject pauseScreenOption_Sound_ON_Music_ON3;
	GameObject pauseScreenOption_Sound_ON_Music_ON4;

	GameObject pauseScreenOption_Sound_ON_Music_OFF;
	GameObject pauseScreenOption_Sound_ON_Music_OFF1;
	GameObject pauseScreenOption_Sound_ON_Music_OFF2;
	GameObject pauseScreenOption_Sound_ON_Music_OFF3;
	GameObject pauseScreenOption_Sound_ON_Music_OFF4;

	GameObject pauseScreenOption_Sound_OFF_Music_ON;
	GameObject pauseScreenOption_Sound_OFF_Music_ON1;
	GameObject pauseScreenOption_Sound_OFF_Music_ON2;
	GameObject pauseScreenOption_Sound_OFF_Music_ON3;
	GameObject pauseScreenOption_Sound_OFF_Music_ON4;

	GameObject pauseScreenOption_Sound_OFF_Music_OFF;
	GameObject pauseScreenOption_Sound_OFF_Music_OFF1;
	GameObject pauseScreenOption_Sound_OFF_Music_OFF2;
	GameObject pauseScreenOption_Sound_OFF_Music_OFF3;
	GameObject pauseScreenOption_Sound_OFF_Music_OFF4;

	// Pause Control Screen
	GameObject pauseScreenControl;
	GameObject pauseScreenControl1;

	// EndScreen
	GameObject winScreen_0_1; // 0 Stars
	GameObject winScreen_0_2;
	GameObject winScreen_0_3;

	GameObject winScreen_1_1; // 1 Star
	GameObject winScreen_1_2;
	GameObject winScreen_1_3;

	GameObject winScreen_2_1; // 2 Stars
	GameObject winScreen_2_2;
	GameObject winScreen_2_3;

	GameObject winScreen_3_1; // 3 Stars
	GameObject winScreen_3_2;
	GameObject winScreen_3_3;

	GameObject loseScreen; // Times up

	GameObject loadScreen;

	GameObject HUD_Tire;
	GameObject HUD_Engine;
	GameObject HUD_PaintCan;
	GameObject HUD_Face_Happy;
	GameObject HUD_Face_Normal;
	GameObject HUD_Face_Angry;
	GameObject HUD_Face_Upset;


	GameObject car1;
	GameObject car2;
	GameObject car3;
	queueType cars;
	//std::vector<GameObject> cars;

	// Particle stuff
	ParticleEmitterSoA particleEmitter;
	ParticleEmitterSoA particleEmitterSmoke;
	ParticleEmitterSoA particleEmitterPaint;
	Texture smoke;
	Texture repairPtcl;
	Texture green;
	Texture red;
	Texture blue;
	Texture paint;

	ShaderProgram passThrough;
	ShaderProgram phongNoTexture;
	ShaderProgram phong;
	ShaderProgram phongAnimation;
	ShaderProgram phongColorSides;
	ShaderProgram toonShadingAnimation;
	ShaderProgram outlineAnimation;
	ShaderProgram toonShadingStatic;
	ShaderProgram outlineStatic;
	ShaderProgram particles;
	ShaderProgram unlit;
	ShaderProgram bright;
	ShaderProgram blur;
	ShaderProgram bloom;
	ShaderProgram bloomScene;
	ShaderProgram phongNormalMap;

	// FBO
	// Fbo with bloom objects already bloomed
	FrameBufferObject fboBloomed;
	// Fbo with orginal non-bloom ojects
	FrameBufferObject fboNonBloom;
	// Fbo with orginal bloom objects 
	FrameBufferObject fboBloom;
	FrameBufferObject fboBrightPass;
	FrameBufferObject fboBlurredBrightPass;
	FrameBufferObject fboBlurredBrightPass1;


	// Bloom
	void brightPass();
	void blurBrightPass();

	Light directionalLight;
	std::vector<Light> pointLights;

	PowerUp characterBuffs;

	glm::mat4 cameraTransform;
	glm::mat4 cameraProjection;
	glm::mat4 lightSpinner;

	bool One_Dimensional_Check(float x1, float x2, float x3, float x4);
	bool Collision_AABB(GameObject &other, GameObject &character);
	bool Nearby_AABB(GameObject &other, GameObject &character);

	void carMovement(float deltaTime);
	void repair(float deltaTime);
	void movement();
	void animation();
	void reset();
	void powerup(float deltaTime);
	void checkPauseDraw();
	void checkMainDraw();
	void mouseClickedMenu();
	void nextLevel();
	void checkLevel();
	void clockTimerDraw();
	void customerTimer(float deltaTime);
	void drawGame();
	void starCalc();

	void initShaders();
	void initParticles();
	void initMap();
	void initCharacter();
	void initCars();
	void initCustomer();
	void initMenu();
	void initPause();
	void initClock();
	void initFBO();

private:
	bool shouldLightSpin = false;
	bool aKeydown = false;
	bool sKeydown = false;
	bool dKeydown = false;
	bool wKeydown = false;
	bool spaceKeydown = false;
	bool zKeydown = false;
	bool eKeydown = false;
	bool qKeydown = false;
	bool num1 = false;
	bool num2 = false;
	bool num3 = false;
	bool leftClick = false;

	// Repair Timer bools
	bool startTimerTire = false;
	bool startTimerEngine = false;
	bool hasEngine = false;
	bool startTimerWorkbench = false;
	bool startTimerTireCar = false;
	bool startTimerEngineCar = false;
	bool startTimerWorkbenchCar = false;

	// Animation bools
	bool repairAnimation = false;
	bool tireRackAnimation = false;
	bool changeTireAnimation = false;
	bool computerAnimation = false;
	bool paintAnimation = false;
	bool engineAnimation = false;
	bool workbenchAnimation = false;


	bool move = true;

	float engineDistance = 0.f;

	glm::mat4 camera;

	// States
	int currentState = MENU;
	int pauseState = PAUSE_DEFAULT;
	int menuState = MENU_DEFAULT;
	int levelState = LEVEL_1_1;
	int tempLevelState = LEVEL_1_1;
	int winState = WIN_DEFAULT_0;
	int starState = STAR_0;

	// Car&Customer movement
	int carState = MOVE_IN_CAR;
	int customerState = CUSTOMER_IN_CAR;
	float carSpeed = 0.2;
	float customerSpeed = 0.1;
	float carOutside = 26.f;
	float carInside = 3.f;
	float customerOut = 6.5f;
	float customerIn = 3.f;
	bool carOnce = false;

	// Main Menu bools
	bool mainLevel = false;
	bool mainLevel_1 = true;
	bool mainOptions = false;
	bool mainControls = false;

	// Pause Menu bools
	bool pauseOptions = false;
	bool pauseControls = false;
	bool sound_ON_OR_OFF = true;
	bool music_ON_OR_OFF = true;

	// Customer Timer 
	float angry_startTime;
	float angry_endTime;
	float angry_totalTime = 15;
	float angry_tempTime;
	bool angry_start = true;
	int angry_tempRepair;
	bool displayAngry = false;
	// Customer Display Reapir Timer
	float display_endTime;
	float display_totalTime = 2;
	bool displayRepair = false;


	// Star Calculations
	int starCalculations = 0;
	bool starOnceAngry = false;
	bool starOnceTimer1 = false;
	bool starOnceTimer2 = false;

	//Sound Engine
	SoundEngine sys;
	Sound sound;
	Sound hammerSound;
	Sound tireSound;
	Sound typingSound;
	Sound paintSound;
	Sound bellSound;
	Sound clickSound;
	Sound smokeSound;
	Sound beltSound;
	Sound itemSound;
	Sound customerSound;

	bool soundStart = true;
	bool soundSwitch = true;
	bool SfxCheck = true;
	bool winCheck = true;
	bool typeCheck = true;
	bool beltCheck = true;
	bool itemCheck = true;
	bool custCheck = true;
	bool smokeCheck = true;
	bool smokeSoundPlaying = false;

	//Sound Channel
	FMOD::Channel *channel = 0;
	FMOD::Channel *channel2 = 0;
	FMOD::Channel *channel3 = 0;
	FMOD::Channel *channel4 = 0;
	FMOD::Channel *channel5 = 0;
	FMOD::Channel *channel6 = 0;
	FMOD::Channel *channel7 = 0;		//Smoke channel #420
	FMOD::Channel *channel8 = 0;		//Dialogue channel

	FMOD_RESULT		result;
	FMOD_VECTOR drumPos;		//Player
	FMOD_VECTOR drumPos2;		//Car
	FMOD_VECTOR drumPos3;		//Computer
	FMOD_VECTOR drumPos4;		//Smoke
	FMOD_VECTOR drumPos5;		//Conveyor Belt
	FMOD_VECTOR drumPos6;		//Customer
	FMOD_VECTOR drumVel;


};