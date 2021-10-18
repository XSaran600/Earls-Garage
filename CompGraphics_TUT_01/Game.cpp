#include "Game.h"
#include <iostream>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Game::Game() {

}
Game::~Game() {
	delete updateTime;

	passThrough.unload();
}
//happends once at the beginning of the game
void Game::initializeGame() {
	updateTime = new Timer();

	glEnable(GL_DEPTH_TEST);

	Light light1, light2;
	// White Light
	light1.positionOrDirection = glm::vec4(0.f, 10.f, 0.f, 1.f);
	light1.originalPosition = light1.positionOrDirection;
	light1.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	light1.diffuse = glm::vec3(1.f, 1.f, 1.f);
	light1.specular = glm::vec3(1.f, 1.f, 1.f);
	light1.specularExponent = 50.f;
	light1.constantAttenuation = 1.f;
	light1.linearAttenuation = 0.1f;
	light1.quadraticAttenuation = 0.01f;

	// Red Light
	light2.positionOrDirection = glm::vec4(0.f, 0.f, 1.f, 1.f);
	light2.originalPosition = light2.positionOrDirection;
	light2.ambient = glm::vec3(0.0f, 0.0f, 0.0f); // Colour
	light2.diffuse = glm::vec3(1.f, 1.f, 1.f); // How strong the colour should be
	light2.specular = glm::vec3(1.f, 1.f, 1.f);
	light2.specularExponent = 50.f;
	light2.constantAttenuation = 1.f;
	light2.linearAttenuation = 0.1f;
	light2.quadraticAttenuation = 0.01f;

	pointLights.push_back(light1);
	pointLights.push_back(light2);

	directionalLight.positionOrDirection = glm::vec4(-1.f, -1.f, -1.f, 1.f);
	directionalLight.ambient = glm::vec3(0.0f);
	directionalLight.diffuse = glm::vec3(1.f);
	directionalLight.specular = glm::vec3(1.f);
	directionalLight.specularExponent = 50.f;

	// Camera
	cameraTransform = glm::translate(cameraTransform, glm::vec3(0.f, 0.f, -19.f));
	cameraTransform = glm::rotate(cameraTransform, glm::radians(35.f), glm::vec3(1.f, 0.f, 0.f));
	camera = cameraTransform;
	cameraProjection = glm::perspective(45.f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 10000.f);
	//0.1 for near plane
	//10000 for far plane

	// Load Screen
	Mesh Load;
	loadScreen = GameObject(Load);
	loadScreen.loadMesh("meshes/UI/Menu.obj");
	loadScreen.loadTexture(TextureType::Diffuse, "textures/Company_Logo.png");
	loadScreen.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	cameraTransform = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -4.f));

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	initShaders();
	loadScreen.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
	glutSwapBuffers();

	cameraTransform = camera;

	sys.Init();

	sound = Sound(&sys);				//Music sound object.
	hammerSound = Sound(&sys);			//SFX sound object.
	tireSound = Sound(&sys);			//SFX sound object.
	paintSound = Sound(&sys);			//SFX sound object.
	typingSound = Sound(&sys);			//SFX sound object.
	bellSound = Sound(&sys);			//SFX sound object.
	clickSound = Sound(&sys);			//SFX sound object.
	smokeSound = Sound(&sys);			//SFX sound object.
	beltSound = Sound(&sys);			//SFX sound object.
	itemSound = Sound(&sys);			//SFX sound object.
	customerSound = Sound(&sys);		//Dialogue SFX sound object.

	sys.listener.pos.x = 0.0f;
	sys.listener.pos.y = 0.0f;
	sys.listener.pos.z = 0.0f;		//Places the FMOD listener object. (Object that picks up sound from sound sources)  **Default centered position**

	result = sys.system->set3DListenerAttributes(0, &sys.listener.pos, &sys.listener.vel, &sys.listener.forward, &sys.listener.up);
	FmodErrorCheck(result);

	//Load a sound
	sound.load("media/main_menu.wav", true, true);
	hammerSound.load("media/Hammer-Metal.wav", true, false);
	tireSound.load("media/Tire_Repair.wav", true, false);
	paintSound.load("media/Paint_Gun_Spray.wav", true, false);
	typingSound.load("media/Typing.wav", true, false);
	clickSound.load("media/Mouse_Click.wav", true, false);
	bellSound.load("media/Engine_Bell.wav", true, false);
	smokeSound.load("media/smoke.wav", true, true);
	beltSound.load("media/ConveyorBelt.wav", true, false);
	itemSound.load("media/itemPick.mp3", true, false);
	customerSound.load("media/customer.wav", true, false);

	//Place the sound
	drumPos.x = 0.0f;			//Player sound source position.
	drumPos.y = 0.0f;
	drumPos.z = 0.0f;

	drumPos2.x = 0.0f;			//Car sound source position.
	drumPos2.y = 0.0f;
	drumPos2.z = 0.0f;

	drumPos3.x = -9.0f;			//Computer sound source position.
	drumPos3.y = 0.0f;
	drumPos3.z = -5.5f;	

	drumPos4.x =  -500.0f;			//Smoke sound source position.
	drumPos4.y =  0.0f;
	drumPos4.z =  -500.0f;

	drumPos5.x =  -7.39f;			//Conveyor belt sound source position.
	drumPos5.y =  0.1f;
	drumPos5.z =  1.56f;

	drumPos6.x = customer.position.x;			//Customer sound source position.
	drumPos6.y = customer.position.y;
	drumPos6.z = customer.position.z;

	drumVel.x = 0.0f;		//Sets velocity to 0 as movement depends on the 3D objects in the scene instead. 
	drumVel.y = 0.0f;
	drumVel.z = 0.0f;

	// Initialize Objects
	initParticles();
	initMap();
	initFBO();
	initCharacter();
	initCars();
	initCustomer();
	initMenu();
	reset();

	PowerUp CharacterBuffs;
}

bool Game::One_Dimensional_Check(float x1, float x2, float x3, float x4)
{
	if (x1 >= x3 && x1 <= x4)
	{
		return true;
	}
	if (x1 <= x3 && x1 >= x4)
	{
		return true;
	}
	else if (x2 >= x3 && x2 <= x4)
	{
		return true;
	}
	else if (x2 <= x3 && x2 >= x4)
	{
		return true;
	}
	else if (x3 >= x1 && x3 <= x2)
	{
		return true;
	}
	else if (x3 <= x1 && x3 >= x2)
	{
		return true;
	}
	else if (x4 >= x1 && x4 <= x2)
	{
		return true;
	}
	else if (x4 <= x1 && x4 >= x2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::Collision_AABB(GameObject &other, GameObject &character)
{
	float otherLeft = other.position.x - other.left;
	float otherRight = other.position.x + other.right;
	float otherDown = other.position.z + other.down;
	float otherUp = other.position.z - other.up;

	float left = character.position.x - character.left;
	float right = character.position.x + character.right;
	float down = character.position.z + character.down;
	float up = character.position.z - character.up;

	bool check_x = One_Dimensional_Check(left, right, otherLeft, otherRight);
	bool check_y = One_Dimensional_Check(down, up, otherDown, otherUp);

	//std::cout << down << " " << up << " " << otherDown << " " << otherUp << std::endl;
	//std::cout << check_y << std::endl;

	if (check_x == true && check_y == true)
	{
		//std::cout << "true" << std::endl;
		return true;
	}
	else
	{
		//std::cout << "false" << std::endl;
		return false;
	}
}

bool Game::Nearby_AABB(GameObject &other, GameObject &character)
{
	float otherLeft = other.position.x - other.left - 0.5f;
	float otherRight = other.position.x + other.right + 0.5f;
	float otherDown = other.position.z + other.down + 0.5f;
	float otherUp = other.position.z - other.up - 0.5f;

	float left = character.position.x - character.left;
	float right = character.position.x + character.right;
	float down = character.position.z + character.down;
	float up = character.position.z - character.up;

	bool check_x = One_Dimensional_Check(left, right, otherLeft, otherRight);
	bool check_y = One_Dimensional_Check(down, up, otherDown, otherUp);

	//std::cout << down << " " << up << " " << otherDown << " " << otherUp << std::endl;
	//std::cout << check_y << std::endl;

	if (check_x == true && check_y == true)
	{
		//std::cout << "true" << std::endl;
		return true;
	}
	else
	{
		//std::cout << "false" << std::endl;
		return false;
	}
}

//Happens once per fram, used to update state of the game
void Game::update() {

	//update timer so we have correct delta time since last update
	updateTime->tick();
	float deltaTime = updateTime->getElapsedTimeSeconds();


	
	if (soundStart != false)
	{
		//Play sound
		channel = sound.play();
		sound.updatePosition(channel, drumPos, drumVel);

		soundStart = false;
	}
	
	if (currentState == TIME_DONE)
	{
		cameraTransform = camera;

		if (spaceKeydown || leftClick)
		{
			cameraTransform = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -4.f));
			currentState = WIN;
			starState = STAR_0;
			winState = WIN_DEFAULT_0;
		}
	}
	else if (currentState == MENU)
	{
		cameraTransform = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -4.f));
		
		if (soundSwitch == true)
		{
			//sound.load("media/main_menu.wav", true, true);
			soundSwitch = false;
		}
	}
	else if (currentState == PAUSE)
	{
		cameraTransform = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -4.f));
	}
	else if (currentState == WIN)
	{
		cameraTransform = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -4.f));

	}
	else if (currentState == GAME)
	{
		if (soundSwitch == true)
		{
			//sound.load("media/game_BG.wav");
			//soundSwitch = false;
		}
		

		cameraTransform = camera;

		// Checks the level
		checkLevel();

		// Updates Level Timer
		character.levelTimer(deltaTime);
		if (character.gameDone)
		{
			starCalculations = starCalculations + 10;
			currentState = TIME_DONE;
		}

		// Does the repair 
		repair(deltaTime);

		// Checks if the customer is angry
		customerTimer(deltaTime);

		// Power-Up Update
		powerup(deltaTime);

		// Character movement update
		movement();

		// Animation Check
		animation();
		
		// Updates Animation
		character.updateAnimation(deltaTime);
		customer.updateAnimation(deltaTime);
		
		// Moves Car and Customer in and out of the scene
		carMovement(deltaTime);

		// Checks if you win
		if (cars.front() == cars.back() && cars.front()->repairDone)
		{
			// Calculates how many stars you earn
			starCalc();

			//Play win effect
			if (winCheck == true)
			{
				channel4 = bellSound.play();
				bellSound.updatePosition(channel4, drumPos, drumVel);

				winCheck = false;
			}

			currentState = WIN;
		}

		// Resets all repair timer bools
		if (cars.front()->repairDone == true && carState == MOVE_OUT_CAR)
		{
			startTimerTire = false;
			startTimerEngine = false;
			startTimerWorkbench = false;
			hasEngine = false;
			startTimerEngineCar = false;
			startTimerTireCar = false;
			startTimerWorkbenchCar = false;
			repairAnimation = false;
			cars.front()->repairDone = false;
		}

		bubble_Engine.translate = glm::translate(glm::mat4(), customer.position);
		bubble_Engine.translate = glm::translate(bubble_Engine.translate, glm::vec3(1.5f, 2.f, 0.f));
		bubble_Paint.translate = glm::translate(glm::mat4(), customer.position);
		bubble_Paint.translate = glm::translate(bubble_Paint.translate, glm::vec3(1.5f, 2.f, 0.f));
		bubble_Tire.translate = glm::translate(glm::mat4(), customer.position);
		bubble_Tire.translate = glm::translate(bubble_Tire.translate, glm::vec3(1.5f, 2.f, 0.f));
		bubble_Angry.translate = glm::translate(glm::mat4(), customer.position);
		bubble_Angry.translate = glm::translate(bubble_Angry.translate, glm::vec3(1.5f, 2.f, 0.f));

		particleEmitter.initialPosition = character.position + glm::vec3(0.f, 1.5f, 2.f);
		particleEmitter.update(deltaTime);

		particleEmitterSmoke.initialPosition = cars.front()->position + glm::vec3(0.0f, 0.0f, 2.0f);
		particleEmitterSmoke.update(deltaTime);

		particleEmitterPaint.update(deltaTime);
	}

	drumPos.x = character.position.x;
	drumPos.y = character.position.y;
	drumPos.z = character.position.z;

	sys.listener.pos.x = character.position.x;
	sys.listener.pos.y = character.position.y;
	sys.listener.pos.z = character.position.z;

	sound.updatePosition(channel, drumPos, drumVel);			//Updates the sound position.

	drumPos6.x = customer.position.x;							//Customer sound source position.
	drumPos6.y = customer.position.y;
	drumPos6.z = customer.position.z;
	
	sys.update();
	
	

}

void Game::draw() {
	
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	if (currentState == TIME_DONE)
	{
		drawGame();
		
	}
	if (currentState == WIN)
	{
		if (starState == STAR_0)// 0 stars
		{
			if (winState == WIN_MAIN_0) // Main
			{
				winScreen_0_2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (winState == WIN_NEXT_0) // Next
			{
				winScreen_0_3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (winState == WIN_DEFAULT_0)
			{
				winScreen_0_1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
		}
		else if (starState == STAR_1)// 1 stars
		{
			if (winState == WIN_MAIN_1) // Main
			{
				winScreen_1_2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (winState == WIN_NEXT_1) // Next
			{
				winScreen_1_3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (winState == WIN_DEFAULT_1)
			{
				winScreen_1_1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
		}
		else if (starState == STAR_2)// 2 stars
		{
			if (winState == WIN_MAIN_2) // Main
			{
				winScreen_2_2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (winState == WIN_NEXT_2) // Next
			{
				winScreen_2_3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (winState == WIN_DEFAULT_2)
			{
				winScreen_2_1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
		}
		else if (starState == STAR_3)// 3 stars
		{
			if (winState == WIN_MAIN_3) // Main
			{
				winScreen_3_2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (winState == WIN_NEXT_3) // Next
			{
				winScreen_3_3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (winState == WIN_DEFAULT_3)
			{
				winScreen_3_1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
		}

	}
	if (currentState == PAUSE)
	{
		// Pause Screen
		if (pauseState == PAUSE_DEFAULT)
		{
			pauseScreen.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_CONTINUE)
		{
			pauseScreen1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTIONS)
		{
			pauseScreen2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_QUIT)
		{
			pauseScreen3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_CONTROL_DEFAULT) // Pause Control Screen
		{
			pauseScreenControl.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_CONTROL_BACK)
		{
			pauseScreenControl1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else // Pause Options
		{
			checkPauseDraw();
		}
	}
	if (currentState == MENU)
	{
		if (menuState == MENU_DEFAULT)
		{
			mainMenu.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_PLAY)
		{
			mainMenu1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL)
		{
			mainMenu2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTIONS)
		{
			mainMenu3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_QUIT)
		{
			mainMenu4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_1_DEFAULT) // Level Select
		{
			mainMenu_Level_1_1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_1_BACK)
		{
			mainMenu_Level_1_2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_1_LEFT)
		{
			mainMenu_Level_1_3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_1_PLAY)
		{
			mainMenu_Level_1_4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_1_RIGHT)
		{
			mainMenu_Level_1_5.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_2_DEFAULT)
		{
			mainMenu_Level_2_1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_2_BACK)
		{
			mainMenu_Level_2_2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_2_LEFT)
		{
			mainMenu_Level_2_3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_2_PLAY)
		{
			mainMenu_Level_2_4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_LEVEL_2_RIGHT)
		{
			mainMenu_Level_2_5.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_CONTROL_DEFAULT) // Main Menu Control Screen
		{
			mainMenuControl.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_CONTROL_BACK)
		{
			mainMenuControl1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else // Main Menu Options
		{
			checkMainDraw();
		}
	}
	if (currentState == GAME)
	{
		drawGame();
	}
	
	

	glutSwapBuffers();
	
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY) {
	switch (key) {
	case 27: //esc key
		break;
	case 'q':
		//exit(1);
		break;
	case 't':
		std::cout << "Total elapsed time: " << updateTime->getCurrentTime() / 1000.f << std::endl;
		break;
	case 'a':
		aKeydown = true;
		break;
	case 's':
		sKeydown = true;
		break;
	case 'd':
		dKeydown = true;
		break;
	case 'w':
		wKeydown = true;
		break;
	case ' ':
		spaceKeydown = true;
		break;
	case 'z':
		zKeydown = true;
		break;
	case 'e':
		eKeydown = true;
		break;
	case '1': // Left
		cameraTransform = glm::translate(cameraTransform, glm::vec3(0.1f, 0.f, 0.f));
		break;
	case '2': // Back
		cameraTransform = glm::translate(cameraTransform, glm::vec3(0.f, 0.f, -0.1f));
		break;
	case '3': // Right
		cameraTransform = glm::translate(cameraTransform, glm::vec3(-0.1f, 0.f, 0.f));
		break;
	case '5': // Forward
		cameraTransform = glm::translate(cameraTransform, glm::vec3(0.f, 0.f, 0.1f));
		break;
	case '-': // Down
		cameraTransform = glm::translate(cameraTransform, glm::vec3(0.f, 0.1f, 0.f));
		break;
	case '+': // Up
		cameraTransform = glm::translate(cameraTransform, glm::vec3(0.f, -0.1f, 0.f));
		break;
	case '4': // Rotate Left
		cameraTransform = glm::rotate(cameraTransform, glm::radians(0.1f), glm::vec3(0.f, -1.f, 0.f));
		break;
	case '6': // Rotate Right
		cameraTransform = glm::rotate(cameraTransform, glm::radians(0.1f), glm::vec3(0.f, 1.f, 0.f));
		break;
	case '7': // Rotate Down 
		cameraTransform = glm::rotate(cameraTransform, glm::radians(0.1f), glm::vec3(1.f, 0.f, 0.f));
		break;
	case '8': // Rotate Up
		cameraTransform = glm::rotate(cameraTransform, glm::radians(0.1f), glm::vec3(-1.f, 0.f, 0.f));
		break;
	case '0': // Reset Camera
		cameraTransform = camera;
		break;
	default:
		break;
	}
}
void Game::keyboardUp(unsigned char key, int mouseX, int mouseY) {
	switch (key)
	{
	case 'a':
		aKeydown = false;
		break;
	case 's':
		sKeydown = false;
		break;
	case 'd':
		dKeydown = false;
		break;
	case 'w':
		wKeydown = false;
		break;
	case ' ':
		spaceKeydown = false;
		break;
	case 'r':
		shouldLightSpin = !shouldLightSpin;
		break;
	case 'x':
		qKeydown = !qKeydown;
		break;
	case 'e':
		eKeydown = false;
		break;
	case 'z':
		zKeydown = false;
	case 27:
		if (currentState == PAUSE)
		{
			currentState = GAME;
			pauseOptions = false;
		}
		else if (currentState == GAME)
		currentState = PAUSE;
		break;
	default:
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y) 
{

	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			//handle left click
			mouseClickedMenu();
			if (currentState == TIME_DONE)
			{
				leftClick = true;
			}
			else
			{
				leftClick = false;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			//handle right click
			std::cout << "X: " << x << " Y: " << y << std::endl;
			break;
		case GLUT_MIDDLE_BUTTON:
			//handle middle mouse
			break;
		default:
			break;
		}
	}
}
void Game::mouseMoved(int x, int y) {
	//std::cout << "X: " << x << " Y: " << y << std::endl;

	if (currentState == PAUSE)
	{
		// Main pause menu
		if (!pauseOptions)
		{
			if (x > 520 && x < 760 && y > 260 && y < 300) // Continue
			{
				pauseState = PAUSE_CONTINUE;
			}
			else if (x > 540 && x < 740 && y > 375 && y < 422) // Options
			{
				pauseState = PAUSE_OPTIONS;
			}
			else if (x > 585 && x < 700 && y > 495 && y < 542) // Quit
			{
				pauseState = PAUSE_QUIT;
			}
			else
			{
				pauseState = PAUSE_DEFAULT;
			}
		}
		else
		{
			// Options pause menu
			if (!pauseControls)
			{
				if (x > 723 && x < 785 && y > 185 && y < 247) // Sound
				{
					pauseState = PAUSE_OPTION_SOUND;
				}
				else if (x > 723 && x < 785 && y > 307 && y < 368) // Music
				{
					pauseState = PAUSE_OPTION_MUSIC;
				}
				else if (x > 534 && x < 755 && y > 430 && y < 471) // Controls
				{
					pauseState = PAUSE_OPTION_CONTROL;
				}
				else if (x > 343 && x < 427 && y > 599 && y < 619) // Back
				{
					pauseState = PAUSE_OPTION_BACK;
				}
				else
				{
					pauseState = PAUSE_OPTION_DEFAULT;
				}
			}
			else // Controls pause menu
			{
				if (x > 343 && x < 427 && y > 599 && y < 619) // Back
				{
					pauseState = PAUSE_CONTROL_BACK;
				}
				else
				{
					pauseState = PAUSE_CONTROL_DEFAULT;
				}
			}
			
		}
	}
	if (currentState == MENU)
	{
		if (mainLevel) // Level Select
		{
			if (mainLevel_1)
			{
				if (x > 430 && x < 489 && y > 221 && y < 327) // Level 1 Left
				{
					menuState = MENU_LEVEL_1_LEFT;
				}
				else if (x > 796 && x < 864 && y > 211 && y < 325) // Level 1 Right
				{
					menuState = MENU_LEVEL_1_RIGHT;
				}
				else if (x > 606 && x < 678 && y > 402 && y < 423) // Level 1 Play
				{
					menuState = MENU_LEVEL_1_PLAY;
				}
				else if (x > 603 && x < 684 && y > 465 && y < 489) // Level 1 Back
				{
					menuState = MENU_LEVEL_1_BACK;
				}
				else
				{
					menuState = MENU_LEVEL_1_DEFAULT;
				}
			}
			else
			{
				if (x > 430 && x < 489 && y > 221 && y < 327) // Level 2 Left
				{
					menuState = MENU_LEVEL_2_LEFT;
				}
				else if (x > 796 && x < 864 && y > 211 && y < 325) // Level 2 Right
				{
					menuState = MENU_LEVEL_2_RIGHT;
				}
				else if (x > 606 && x < 678 && y > 402 && y < 423) // Level 2 Play
				{
					menuState = MENU_LEVEL_2_PLAY;
				}
				else if (x > 603 && x < 684 && y > 465 && y < 489) // Level 2 Back
				{
					menuState = MENU_LEVEL_2_BACK;
				}
				else
				{
					menuState = MENU_LEVEL_2_DEFAULT;
				}
			}
		}
		else if (mainOptions) // Options main menu
		{
			if (mainControls) // Controls menu
			{
				if (x > 594 && x < 655 && y > 470 && y < 496) // Back
				{
					menuState = MENU_CONTROL_BACK;
				}
				else
				{
					menuState = MENU_CONTROL_DEFAULT;
				}
			}
			else // Options main menu
			{
				if (x > 666 && x < 695 && y > 236 && y < 263) // Sound
				{
					menuState = MENU_OPTION_SOUND;
				}
				else if (x > 666 && x < 695 && y > 314 && y < 343) // Music
				{
					menuState = MENU_OPTION_MUSIC;
				}
				else if (x > 575 && x < 674 && y > 393 && y < 418) // Controls
				{
					menuState = MENU_OPTION_CONTROL;
				}
				else if (x > 594 && x < 654 && y > 470 && y < 495) // Back
				{
					menuState = MENU_OPTION_BACK;
				}
				else
				{
					menuState = MENU_OPTION_DEFAULT;
				}
			}
		}
		else // Main Menu
		{
			if (x > 598 && x < 660 && y > 170 && y < 214) // Play
			{
				menuState = MENU_PLAY;
			}
			else if (x > 554 && x < 703 && y > 267 && y < 314) // Level Select
			{
				menuState = MENU_LEVEL;
			}
			else if (x > 580 && x < 680 && y > 369 && y < 434) // Options
			{
				menuState = MENU_OPTIONS;
			}
			else if (x > 597 && x < 660 && y > 478 && y < 512) // Quit
			{
				menuState = MENU_QUIT;
			}
			else
			{
				menuState = MENU_DEFAULT;
			}
		}
	}
	if (currentState == WIN)
	{
		if (starState == STAR_0)// 0 stars
		{
			if (x > 434 && x < 607 && y > 584 && y < 608) // Main
			{
				winState = WIN_MAIN_0;
			}
			else if (x > 684 && x < 845 && y > 589 && y < 609) // Next
			{
				winState = WIN_NEXT_0;
			}
			else
			{
				winState = WIN_DEFAULT_0;
			}
		}
		else if (starState == STAR_1)// 1 stars
		{
			if (x > 434 && x < 607 && y > 584 && y < 608) // Main
			{
				winState = WIN_MAIN_1;
			}
			else if (x > 684 && x < 845 && y > 589 && y < 609) // Next
			{
				winState = WIN_NEXT_1;
			}
			else
			{
				winState = WIN_DEFAULT_1;
			}
		}
		else if (starState == STAR_2)// 2 stars
		{
			if (x > 434 && x < 607 && y > 584 && y < 608) // Main
			{
				winState = WIN_MAIN_2;
			}
			else if (x > 684 && x < 845 && y > 589 && y < 609) // Next
			{
				winState = WIN_NEXT_2;
			}
			else
			{
				winState = WIN_DEFAULT_2;
			}
		}
		else if (starState == STAR_3)// 3 stars
		{
			if (x > 434 && x < 607 && y > 584 && y < 608) // Main
			{
				winState = WIN_MAIN_3;
			}
			else if (x > 684 && x < 845 && y > 589 && y < 609) // Next
			{
				winState = WIN_NEXT_3;
			}
			else
			{
				winState = WIN_DEFAULT_3;
			}
		}
		
	}
}
void Game::mouseClickedMenu()
{
	// When the LEFT MOUSE BUTTON is clicked

	// Main Menu
	if (currentState == MENU)
	{
		if (menuState == MENU_PLAY)
		{
			soundSwitch = true;
			currentState = GAME;
		}
		if (menuState == MENU_QUIT)
		{
			exit(1);
		}
		// Menu Level Select
		if (menuState == MENU_LEVEL)
		{
			// When level select is click go to the level select menu
			menuState = MENU_LEVEL_1_DEFAULT;
			mainLevel_1 = true;
			mainLevel = true;
		}
		if (menuState == MENU_LEVEL_1_LEFT || menuState == MENU_LEVEL_1_RIGHT)
		{
			menuState = MENU_LEVEL_2_DEFAULT;
			mainLevel_1 = false;
		}
		else if (menuState == MENU_LEVEL_2_LEFT || menuState == MENU_LEVEL_2_RIGHT)
		{
			menuState = MENU_LEVEL_1_DEFAULT;
			mainLevel_1 = true;
		}
		if (menuState == MENU_LEVEL_1_BACK || menuState == MENU_LEVEL_2_BACK)
		{
			// When BACK is click in the level select menu 
			// go back to the main menu
			menuState = MENU_DEFAULT;
			mainLevel = false;
		}
		if (menuState == MENU_LEVEL_1_PLAY)
		{
			levelState = LEVEL_1_1;
			currentState = GAME;
		}
		if (menuState == MENU_LEVEL_2_PLAY)
		{
			levelState = LEVEL_1_2;
			currentState = GAME;
		}
		// Main Menu Options
		if (menuState == MENU_OPTIONS)
		{
			// When option is clicked go to the option menu
			menuState = MENU_OPTION_DEFAULT;
			mainOptions = true;
		}
		if (menuState == MENU_OPTION_BACK)
		{
			// When BACK is click in the option menu 
			// go back to the main menu
			menuState = MENU_DEFAULT;
			mainOptions = false;
		}
		if (menuState == MENU_OPTION_SOUND)
		{
			// Changes if the sound is on or off
			sound_ON_OR_OFF = !sound_ON_OR_OFF;
		}
		if (menuState == MENU_OPTION_MUSIC)
		{
			// Changes if the music is on or off
			music_ON_OR_OFF = !music_ON_OR_OFF;
		}

		// Main Menu Controls
		if (menuState == MENU_OPTION_CONTROL)
		{
			// When controls is click go to the control menu
			menuState = MENU_CONTROL_DEFAULT;
			mainControls = true;
		}
		if (menuState == MENU_CONTROL_BACK)
		{
			// When BACK is click in the control menu 
			// go back to the option menu
			menuState = MENU_OPTION_DEFAULT;
			mainControls = false;
		}
	}
	// Pause Menu
	else if (currentState == PAUSE)
	{
		// Main Pause
		if (pauseState == PAUSE_CONTINUE)
		{
			currentState = GAME;
		}
		if (pauseState == PAUSE_QUIT)
		{
			exit(1);
		}
		// Pause Options
		if (pauseState == PAUSE_OPTIONS)
		{
			// When option is clicked go to the option menu
			pauseState = PAUSE_OPTION_DEFAULT;
			pauseOptions = true;
		}
		if (pauseState == PAUSE_OPTION_BACK)
		{
			// When BACK is click in the option menu 
			// go back to the pause menu
			pauseState = PAUSE_DEFAULT;
			pauseOptions = false;
		}
		if (pauseState == PAUSE_OPTION_SOUND)
		{
			// Changes if the sound is on or off
			sound_ON_OR_OFF = !sound_ON_OR_OFF;
		}
		if (pauseState == PAUSE_OPTION_MUSIC)
		{
			// Changes if the music is on or off
			music_ON_OR_OFF = !music_ON_OR_OFF;
		}

		// Pause Controls
		if (pauseState == PAUSE_OPTION_CONTROL)
		{
			// When controls is click go to the control menu
			pauseState = PAUSE_CONTROL_DEFAULT;
			pauseControls = true;
		}
		if (pauseState == PAUSE_CONTROL_BACK)
		{
			// When BACK is click in the control menu 
			// go back to the option menu
			pauseState = PAUSE_OPTION_DEFAULT;
			pauseControls = false;
		}
	}
	else if (currentState == WIN)
	{
		if (winState == WIN_MAIN_0 || winState == WIN_MAIN_1 || winState == WIN_MAIN_2 || winState == WIN_MAIN_3)
		{
			reset(); // Resets Variables
			menuState = MENU_DEFAULT;
			currentState = MENU; // Returns to main menu
		}
		if (winState == WIN_NEXT_0) // Redo Level
		{
			int tempLevel = levelState; // Saves current level
			reset(); // Resets Variables
			levelState = tempLevel; // Sets the level back to orginal level
			checkLevel(); // Checks if level has changed
			cameraTransform = camera;
			currentState = GAME; // Returns to game
		}
		if (winState == WIN_NEXT_1 || winState == WIN_NEXT_2 || winState == WIN_NEXT_3) // Go to next level
		{
			if (levelState == LEVEL_1_1)
			{
				std::cout << "Level 1" << std::endl;
				reset();
				levelState = LEVEL_1_1;
				nextLevel();
				cameraTransform = camera;
				currentState = GAME; // Returns to game
			}
			else
			{
				std::cout << "Level 2" << std::endl;
				reset();
				levelState = LEVEL_1_2;
				nextLevel();
			}
		}
	}
}

void Game::carMovement(float deltaTime)
{
	if (Collision_AABB(*cars.front(), character)) // Check and move
	{
		character.translate = glm::translate(character.translate, glm::vec3(-1 * character.playerSpeed, 0.f, 0.f));
		character.position = character.position + glm::vec3(-1 * character.playerSpeed, 0.f, 0.f);
		character.rotate = glm::rotate(glm::mat4(), glm::radians(270.f), glm::vec3(0.f, 1.f, 0.f)); // Look left
		character.previousPosition = character.position;
	}
	if (Collision_AABB(customer, character)) // Check and move
	{
		character.translate = glm::translate(character.translate, glm::vec3(0.f, 0.f, character.playerSpeed));
		character.position = character.position + glm::vec3(0.f, 0.f, character.playerSpeed);
		character.rotate = glm::rotate(glm::mat4(), glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f)); // Look forward
		character.previousPosition = character.position;
	}

	// Car moves into scene
	if (carState == MOVE_IN_CAR)
	{
		if (cars.front()->position.z > carInside)
		{
			cars.front()->translate = glm::translate(cars.front()->translate, glm::vec3(0.f, 0.f, carSpeed * -1));
			cars.front()->position = cars.front()->position + glm::vec3(0.f, 0.f, carSpeed * -1);

			customer.translate = cars.front()->translate;
			customer.position = cars.front()->position;
			customerState = CUSTOMER_IN_CAR;
		}
		else
		{
			drumPos2.x = cars.front()->position.x;
			drumPos2.y = cars.front()->position.y;
			drumPos2.z = cars.front()->position.z;

			carState = MOVE_IN_CUSTOMER;
		}
	}
	// Customer moves to couch
	if (carState == MOVE_IN_CUSTOMER)
	{
		if (customer.position.x < customerOut)
		{
			customer.translate = glm::translate(customer.translate, glm::vec3(customerSpeed, 0.f, 0.f));
			customer.position = customer.position + glm::vec3(customerSpeed, 0.f, 0.f);
			customerState = CUSTOMER_TO_COUCH;

		}
		else if (cars.front()->repairDone)
		{
			carOnce = false;
			carState = MOVE_OUT_CUSTOMER;
		}
		else
		{
			customerState = CUSTOMER_AT_COUCH;
		}
	}
	// Customer moves to car
	if (carState == MOVE_OUT_CUSTOMER)
	{
		if (customer.position.x > customerIn)
		{
			customer.translate = glm::translate(customer.translate, glm::vec3(customerSpeed * -1, 0.f, 0.f));
			customer.position = customer.position + glm::vec3(customerSpeed * -1, 0.f, 0.f);
			customerState = CUSTOMER_TO_CAR;
		}
		else
		{
			carState = MOVE_OUT_CAR;
		}
	}
	// Car moves out of scene
	if (carState == MOVE_OUT_CAR)
	{
		if (cars.front()->position.z < carOutside)
		{
			cars.front()->translate = glm::translate(cars.front()->translate, glm::vec3(0.f, 0.f, carSpeed));
			cars.front()->position = cars.front()->position + glm::vec3(0.f, 0.f, carSpeed);

			customer.translate = cars.front()->translate;
			customer.position = cars.front()->position;
			customerState = CUSTOMER_IN_CAR;
		}
		else
		{
			// Switchs to next car
			if (cars.front() != cars.back())
			{
				cars.deleteQueue();
			}
			carState = MOVE_IN_CAR;
		}
	}
	
}

void Game::repair(float deltaTime)
{
	// Checks if repair needs to be done
	if (!cars.front()->repairDone)
	{
		if (cars.front()->currentRepair == TIRE)
		{

			if (startTimerTire) // Checks if player pressed space near the tireRack
			{
				if (!tireRack.timeDone) // Checks if the timer isn't done
				{
					move = false;
					tireRackAnimation = true;
					tireRack.timer(deltaTime, 3);
				}
				if (tireRack.timeDone == true) // Tire is done and are able to move
				{
					if (itemCheck == true)
					{
						channel3 = itemSound.play();
						itemSound.updatePosition(channel3, drumPos, drumVel);

						itemCheck = false;
					}

					tireRackAnimation = false;
					move = true;
				}
				if (Nearby_AABB(*cars.front(), character) && tireRack.timeDone == true && startTimerTireCar) // Checks if player is near car and if tireRack is done and has click space
				{
					move = false;
					changeTireAnimation = true;
					carTire.timer(deltaTime, 3);

					//Play sound effect
					if (SfxCheck == true)
					{
						channel2 = tireSound.play();
						hammerSound.updatePosition(channel2, drumPos2, drumVel);

						SfxCheck = false;
					}
				}
				if (carTire.timeDone == true) // Checks if carTire repair is done
				{
					changeTireAnimation = false;
					move = true;
					tireRack.timeDone = false;
					carTire.timeDone = false;
					startTimerTireCar = false;
					startTimerTire = false;
					repairAnimation = false;
					cars.front()->repairDone = true;

					SfxCheck = true;
					itemCheck = true;
					custCheck = true;
				}
			}
		}
		if (cars.front()->currentRepair == ENGINE)
		{

			drumPos4.x = 0.0f;			//Smoke sound source position.
			drumPos4.y = 0.0f;
			drumPos4.z = 0.0f;

			if (smokeCheck == true)
			{
				channel7 = smokeSound.play();
				smokeSound.updatePosition(channel7, drumPos4, drumVel);

				smokeSoundPlaying = true;
				smokeCheck = false;
			}

			if (startTimerEngine) // Checks if player pressed space near the Computer
			{
				if (!computer.timeDone) // Checks if the timer isn't done
				{
					move = false;
					computerAnimation = true;
					computer.timer(deltaTime, 1);

					//Play sound effect
					if (typeCheck == true)
					{
						channel3 = typingSound.play();
						typingSound.updatePosition(channel3, drumPos3, drumVel);

						channel4 = clickSound.play();
						clickSound.updatePosition(channel4, drumPos3, drumVel);

						typeCheck = false;
					}
				}
				if (computer.timeDone == true) // Computer is done and are able to move
				{
					computerAnimation = false;
					move = true;

					if (beltCheck == true)
					{
						channel5 = beltSound.play();
						beltSound.updatePosition(channel5, drumPos5, drumVel);

						beltCheck = false;
					}

					typeCheck = true;
				}
				if (Nearby_AABB(*cars.front(), character) && hasEngine && startTimerEngineCar) // Checks if player is near car and if the player has the engine and has clicked space
				{
					move = false;
					engineAnimation = true;
					engine.timer(deltaTime, 1);

					//Play sound effect
					if (SfxCheck == true)
					{
						channel2 = hammerSound.play();
						hammerSound.updatePosition(channel2, drumPos2, drumVel);

						SfxCheck = false;
					}
				}
				if (engine.timeDone == true) // Checks if engine repair is done
				{
					engineAnimation = false;
					move = true;
					computer.timeDone = false;
					engine.timeDone = false;
					startTimerEngineCar = false;
					hasEngine = false;
					startTimerTire = false;
					repairAnimation = false;
					cars.front()->repairDone = true;

					drumPos4.x = -500.0f;				//Smoke sound source position.
					drumPos4.y = 0.0f;
					drumPos4.z = -500.0f;

					smokeSound.updatePosition(channel7, drumPos4, drumVel);
					
					smokeCheck = true;
					SfxCheck = true;
					custCheck = true;
				}
			}
		}
		if (cars.front()->currentRepair == PAINT)
		{

			if (startTimerWorkbench) // Checks if player pressed space near the workbench
			{
				if (!workbench.timeDone) // Checks if the timer isn't done
				{
					move = false;
					workbenchAnimation = true;
					workbench.timer(deltaTime, 2);
				}
				if (workbench.timeDone == true) // workbench is done and are able to move
				{
					if (itemCheck == true)
					{
						channel3 = itemSound.play();
						itemSound.updatePosition(channel3, drumPos, drumVel);

						itemCheck = false;
					}

					workbenchAnimation = false;
					move = true;
				}
				if (Nearby_AABB(*cars.front(), character) && workbench.timeDone == true && startTimerWorkbenchCar) // Checks if player is near car and if workbench is done and has click space
				{
					move = false;
					paintAnimation = true;
					paintGun.timer(deltaTime, 2);

					//Play sound effect
					if (SfxCheck == true)
					{
						channel2 = paintSound.play();
						hammerSound.updatePosition(channel2, drumPos2, drumVel);

						SfxCheck = false;
					}
				}
				if (paintGun.timeDone == true) // Checks if carTire repair is done
				{
					cars.front()->loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/YellowCar_Texture.png");
					paintAnimation = false;
					move = true;
					workbench.timeDone = false;
					paintGun.timeDone = false;
					startTimerWorkbenchCar = false;
					startTimerWorkbench = false;
					repairAnimation = false;
					cars.front()->repairDone = true;

					SfxCheck = true;
					itemCheck = true;
					custCheck = true;
				}
			}
		}

	}

	if (computer.timeDone && !hasEngine && engineDistance <= 3.f) // Engine conveyor belt 
	{
		engine.translate = glm::translate(engine.translate, glm::vec3(0.2f, 0.f, 0.f));
		engineDistance = engineDistance + 0.2f;
	}
}

void Game::movement()
{

	// KEYBOARD PRESSES
	if (move)
	{
		if (aKeydown)
		{
			character.translate = glm::translate(character.translate, glm::vec3(-1 * character.playerSpeed, 0.f, 0.f));
			character.position = character.position + glm::vec3(-1 * character.playerSpeed, 0.f, 0.f);
			character.rotate = glm::rotate(glm::mat4(), glm::radians(270.f), glm::vec3(0.f, 1.f, 0.f)); // Look left

			particleEmitterPaint.initialForceMin = glm::vec3(-5.0f, -1.0f, -1.0f);
			particleEmitterPaint.initialForceMax = glm::vec3(-4.0f, 1.0f, 1.0f);

			carTire.rotate = glm::rotate(glm::mat4(), glm::radians(270.f), glm::vec3(0.f, 1.f, 0.f));
			engineShow.rotate = glm::rotate(glm::mat4(), glm::radians(270.f), glm::vec3(0.f, 1.f, 0.f));
			paintGunShow.rotate = glm::rotate(glm::mat4(), glm::radians(270.f), glm::vec3(0.f, 1.f, 0.f));

			carTire.translate = glm::translate(glm::mat4(), character.position);
			engineShow.translate = glm::translate(glm::mat4(), character.position);
			paintGunShow.translate = glm::translate(glm::mat4(), character.position);
			carTire.translate = glm::translate(carTire.translate, glm::vec3(-0.5f, 1.f, 0.f));
			engineShow.translate = glm::translate(engineShow.translate, glm::vec3(-1.f, 1.f, 0.f));
			paintGunShow.translate = glm::translate(paintGunShow.translate, glm::vec3(-0.5f, 1.f, 0.f));
			particleEmitterPaint.initialPosition = character.position + glm::vec3(-0.5f, 1.f, 0.f);
		}
		if (dKeydown)
		{
			character.translate = glm::translate(character.translate, glm::vec3(character.playerSpeed, 0.f, 0.f));
			character.position = character.position + glm::vec3(character.playerSpeed, 0.f, 0.f);
			character.rotate = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f)); // Look right

			particleEmitterPaint.initialForceMin = glm::vec3(5.0f, -1.0f, -1.0f);
			particleEmitterPaint.initialForceMax = glm::vec3(4.0f, 1.0f, 1.0f);

			carTire.rotate = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
			engineShow.rotate = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
			paintGunShow.rotate = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));

			carTire.translate = glm::translate(glm::mat4(), character.position);
			engineShow.translate = glm::translate(glm::mat4(), character.position);
			paintGunShow.translate = glm::translate(glm::mat4(), character.position);
			carTire.translate = glm::translate(carTire.translate, glm::vec3(0.5f, 1.f, 0.f));
			engineShow.translate = glm::translate(engineShow.translate, glm::vec3(1.f, 1.f, 0.f));
			paintGunShow.translate = glm::translate(paintGunShow.translate, glm::vec3(0.5f, 1.f, 0.f));
			particleEmitterPaint.initialPosition = character.position + glm::vec3(0.5f, 1.f, 0.f);
		}
		if (Collision_AABB(workbench, character) || // Checks for Collision
			Collision_AABB(couch, character) ||
			Collision_AABB(tireRack, character) ||
			Collision_AABB(conveyor, character) ||
			Collision_AABB(garbage, character) ||
			Collision_AABB(computer, character) ||
			Collision_AABB(workbench, character) ||
			Collision_AABB(*cars.front(), character) ||
			(Collision_AABB(customer, character)) ||
			!Collision_AABB(map, character))
		{
			character.translate = glm::translate(glm::mat4(), character.previousPosition);
			character.position = character.previousPosition;

		}
		else
		{
			character.previousPosition = character.position;
		}
		if (sKeydown)
		{
			character.translate = glm::translate(character.translate, glm::vec3(0.f, 0.f, character.playerSpeed));
			character.position = character.position + glm::vec3(0.f, 0.f, character.playerSpeed);
			character.rotate = glm::rotate(glm::mat4(), glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f)); // Look forward

			particleEmitterPaint.initialForceMin = glm::vec3(-1.0f, -1.0f, 5.0f);
			particleEmitterPaint.initialForceMax = glm::vec3(1.0f, 1.0f, 4.0f);

			carTire.rotate = glm::rotate(glm::mat4(), glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
			engineShow.rotate = glm::rotate(glm::mat4(), glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
			paintGunShow.rotate = glm::rotate(glm::mat4(), glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));

			carTire.translate = glm::translate(glm::mat4(), character.position);
			engineShow.translate = glm::translate(glm::mat4(), character.position);
			paintGunShow.translate = glm::translate(glm::mat4(), character.position);
			carTire.translate = glm::translate(carTire.translate, glm::vec3(0.f, 1.f, 0.5f));
			engineShow.translate = glm::translate(engineShow.translate, glm::vec3(0.f, 1.f, 1.f));
			paintGunShow.translate = glm::translate(paintGunShow.translate, glm::vec3(0.f, 1.f, 0.5f));
			particleEmitterPaint.initialPosition = character.position + glm::vec3(0.f, 1.f, 0.5f);
		}
		if (wKeydown)
		{
			character.translate = glm::translate(character.translate, glm::vec3(0.f, 0.f, -1 * character.playerSpeed));
			character.position = character.position + glm::vec3(0.f, 0.f, -1 * character.playerSpeed);
			character.rotate = glm::rotate(glm::mat4(), glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f)); // Look back
			
			particleEmitterPaint.initialForceMin = glm::vec3(-1.0f, -1.0f, -5.0f);
			particleEmitterPaint.initialForceMax = glm::vec3(1.0f, 1.0f, -4.0f);

			carTire.rotate = glm::rotate(glm::mat4(), glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
			engineShow.rotate = glm::rotate(glm::mat4(), glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
			paintGunShow.rotate = glm::rotate(glm::mat4(), glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

			carTire.translate = glm::translate(glm::mat4(), character.position);
			engineShow.translate = glm::translate(glm::mat4(), character.position);
			paintGunShow.translate = glm::translate(glm::mat4(), character.position);
			carTire.translate = glm::translate(carTire.translate, glm::vec3(0.f, 1.f, -0.5f));
			engineShow.translate = glm::translate(engineShow.translate, glm::vec3(0.f, 1.f, -0.5f));
			paintGunShow.translate = glm::translate(paintGunShow.translate, glm::vec3(0.f, 1.f, -0.5f));
			particleEmitterPaint.initialPosition = character.position + glm::vec3(0.f, 1.f, -0.5f);
		}
		if (Collision_AABB(workbench, character) || // Checks for Collision
			Collision_AABB(couch, character) ||
			Collision_AABB(tireRack, character) ||
			Collision_AABB(conveyor, character) ||
			Collision_AABB(garbage, character) ||
			Collision_AABB(computer, character) ||
			Collision_AABB(workbench, character) ||
			Collision_AABB(*cars.front(), character) ||
			(Collision_AABB(customer, character)) ||
			!Collision_AABB(map, character))
		{
			character.translate = glm::translate(glm::mat4(), character.previousPosition);
			character.position = character.previousPosition;


		}
		else
		{
			character.previousPosition = character.position;
		}
		if (sKeydown && dKeydown)
		{
			character.rotate = glm::rotate(glm::mat4(), glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f)); // Look back-right

		}
		if (sKeydown && aKeydown)
		{
			character.rotate = glm::rotate(glm::mat4(), glm::radians(315.f), glm::vec3(0.f, 1.f, 0.f)); // Look back-left

		}
		if (wKeydown && dKeydown)
		{
			character.rotate = glm::rotate(glm::mat4(), glm::radians(135.f), glm::vec3(0.f, 1.f, 0.f)); // Look forward-right

		}
		if (wKeydown && aKeydown)
		{
			character.rotate = glm::rotate(glm::mat4(), glm::radians(225.f), glm::vec3(0.f, 1.f, 0.f)); // Look forward-left
		}
	}

	if (spaceKeydown)
	{
		if (carState == MOVE_IN_CUSTOMER && !cars.front()->repairDone) 
		{
			if (!cars.front()->repairDone) // Checks if repair is not done
			{
				if (cars.front()->currentRepair == TIRE)
				{
					if (Nearby_AABB(tireRack, character)) // Tire repair check
					{
						startTimerTire = true;
					}
					else if (Nearby_AABB(*cars.front(), character) && startTimerTire) // Car tire check
					{
						repairAnimation = true;
						startTimerTireCar = true;
					}
				}
				if (cars.front()->currentRepair == ENGINE)
				{
					if (Nearby_AABB(computer, character)) // Computer check
					{
						startTimerEngine = true;
					}
					else if (Nearby_AABB(conveyor, character) && computer.timeDone && startTimerEngine) // Conveyor belt check
					{
						if (itemCheck == true)
						{
							channel3 = itemSound.play();
							itemSound.updatePosition(channel3, drumPos, drumVel);

							itemCheck = false;
						}

						hasEngine = true;
					}
					else if (Nearby_AABB(*cars.front(), character) && hasEngine) // Car engine check
					{
						repairAnimation = true;
						startTimerEngineCar = true;
						itemCheck = true;
					}
				}
				if (cars.front()->currentRepair == PAINT)
				{
					if (Nearby_AABB(workbench, character)) // Workbench check
					{
						startTimerWorkbench = true;
					}
					else if (Nearby_AABB(*cars.front(), character) && startTimerWorkbench) // Car paintgun check
					{
						repairAnimation = true;
						startTimerWorkbenchCar = true;
					}
				}

			}
		}
	}
}

void Game::animation()
{
	if ((wKeydown || aKeydown || sKeydown || dKeydown) && move)
	{
		character.playAnimation("Walk");
	}
	else if ((!wKeydown && !aKeydown && !sKeydown && !dKeydown) && move)
	{
		character.playAnimation("Idle");
	}
	else if (tireRackAnimation) // Picking up TireRack Animation
	{
		character.playAnimation("TireRack");
	}
	else if (changeTireAnimation) // Changing the Tire Animation
	{
		character.playAnimation("TireRepair");
	}
	else if (engineAnimation) // Engine Repair Animation
	{
		character.playAnimation("Repair");
	}
	else if (computerAnimation) // Ordering computer Animation
	{
		character.playAnimation("Computer");
	}
	else if (paintAnimation) // Painting car Animation
	{
		character.playAnimation("Paint");
	}
	else if (workbenchAnimation) // Picking up paint Animation
	{
		character.playAnimation("Pickup_Paint");
	}

	if (customerState == CUSTOMER_IN_CAR)
	{
		customer.playAnimation("CarSit");
	}
	else if (customerState == CUSTOMER_TO_COUCH)
	{
		customer.playAnimation("GetOut");
	}
	else if (customerState == CUSTOMER_AT_COUCH)
	{
		customer.playAnimation("CouchSit");
	}
	else if (customerState == CUSTOMER_TO_CAR)
	{
		customer.playAnimation("GetIn");
	}
}

void Game::reset()
{
	character.resetTimer();

	shouldLightSpin = false;
	aKeydown = false;
	sKeydown = false;
	dKeydown = false;
	wKeydown = false;
	spaceKeydown = false;
	zKeydown = false;
	eKeydown = false;
	qKeydown = false;
	num1 = false;
	num2 = false;
	num3 = false;
	leftClick = false;

	// Repair Timer bools
	startTimerTire = false;
	startTimerEngine = false;
	hasEngine = false;
	startTimerWorkbench = false;
	startTimerTireCar = false;
	startTimerEngineCar = false;
	startTimerWorkbenchCar = false;

	// Animation bools
	repairAnimation = false;
	tireRackAnimation = false;
	changeTireAnimation = false;
	computerAnimation = false;
	paintAnimation = false;
	engineAnimation = false;
	workbenchAnimation = false;

	move = true;

	engineDistance = 0.f;

	// States
	currentState = MENU;
	pauseState = PAUSE_DEFAULT;
	menuState = MENU_DEFAULT;
	levelState = LEVEL_1_1;
	tempLevelState = LEVEL_1_1;
	winState = WIN_DEFAULT_0;
	starState = STAR_0;

	// Car&Customer movement
	carState = MOVE_IN_CAR;
	customerState = CUSTOMER_IN_CAR;
	carSpeed = 0.2;
	customerSpeed = 0.1;
	carOutside = 26.f;
	carInside = 3.f;
	customerOut = 6.5f;
	customerIn = 3.f;
	carOnce = false;

	// Main Menu bools
	mainLevel = false;
	mainLevel_1 = true;
	mainOptions = false;
	mainControls = false;

	// Pause Menu bools
	pauseOptions = false;
	pauseControls = false;
	sound_ON_OR_OFF = true;
	music_ON_OR_OFF = true;

	// Customer Timer 
	angry_startTime;
	angry_endTime;
	angry_totalTime = 15;
	angry_tempTime;
	angry_start = true;
	angry_tempRepair;
	displayAngry = false;
	// Customer Display Reapir Timer
	display_endTime;
	display_totalTime = 2;
	displayRepair = false;

	// Star Calculations
	starCalculations = 0;
	starOnceAngry = false;
	starOnceTimer1 = false;
	starOnceTimer2 = false;

	// Sound
	soundSwitch = true;
	SfxCheck = true;
	winCheck = true;
	typeCheck = true;
	beltCheck = true;
	itemCheck = true;
	custCheck = true;
	smokeCheck = true;

	if (smokeSoundPlaying == true)
	{
		drumPos4.x = -500.0f;			//Smoke sound source position.
		drumPos4.y = 0.0f;
		drumPos4.z = -500.0f;

		smokeSound.updatePosition(channel7, drumPos4, drumVel);
	}

	// Gameobject Repair Timer Bools
	tireRack.timeDone = false;
	carTire.timeDone = false;
	workbench.timeDone = false;
	paintGun.timeDone = false;
	computer.timeDone = false;
	engine.timeDone = false;


	// Set character animation to idle
	character.playAnimation("Idle");

	// Car Reset
	car1.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/BlueCar_Texture.png");
	car1.currentRepair = TIRE;
	car1.repairDone = false;
	car1.position = glm::vec3(0.f, 0.f, carOutside);
	car1.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
	car1.left = 3.f;
	car1.right = 3.f;
	car1.down = 3.5f;
	car1.up = 5.f;

	car2.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/GreenCar_Texture.png");
	car2.currentRepair = PAINT;
	car2.repairDone = false;
	car2.position = glm::vec3(0.f, 0.f, carOutside);
	car2.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
	car2.left = 3.f;
	car2.right = 3.f;
	car2.down = 3.5f;
	car2.up = 5.f;

	car3.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/DarkCar_Texture.png");
	car3.currentRepair = ENGINE;
	car3.repairDone = false;
	car3.position = glm::vec3(0.f, 0.f, carOutside);
	car3.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
	car3.left = 3.f;
	car3.right = 3.f;
	car3.down = 3.5f;
	car3.up = 5.f;

	cars.destroyQueue();

	cars.addQueue(&car1);
	cars.addQueue(&car2);
	cars.addQueue(&car3);

	// Position Resets
	customer.position = glm::vec3(customerIn, 0.f, carOutside);
	customer.translate = glm::translate(glm::mat4(), customer.position);

	character.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -9.f));
	character.rotate = glm::rotate(glm::mat4(), glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f)); // Look forward
	character.position = glm::vec3(0.f, 0.f, -9.f);
	character.previousPosition = glm::vec3(0.f, 0.f, -9.f);

	// Map
	paintGun.translate = glm::translate(glm::mat4(), glm::vec3(7.f, 1.5f, -9.f));
	paintGunShow.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 2.5f, -9.f));
	carTire.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -9.f));
	engineShow.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 2.5f, -9.f));
	engine.translate = glm::translate(glm::mat4(), glm::vec3(-11.f, 2.5f, 0.f));
	
	// HUD what the character is holding
	HUD_Tire.translate = glm::translate(glm::mat4(), glm::vec3(-0.8f, 5.25f, 6.8f));
	HUD_Tire.rotate = glm::rotate(HUD_Tire.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_Engine.translate = glm::translate(glm::mat4(), glm::vec3(-0.8f, 5.25f, 6.8f));
	HUD_Engine.rotate = glm::rotate(HUD_Engine.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_PaintCan.translate = glm::translate(glm::mat4(), glm::vec3(-0.8f, 5.25f, 6.8f));
	HUD_PaintCan.rotate = glm::rotate(HUD_PaintCan.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));

	// HUD how angry the customer is
	HUD_Face_Happy.translate = glm::translate(glm::mat4(), glm::vec3(0.8f, 5.25f, 6.8f));
	HUD_Face_Happy.rotate = glm::rotate(HUD_Face_Happy.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_Face_Normal.translate = glm::translate(glm::mat4(), glm::vec3(0.8f, 5.25f, 6.8f));
	HUD_Face_Normal.rotate = glm::rotate(HUD_Face_Normal.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_Face_Upset.translate = glm::translate(glm::mat4(), glm::vec3(0.8f, 5.25f, 6.8f));
	HUD_Face_Upset.rotate = glm::rotate(HUD_Face_Upset.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_Face_Angry.translate = glm::translate(glm::mat4(), glm::vec3(0.8f, 5.25f, 6.8f));
	HUD_Face_Angry.rotate = glm::rotate(HUD_Face_Angry.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));

}

void Game::powerup(float deltaTime)
{
	if (num1)
	{
		character.energyDrink = true;

		if (character.energyDrink == false)
		{
			std::cout << "No energy drink available." << std::endl;
		}
		num1 = false;
	}
	//Power up activation checks.
	if (character.energyDrink == true)
	{
		characterBuffs.activateBuff(character, BUFF_SPEED, deltaTime);
	}

	if (num2 == true)
	{
		std::cout << "repairDown called." << std::endl;
		characterBuffs.activateBuff(character, BUFF_REPAIR, deltaTime);
		num2 = false;
	}

	if (num3 == true)
	{
		std::cout << "LevelTimeUp called." << std::endl;
		characterBuffs.activateBuff(character, BUFF_LEVEL, deltaTime);
		num3 = false;
	}
}

void Game::initShaders()
{
	if (!phongNoTexture.load("shaders/Phong.vert", "shaders/PhongNoTexture.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}
	if (!phong.load("shaders/Phong.vert", "shaders/Phong.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}
	if (!phongColorSides.load("shaders/PhongColorSides.vert", "shaders/PhongColorSides.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}
	if (!phongAnimation.load("shaders/PhongAnimation.vert", "shaders/PhongAnimation.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}
	if (!toonShadingAnimation.load("shaders/ToonShading.vert", "shaders/ToonShading.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}
	if (!outlineAnimation.load("shaders/ToonShading.vert", "shaders/OutlineBlack.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}
	if (!toonShadingStatic.load("shaders/Phong.vert", "shaders/ToonShading.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}
	if (!outlineStatic.load("shaders/Phong.vert", "shaders/Outline.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}

	if (!particles.load("shaders/passthrough.vert", "shaders/particles_g.glsl", "shaders/unlitTexture_f.glsl")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}

	if (!unlit.load("shaders/default_v.glsl", "shaders/unlitTexture_f.glsl")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}

	if (!bright.load("shaders/default_v.glsl", "shaders/bright_f.glsl")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}

	if (!blur.load("shaders/default_v.glsl", "shaders/gaussianBlur_f.glsl")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}

	if (!bloom.load("shaders/default_v.glsl", "shaders/bloomComposite_f.glsl")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}

	if (!bloomScene.load("shaders/default_v.glsl", "shaders/bloomSceneComposite_f.glsl")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}

	if (!phongNormalMap.load("shaders/Phong.vert", "shaders/PhongNormalMap.frag")) {
		std::cout << "Shaders failed to initialzed" << std::endl;
		system("pause");
		exit(0);
	}
}

void Game::initParticles()
{
	smoke.load("textures/smoke_256_dm.png");
	green.load("textures/green.png");
	red.load("textures/red.png");
	blue.load("textures/blue.png");
	repairPtcl.load("textures/sparkle.png");
	paint.load("textures/Paint.png");

	particleEmitter.lifeRange = glm::vec3(1.0f, 2.0f, 0.0f);
	particleEmitter.initialForceMin = glm::vec3(-2.0f, 0.0f, -2.0f);
	particleEmitter.initialForceMax = glm::vec3(2.0f, 2.0f, 2.0f);
	particleEmitter.initialize(100);
	particleEmitter.play();

	particleEmitterSmoke.lifeRange = glm::vec3(1.0f, 2.0f, 0.0f);
	particleEmitterSmoke.initialForceMin = glm::vec3(-2.0f, 5.0f, -2.0f);
	particleEmitterSmoke.initialForceMax = glm::vec3(2.0f, 6.0f, 2.0f);
	particleEmitterSmoke.initialize(1000);
	particleEmitterSmoke.play();

	particleEmitterPaint.lifeRange = glm::vec3(1.0f, 2.0f, 0.0f);
	particleEmitterPaint.initialForceMin = glm::vec3(-4.0f, 0.0f, -1.0f);
	particleEmitterPaint.initialForceMax = glm::vec3(-3.0f, 5.0f, 1.0f);
	particleEmitterPaint.initialize(1000);
	particleEmitterPaint.play();
}

void Game::initMap()
{
	Mesh Map;
	map = GameObject(Map);
	map.loadMesh("meshes/Map/Room.obj");
	map.loadTexture(TextureType::Diffuse, "textures/Map/Room.png");
	map.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
	map.loadTexture(TextureType::Normal, "textures/Normal Maps/RoomNormalMap.png");

	Mesh Palmtree1;
	palmtree1 = GameObject(Palmtree1);
	palmtree1.loadMesh("meshes/Map/PalmTree1.obj");
	palmtree1.loadTexture(TextureType::Diffuse, "textures/Map/PalmTree1.png");
	palmtree1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Palmtree2;
	palmtree2 = GameObject(Palmtree2);
	palmtree2.loadMesh("meshes/Map/PalmTree2.obj");
	palmtree2.loadTexture(TextureType::Diffuse, "textures/Map/PalmTree2.png");
	palmtree2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Couch;
	couch = GameObject(Couch);
	couch.loadMesh("meshes/Map/Couch.obj");
	couch.loadTexture(TextureType::Diffuse, "textures/Map/Couch.png");
	couch.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh TireRack;
	tireRack = GameObject(TireRack);
	tireRack.loadMesh("meshes/Map/TireRack.obj");
	tireRack.loadTexture(TextureType::Diffuse, "textures/Map/TireRack.png");
	tireRack.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Conveyor;
	conveyor = GameObject(Conveyor);
	conveyor.loadMesh("meshes/Map/Conveyor.obj");
	conveyor.loadTexture(TextureType::Diffuse, "textures/Map/Conveyor.png");
	conveyor.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Garbage;
	garbage = GameObject(Garbage);
	garbage.loadMesh("meshes/Map/Garbage.obj");
	garbage.loadTexture(TextureType::Diffuse, "textures/Map/Garbage.png");
	garbage.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Computer;
	computer = GameObject(Computer);
	computer.loadMesh("meshes/Map/Computer.obj");
	computer.loadTexture(TextureType::Diffuse, "textures/Map/Computer.png");
	computer.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh PaintGun;
	paintGun = GameObject(PaintGun);
	paintGun.loadMesh("meshes/Map/Paint_Gun.obj");
	paintGun.loadTexture(TextureType::Diffuse, "textures/Map/Paint_Gun_diffuse.png");
	paintGun.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh PaintGunShow;
	paintGunShow = GameObject(PaintGunShow);
	paintGunShow.loadMesh("meshes/Map/Paint_Gun.obj");
	paintGunShow.loadTexture(TextureType::Diffuse, "textures/Map/Paint_Gun_diffuse.png");
	paintGunShow.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Workbench;
	workbench = GameObject(Workbench);
	workbench.loadMesh("meshes/Map/Workbench.obj");
	workbench.loadTexture(TextureType::Diffuse, "textures/Map/Workbench.png");
	workbench.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh CarTire;
	carTire = GameObject(CarTire);
	carTire.loadMesh("meshes/Map/CarTire.obj");
	carTire.loadTexture(TextureType::Diffuse, "textures/Map/CarTire.png");
	carTire.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Engine;
	engine = GameObject(Engine);
	engine.loadMesh("meshes/Map/Engine.obj");
	engine.loadTexture(TextureType::Diffuse, "textures/Map/Gray_Ash.png");
	engine.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh EngineShow;
	engineShow = GameObject(EngineShow);
	engineShow.loadMesh("meshes/Map/Engine.obj");
	engineShow.loadTexture(TextureType::Diffuse, "textures/Map/Gray_Ash.png");
	engineShow.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	// HUD
	Mesh hud1;
	HUD_Tire = GameObject(hud1);
	HUD_Tire.loadMesh("meshes/UI/HUD.obj");
	HUD_Tire.loadTexture(TextureType::Diffuse, "textures/HUD/Tire.png");
	HUD_Tire.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh hud2;
	HUD_Engine = GameObject(hud2);
	HUD_Engine.loadMesh("meshes/UI/HUD.obj");
	HUD_Engine.loadTexture(TextureType::Diffuse, "textures/HUD/Engine.png");
	HUD_Engine.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh hud3;
	HUD_PaintCan = GameObject(hud3);
	HUD_PaintCan.loadMesh("meshes/UI/HUD.obj");
	HUD_PaintCan.loadTexture(TextureType::Diffuse, "textures/HUD/Paint_Can.png");
	HUD_PaintCan.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh hud4;
	HUD_Face_Happy = GameObject(hud4);
	HUD_Face_Happy.loadMesh("meshes/UI/HUD.obj");
	HUD_Face_Happy.loadTexture(TextureType::Diffuse, "textures/HUD/Happy.png");
	HUD_Face_Happy.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh hud5;
	HUD_Face_Normal = GameObject(hud5);
	HUD_Face_Normal.loadMesh("meshes/UI/HUD.obj");
	HUD_Face_Normal.loadTexture(TextureType::Diffuse, "textures/HUD/Normal.png");
	HUD_Face_Normal.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh hud6;
	HUD_Face_Upset = GameObject(hud6);
	HUD_Face_Upset.loadMesh("meshes/UI/HUD.obj");
	HUD_Face_Upset.loadTexture(TextureType::Diffuse, "textures/HUD/Upset.png");
	HUD_Face_Upset.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh hud7;
	HUD_Face_Angry = GameObject(hud7);
	HUD_Face_Angry.loadMesh("meshes/UI/HUD.obj");
	HUD_Face_Angry.loadTexture(TextureType::Diffuse, "textures/HUD/Angry.png");
	HUD_Face_Angry.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	map.translate = glm::translate(map.translate, glm::vec3(0.f, 0.f, 0.f));
	map.position = glm::vec3(0.f, 0.f, 0.f);
	map.left = 9.f;
	map.right = 9.f;
	map.down = 9.f;
	map.up = 9.f;

	couch.translate = glm::translate(couch.translate, glm::vec3(7.5f, 0.f, 3.5f));
	couch.position = glm::vec3(7.5f, 0.f, 3.5f);
	couch.left = .3f;
	couch.right = 1.9f;
	couch.down = 2.7f;
	couch.up = 2.7f;

	tireRack.translate = glm::translate(tireRack.translate, glm::vec3(-10.f, 0.f, -10.f));
	tireRack.position = glm::vec3(-10.f, 0.f, -10.f);
	tireRack.left = 0.f;
	tireRack.right = 4.f;
	tireRack.down = 1.5f;
	tireRack.up = 0.f;

	conveyor.translate = glm::translate(conveyor.translate, glm::vec3(-7.39f, 0.1f, 1.56f));
	conveyor.position = glm::vec3(-7.39f, 0.1f, 1.56f);
	conveyor.left = 3.3f;
	conveyor.right = 0.f;
	conveyor.down = 0.f;
	conveyor.up = 3.3f;

	garbage.translate = glm::translate(garbage.translate, glm::vec3(8.5f, 0.1f, 7.5f));
	garbage.position = glm::vec3(8.5f, 0.1f, 7.5f);
	garbage.left = 1.f;
	garbage.right = 1.f;
	garbage.down = 0.5f;
	garbage.up = 0.5f;

	computer.translate = glm::translate(computer.translate, glm::vec3(-9.f, 0.f, 5.5f));
	computer.position = glm::vec3(-9.f, 0.f, 5.5f);
	computer.left = 0.75f;
	computer.right = 0.75f;
	computer.down = 0.75f;
	computer.up = 0.75f;

	workbench.translate = glm::translate(workbench.translate, glm::vec3(7.f, 0.f, -9.f));
	workbench.position = glm::vec3(7.f, 0.f, -9.f);
	workbench.left = 2.5f;
	workbench.right = 2.5f;
	workbench.down = 1.f;
	workbench.up = 1.f;

	palmtree1.translate = glm::translate(palmtree1.translate, glm::vec3(18.f, 0.f, -10.f));
	palmtree2.translate = glm::translate(palmtree2.translate, glm::vec3(-18.f, 0.f, -10.f));
	paintGun.translate = glm::translate(paintGun.translate, glm::vec3(7.f, 1.5f, -9.f));
	paintGunShow.translate = glm::translate(engineShow.translate, glm::vec3(0.f, 2.5f, -9.f));
	carTire.translate = glm::translate(carTire.translate, glm::vec3(0.f, 0.f, -9.f));
	engineShow.translate = glm::translate(engineShow.translate, glm::vec3(0.f, 2.5f, -9.f));
	engine.translate = glm::translate(engine.translate, glm::vec3(-11.f, 2.5f, 0.f));

	//HUD_Tire.translate = glm::translate(HUD_Tire.translate, glm::vec3(-6.4f, 4.f, 11.4f));
	//HUD_Engine.translate = glm::translate(HUD_Engine.translate, glm::vec3(-6.4f, 4.f, 11.4f));
	//HUD_PaintCan.translate = glm::translate(HUD_PaintCan.translate, glm::vec3(-6.4f, 4.f, 11.4f));
	

	HUD_Tire.translate = glm::translate(glm::mat4(), glm::vec3(-0.8f, 5.25f, 6.8f));
	HUD_Tire.rotate = glm::rotate(HUD_Tire.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_Engine.translate = glm::translate(glm::mat4(), glm::vec3(-0.8f, 5.25f, 6.8f));
	HUD_Engine.rotate = glm::rotate(HUD_Engine.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_PaintCan.translate = glm::translate(glm::mat4(), glm::vec3(-0.8f, 5.25f, 6.8f));
	HUD_PaintCan.rotate = glm::rotate(HUD_PaintCan.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));

	HUD_Face_Happy.translate = glm::translate(glm::mat4(), glm::vec3(0.8f, 5.25f, 6.8f));
	HUD_Face_Happy.rotate = glm::rotate(HUD_Face_Happy.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_Face_Normal.translate = glm::translate(glm::mat4(), glm::vec3(0.8f, 5.25f, 6.8f));
	HUD_Face_Normal.rotate = glm::rotate(HUD_Face_Normal.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_Face_Upset.translate = glm::translate(glm::mat4(), glm::vec3(0.8f, 5.25f, 6.8f));
	HUD_Face_Upset.rotate = glm::rotate(HUD_Face_Upset.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));
	HUD_Face_Angry.translate = glm::translate(glm::mat4(), glm::vec3(0.8f, 5.25f, 6.8f));
	HUD_Face_Angry.rotate = glm::rotate(HUD_Face_Angry.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));

	initClock();
}

void Game::initCharacter()
{
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Walk_1.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Walk_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Walk_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Walk_4.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Walk_5.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Walk_6.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Walk_7.obj");
	character.setAnimations("Walk", 7);

	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_1.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_4.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_5.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_6.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_7.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_8.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_9.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_10.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_11.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_12.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_13.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_14.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_15.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_16.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Repair_1.obj");
	character.setAnimations("Repair", 17);

	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_1.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_4.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_5.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_6.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_7.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_8.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Idle_1.obj");
	character.setAnimations("Idle", 11);

	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_1.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_4.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_5.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_6.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_7.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_8.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_9.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_10.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_11.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_12.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_13.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_14.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_15.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_16.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_17.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_18.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_19.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_20.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Computer_1.obj");
	character.setAnimations("Computer", 21);

	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_1.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_4.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_5.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_6.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_7.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_8.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_9.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_10.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_11.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_12.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_13.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Paint_1.obj");
	character.setAnimations("Paint", 14);

	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_1.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_4.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_5.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_6.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_7.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_8.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_9.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_10.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_11.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRack_1.obj");
	character.setAnimations("TireRack", 12);


	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_1.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_4.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_5.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_6.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_7.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_8.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_9.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_10.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_11.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_12.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_Pickup_Paint_1.obj");
	character.setAnimations("Pickup_Paint", 13);

	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_1.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_2.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_3.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_4.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_5.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_6.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_7.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_8.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_9.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_10.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_11.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_12.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_13.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_14.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_15.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_16.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_17.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_18.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_19.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_20.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_21.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_22.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_23.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_24.obj");
	character.loadMorphTarget("meshes/Main Character Animation/Main_Character_TireRepair_25.obj");
	character.setAnimations("TireRepair", 25);

	character.loadTexture(TextureType::Diffuse, "textures/Characters/Main_Character/Main_Character_diffuse (3).png");
	character.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
	character.playAnimation("Idle");

	character.translate = glm::translate(character.translate, glm::vec3(0.f, 0.f, -9.f));
	character.position = glm::vec3(0.f, 0.f, -9.f);
	character.previousPosition = glm::vec3(0.f, 0.f, -9.f);
	character.left = 0.5f;
	character.right = 0.5f;
	character.down = 0.5f;
	character.up = 0.5f;
}

void Game::initCars()
{
	cars.initializeQueue();
	cars.setMaxQueueSize(3);

	Mesh Car1;
	car1 = GameObject(Car1);
	car1.loadMesh("meshes/Vehicles/CarModel.obj");
	car1.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/BlueCar_Texture.png");
	car1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
	car1.currentRepair = TIRE;
	car1.position = glm::vec3(0.f, 0.f, carOutside);
	car1.translate = glm::translate(car1.translate, car1.position);
	car1.left = 3.f;
	car1.right = 3.f;
	car1.down = 3.5f;
	car1.up = 5.f;

	Mesh Car2;
	car2 = GameObject(Car2);
	car2.loadMesh("meshes/Vehicles/CarModel.obj");
	car2.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/GreenCar_Texture.png");
	car2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
	car2.currentRepair = PAINT;
	car2.position = glm::vec3(0.f, 0.f, carOutside);
	car2.translate = glm::translate(car2.translate, car2.position);
	car2.left = 3.f;
	car2.right = 3.f;
	car2.down = 3.5f;
	car2.up = 5.f;

	Mesh Car3;
	car3 = GameObject(Car3);
	car3.loadMesh("meshes/Vehicles/CarModel.obj");
	car3.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/DarkCar_Texture.png");
	car3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
	car3.currentRepair = ENGINE;
	car2.position = glm::vec3(0.f, 0.f, carOutside);
	car2.translate = glm::translate(car2.translate, car2.position);
	car2.left = 3.f;
	car2.right = 3.f;
	car2.down = 3.5f;
	car2.up = 5.f;


	cars.addQueue(&car1);
	cars.addQueue(&car2);
	cars.addQueue(&car3);

}

void Game::initCustomer()
{
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_1.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_2.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_3.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_4.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_5.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_6.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_7.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_8.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_9.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_10.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_11.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_12.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_13.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_14.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_15.obj");
	customer.setAnimations("GetOut", 16);

	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_1.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_1.obj");
	customer.setAnimations("CarSit", 2);

	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_15.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_Out_15.obj");
	customer.setAnimations("CouchSit", 2);

	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_1.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_2.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_3.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_4.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_5.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_6.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_7.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_8.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_9.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_10.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_11.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_12.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_13.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_14.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_15.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_16.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_17.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_18.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_19.obj");
	customer.loadMorphTarget("meshes/Customer/Customer Animation/Customer_Get_In_20.obj");

	customer.setAnimations("GetIn", 20);

	customer.loadTexture(TextureType::Diffuse, "textures/Characters/Customer/Sub_Character2.2_diffuse.png");
	customer.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
	customer.playAnimation("CarSit");

	customer.position = glm::vec3(customerIn, 0.f, carOutside);
	customer.translate = glm::translate(customer.translate, customer.position);
	customer.left = 0.5f;
	customer.right = 0.5f;
	customer.down = 0.5f;
	customer.up = 0.5f;

	Mesh Bubble;
	Bubble.loadFromFile("meshes/Request/Request.obj");

	bubble_Tire = GameObject(Bubble);
	bubble_Tire.loadTexture(TextureType::Diffuse, "textures/Customer Request/Request_Tires.png");
	bubble_Tire.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	bubble_Engine = GameObject(Bubble);
	bubble_Engine.loadTexture(TextureType::Diffuse, "textures/Customer Request/Request_Engine.png");
	bubble_Engine.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	bubble_Paint = GameObject(Bubble);
	bubble_Paint.loadTexture(TextureType::Diffuse, "textures/Customer Request/Request_Paint.png");
	bubble_Paint.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	bubble_Angry = GameObject(Bubble);
	bubble_Angry.loadTexture(TextureType::Diffuse, "textures/Customer Request/AngryCust.png");
	bubble_Angry.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	bubble_Tire.translate = glm::translate(bubble_Tire.translate, customer.position);
	bubble_Engine.translate = glm::translate(bubble_Engine.translate, customer.position);
	bubble_Paint.translate = glm::translate(bubble_Paint.translate, customer.position);
	bubble_Angry.translate = glm::translate(bubble_Angry.translate, customer.position);

}

void Game::initMenu()
{
	// Main Menu
	Mesh Menu;
	Menu.loadFromFile("meshes/UI/Menu.obj");

	mainMenu = GameObject(Menu);
	mainMenu.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/MainMenu.png");
	mainMenu.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu1 = GameObject(Menu);
	mainMenu1.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/MainMenu_Play.png");
	mainMenu1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu2 = GameObject(Menu);
	mainMenu2.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/MainMenu_Select.png");
	mainMenu2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu3 = GameObject(Menu);
	mainMenu3.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/MainMenu_Options.png");
	mainMenu3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu4 = GameObject(Menu);
	mainMenu4.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/MainMenu_Quit.png");
	mainMenu4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	// Main Menu Level Select
	mainMenu_Level_1_1 = GameObject(Menu);
	mainMenu_Level_1_1.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 1/LevelSelect_Level1.png");
	mainMenu_Level_1_1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_1_2 = GameObject(Menu);
	mainMenu_Level_1_2.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 1/LevelSelect_Level1_Back.png");
	mainMenu_Level_1_2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_1_3 = GameObject(Menu);
	mainMenu_Level_1_3.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 1/LevelSelect_Level1_Left.png");
	mainMenu_Level_1_3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_1_4 = GameObject(Menu);
	mainMenu_Level_1_4.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 1/LevelSelect_Level1_Play.png");
	mainMenu_Level_1_4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_1_5 = GameObject(Menu);
	mainMenu_Level_1_5.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 1/LevelSelect_Level1_Right.png");
	mainMenu_Level_1_5.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_2_1 = GameObject(Menu);
	mainMenu_Level_2_1.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 2/LevelSelect_Level2.png");
	mainMenu_Level_2_1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_2_2 = GameObject(Menu);
	mainMenu_Level_2_2.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 2/LevelSelect_Level2_Back.png");
	mainMenu_Level_2_2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_2_3 = GameObject(Menu);
	mainMenu_Level_2_3.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 2/LevelSelect_Level2_Left.png");
	mainMenu_Level_2_3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_2_4 = GameObject(Menu);
	mainMenu_Level_2_4.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 2/LevelSelect_Level2_Play.png");
	mainMenu_Level_2_4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenu_Level_2_5 = GameObject(Menu);
	mainMenu_Level_2_5.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Level Select/Level 2/LevelSelect_Level2_Right.png");
	mainMenu_Level_2_5.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	// Main Menu Control
	mainMenuControl = GameObject(Menu);
	mainMenuControl.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/Controls/MainMenu_Controls.png");
	mainMenuControl.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuControl1 = GameObject(Menu);
	mainMenuControl1.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/Controls/MainMenu_Controls_Back.png");
	mainMenuControl1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	// Main Menu Option
	// Sound & Music ON
	mainMenuOption_Sound_ON_Music_ON = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_ON.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/MainMenu_Options.png");
	mainMenuOption_Sound_ON_Music_ON.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_ON_Music_ON1 = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_ON1.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/MainMenu_OptionsSound.png");
	mainMenuOption_Sound_ON_Music_ON1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_ON_Music_ON2 = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_ON2.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/MainMenu_OptionsMusic.png");
	mainMenuOption_Sound_ON_Music_ON2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_ON_Music_ON3 = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_ON3.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/MainMenu_OptionsControls.png");
	mainMenuOption_Sound_ON_Music_ON3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_ON_Music_ON4 = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_ON4.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/MainMenu_OptionsBack.png");
	mainMenuOption_Sound_ON_Music_ON4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	// Sound & Music OFF
	mainMenuOption_Sound_OFF_Music_OFF = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_OFF.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound and Music/MainMenu_noSoundnoMusic.png");
	mainMenuOption_Sound_OFF_Music_OFF.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_OFF_Music_OFF1 = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_OFF1.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound and Music/MainMenu_noSoundnoMusic_Sound.png");
	mainMenuOption_Sound_OFF_Music_OFF1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_OFF_Music_OFF2 = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_OFF2.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound and Music/MainMenu_noSoundnoMusic_Music.png");
	mainMenuOption_Sound_OFF_Music_OFF2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_OFF_Music_OFF3 = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_OFF3.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound and Music/MainMenu_noSoundnoMusic_Controls.png");
	mainMenuOption_Sound_OFF_Music_OFF3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_OFF_Music_OFF4 = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_OFF4.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound and Music/MainMenu_noSoundnoMusic_Back.png");
	mainMenuOption_Sound_OFF_Music_OFF4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	// Sound ON & Music OFF
	mainMenuOption_Sound_ON_Music_OFF = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_OFF.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Music/MainMenu_noMusic.png");
	mainMenuOption_Sound_ON_Music_OFF.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_ON_Music_OFF1 = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_OFF1.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Music/MainMenu_noMusic_Sound.png");
	mainMenuOption_Sound_ON_Music_OFF1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_ON_Music_OFF2 = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_OFF2.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Music/MainMenu_noMusic_Music.png");
	mainMenuOption_Sound_ON_Music_OFF2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_ON_Music_OFF3 = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_OFF3.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Music/MainMenu_noMusic_Controls.png");
	mainMenuOption_Sound_ON_Music_OFF3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_ON_Music_OFF4 = GameObject(Menu);
	mainMenuOption_Sound_ON_Music_OFF4.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Music/MainMenu_noMusic_Back.png");
	mainMenuOption_Sound_ON_Music_OFF4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	// Sound OFF & Music ON
	mainMenuOption_Sound_OFF_Music_ON = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_ON.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound/MainMenu_noSound.png");
	mainMenuOption_Sound_OFF_Music_ON.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_OFF_Music_ON1 = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_ON1.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound/MainMenu_noSound_Sound.png");
	mainMenuOption_Sound_OFF_Music_ON1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_OFF_Music_ON2 = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_ON2.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound/MainMenu_noSound_Music.png");
	mainMenuOption_Sound_OFF_Music_ON2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_OFF_Music_ON3 = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_ON3.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound/MainMenu_noSound_Controls.png");
	mainMenuOption_Sound_OFF_Music_ON3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	mainMenuOption_Sound_OFF_Music_ON4 = GameObject(Menu);
	mainMenuOption_Sound_OFF_Music_ON4.loadTexture(TextureType::Diffuse, "textures/Menus/Main_Menu/Options/No Sound/MainMenu_noSound_Back.png");
	mainMenuOption_Sound_OFF_Music_ON4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shinys

	// End Screen
	loseScreen = GameObject(Menu);
	loseScreen.loadTexture(TextureType::Diffuse, "textures/Menus/TimesUp.png");
	loseScreen.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	winScreen_0_1 = GameObject(Menu);
	winScreen_0_1.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/0_Stars/EndScreenNoStars.png");
	winScreen_0_1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	winScreen_0_2 = GameObject(Menu);
	winScreen_0_2.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/0_Stars/EndScreenNoStars_Main.png");
	winScreen_0_2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	winScreen_0_3 = GameObject(Menu);
	winScreen_0_3.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/0_Stars/EndScreenNoStars_Redo.png");
	winScreen_0_3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	winScreen_1_1 = GameObject(Menu);
	winScreen_1_1.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/1_Stars/EndScreen1Stars.png");
	winScreen_1_1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
			  
	winScreen_1_2 = GameObject(Menu);
	winScreen_1_2.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/1_Stars/EndScreen1Stars_Main.png");
	winScreen_1_2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
			  
	winScreen_1_3 = GameObject(Menu);
	winScreen_1_3.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/1_Stars/EndScreen1Stars_Next.png");
	winScreen_1_3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	winScreen_2_1 = GameObject(Menu);
	winScreen_2_1.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/2_Stars/EndScreen2Stars.png");
	winScreen_2_1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
			  
	winScreen_2_2 = GameObject(Menu);
	winScreen_2_2.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/2_Stars/EndScreen2Stars_Main.png");
	winScreen_2_2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
			  
	winScreen_2_3 = GameObject(Menu);
	winScreen_2_3.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/2_Stars/EndScreen2Stars_Next.png");
	winScreen_2_3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	winScreen_3_1 = GameObject(Menu);
	winScreen_3_1.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/3_Stars/EndScreen3Stars.png");
	winScreen_3_1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
			  
	winScreen_3_2 = GameObject(Menu);
	winScreen_3_2.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/3_Stars/EndScreen3Stars_Main.png");
	winScreen_3_2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny
			  
	winScreen_3_3 = GameObject(Menu);
	winScreen_3_3.loadTexture(TextureType::Diffuse, "textures/Menus/EndScreen/Level_1_End_Screen/3_Stars/EndScreen3Stars_Next.png");
	winScreen_3_3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	loseScreen.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 4.5f, 6.5f));
	loseScreen.rotate = glm::rotate(loseScreen.translate, glm::radians(35.f), glm::vec3(-1.f, 0.f, 0.f));

	initPause();
}

void Game::initPause()
{
	// Pause Screen
	Mesh Pause;
	Pause.loadFromFile("meshes/UI/Menu.obj");

	pauseScreen = GameObject(Pause);
	pauseScreen.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Pause.png");
	pauseScreen.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreen1 = GameObject(Pause);
	pauseScreen1.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/PauseContinue.png");
	pauseScreen1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreen2 = GameObject(Pause);
	pauseScreen2.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/PauseOptions.png");
	pauseScreen2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreen3 = GameObject(Pause);
	pauseScreen3.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/PauseQuit.png");
	pauseScreen3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

																				// Pause Screen Control
	pauseScreenControl = GameObject(Pause);
	pauseScreenControl.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Controls/Controls.png");
	pauseScreenControl.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenControl1 = GameObject(Pause);
	pauseScreenControl1.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Controls/ControlsBack.png");
	pauseScreenControl1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	// Pause Screen Option
	// Sound & Music ON
	pauseScreenOption_Sound_ON_Music_ON = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_ON.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music On/Options.png");
	pauseScreenOption_Sound_ON_Music_ON.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_ON_Music_ON1 = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_ON1.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music On/Options_Sound.png");
	pauseScreenOption_Sound_ON_Music_ON1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_ON_Music_ON2 = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_ON2.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music On/Options_Music.png");
	pauseScreenOption_Sound_ON_Music_ON2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_ON_Music_ON3 = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_ON3.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music On/Options_Controls.png");
	pauseScreenOption_Sound_ON_Music_ON3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_ON_Music_ON4 = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_ON4.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music On/Options_Back.png");
	pauseScreenOption_Sound_ON_Music_ON4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

																										// Sound & Music OFF
	pauseScreenOption_Sound_OFF_Music_OFF = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_OFF.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music Off/Options_None.png");
	pauseScreenOption_Sound_OFF_Music_OFF.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_OFF_Music_OFF1 = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_OFF1.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music Off/Options_None_Sound.png");
	pauseScreenOption_Sound_OFF_Music_OFF1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_OFF_Music_OFF2 = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_OFF2.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music Off/Options_None_Music.png");
	pauseScreenOption_Sound_OFF_Music_OFF2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_OFF_Music_OFF3 = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_OFF3.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music Off/Options_None_Controls.png");
	pauseScreenOption_Sound_OFF_Music_OFF3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_OFF_Music_OFF4 = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_OFF4.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound & Music Off/Options_None_Back.png");
	pauseScreenOption_Sound_OFF_Music_OFF4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

																										  // Sound ON & Music OFF
	pauseScreenOption_Sound_ON_Music_OFF = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_OFF.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_On_Music_Off/Options_noMusic.png");
	pauseScreenOption_Sound_ON_Music_OFF.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_ON_Music_OFF1 = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_OFF1.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_On_Music_Off/Options_noMusic_Sound.png");
	pauseScreenOption_Sound_ON_Music_OFF1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_ON_Music_OFF2 = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_OFF2.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_On_Music_Off/Options_noMusic_Music.png");
	pauseScreenOption_Sound_ON_Music_OFF2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_ON_Music_OFF3 = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_OFF3.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_On_Music_Off/Options_noMusic_Controls.png");
	pauseScreenOption_Sound_ON_Music_OFF3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_ON_Music_OFF4 = GameObject(Pause);
	pauseScreenOption_Sound_ON_Music_OFF4.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_On_Music_Off/Options_noMusic_Back.png");
	pauseScreenOption_Sound_ON_Music_OFF4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

																										 // Sound OFF & Music ON
	pauseScreenOption_Sound_OFF_Music_ON = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_ON.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_Off_Music_On/Options_noSounds.png");
	pauseScreenOption_Sound_OFF_Music_ON.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_OFF_Music_ON1 = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_ON1.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_Off_Music_On/Options_noSounds_Sound.png");
	pauseScreenOption_Sound_OFF_Music_ON1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_OFF_Music_ON2 = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_ON2.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_Off_Music_On/Options_noSounds_Music.png");
	pauseScreenOption_Sound_OFF_Music_ON2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_OFF_Music_ON3 = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_ON3.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_Off_Music_On/Options_noSounds_Controls.png");
	pauseScreenOption_Sound_OFF_Music_ON3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	pauseScreenOption_Sound_OFF_Music_ON4 = GameObject(Pause);
	pauseScreenOption_Sound_OFF_Music_ON4.loadTexture(TextureType::Diffuse, "textures/Menus/Pause/Options/Sound_Off_Music_On/Options_noSounds_Back.png");
	pauseScreenOption_Sound_OFF_Music_ON4.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

}

void Game::checkPauseDraw()
{
	if (sound_ON_OR_OFF && music_ON_OR_OFF) // Sound & Music is ON
	{
		if (pauseState == PAUSE_OPTION_DEFAULT) // Pause Option Screen
		{
			pauseScreenOption_Sound_ON_Music_ON.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_SOUND)
		{
			pauseScreenOption_Sound_ON_Music_ON1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_MUSIC)
		{
			pauseScreenOption_Sound_ON_Music_ON2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_CONTROL)
		{
			pauseScreenOption_Sound_ON_Music_ON3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_BACK)
		{
			pauseScreenOption_Sound_ON_Music_ON4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
	}
	else if (sound_ON_OR_OFF && !music_ON_OR_OFF) // Sound ON & Music OFF
	{
		if (pauseState == PAUSE_OPTION_DEFAULT) // Pause Option Screen
		{
			pauseScreenOption_Sound_ON_Music_OFF.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_SOUND)
		{
			pauseScreenOption_Sound_ON_Music_OFF1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_MUSIC)
		{
			pauseScreenOption_Sound_ON_Music_OFF2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_CONTROL)
		{
			pauseScreenOption_Sound_ON_Music_OFF3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_BACK)
		{
			pauseScreenOption_Sound_ON_Music_OFF4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
	}
	else if (!sound_ON_OR_OFF && music_ON_OR_OFF) // Sound OFF & Music ON
	{
		if (pauseState == PAUSE_OPTION_DEFAULT) // Pause Option Screen
		{
			pauseScreenOption_Sound_OFF_Music_ON.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_SOUND)
		{
			pauseScreenOption_Sound_OFF_Music_ON1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_MUSIC)
		{
			pauseScreenOption_Sound_OFF_Music_ON2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_CONTROL)
		{
			pauseScreenOption_Sound_OFF_Music_ON3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_BACK)
		{
			pauseScreenOption_Sound_OFF_Music_ON4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
	}
	else if (!sound_ON_OR_OFF && !music_ON_OR_OFF) // Sound & Music is OFF
	{
		if (pauseState == PAUSE_OPTION_DEFAULT) // Pause Option Screen
		{
			pauseScreenOption_Sound_OFF_Music_OFF.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_SOUND)
		{
			pauseScreenOption_Sound_OFF_Music_OFF1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_MUSIC)
		{
			pauseScreenOption_Sound_OFF_Music_OFF2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_CONTROL)
		{
			pauseScreenOption_Sound_OFF_Music_OFF3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (pauseState == PAUSE_OPTION_BACK)
		{
			pauseScreenOption_Sound_OFF_Music_OFF4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
	}

}

void Game::checkMainDraw()
{
	if (sound_ON_OR_OFF && music_ON_OR_OFF) // Sound & Music is ON
	{
		if (menuState == MENU_OPTION_DEFAULT) // Main Menu Option Screen
		{
			mainMenuOption_Sound_ON_Music_ON.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_SOUND)
		{
			mainMenuOption_Sound_ON_Music_ON1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_MUSIC)
		{
			mainMenuOption_Sound_ON_Music_ON2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_CONTROL)
		{
			mainMenuOption_Sound_ON_Music_ON3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_BACK)
		{
			mainMenuOption_Sound_ON_Music_ON4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
	}
	else if (sound_ON_OR_OFF && !music_ON_OR_OFF) // Sound ON & Music OFF
	{
		if (menuState == MENU_OPTION_DEFAULT) // Main Menu Option Screen
		{
			mainMenuOption_Sound_ON_Music_OFF.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_SOUND)
		{
			mainMenuOption_Sound_ON_Music_OFF1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_MUSIC)
		{
			mainMenuOption_Sound_ON_Music_OFF2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_CONTROL)
		{
			mainMenuOption_Sound_ON_Music_OFF3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_BACK)
		{
			mainMenuOption_Sound_ON_Music_OFF4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
	}
	else if (!sound_ON_OR_OFF && music_ON_OR_OFF) // Sound OFF & Music ON
	{
		if (menuState == MENU_OPTION_DEFAULT) // Main Menu Option Screen
		{
			mainMenuOption_Sound_OFF_Music_ON.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_SOUND)
		{
			mainMenuOption_Sound_OFF_Music_ON1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_MUSIC)
		{
			mainMenuOption_Sound_OFF_Music_ON2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_CONTROL)
		{
			mainMenuOption_Sound_OFF_Music_ON3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_BACK)
		{
			mainMenuOption_Sound_OFF_Music_ON4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
	}
	else if (!sound_ON_OR_OFF && !music_ON_OR_OFF) // Sound & Music is OFF
	{
		if (menuState == MENU_OPTION_DEFAULT) // Main menu Option Screen
		{
			mainMenuOption_Sound_OFF_Music_OFF.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_SOUND)
		{
			mainMenuOption_Sound_OFF_Music_OFF1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_MUSIC)
		{
			mainMenuOption_Sound_OFF_Music_OFF2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_CONTROL)
		{
			mainMenuOption_Sound_OFF_Music_OFF3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
		else if (menuState == MENU_OPTION_BACK)
		{
			mainMenuOption_Sound_OFF_Music_OFF4.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		}
	}
}

void Game::nextLevel()
{
	if (levelState == LEVEL_1_1)
	{
		reset(); // Resets Variables
		std::cout << "LEVEL 1_1 to LEVEL 1_2" << std::endl;
		car1.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/BlueCar_Texture.png");
		car1.currentRepair = PAINT;
		car1.repairDone = false;
		car1.position = glm::vec3(0.f, 0.f, carOutside);
		car1.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
		car1.left = 3.f;
		car1.right = 3.f;
		car1.down = 3.5f;
		car1.up = 5.f;

		car2.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/GreenCar_Texture.png");
		car2.currentRepair = PAINT;
		car2.repairDone = false;
		car2.position = glm::vec3(0.f, 0.f, carOutside);
		car2.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
		car2.left = 3.f;
		car2.right = 3.f;
		car2.down = 3.5f;
		car2.up = 5.f;

		car3.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/DarkCar_Texture.png");
		car3.currentRepair = PAINT;
		car3.repairDone = false;
		car3.position = glm::vec3(0.f, 0.f, carOutside);
		car3.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
		car3.left = 3.f;
		car3.right = 3.f;
		car3.down = 3.5f;
		car3.up = 5.f;

		character.levelTime = 80.f;
		
		levelState = LEVEL_1_2;
	}
	else if (levelState == LEVEL_1_2)
	{
		reset(); // Resets Variables
		menuState = MENU_DEFAULT;
		currentState = MENU; // Returns to main menu
	}
}

void Game::checkLevel()
{
	if (tempLevelState != levelState)
	{
		if (levelState == LEVEL_1_1)
		{
			car1.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/BlueCar_Texture.png");
			car1.currentRepair = TIRE;
			car1.repairDone = false;
			car1.position = glm::vec3(0.f, 0.f, carOutside);
			car1.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
			car1.left = 3.f;
			car1.right = 3.f;
			car1.down = 3.5f;
			car1.up = 5.f;

			car2.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/GreenCar_Texture.png");
			car2.currentRepair = PAINT;
			car2.repairDone = false;
			car2.position = glm::vec3(0.f, 0.f, carOutside);
			car2.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
			car2.left = 3.f;
			car2.right = 3.f;
			car2.down = 3.5f;
			car2.up = 5.f;

			car3.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/DarkCar_Texture.png");
			car3.currentRepair = ENGINE;
			car3.repairDone = false;
			car3.position = glm::vec3(0.f, 0.f, carOutside);
			car3.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
			car3.left = 3.f;
			car3.right = 3.f;
			car3.down = 3.5f;
			car3.up = 5.f;

			character.levelTime = 99.f;
		}
		else if (levelState == LEVEL_1_2)
		{
			car1.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/BlueCar_Texture.png");
			car1.currentRepair = ENGINE;
			car1.repairDone = false;
			car1.position = glm::vec3(0.f, 0.f, carOutside);
			car1.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
			car1.left = 3.f;
			car1.right = 3.f;
			car1.down = 3.5f;
			car1.up = 5.f;

			car2.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/GreenCar_Texture.png");
			car2.currentRepair = TIRE;
			car2.repairDone = false;
			car2.position = glm::vec3(0.f, 0.f, carOutside);
			car2.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
			car2.left = 3.f;
			car2.right = 3.f;
			car2.down = 3.5f;
			car2.up = 5.f;

			car3.loadTexture(TextureType::Diffuse, "textures/Vehicles/Cars/DarkCar_Texture.png");
			car3.currentRepair = PAINT;
			car3.repairDone = false;
			car3.position = glm::vec3(0.f, 0.f, carOutside);
			car3.translate = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, carOutside));
			car3.left = 3.f;
			car3.right = 3.f;
			car3.down = 3.5f;
			car3.up = 5.f;

			character.levelTime = 80.f;
		}
	}
	tempLevelState = levelState;
}

void Game::initClock()
{
	Mesh Clock;
	clock = GameObject(Clock);
	clock.loadMesh("meshes/Map/Clock.obj");
	clock.loadTexture(TextureType::Diffuse, "textures/Clock/Clock.png");
	clock.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Clock1;
	clock1 = GameObject(Clock1);
	clock1.loadMesh("meshes/Map/Clock.obj");
	clock1.loadTexture(TextureType::Diffuse, "textures/Clock/Clock_oneThirdTime.png");
	clock1.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Clock2;
	clock2 = GameObject(Clock2);
	clock2.loadMesh("meshes/Map/Clock.obj");
	clock2.loadTexture(TextureType::Diffuse, "textures/Clock/Clock_twoThirdsTime.png");
	clock2.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	Mesh Clock3;
	clock3 = GameObject(Clock3);
	clock3.loadMesh("meshes/Map/Clock.obj");
	clock3.loadTexture(TextureType::Diffuse, "textures/Clock/Clock_noTime.png");
	clock3.loadTexture(TextureType::Specular, "textures/noSpecular.png"); // Not Shiny

	clock.translate = glm::translate(clock.translate, glm::vec3(0.f, 2.7f, -10.f));
	clock1.translate = glm::translate(clock1.translate, glm::vec3(0.f, 2.7f, -10.f));
	clock2.translate = glm::translate(clock2.translate, glm::vec3(0.f, 2.7f, -10.f));
	clock3.translate = glm::translate(clock3.translate, glm::vec3(0.f, 2.7f, -10.f));
}

void Game::clockTimerDraw()
{
	if (character.tempLevelTime < (character.endLevelTime/3)) // If the time is less than one thirds the way done
	{
		clock.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

	}
	else if (character.tempLevelTime < ((character.endLevelTime / 3) * 2)) // If the time is less than two thirds the way done
	{
		clock1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		if (!starOnceTimer1)
		{
			starCalculations = starCalculations + 1;
			starOnceTimer1 = true;
		}

	}
	else if (character.tempLevelTime < ((character.endLevelTime / 3) * 3)) // If the time is less than three thirds the way done
	{
		clock2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		if (!starOnceTimer2)
		{
			starCalculations = starCalculations + 1;
			starOnceTimer2 = true;
		}
	}
	else if (character.tempLevelTime >= character.endLevelTime || currentState == TIME_DONE)// If time is done
	{
		clock3.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
	}
}

void Game::customerTimer(float deltaTime)
{
	if (customerState == CUSTOMER_AT_COUCH)
	{
		// Start timer
		if (angry_start)
		{
			angry_startTime = deltaTime;
			angry_endTime = angry_startTime + angry_totalTime;
			display_endTime = angry_startTime + display_totalTime;
			angry_tempRepair = cars.front()->currentRepair;
			angry_start = false;
		}
		// Update timer
		angry_tempTime = angry_tempTime + deltaTime;
		// Check if timer reachs limit
		if (angry_tempTime > angry_endTime)
		{
			// Angry customer symbol
			displayAngry = true;
			if (!starOnceAngry)
			{
				starCalculations = starCalculations + 1;
				starOnceAngry = true;
			}
		}
		// Check if timer hasn't reached limit
		if (angry_tempTime < display_endTime)
		{
			// Displays Repair
			displayRepair = true;
		}
		else
		{
			displayRepair = false;
		}

		// Checks if repair is done
		if (angry_tempRepair != cars.front()->currentRepair)
		{
			// Resets Timer
			angry_start = true;
			angry_tempTime = 0.0f;
			displayAngry = false;
			displayRepair = false;
			starOnceAngry = false;
		}
		// Updates to current repair
		angry_tempRepair = cars.front()->currentRepair;
	}
	else
	{
		// Resets Variables
		angry_start = true;
		angry_tempTime = 0.0f;
		displayAngry = false;
		displayRepair = false;
		starOnceAngry = false;
	}
}

void Game::drawGame()
{
	// Clear FBOs
	// Non-Bloom Scene
	fboNonBloom.bindFrameBufferForDrawing();
	fboNonBloom.clearFrameBuffer(glm::vec4(0.0));
	fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Bloom Scene
	fboBloom.bindFrameBufferForDrawing();
	fboBloom.clearFrameBuffer(glm::vec4(0.0));
	fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (cars.front()->currentRepair == TIRE && tireRack.timeDone == false && customerState == CUSTOMER_AT_COUCH) // If repair is a tire repair make the tirerack toon shading
	{
		fboBloom.bindFrameBufferForDrawing();
		// Toon Shading + Outline
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3);
		tireRack.draw(outlineStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		tireRack.draw(toonShadingStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
		glDisable(GL_CULL_FACE);
		// End of Toon shading
		fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else
	{
		fboNonBloom.bindFrameBufferForDrawing();
		tireRack.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	

	if (cars.front()->currentRepair == PAINT && workbench.timeDone == false && customerState == CUSTOMER_AT_COUCH)
	{
		fboBloom.bindFrameBufferForDrawing();
		// Toon Shading + Outline
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3);
		workbench.draw(outlineStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		workbench.draw(toonShadingStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
		glDisable(GL_CULL_FACE);
		// End of Toon shading
		fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else
	{
		fboNonBloom.bindFrameBufferForDrawing();
		workbench.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	
	if (cars.front()->currentRepair == ENGINE && customerState == CUSTOMER_AT_COUCH)
	{
		if (computer.timeDone == false)
		{
			fboBloom.bindFrameBufferForDrawing();
			// Toon Shading + Outline
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3);
			computer.draw(outlineStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			computer.draw(toonShadingStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
			glDisable(GL_CULL_FACE);
			// End of Toon shading
			fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		else
		{
			fboNonBloom.bindFrameBufferForDrawing();
			computer.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
		}

		if (computer.timeDone == true && !hasEngine) // Checks if engine is on conveyor belt
		{

			fboBloom.bindFrameBufferForDrawing();

			engine.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

			// Toon Shading + Outline
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3);
			conveyor.draw(outlineStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			conveyor.draw(toonShadingStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
			glDisable(GL_CULL_FACE);
			// End of Toon shading
			fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
		}
	}
	else
	{
		fboNonBloom.bindFrameBufferForDrawing();
		computer.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	// Character
	fboNonBloom.bindFrameBufferForDrawing();
	// Toon Shading + Outline
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3);
	character.drawAnimation(outlineAnimation, cameraTransform, cameraProjection, pointLights, directionalLight);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	character.drawAnimation(toonShadingAnimation, cameraTransform, cameraProjection, pointLights, directionalLight);
	glDisable(GL_CULL_FACE);
	// End of Toon shading
	fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);


	fboNonBloom.bindFrameBufferForDrawing();
	palmtree1.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
	palmtree2.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
	garbage.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
	couch.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
	map.draw(phongNormalMap, cameraTransform, cameraProjection, pointLights, directionalLight);
	conveyor.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
	garbage.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

	clockTimerDraw();

	customer.drawAnimation(phong, cameraTransform, cameraProjection, pointLights, directionalLight);


	if (!cars.front()->repairDone) // If repair is not done
	{
		if (Nearby_AABB(customer, character)) // Draws the bubble if you are nearby
		{
			//Play Customer Dialogue
			if (custCheck == true)
			{
				channel8 = customerSound.play();
				customerSound.updatePosition(channel8, drumPos6, drumVel);

				custCheck = false;
			}

			if (cars.front()->currentRepair == ENGINE)
			{
				bubble_Engine.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (cars.front()->currentRepair == TIRE)
			{
				bubble_Tire.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
			else if (cars.front()->currentRepair == PAINT)
			{
				bubble_Paint.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}
		}
		else
		{
			if (displayAngry)
			{
				bubble_Angry.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
			}

			if (displayRepair)
			{
				if (cars.front()->currentRepair == ENGINE)
				{
					bubble_Engine.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
				}
				else if (cars.front()->currentRepair == TIRE)
				{
					bubble_Tire.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
				}
				else if (cars.front()->currentRepair == PAINT)
				{
					bubble_Paint.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
				}
			}
		}
		
	}
	fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Draw Cars
	if (tireRack.timeDone || workbench.timeDone || hasEngine)
	{
		fboBloom.bindFrameBufferForDrawing();
		// Toon Shading + Outline
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3);
		cars.front()->draw(outlineStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		cars.front()->draw(toonShadingStatic, cameraTransform, cameraProjection, pointLights, directionalLight);
		glDisable(GL_CULL_FACE);
		// End of Toon shading
		fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else
	{
		fboNonBloom.bindFrameBufferForDrawing();
		cars.front()->draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}


	// Draw Particles
	if (engineAnimation || changeTireAnimation)
	{
		fboNonBloom.bindFrameBufferForDrawing();
		particleEmitter.draw(particles, cameraTransform, cameraProjection, repairPtcl);
		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
		fboBloom.bindFrameBufferForDrawing();
		particleEmitter.draw(particles, cameraTransform, cameraProjection, repairPtcl);
		fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else if (paintAnimation)
	{
		fboNonBloom.bindFrameBufferForDrawing();
		particleEmitterPaint.draw(particles, cameraTransform, cameraProjection, paint);
		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
		fboBloom.bindFrameBufferForDrawing();
		particleEmitterPaint.draw(particles, cameraTransform, cameraProjection, paint);
		fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else
	{
		if (cars.front()->currentRepair == ENGINE && customerState == CUSTOMER_AT_COUCH)
		{
			fboNonBloom.bindFrameBufferForDrawing();
			particleEmitterSmoke.draw(particles, cameraTransform, cameraProjection, smoke);
			fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
			fboBloom.bindFrameBufferForDrawing();
			particleEmitterSmoke.draw(particles, cameraTransform, cameraProjection, smoke);
			fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
		}
	}

	if (tireRack.timeDone == true) // Checks if you have a tire in your hand
	{
		fboNonBloom.bindFrameBufferForDrawing();
		carTire.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		HUD_Tire.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	if (workbench.timeDone == true) // Checks if you have a paintgun in your hand
	{
		fboNonBloom.bindFrameBufferForDrawing();
		paintGunShow.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		HUD_PaintCan.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else
	{
		fboNonBloom.bindFrameBufferForDrawing();
		paintGun.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);
		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	if (hasEngine == true) // Checks if you have a engine in your hand
	{
		fboNonBloom.bindFrameBufferForDrawing();
		engineShow.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		HUD_Engine.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}


	if (starCalculations <= 1)
	{
		fboNonBloom.bindFrameBufferForDrawing();

		// 3 Stars
		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		HUD_Face_Happy.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else if (starCalculations <= 2)
	{
		fboNonBloom.bindFrameBufferForDrawing();

		// 2 Stars
		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		HUD_Face_Normal.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else if (starCalculations <= 3)
	{
		fboNonBloom.bindFrameBufferForDrawing();

		// 1 Stars
		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		HUD_Face_Upset.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else if (starCalculations > 3)
	{
		fboNonBloom.bindFrameBufferForDrawing();

		// 1 Stars
		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		HUD_Face_Angry.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	// Times done
	if (currentState == TIME_DONE)
	{
		// Bloom
		fboBloom.bindFrameBufferForDrawing();
		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		loseScreen.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);

		glDisable(GL_BLEND);
		fboBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

		// Non Bloom
		fboNonBloom.bindFrameBufferForDrawing();
		glEnable(GL_BLEND); // Tells OpenGL to use the alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		// Draw
		loseScreen.draw(phong, cameraTransform, cameraProjection, pointLights, directionalLight);

		glDepthMask(GL_TRUE);

		glDisable(GL_BLEND);
		fboNonBloom.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	// Bloom Calculations
	brightPass();
	blurBrightPass();
	fboBloomed.bindFrameBufferForDrawing();
	fboBloomed.clearFrameBuffer(glm::vec4(0.0));
	fboBlurredBrightPass.bindTextureForSampling(0, GL_TEXTURE0); // Blurred Bright Pass
	fboBloom.bindTextureForSampling(0, GL_TEXTURE1); // Orginal Bloom Objects
	quad.drawQuad(bloom, fboBlurredBrightPass);
	fboBlurredBrightPass.unbindTexture(GL_TEXTURE0);
	fboBloom.unbindTexture(GL_TEXTURE1);
	fboBloomed.unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Non-Bloom and Bloom Scene put together
	fboBloomed.bindTextureForSampling(0, GL_TEXTURE0); // Bloom objects already bloomed
	fboNonBloom.bindTextureForSampling(0, GL_TEXTURE1); // Orginal Non-Bloom objects
	fboBlurredBrightPass.bindTextureForSampling(0, GL_TEXTURE2); // The blured
	fboNonBloom.bindDepthTextureForSampling(GL_TEXTURE3); // Orginal Non-Bloom objects
	fboBloom.bindDepthTextureForSampling(GL_TEXTURE4); // Orginal Bloom objects

	FrameBufferObject::unbindFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	FrameBufferObject::clearFrameBuffer(glm::vec4(1.0));
	glDisable(GL_BLEND);

	quad.drawQuad(bloomScene, fboBlurredBrightPass);

	fboBloomed.unbindTexture(GL_TEXTURE0);
	fboNonBloom.unbindTexture(GL_TEXTURE1);
	fboBlurredBrightPass.unbindTexture(GL_TEXTURE2);
	fboNonBloom.unbindTexture(GL_TEXTURE3);
	fboBloom.unbindTexture(GL_TEXTURE4);
}

void Game::initFBO()
{
	Mesh Quad;
	quad = GameObject(Quad);
	quad.loadMesh("meshes/quad.obj");

	fboBloomed.createFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 1, true);
	fboNonBloom.createFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 1, true);
	fboBloom.createFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 1, true);
	fboBrightPass.createFrameBuffer(WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8, 1, true);
	fboBlurredBrightPass.createFrameBuffer(WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8, 1, true);
	fboBlurredBrightPass1.createFrameBuffer(WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8, 1, true);
}

// Function from intermediate CG Lab 2
void Game::brightPass()
{
	fboBrightPass.bindFrameBufferForDrawing();
	fboBrightPass.clearFrameBuffer(glm::vec4(0.0));

	fboBloom.bindTextureForSampling(0, GL_TEXTURE0);

	quad.drawQuad(bright, fboBlurredBrightPass);

	fboBloom.unbindTexture(GL_TEXTURE0);

	fboBrightPass.unbindFrameBuffer(fboBrightPass.getWidth(), fboBrightPass.getHeight());
}
// Function from intermediate CG Lab 2
void Game::blurBrightPass()
{
	fboBlurredBrightPass.bindFrameBufferForDrawing();
	fboBlurredBrightPass.clearFrameBuffer(glm::vec4(0.0));

	fboBrightPass.bindTextureForSampling(0, GL_TEXTURE0);

	quad.drawQuad(blur, fboBlurredBrightPass);

	fboBrightPass.unbindTexture(GL_TEXTURE0);
	fboBlurredBrightPass.unbindFrameBuffer(fboBlurredBrightPass.getWidth(), fboBlurredBrightPass.getHeight());

	for (int i = 0; i < 20; i++) // Multiple blurs
	{
		fboBlurredBrightPass1.bindFrameBufferForDrawing();
		fboBlurredBrightPass1.clearFrameBuffer(glm::vec4(0.0));

		fboBlurredBrightPass.bindTextureForSampling(0, GL_TEXTURE0);

		quad.drawQuad(blur, fboBlurredBrightPass);

		fboBlurredBrightPass.unbindTexture(GL_TEXTURE0);
		fboBlurredBrightPass1.unbindFrameBuffer(fboBlurredBrightPass1.getWidth(), fboBlurredBrightPass1.getHeight());


		fboBlurredBrightPass.bindFrameBufferForDrawing();
		fboBlurredBrightPass.clearFrameBuffer(glm::vec4(0.0));

		fboBlurredBrightPass1.bindTextureForSampling(0, GL_TEXTURE0);

		quad.drawQuad(blur, fboBlurredBrightPass);

		fboBlurredBrightPass1.unbindTexture(GL_TEXTURE0);
		fboBlurredBrightPass.unbindFrameBuffer(fboBlurredBrightPass.getWidth(), fboBlurredBrightPass.getHeight());

	}
	fboBlurredBrightPass.bindFrameBufferForDrawing();
}

void Game::starCalc()
{
	if (starCalculations <= 1)
	{
		// 3 Stars
		winState = WIN_DEFAULT_3;
		starState = STAR_3;
	}
	else if (starCalculations <= 2)
	{
		// 2 Stars
		winState = WIN_DEFAULT_2;
		starState = STAR_2;
	}
	else if (starCalculations <= 3)
	{
		// 1 Stars
		winState = WIN_DEFAULT_1;
		starState = STAR_1;
	}
	else if (starCalculations > 3)
	{
		// 0 Stars
		winState = WIN_DEFAULT_0;
		starState = STAR_0;
	}
	
}