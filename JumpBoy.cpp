

#include <iostream>
#include "raylib.h"
#include <vector>

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
	//Update running time
	data.runningTime += deltaTime;
	if (data.runningTime >= data.updateTime)
	{
		data.runningTime = 0.0;
		//Update animation frame
		data.rec.x = data.frame * data.rec.width;
		data.frame++;
		if (data.frame > maxFrame)
		{
			data.frame = 0;
		}
	}
	return data;
}

int main()
{
	InitAudioDevice();
	Music music = LoadMusicStream("textures/song.mp3");
	music.looping = false;
	float pitch = 1.0f;

	PlayMusicStream(music);

	float timePlayed = 0.0f;
	bool pause = false;



	

	//Array with window dimensions
	int windowDimensions[2];
	windowDimensions[0] = 1300;
	windowDimensions[1] = 800;

	

	//Initialize window
	InitWindow(windowDimensions[0], windowDimensions[1], "Jump Boy");

	//Acceleration due to gravity (pixels/frame)/frame
	const int gravity = 1000;
	const int jumpVel = -600;

	int velocity = 0;
	
	//Backgroung textures
	Texture2D background = LoadTexture("textures/far-buildings.png");
	float bgX{};

	Texture2D midground = LoadTexture("textures/back-buildings.png");
	float mgX{};

	Texture2D foreground = LoadTexture("textures/foreground.png");
	float fgX{};

	bool collision = 0;

	//Nebula X velocity (pixels/second)
	int nebVel = -600;

	//Enemy nebula1 variables
	Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
	
	const int sizeOfNebule = 20;
	
	AnimData nebule[sizeOfNebule]{};

	for (int i = 0; i < sizeOfNebule; i++)
	{
		nebule[i].rec.x = 0.0;
		nebule[i].rec.y = 0.0;
		nebule[i].rec.width = nebula.width / 8;
		nebule[i].rec.height = nebula.height / 8;
		nebule[i].pos.y = windowDimensions[1] - nebula.height / 8;
		nebule[i].frame = 0;
		nebule[i].runningTime = 0.0;
		nebule[i].updateTime = 1.0/16.0;

		nebule[i].pos.x = windowDimensions[0] + i * 1000;
	}
	
	
	float finishLine{ nebule[sizeOfNebule - 1].pos.x };

	//Animation texture
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	AnimData scarfyData;
	scarfyData.rec.width = scarfy.width / 6;
	scarfyData.rec.height = scarfy.height;
	scarfyData.rec.x = 0;
	scarfyData.rec.y = 0;
	scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
	scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0.0;

	bool isInAir;

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		UpdateMusicStream(music);
		SetMusicPitch(music, pitch);


		//Delta time (time since last frame)
		const float dT = GetFrameTime();

		BeginDrawing();
		ClearBackground(WHITE);

		bgX -= 20 * dT;

		if (bgX <= -background.width * 2)
		{
			bgX = 0.0;
		}
		//Scroll midground
		mgX -= 40 * dT;

		if (mgX <= -background.width * 2)
		{
		 mgX = 0.0;
		}
		//Scroll forefground
		fgX -= 80 * dT;

		if (fgX <= -background.width * 2)
		{
			fgX = 0.0;
		}
		

		//Draw the backfround
		Vector2 bg1Pos{bgX, 0.0};
		DrawTextureEx(background, bg1Pos, 0.0, 6.0, WHITE);
		Vector2 bg2Pos{ bgX + background.width * 2, 0.0 };
		DrawTextureEx(background, bg2Pos, 0.0, 6.0, WHITE);

		//Draw midground
		Vector2 mg1Pos{ mgX, 0.0 };
		DrawTextureEx(midground, mg1Pos, 0.0, 6.0, WHITE);
		Vector2 mg2Pos{ mgX + midground.width * 2, 0.0 };
		DrawTextureEx(midground, mg2Pos, 0.0, 6.0, WHITE);

		
		//Draw forground
		Vector2 fg1Pos{ fgX, 0.0 };
		DrawTextureEx(foreground, fg1Pos, 0.0, 6.0, WHITE);
		Vector2 fg2Pos{ fgX + foreground.width * 2, 0.0 };
		DrawTextureEx(foreground, fg2Pos, 0.0, 6.0, WHITE);
	

		if (isOnGround(scarfyData, windowDimensions[1]))
		{
			//Recatangle is on the ground
			velocity = 0;
			isInAir = false;
		}

		else
		{
			//Rectangle is on the air
			velocity += gravity * dT;
			isInAir = true;
		}


		//Set jumping button

		if (IsKeyPressed(KEY_SPACE) && !isInAir)
		{
			velocity += jumpVel;
		}


		for (int i = 0; i < sizeOfNebule; i++)
		{   //Update position of each nebule
			nebule[i].pos.x += nebVel * dT;
		}

		//Update finish line
		finishLine += nebVel * dT;

		//Update nebula position
		nebule[1].pos.x += nebVel * dT;


		//Update scarfy animation frame
		scarfyData.pos.y += velocity * dT;

		if (!isInAir)
		{
			scarfyData = updateAnimData(scarfyData, dT, 5);
		}

		for (int i = 0; i < sizeOfNebule; i++)
		{
			nebule[i] = updateAnimData(nebule[i], dT, 7);
		}

		
		for (AnimData nebula : nebule)
		{
			float pad{20};
			Rectangle nebRec{
				nebula.pos.x + pad,
				nebula.pos.y + pad,
				nebula.rec.width - 2*pad,
				nebula.rec.height - 2*pad,
			};
			Rectangle scarfyRec{
				scarfyData.pos.x,
				scarfyData.pos.y,
				scarfyData.rec.width,
				scarfyData.rec.height,
			};

				if (CheckCollisionRecs(nebRec, scarfyRec))
				{
					collision = true;
				}
		}

		if (collision)
		{
			DrawText("Game Over!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, RED);
		}
		else if (scarfyData.pos.x >= finishLine)
		{
			//Win the game
			DrawText("YoU Win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, RED);
		}
		else
		{
			
			for (int i = 0; i < sizeOfNebule; i++)
			{
				//Draw Nebula1(our enemy)
				DrawTextureRec(nebula, nebule[i].rec, nebule[i].pos, WHITE);
			}
		}

		
		
		//Draw Scarfy(our hero)
		DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
		
		EndDrawing();

	}

	         
	UnloadTexture(nebula);
	UnloadTexture(scarfy);
	UnloadTexture(background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
	CloseWindow();

	
}



