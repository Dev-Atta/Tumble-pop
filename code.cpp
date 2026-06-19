#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1136;
int screen_y = 896;

void display_level(RenderWindow& window, char**lvl, Texture& bgTex,Sprite& bgSprite,Texture& blockTexture,Sprite& blockSprite, const int height, const int width, const int cell_size)
{
	window.draw(bgSprite);

	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			if (lvl[i][j] == '#')
			{
				blockSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(blockSprite);
			}
		}
	}

}

void level_one(char** lvl, const int height, const int width)
{
    // Clear level
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            lvl[i][j] = ' ';
        }
    }

    // Bottom platform
    for (int i = 0; i < width; i++)
    {
        lvl[10][i] = '#';
    }

    // Left wall
    for (int i = 0; i < height; i++)
    {
        lvl[i][0] = '#';
    }

    // Right wall
    for (int i = 0; i < height; i++)
    {
        lvl[i][width - 1] = '#';
    }

    // Top platform (lower top)
    for (int i = 3; i <= width - 4; i++)
    {
        lvl[2][i] = '#';
    }

    // 3rd row
   // lvl[3][8] = '#';
    //lvl[3][9] = '#';
    //lvl[3][13] = '#';

    // 4th row platform
    lvl[4][1]  = '#';
    lvl[4][2]  = '#';
    lvl[4][3]  = '#';
    lvl[4][4]  = '#';

    lvl[4][7]  = '#';
    lvl[4][8]  = '#';
    lvl[4][9]  = '#';
    lvl[4][10] = '#';

    lvl[4][13] = '#';
    lvl[4][14] = '#';
    lvl[4][15] = '#';
    lvl[4][16] = '#';

    // 5th row
  //  lvl[5][7]  = '#';
   // lvl[5][10] = '#';

    // 6th row platform
    for (int i = 3; i <= width - 4; i++)
    {
        lvl[6][i] = '#';
    }

    lvl[6][8] = '.';
    lvl[6][9] = '.';;

    // 7th row
 //   lvl[7][7]  = '#';
  //  lvl[7][10] = '#';

    // 8th row platform
    lvl[8][1]  = '#';
    lvl[8][2]  = '#';
    lvl[8][3]  = '#';
    lvl[8][4]  = '#';

    lvl[8][7]  = '#';
    lvl[8][8]  = '#';
    lvl[8][9]  = '#';
    lvl[8][10] = '#';

    lvl[8][13] = '#';
    lvl[8][14] = '#';
    lvl[8][15] = '#';
    lvl[8][16] = '#';
}



void update_ghosts(float ghost_x[], float ghost_y[], float ghost_speed[], bool ghost_movingRight[], int ghost_platform[], Clock ghost_directionTimer[], float ghost_pauseDuration[], int numGhosts, char** lvl, const int cell_size, const int width, bool ghost_active[])
{
    for (int i = 0; i < numGhosts; i++)
    {
        if (!ghost_active[i]) continue;  // Skip inactive ghosts
        
        // Check if ghost should pause and change direction
        if (ghost_directionTimer[i].getElapsedTime().asSeconds() > ghost_pauseDuration[i])
        {
            // Randomly decide to change direction (30% chance)
            if (rand() % 10 < 3)
            {
                ghost_movingRight[i] = !ghost_movingRight[i];
            }
            ghost_directionTimer[i].restart();
            ghost_pauseDuration[i] = 1.5f + (rand() % 20) / 10.0f;  // Random pause 1.5-3.5 seconds
        }
        
        // Move ghost
        if (ghost_movingRight[i])
        {
            ghost_x[i] += ghost_speed[i];
            
            // Check right boundary (wall, end of platform, or gap)
            int gridX = (int)(ghost_x[i] + 48) / cell_size;  // Check front edge of ghost
            
            // Boundary check
            if (gridX >= width) gridX = width - 1;
            
            // Check if next position is NOT a solid block (wall, gap, or empty space)
            if (gridX >= width - 1 || lvl[ghost_platform[i]][gridX] != '#')
            {
                ghost_movingRight[i] = false;
                ghost_x[i] -= ghost_speed[i]; // Revert movement
            }
        }
        else
        {
            ghost_x[i] -= ghost_speed[i];
            
            // Check left boundary (wall, end of platform, or gap)
            int gridX = (int)(ghost_x[i]) / cell_size;  // Check front edge of ghost
            
            // Boundary check
            if (gridX < 0) gridX = 0;
            
            // Check if next position is NOT a solid block (wall, gap, or empty space)
            if (gridX <= 0 || lvl[ghost_platform[i]][gridX] != '#')
            {
                ghost_movingRight[i] = true;
                ghost_x[i] += ghost_speed[i]; // Revert movement
            }
        }
    }
}



void draw_ghosts(RenderWindow& window, float ghost_x[], float ghost_y[], bool ghost_movingRight[], int numGhosts, Sprite& GhostSprite, bool ghost_active[])
{
    for (int i = 0; i < numGhosts; i++)
    {
        if (!ghost_active[i]) continue;  // Don't draw inactive ghosts
        
        GhostSprite.setPosition(ghost_x[i], ghost_y[i]);
        
        // Since left side is the face of ghost:
        // - When moving LEFT: show normal sprite (face pointing left)
        // - When moving RIGHT: flip sprite horizontally (face pointing right)
        
        if (ghost_movingRight[i])
        {
            // Moving right - flip the sprite horizontally
            GhostSprite.setScale(-2, 2);
            GhostSprite.setOrigin(24, 0);  // Origin at right side (24 = half of original width ~48/2)
        }
        else
        {
            // Moving left - normal orientation (face already points left)
            GhostSprite.setScale(2, 2);
            GhostSprite.setOrigin(0, 0);
        }
        
        window.draw(GhostSprite);
    }
}


void update_skeletons(float skeleton_x[], float skeleton_y[], float skeleton_speed[], bool skeleton_movingRight[], int skeleton_platform[], float skeleton_velocityY[], bool skeleton_onGround[], Clock skeleton_actionTimer[], float skeleton_actionDuration[], int numSkeletons, char** lvl, const int cell_size, const int width, const int height, bool skeleton_active[])
{
    const float skeleton_gravity = 1.0f;
    const float skeleton_jumpStrength = -15.0f;
    const int skeleton_width = 48;  // Approximate width (24 * 2 scale)
    const int skeleton_height = 78; // Approximate height (32 * 2 scale)

    for (int i = 0; i < numSkeletons; i++)
    {
        if (!skeleton_active[i]) continue;  // Skip inactive skeletons
        
        // Check if skeleton should perform random action (stop, change direction, or jump)
        if (skeleton_actionTimer[i].getElapsedTime().asSeconds() > skeleton_actionDuration[i])
        {
            int action = rand() % 10;
            
            if (action < 3)  // 30% chance - change horizontal direction
            {
                skeleton_movingRight[i] = !skeleton_movingRight[i];
            }
            else if (action < 7 && skeleton_onGround[i])  // 40% chance - jump (changed to action < 7)
            {
                skeleton_velocityY[i] = skeleton_jumpStrength;
                skeleton_onGround[i] = false;
            }
            // else 30% chance - continue current movement
            
            skeleton_actionTimer[i].restart();
            skeleton_actionDuration[i] = 1.0f + (rand() % 30) / 10.0f;  // Random 1.0-4.0 seconds
        }
        
        // Horizontal movement
        if (skeleton_movingRight[i])
        {
            skeleton_x[i] += skeleton_speed[i];
            
            // Check right boundary
            int gridX = (int)(skeleton_x[i] + skeleton_width) / cell_size;
            if (gridX >= width) gridX = width - 1;
            
            int gridY = (int)(skeleton_y[i] + skeleton_height / 2) / cell_size;
            if (gridY >= height) gridY = height - 1;
            
            // Hit wall or gap - turn around
            if (gridX >= width - 1 || lvl[gridY][gridX] == '#')
            {
                skeleton_movingRight[i] = false;
                skeleton_x[i] -= skeleton_speed[i];
            }
        }
        else
        {
            skeleton_x[i] -= skeleton_speed[i];
            
            // Check left boundary
            int gridX = (int)(skeleton_x[i]) / cell_size;
            if (gridX < 0) gridX = 0;
            
            int gridY = (int)(skeleton_y[i] + skeleton_height / 2) / cell_size;
            if (gridY >= height) gridY = height - 1;
            
            // Hit wall or gap - turn around
            if (gridX <= 0 || lvl[gridY][gridX] == '#')
            {
                skeleton_movingRight[i] = true;
                skeleton_x[i] += skeleton_speed[i];
            }
        }
        
        // Apply gravity
        skeleton_y[i] += skeleton_velocityY[i];
        
        // Check ground collision
        int gridBottom = (int)(skeleton_y[i] + skeleton_height) / cell_size;
        int gridLeft = (int)(skeleton_x[i]) / cell_size;
        int gridRight = (int)(skeleton_x[i] + skeleton_width) / cell_size;
        
        if (gridBottom >= height) gridBottom = height - 1;
        if (gridLeft < 0) gridLeft = 0;
        if (gridRight >= width) gridRight = width - 1;
        
        // Check if standing on solid ground
        if (lvl[gridBottom][gridLeft] == '#' || lvl[gridBottom][gridRight] == '#')
        {
            skeleton_onGround[i] = true;
            skeleton_velocityY[i] = 0;
            skeleton_y[i] = (gridBottom * cell_size) - skeleton_height;
            skeleton_platform[i] = gridBottom;  // Update current platform
        }
        else
        {
            skeleton_onGround[i] = false;
            skeleton_velocityY[i] += skeleton_gravity;
            if (skeleton_velocityY[i] > 20) skeleton_velocityY[i] = 20;  // Terminal velocity
        }
    }
}




void draw_skeletons(RenderWindow& window, float skeleton_x[], float skeleton_y[], bool skeleton_movingRight[], int numSkeletons, Sprite& SkeletonSprite, bool skeleton_active[])
{
    for (int i = 0; i < numSkeletons; i++)
    {
        if (!skeleton_active[i]) continue;  // Don't draw inactive skeletons
        
        SkeletonSprite.setPosition(skeleton_x[i], skeleton_y[i]);
        
        // Flip sprite based on direction
        if (skeleton_movingRight[i])
        {
            SkeletonSprite.setScale(-2, 2);  // Flipped
            SkeletonSprite.setOrigin(24, 0);  // Adjust origin
        }
        else
        {
            SkeletonSprite.setScale(2, 2);   // Normal
            SkeletonSprite.setOrigin(0, 0);
        }
        
        window.draw(SkeletonSprite);
    }
}

void player_movement(char** lvl, float& player_x, float& player_y, const float& speed, float& velocityY, const float& jumpStrength, bool& onGround, const int cell_size, int& PlayerHeight, int& PlayerWidth, const int height, const int width, Sprite& PlayerSprite, Sprite& BagSprite, bool& facingRight)
{
    // Left movement (A key)
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        player_x -= speed;
        facingRight = false;
        
        // Calculate grid positions
        int grid_left = (int)(player_x) / cell_size;
        int grid_mid_y = (int)(player_y + PlayerHeight/2) / cell_size;
        int grid_bottom = (int)(player_y + PlayerHeight - 1) / cell_size;
        
        // Boundary checks
        if (grid_left < 0) grid_left = 0;
        if (grid_mid_y >= height) grid_mid_y = height - 1;
        if (grid_bottom >= height) grid_bottom = height - 1;
        
        char left_mid = lvl[grid_mid_y][grid_left];
        char left_bot = lvl[grid_bottom][grid_left];
        
        if (left_mid == '#' || left_bot == '#')
            player_x += speed;
    }
    
    // Right movement (D key)
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        player_x += speed;
        facingRight = true;
        
        // Calculate grid positions
        int grid_right = (int)(player_x + PlayerWidth) / cell_size;
        int grid_mid_y = (int)(player_y + PlayerHeight/2) / cell_size;
        int grid_bottom = (int)(player_y + PlayerHeight - 1) / cell_size;
        
        // Boundary checks
        if (grid_right >= width) grid_right = width - 1;
        if (grid_mid_y >= height) grid_mid_y = height - 1;
        if (grid_bottom >= height) grid_bottom = height - 1;
        
        char right_mid = lvl[grid_mid_y][grid_right];
        char right_bot = lvl[grid_bottom][grid_right];
        
        if (right_mid == '#' || right_bot == '#')
            player_x -= speed;
    }
    
    // Jump (W key)
    if (Keyboard::isKeyPressed(Keyboard::Up))
    {
        if (onGround)
        {
            velocityY = jumpStrength;
            onGround = false;
        }
    }
    
    // Update sprite orientation based on facing direction
    if (facingRight)
    {
        // ===== FACING RIGHT =====
        PlayerSprite.setScale(-3, 3);  // Flip horizontally
        PlayerSprite.setOrigin(32, 0); // Adjust origin (32 = 96/3, half of original width)
        
        // Position bag physically attached at the back (left side) when facing right
        // Using SAME logic as left-facing (mirrored)
        BagSprite.setScale(3, 3);  // Normal orientation
        BagSprite.setOrigin(0, 16);  // Origin at right edge of bag
        BagSprite.setPosition(player_x - 5, player_y + PlayerHeight / 2 + 10);  // Right edge of bag touches left edge of player
    }
    else
    {
        // FACING LEFT
        PlayerSprite.setScale(3, 3);   // Normal
        PlayerSprite.setOrigin(0, 0);  // Normal origin
        
        // Position bag physically attached at the back (right side) when facing left
        BagSprite.setScale(-3, 3);  // Flipped horizontally
        BagSprite.setOrigin(0, 16);  // Origin at left edge (becomes right when flipped)
        BagSprite.setPosition(player_x + PlayerWidth + 5, player_y + PlayerHeight / 2 + 10);  // Left edge of bag touches right edge of player
    }
}




void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth)
{
	offset_y = player_y;
	offset_y += velocityY;

	// Only check bottom collision when falling down (velocityY >= 0)
	// Don't check collision when jumping up (velocityY < 0)
	if (velocityY >= 0)
	{
		char bottom_left_down = lvl[(int)(offset_y + Pheight) / cell_size][(int)(player_x) / cell_size];
		char bottom_right_down = lvl[(int)(offset_y + Pheight) / cell_size][(int)(player_x + Pwidth) / cell_size];
		char bottom_mid_down = lvl[(int)(offset_y + Pheight) / cell_size][(int)(player_x + Pwidth / 2) / cell_size];

		if (bottom_left_down == '#' || bottom_mid_down == '#' || bottom_right_down == '#')
		{
			onGround = true;
		}
		else
		{
			player_y = offset_y;
			onGround = false;
		}
	}
	else
	{
		// When jumping up, just update position without checking collision
		player_y = offset_y;
		onGround = false;
	}

	if (!onGround)
	{
		velocityY += gravity;
		if (velocityY >= terminal_Velocity) velocityY = terminal_Velocity;
	}
	else
	{
		velocityY = 0;
	}
}

bool check_collision(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}




void vacuum_suction(float player_x, float player_y, int PlayerWidth, int PlayerHeight, 
                    bool facingRight, bool showVacuum,
                    float ghost_x[], float ghost_y[], bool ghost_active[], int numGhosts,
                    float skeleton_x[], float skeleton_y[], bool skeleton_active[], int numSkeletons,
                    const int vacuumRange)
{
    if (!showVacuum) return;  // Only suck when space is pressed
    
    const float suctionSpeed = 4.0f;  // Speed at which enemies are pulled horizontally
    const float disappearDistance = vacuumRange / 2.0f;  // Disappear at mid-range (150 pixels)
    const float verticalTolerance = 30.0f;  // Reduced vertical range (only same level as player)
    
    // Calculate player center position
    float playerCenterX = player_x + PlayerWidth / 2;
    float playerCenterY = player_y + PlayerHeight / 2;
    
    // Suck ghosts
    for (int i = 0; i < numGhosts; i++)
    {
        if (!ghost_active[i]) continue;
        
        // Calculate ghost center
        float ghostCenterX = ghost_x[i] + 24;  // 48/2 = 24 (half ghost width)
        float ghostCenterY = ghost_y[i] + 24;  // 48/2 = 24 (half ghost height)
        
        // Calculate horizontal distance from ghost to player
        float horizontalDistance = abs(ghostCenterX - playerCenterX);
        float verticalDistance = abs(ghostCenterY - playerCenterY);
        
        // STRICT vertical check - only same level enemies
        if (verticalDistance > verticalTolerance) continue;  // Skip enemies above/below
        
        // Check if ghost is within vacuum range
        bool inRange = false;
        
        if (facingRight)
        {
            // Facing right - suck enemies from the RIGHT side only (in front)
            if (ghostCenterX > playerCenterX && horizontalDistance <= vacuumRange)
            {
                inRange = true;
            }
        }
        else
        {
            // Facing left - suck enemies from the LEFT side only (in front)
            if (ghostCenterX < playerCenterX && horizontalDistance <= vacuumRange)
            {
                inRange = true;
            }
        }
        
        if (inRange)
        {
            // Check if enemy reached mid-point (disappear zone)
            if (horizontalDistance <= disappearDistance)
            {
                ghost_active[i] = false;  // Enemy disappears at mid-range
            }
            else
            {
                // Pull ghost toward player HORIZONTALLY ONLY
                if (facingRight)
                {
                    ghost_x[i] -= suctionSpeed;  // Pull left (toward player)
                }
                else
                {
                    ghost_x[i] += suctionSpeed;  // Pull right (toward player)
                }
            }
        }
    }
    
    // Suck skeletons
    for (int i = 0; i < numSkeletons; i++)
    {
        if (!skeleton_active[i]) continue;
        
        // Calculate skeleton center
        float skeletonCenterX = skeleton_x[i] + 24;  // 48/2 = 24 (half skeleton width)
        float skeletonCenterY = skeleton_y[i] + 39;  // 78/2 = 39 (half skeleton height)
        
        // Calculate horizontal distance from skeleton to player
        float horizontalDistance = abs(skeletonCenterX - playerCenterX);
        float verticalDistance = abs(skeletonCenterY - playerCenterY);
        
        // STRICT vertical check - only same level enemies
        if (verticalDistance > verticalTolerance) continue;  // Skip enemies above/below
        
        // Check if skeleton is within vacuum range
        bool inRange = false;
        
        if (facingRight)
        {
            // Facing right - suck enemies from the RIGHT side only (in front)
            if (skeletonCenterX > playerCenterX && horizontalDistance <= vacuumRange)
            {
                inRange = true;
            }
        }
        else
        {
            // Facing left - suck enemies from the LEFT side only (in front)
            if (skeletonCenterX < playerCenterX && horizontalDistance <= vacuumRange)
            {
                inRange = true;
            }
        }
        
        if (inRange)
        {
            // Check if enemy reached mid-point (disappear zone)
            if (horizontalDistance <= disappearDistance)
            {
                skeleton_active[i] = false;  // Enemy disappears at mid-range
            }
            else
            {
                // Pull skeleton toward player HORIZONTALLY ONLY
                if (facingRight)
                {
                    skeleton_x[i] -= suctionSpeed;  // Pull left (toward player)
                }
                else
                {
                    skeleton_x[i] += suctionSpeed;  // Pull right (toward player)
                }
            }
        }
    }
}

bool check_all_enemies_captured(bool ghost_active[], int numGhosts, bool skeleton_active[], int numSkeletons)
{
    // Check if any ghost is still active
    for (int i = 0; i < numGhosts; i++)
    {
        if (ghost_active[i]) return false;  // At least one ghost is still active
    }
    
    // Check if any skeleton is still active
    for (int i = 0; i < numSkeletons; i++)
    {
        if (skeleton_active[i]) return false;  // At least one skeleton is still active
    }
    
    return true;  // All enemies captured!
}




int main()
{

	RenderWindow window(VideoMode(screen_x, screen_y), "Tumble-POP", Style::Resize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
       

bool facingRight = false;  // Track player facing direction

	//level specifics
        const int cell_size = 64;
        const int height = 14;
        const int width  = 18;
	

	//level and background textures and sprites
	Texture bgTex;
	Sprite bgSprite;
	Texture blockTexture;
	Sprite blockSprite;

	bgTex.loadFromFile("Data/bg.png");
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0,0);

	blockTexture.loadFromFile("Data/block1.png");
	blockSprite.setTexture(blockTexture);

	//Music initialisation
	Music lvlMusic;

	lvlMusic.openFromFile("Data/mus.ogg");
	lvlMusic.setVolume(20);
	lvlMusic.play();
	lvlMusic.setLoop(true);

	//player data
	float player_x = 70; //500
	float player_y = 535; //150

	float speed = 5;

	const float jumpStrength = -18; // Initial jump velocity
	const float gravity = 1;  // Gravity acceleration

	bool isJumping = false;  // Track if jumping

	bool up_collide = false;
	bool left_collide = false;
	bool right_collide = false;

	Texture PlayerTexture;
	Sprite PlayerSprite;

	bool onGround = false;

	float offset_x = 0;
	float offset_y = 0;
	float velocityY = 0;

	float terminal_Velocity = 20;

	int PlayerHeight = 102;
	int PlayerWidth = 96;

	bool up_button = false;

	char top_left = '\0';
	char top_right = '\0';
	char top_mid = '\0';

	char left_mid = '\0';
	char right_mid = '\0';

	char bottom_left = '\0';
	char bottom_right = '\0';
	char bottom_mid = '\0';

	char bottom_left_down = '\0';
	char bottom_right_down = '\0';
	char bottom_mid_down = '\0';

	char top_right_up = '\0';
	char top_mid_up = '\0';
	char top_left_up = '\0';

	PlayerTexture.loadFromFile("Data/player.png");
	PlayerSprite.setTexture(PlayerTexture);
	PlayerSprite.setScale(3,3);
	PlayerSprite.setPosition(player_x, player_y);


// Collision and Game Over variables
bool playerHit = false;
Clock gameOverTimer;
bool showGameOver = false;

// Load player hit sprite 
Texture PlayerHitTexture;
PlayerHitTexture.loadFromFile("Data/player1.png");

// Player won
bool playerWon = false;
Clock winTimer;
bool showYouWin = false;



	
	// Bag sprite
	Texture BagTexture;
	Sprite BagSprite;
	
	BagTexture.loadFromFile("Data/bag.png");
	BagSprite.setTexture(BagTexture);
BagSprite.setScale(3, 3);  // Same size as player


// Vacuum sprite
Texture VacuumTexture;
Sprite VacuumSprite;
bool showVacuum = false;  // Controls when vacuum is visible

VacuumTexture.loadFromFile("Data/vaccum.png");
VacuumSprite.setTexture(VacuumTexture);
VacuumSprite.setScale(3, 3);  // Match player scale
VacuumSprite.setColor(Color(255, 255, 255, 100));  // 100 alpha (50 per. transparent)

// Vacuum range constant
const int vacuumRange = 200;  // Horizontal suction range in pixels
        
// ========== ADD GHOST DECLARATIONS HERE ==========
// Ghost data
const int numGhosts = 8;

float ghost_x[8];
float ghost_y[8];
float ghost_speed[8];
bool ghost_movingRight[8];
int ghost_platform[8];
Clock ghost_directionTimer[8];
float ghost_pauseDuration[8];

Texture GhostTexture;
Sprite GhostSprite;

GhostTexture.loadFromFile("Data/ghostk.png");
GhostSprite.setTexture(GhostTexture);
GhostSprite.setScale(2, 2);


// Initialize 8 ghosts on different platforms
ghost_x[0] = 200;
ghost_y[0] = (2 * cell_size) - 60;
ghost_speed[0] = 1.5f;
ghost_movingRight[0] = true;
ghost_platform[0] = 2;
ghost_pauseDuration[0] = 2.0f;

// Ghost 1 - Top platform (row 2)
ghost_x[1] = 600;
ghost_y[1] = (2 * cell_size) - 60;
ghost_speed[1] = 1.2f;
ghost_movingRight[1] = false;
ghost_platform[1] = 2;
ghost_pauseDuration[1] = 1.5f;

// Ghost 2 - 4th row platform
ghost_x[2] = 150;
ghost_y[2] = (4 * cell_size) - 60;
ghost_speed[2] = 1.8f;
ghost_movingRight[2] = true;
ghost_platform[2] = 4;
ghost_pauseDuration[2] = 2.5f;

// Ghost 3 - 4th row platform
ghost_x[3] = 800;
ghost_y[3] = (4 * cell_size) - 60;
ghost_speed[3] = 1.3f;
ghost_movingRight[3] = false;
ghost_platform[3] = 4;
ghost_pauseDuration[3] = 1.8f;

// Ghost 4 - 6th row platform (LEFT SIDE - before the gap)
ghost_x[4] = 200;  // Position on left section (columns 3-7)
ghost_y[4] = (6 * cell_size) - 60;
ghost_speed[4] = 1.6f;
ghost_movingRight[4] = true;
ghost_platform[4] = 6;
ghost_pauseDuration[4] = 2.2f;

// Ghost 5 - 6th row platform (RIGHT SIDE - after the gap)
ghost_x[5] = 700;  // Position on right section (columns 10-13)
ghost_y[5] = (6 * cell_size) - 60;
ghost_speed[5] = 1.4f;
ghost_movingRight[5] = false;
ghost_platform[5] = 6;
ghost_pauseDuration[5] = 1.6f;

// Ghost 6 - 8th row platform
ghost_x[6] = 250;
ghost_y[6] = (8 * cell_size) - 60;
ghost_speed[6] = 1.7f;
ghost_movingRight[6] = true;
ghost_platform[6] = 8;
ghost_pauseDuration[6] = 2.0f;

// Ghost 7 - 8th row platform (RIGHT SIDE)
ghost_x[7] = 900;  // Changed from 650 to 900 (further right)
ghost_y[7] = (8 * cell_size) - 60;
ghost_speed[7] = 1.5f;
ghost_movingRight[7] = false;  // Moving left from right side
ghost_platform[7] = 8;
ghost_pauseDuration[7] = 1.9f;



bool ghost_active[8] = {true, true, true, true, true, true, true, true};


// Skeleton data
const int numSkeletons = 4;


float skeleton_x[4];
float skeleton_y[4];
float skeleton_speed[4];

bool skeleton_movingRight[4];
int skeleton_platform[4];
float skeleton_velocityY[4];
bool skeleton_onGround[4];
Clock skeleton_actionTimer[4];
float skeleton_actionDuration[4];

Texture SkeletonTexture;
Sprite SkeletonSprite;

SkeletonTexture.loadFromFile("Data/skeletion.png");  // Correct filename
SkeletonSprite.setTexture(SkeletonTexture);
SkeletonSprite.setScale(2, 2);  // Adjust scale as needed



// Initialize 4 skeletons on different platforms
const int skeletonYOffset = 80;  // Changed from 64 to 80 to lift skeleton above blocks


// Skeleton 0 - 2nd row platform (LEFT side)

skeleton_x[0] = 250;
skeleton_y[0] = (2 * cell_size) - skeletonYOffset;
skeleton_speed[0] = 2.0f;
skeleton_movingRight[0] = true;
skeleton_platform[0] = 2;
skeleton_velocityY[0] = 0;
skeleton_onGround[0] = true;
skeleton_actionDuration[0] = 2.0f;


// Skeleton 1 - 2nd row platform (RIGHT side)
skeleton_x[1] = 750;
skeleton_y[1] = (2 * cell_size) - skeletonYOffset;
skeleton_speed[1] = 1.8f;
skeleton_movingRight[1] = false;
skeleton_platform[1] = 2;
skeleton_velocityY[1] = 0;
skeleton_onGround[1] = true;
skeleton_actionDuration[1] = 2.5f;


// Skeleton 2 - 6th row platform (LEFT section)
skeleton_x[2] = 250;
skeleton_y[2] = (6 * cell_size) - skeletonYOffset;
skeleton_speed[2] = 2.2f;
skeleton_movingRight[2] = true;
skeleton_platform[2] = 6;
skeleton_velocityY[2] = 0;
skeleton_onGround[2] = true;
skeleton_actionDuration[2] = 1.8f;


// Skeleton 3 - 6th row platform (RIGHT section)
skeleton_x[3] = 750;
skeleton_y[3] = (6 * cell_size) - skeletonYOffset;
skeleton_speed[3] = 1.5f;
skeleton_movingRight[3] = false;
skeleton_platform[3] = 6;
skeleton_velocityY[3] = 0;
skeleton_onGround[3] = true;
skeleton_actionDuration[3] = 2.2f;



bool skeleton_active[4] = {true, true, true, true};


     
 //  LEVEL ONE

	char** lvl = new char*[height];
	for (int i = 0; i < height; i++)
	{
	    lvl[i] = new char[width];
	}

	level_one(lvl, height, width);  


	Event ev;
	//main loop
	while (window.isOpen())
	{

		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed) 
			{
				window.close();
			}

			if (ev.type == Event::KeyPressed)
			{
			}

		}

		//presing escape to close
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

window.clear();


// Only allow movement if player hasn't been hit AND hasn't won
if (!playerHit && !playerWon)
{
    player_movement(lvl, player_x, player_y, speed, velocityY, jumpStrength, onGround, cell_size, PlayerHeight, PlayerWidth, height, width, PlayerSprite, BagSprite, facingRight);
}
// Vacuum control
if (Keyboard::isKeyPressed(Keyboard::Space) && !playerHit)
{
    showVacuum = true;
}
else
{
    showVacuum = false;
}

// Update vacuum position based on player facing direction - only if not hit
if (!playerHit)
{
    if (facingRight)
    {
        // Facing right - vacuum on LEFT middle edge of player
        VacuumSprite.setScale(-3, 3);
        VacuumSprite.setOrigin(74, 12);
        VacuumSprite.setPosition(player_x, player_y + PlayerHeight / 2);
    }
    else
    {
        // Facing left - vacuum on LEFT middle edge of player (normal orientation)
        VacuumSprite.setScale(3, 3);
        VacuumSprite.setOrigin(42, 12);
        VacuumSprite.setPosition(player_x, player_y + PlayerHeight / 2);
    }
}



 
// Vacuum control
if (Keyboard::isKeyPressed(Keyboard::Space) && !playerHit && !playerWon)
{
    showVacuum = true;
}
else
{
    showVacuum = false;
}
    // Update vacuum position based on player facing direction
    if (facingRight)
    {
        // Facing right - vacuum on LEFT middle edge of player
        VacuumSprite.setScale(-3, 3);  // Flip vacuum
        VacuumSprite.setOrigin(74, 12);  // Adjust origin for flipped sprite
        VacuumSprite.setPosition(player_x, player_y + PlayerHeight / 2);
    }
    else
    {
        // Facing left - vacuum on LEFT middle edge of player (normal orientation)
        VacuumSprite.setScale(3, 3);  // Normal scale
        VacuumSprite.setOrigin(42, 12);  // Origin at left edge
        VacuumSprite.setPosition(player_x, player_y + PlayerHeight / 2);
    }

// Call vacuum suction
vacuum_suction(player_x, player_y, PlayerWidth, PlayerHeight, facingRight, showVacuum,
               ghost_x, ghost_y, ghost_active, numGhosts,
               skeleton_x, skeleton_y, skeleton_active, numSkeletons,
               vacuumRange);


//  WIN CONDITION CHECK 
if (!playerHit && !playerWon)  // Only check if player hasn't been hit and hasn't won yet
{
    if (check_all_enemies_captured(ghost_active, numGhosts, skeleton_active, numSkeletons))
    {
        playerWon = true;
        winTimer.restart();
    }
}

// Check if 1 second has passed after winning
if (playerWon && winTimer.getElapsedTime().asSeconds() >= 1.0f)
{
    showYouWin = true;
}
//  END WIN CONDITION CHECK 




player_gravity(lvl, offset_y, velocityY, onGround, gravity, terminal_Velocity,
               player_x, player_y, cell_size, PlayerHeight, PlayerWidth);

// Update ghosts - only if player not hit
if (!playerHit && !playerWon)
{
    update_ghosts(ghost_x, ghost_y, ghost_speed, ghost_movingRight, ghost_platform, ghost_directionTimer, ghost_pauseDuration, numGhosts, lvl, cell_size, width, ghost_active);
}
// Update skeletons - only if player not hit
if (!playerHit && !playerWon)
{
    update_skeletons(skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight, skeleton_platform, skeleton_velocityY, skeleton_onGround, skeleton_actionTimer, skeleton_actionDuration, numSkeletons, lvl, cell_size, width, height, skeleton_active);
}



// COLLISION DETECTION between plpayer and enemy
if (!playerHit)  // Only check collisions if player hasn't been hit yet
{
    // Check collision with ghosts - only lower half of player
    for (int i = 0; i < numGhosts; i++)
    {
        if (!ghost_active[i]) continue;  // Skip inactive ghosts
        if (check_collision(player_x, player_y + PlayerHeight/2, PlayerWidth, PlayerHeight/2, 
                          ghost_x[i], ghost_y[i], 48, 48))
        {
            playerHit = true;
            PlayerSprite.setTexture(PlayerHitTexture);  // Change to hit sprite
            gameOverTimer.restart();
            break;
        }
    }
    
    // Check collision with skeletons (if not already hit by ghost) - only lower half of player
    if (!playerHit)
    {
        for (int i = 0; i < numSkeletons; i++)
        {
            if (!skeleton_active[i]) continue;  // Skip inactive skeletons
            if (check_collision(player_x, player_y + PlayerHeight/2, PlayerWidth, PlayerHeight/2, 
                              skeleton_x[i], skeleton_y[i], 48, 78))
            {
                playerHit = true;
                PlayerSprite.setTexture(PlayerHitTexture);  // Change to hit sprite
                gameOverTimer.restart();
                break;
            }
        }
    }
}
// Check if 1 second have passed after collision
if (playerHit && gameOverTimer.getElapsedTime().asSeconds() >= 1.0f)
{
    showGameOver = true;
}
// END COLLISION DETECTION  




// Draw everything
display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, height, width, cell_size);

// Draw ghosts
draw_ghosts(window, ghost_x, ghost_y, ghost_movingRight, numGhosts, GhostSprite, ghost_active);


// Draw skeletons
draw_skeletons(window, skeleton_x, skeleton_y, skeleton_movingRight, numSkeletons, SkeletonSprite, skeleton_active);


// Draw bag first (behind player) - only if player not hit
if (!playerHit)
{
    window.draw(BagSprite);
}

// Draw player
PlayerSprite.setPosition(player_x, player_y);
window.draw(PlayerSprite);

// Draw vacuum ONLY if space is pressed - ADD THIS
if (showVacuum)
{
    window.draw(VacuumSprite);
}
// Draw vacuum ONLY if space is pressed
if (showVacuum)
{
    window.draw(VacuumSprite);
}

// GAME OVER TEXT
if (showGameOver)
{
    // Create a simple rectangle background for visibility
    RectangleShape overlay(Vector2f(screen_x, screen_y));
    overlay.setFillColor(Color(0, 0, 0, 150));  // Semi-transparent black
    window.draw(overlay);
    
    Font font;
    // Try multiple font paths
    bool fontLoaded = false;
    
    if (font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf"))
    {
        fontLoaded = true;
    }
    else if (font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf"))
    {
        fontLoaded = true;
    }
    else if (font.loadFromFile("arial.ttf"))
    {
        fontLoaded = true;
    }
    
    if (fontLoaded)
    {
        Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(120);
        gameOverText.setFillColor(Color::Red);
        gameOverText.setStyle(Text::Bold);
        
        // Adding black outline for better visibility
        gameOverText.setOutlineColor(Color::Black);
        gameOverText.setOutlineThickness(5);
        
        // Center the text
        FloatRect textBounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(textBounds.width / 3.2, textBounds.height / 3.2);
        gameOverText.setPosition(screen_x / 3.2, screen_y / 3.2);
        
        window.draw(gameOverText);
    }
}
// END GAME OVER TEXT 



// YOU WIN TEXT 
if (showYouWin)
{
    // Create a simple rectangle background for visibility
    RectangleShape winOverlay(Vector2f(screen_x, screen_y));
    winOverlay.setFillColor(Color(0, 0, 0, 150));  // Semi-transparent black
    window.draw(winOverlay);
    
    Font font;
    // Try multiple font paths
    bool fontLoaded = false;
    
    if (font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf"))
    {
        fontLoaded = true;
    }
    else if (font.loadFromFile("arial.ttf"))
    {
        fontLoaded = true;
    }
    
    if (fontLoaded)
    {
        Text youWinText;
        youWinText.setFont(font);
        youWinText.setString("YOU WIN!");
        youWinText.setCharacterSize(120);
        youWinText.setFillColor(Color::Green);  // Green color for win
        youWinText.setStyle(Text::Bold);
        
        // Add black outline for better visibility
        youWinText.setOutlineColor(Color::Black);
        youWinText.setOutlineThickness(5);
        
        // Center the text
        FloatRect textBounds = youWinText.getLocalBounds();
        youWinText.setOrigin(textBounds.width / 3, textBounds.height / 3);
        youWinText.setPosition(screen_x / 3, screen_y / 3);
        
        window.draw(youWinText);
    }
}
// END YOU WIN TEXT 

window.display();


	}

	//stopping music and deleting level array
	lvlMusic.stop();
	for (int i = 0; i < height; i++)
	{
		delete[] lvl[i];
	}
	delete[] lvl;

	return 0;
}
