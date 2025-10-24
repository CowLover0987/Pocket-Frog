🎮 Development Commentary – Pocket Frog 

1. Task Description 

Summary of Assignment: 

The task is to create a game surrounding the theme ‘Pocket’ and to be built using the library Raylib. 

Project Goal: 

My goal for this project is to create a simple side scroller game, the mechanics taking inspiration to the arcade game Super Mario Bros and Google’s Dino Run. The in-game goal is point collecting and then beating previous high scores, all of which is to be saved for the player’s next playthrough. 

The addition of relevant graphics but also vibe matching audio and sound affects as to set the scene for the player. 

The style and graphics' purpose are to show the theme to the player of ‘Pocket’, more specifically my pocket, that’s filled with wool, thread and scraps from my different crafting projects. All of which is reflected in the player, enemy, background and objects design. 

Technologies Used: 

Tools/Software: 

* Visual Studio - IDE 

* Raylib Library - programming library for creating games and graphical applications 

* Krita - open-source digital painting and animation application 

* Sketchbook - a digital drawing, painting, and sketching application 

* Copilot - an AI-powered assistant designed to boost productivity and help with tasks 

* Typedown - Markdown editor built for Windows 

* Pixabay - an online platform offering a large library of free stock media 

* Jsfxr - a JavaScript-based tool that generates retro 8-bit sound effects for games 
2. Development Process 

2.1 Planning and Concept 

The idea for this game came from what was in my pocket at the time of its assignment; a plushie frog and half-finished crochet project. My plushie frog; Duke Leggington II became the main protagonist and the random, small and unusable scraps of wool left in my pockets became the antagonist. 

Thinking first project, I though simply and I came to the conclusion of a side on game like early games. From that I started thinking about the Dino Run game which is simple and has a working point and high score system, but unlike that I wanted an attacking system rather than just a jumping system for points since I already had an enemy in mind. 

Figure 1: 

![loading-ag-677](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/IMG_20251023_150312855_MFNR.jpg) 

Early sketch of my protagonist and mechanic notes. 

Figure 2: 

![loading-ag-679](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/IMG_20251023_150326105_MFNR.jpg) 

Figure 3: 

![loading-ag-681](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/IMG_20251023_154107388_MFNR.jpg) 

2.2 Design and Prototyping 

* Explain your initial prototype or wireframe. 

I started building the basic movements with a blue rectangle as my player and little green squares as objects to jump over. 

The red lines are the visual representation of the world bounds to keep the player within the world space. 

The welcome message at the top corner is the start of any future UI messages and score keeping that sticks to the players viewport. 

* Describe your early builds, gameplay loop, and scene setup. 

After completing the movement, jump and collisions for the player I moved on to the beginning of an enemy.  

A large orange square became the enemy placeholder, reusing the player collision, I added a collision to the enemy before adding a wander state so that it moved left and right on its own. 

The game loop came later on, after all movement and player<-->enemy interactions were completed, the loop is for the spawning of enemies and bushed as the player moves further in the game and any that are a certain way behind the player get unloaded. 

* Mention any iterations you went through. 

Figure 4:

 ![loading-ag-683](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Screenshot%202025-10-23%20152728.png) 

Initial prototype showing player movement testing. 

2.3 Implementation 

* Explain how main systems were built: 

* Player movement 

Inputs from the arrow keys on the keyboard are used to trigger the players movement a certain amount of distance.  

The jump also takes a keyboard input but sends the player in a vertical movement in the same direction that the player is facing. 

Attacking also takes a keyboard in put but from the space bar and does not move the player, it triggers the graphic frame change and activates a set attack timer for the showing of the attack frame, also activating an additional collision box in front of the player which when overlapping with an enemy deals damage to the enemies health. 

Player’s Movement Code Snippet: 

// Move left or right based on keyboard input 

    if (IsKeyDown(KEY_LEFT)) { 

        move.x -= 200 * dt; 

        facingRight = false; 

    } 

    if (IsKeyDown(KEY_RIGHT)) { 

        move.x += 200 * dt; 

        facingRight = true; 

    } 

    // Try to move the frog, checking for bush collisions 

    TryMove(move, bushes); 

    // Jump if the player presses UP and is on the ground 

    if (IsKeyPressed(KEY_UP) && isOnGround) { 

        velocity.y = jumpForce; 

        isOnGround = false; 

        PlaySound(jumpSound); 

    } 

    // Start attacking if SPACE is pressed 

    if (IsKeyPressed(KEY_SPACE) && !isAttacking) { 

        isAttacking = true; 

        attackTimer = attackDuration; 

PlaySound(attackSound); 

    } 

* Interactions 

Interactions between player, objects and enemies are handled by collision boxes that dictate what happened when they are overlapped. 

The main collision boxed for each object are to prevent them from moving through each other and for taking damage while the two addition collision boxes for the player and enemy are to dealing damage to each other. 

Player Collision Code Snippet: 

// Returns the player's hitbox — used to detect collisions with enemies or bushes 

Rectangle Player::GetHitbox() const { 

    return hitbox; 

} 

// Returns the sword's hitbox when the player is attacking 

Rectangle Player::GetAttackHitbox() const { 

} 

// Check if the player is trying to move sideways into a bush. 

bool blockedX = false; 

    for (const Bush& bush : bushes) { 

        Rectangle bushCollider = bush.GetCollider(); 

        bool verticallyAligned = proposedX.y + proposedX.height > bushCollider.y && 

            proposedX.y < bushCollider.y + bushCollider.height; 

        if (verticallyAligned && CheckCollisionRecs(proposedX, bushCollider)) { 

            blockedX = true; 

            break; 

        } 

    } 

    if (!blockedX) { 

        position.x = proposedX.x; 

    } 

Enemy Collision Code Snippet: 

// This returns the enemy's hitbox — the area where it can be attacked 

Rectangle Enemy::GetCollider() const { 

    return { position.x - 50, position.y - 50, width, height }; 

} 

    // Predict where the enemy wants to move 

    Rectangle proposed = { position.x + delta.x, position.y + delta.y, width, height }; 

    // Keep the enemy inside the level boundaries 

    proposed.y = Clamp(proposed.y, levelBounds.y, levelBounds.y + levelBounds.height - height); 

    // Check if the enemy would bump into a bush 

    for (const Bush& bush : bushes) { 

        if (CheckCollisionRecs(proposed, bush.GetCollider())) { 

            return; // Stop moving if blocked 

        } 

    } 

* UI / menus 

The UI options are displayed on the bottom of the screen during game for exiting correctly with saving and on the GAME OVER scene for either exiting or replaying.  

The same as the controls displayed on the UI system the high score and current score are displayed. 

UI code snippet: 

// Create a string that shows how many enemies have been defeated (wool collected) 

std::string counterText = "Wool Collected: " + std::to_string(enemiesDefeated); 

// Measure how wide the text will be when drawn, so we can align it properly on screen 

int textWidth = MeasureText(counterText.c_str(), 20); 

// Draw the wool counter near the top-right corner, using the measured width to position it 

DrawTextEx(dyslexicFont, counterText.c_str(), { (float)(GetScreenWidth() - textWidth - 220), 20.0f }, 50, 2, WHITE); 

// Draw the high score just below the wool counter, slightly offset to align visually 

DrawTextEx(dyslexicFont, TextFormat("High Score: %d", highScore), { (float)(GetScreenWidth() - textWidth - 217), 50.0f }, 50, 2, WHITE); 

// Show the control instructions at the bottom of the screen so players know how to play 

DrawTextEx(dyslexicFont, "Controls: Left & Right = Arrow Keys, Jump = Up Arrow Key, Attak = Space Bar, Exit = Esc", { 20, 670 }, 25, 2, WHITE); 

// Display the music status (muted or on) depending on the current setting 

if (isMuted) { 

    // If music is muted, show a gray message 

    DrawTextEx(dyslexicFont, "Music: Muted (Press M)", { 180, 60 }, 30, 2, GRAY); 

} else { 

    // If music is playing, show a white message with instructions to mute 

    DrawTextEx(dyslexicFont, "Music: On (Press M to mute)", { 180, 60 }, 30, 2, WHITE); 

} 

* Audio / Sound affects 

Audio is played in the background when the game starts and can be muted, as displayed on screen during play and reads either to mute the music or that it is already muted, by pressing a keyboard key. 

Sound effects can be heard when they are active during certain moments of the player such as attacking and jumping. 

Audio/Sound Affects Code Snippet: 

Music bgMusic; 

bool isMuted = false; 

float musicVolume = 0.5f; // Default volume 

    bgMusic = LoadMusicStream("Resource Files/Music and Sound Affects/background_music.mp3"); 

    PlayMusicStream(bgMusic); 

// Load sounds as member variables jumpSound = LoadSound("Resource Files/Music and Sound Affects/jump_sound_affect.wav"); attackSound = LoadSound("Resource Files/Music and Sound Affects/attack_sound_affect.wav"); 

    // Start attacking if SPACE is pressed 

    if (IsKeyPressed(KEY_SPACE) && !isAttacking) { 

        isAttacking = true; 

        attackTimer = attackDuration; 

PlaySound(attackSound); 

    } 

    // Jump if the player presses UP and is on the ground 

    if (IsKeyPressed(KEY_UP) && isOnGround) { 

        velocity.y = jumpForce; 

        isOnGround = false; 

        PlaySound(jumpSound); 

    }  

Figure 5: 

![loading-ag-685](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/IMG_20251023_154044100_MFNR.jpg)![loading-ag-687](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/IMG_20251023_154048467_MFNR.jpg) 

Examples of my methods of noting daily accomplishments and setting minimums and goals for the next day. 

2.4 Testing and Iteration 

The game is tested via its early access on itch.io with a QR code link to a form with question on graphics, audio, gameplay and general feedback. 

2.5 Feedback and Adjustments 

Feedback is gathered via a google form, the form is accessed by a QR code that is displayed on the game window, within the form is the link to a bug report if any bugs are found by testers. 

The testers are asked to answer the following qualitive and quantitive questions: 

1. Do we have consent to collect your data from this form? 

2. Would you like to be credited as a game tester? 

3. If Yes, please enter the name you wish to be credited to 

4. What platform did you play on? 

5. How long did you play for? 

6. How was the over all gameplay? 

7. Is there anything about the gameplay you want to add? 

8. How would you rate the character graphics? 

9. Is there anything about the graphics you want to add? 

10. Was there anything confusing or unclear during gameplay? 

11. Was the dyslexia-friendly font helpful or noticeable? 

12. How would you rate the music choice? 

13. How would you rate the sound affect creation? 

14. Did you encounter any bugs? 

15. If yes, please scan QR code to go to my Bug survey 

16. Was there anything that stood out? 

17. What would you change or improve?   

18. Would you recommend this game to others? Why or why not? 
* Summarize the main findings and how you acted on them. 

Figure 5: 

 ![loading-ag-689](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Screenshot%202025-10-24%20150221.png) 

An early figure depicting the three responses from the three levels of development in which were tester indicating that the time played grew as development finalised. 

The tester who chose 1-2 mins tested during the first stages where only basic movement and player, bush and enemy interactions were created with only beginning graphics. 

The second option of 3-5 mins was the net stage where the game loop had been implemented but at a lower spawn rate and not all graphics had yet to be implemented so visually it was also uninteresting. 

The testers who played for more than 5 minutes was given the latest version of Pocket frog with a higher spawn rate game loop, all graphics and UI implemented, and the audio and sound affects had been added.  

Both qualative questions, ‘Is there anything about the graphics you want to add?’ and ‘What would you change or improve?’, received overlapping feedback for expansion. 

A few respoces for ‘Is there anything about the graphics you want to add?’: 

* ‘More weapon and more monster’  

* ‘Maybe can add weather’ 

A few respoces for ‘What would you change or improve?’: 

* ‘I might want to add different attack methods or increase the reward system.’ 

* ‘I wouldn’t change anything however I know it’s in development so you may have this planned already but you could add some platforms again I know it’s early development so you may have this already mapped out’ 

Agreeing with the ideas for expansion beyond the simplicity of a running side scroller with a singular point system, all expansion ideas for improvement go into my future plans for development. 

Among the positive feedback: 

For all rating questions no response went below 4/5 stars. 

* How was the over all gameplay?: 

![loading-ag-691](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Screenshot%202025-10-24%20150233.png) 

* How would you rate the character graphics?: 

![](data:image/png;base64,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) 

3. Declared Assets 
* Code Snippets 

raylib - examples (s.d.) At: [https://www.raylib.com](https://www.raylib.com/) (Accessed 24/10/2025). 

* Background Music 

Halloween Kids Background Music | Royalty-free Music (s.d.) At: [Halloween Kids Background Music | Royalty-free Music - Pixabay](https://pixabay.com/music/scary-childrens-tunes-halloween-kids-background-music-417482/) (Accessed 23/10/2025). 

* Accessible Font 

OpenDyslexic (s.d.) At: [https://opendyslexic.org/](https://opendyslexic.org/) (Accessed 23/10/2025). 

* The background, player and enemy graphics 

Microsoft Copilot: Your AI companion (s.d.) At: [https://copilot.microsoft.com](https://copilot.microsoft.com/) (Accessed 23/10/2025). 

4. Discussion and Reflection 

4.1 Challenges Faced and Solutions 

| Challenge                                                       | Solution                                                                                                                                                                                                                                                         |
| --------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Landing on top of a bush would make the player stick            | The collision didn’t see a difference from the player colliding with the player from the side and landing on top, I had to update the collision to check the collision direction and to allow for landing on top.                                                |
| No bound collisions                                             | The player was able to move completely off screen, adding in a large box inverted collision the player stayed within those level bounds.                                                                                                                         |
| Graphic files wouldn’t load successfully                        | The graphic files weren’t loading when the game was being played, the files weren’t in the correct place from the game to locate then load the, moving them fixed that.                                                                                          |
| Enemies would fly through objects                               | The collision for the enemy had only been set up for attacking the player, updating this allowed for the enemy collision to prevent it from moving through objects and to be stopped by them, giving the player a means to stop the enemy from getting to them.  |
| Attacking the enemy failed                                      | The collision box for the needle sword was not created properly so despite the animation frame appearing there was no collision to deal damage to the enemy, properly creating and timing it to the attack frames appearance corrected that.                     |
| Counting enemies defeated/player health not updating UI display | The function handling damage taken/enemies defeated wasn’t incrementing the variable, so I reordered where the incrementation rook place.                                                                                                                        |
| High score not updating                                         | The update score function was ordered incorrectly so that the application closed before saving the update, reorganising the code allowed for the score to be saved before closing.                                                                               |
| Enemy spawning incorrectly                                      | The enemies were still attached to the world level bounds and wouldn’t spawn the previous world bound, removing that clamp allowed for their spawning further in the game as wanted, also increased the chance of spawn to create more chances to gain ‘points’. |

4.2 Future Development 

* Planned features or expansions. 

* Enemy expansion – the idea for the enemies came was first defined to have several forms, each with a slightly different shape but more importantly to have different colours so despite the fact each kill still equals a point, there would be a separate counter for the colour of wool collected. This would expand the main goal into needing to collect certain colours for either a prize, upgrade or that gathered wool would create/crochet something, the something becoming like a medal/badge that would be saved and unique to the player’s progress. 

* Platforms – at the moment Duke Leggington walks across a flat landscape which is somewhat boring and minimises the use of the jump mechanic, adding more platforms also allowed for more places for the spawning of enemies which could expand the types of enemies more, only some like to be up high for example, it would also add more exploration and dept to the players time. 

* Alternate forms of attack – the player plays as Duke Leggington and during development the idea of using the frog tongue as a form of attack was dismissed as the needle is within the theme of (crocheter’s) pocket but adding it as another form to take out enemies allows for long distance combat. 

* Levels/other locations – Adding levels will allow for the added goals of different types of colour wool to collect to be changed each level and allow for a range of goals to be set but not all at once creating a pace for the player. 

* Possible optimizations or refactoring ideas. 

The player animation became a style choice but creating a walking/jumping/attacking animation cycle rather than a singular frame may be a player satisfaction change needed. 

5. References 
* Play the original Super Mario Bros Game Online (s.d.) At: [https://supermarioplay.com](https://supermarioplay.com/) (Accessed 24/10/2025). 

* Chrome Dino Game (s.d.) At: [https://chromedino.com](https://chromedino.com/) (Accessed 24/10/2025). 

* Visual Studio: IDE and Code Editor for Software Development (s.d.) At: [https://visualstudio.microsoft.com](https://visualstudio.microsoft.com/) (Accessed 24/10/2025). 

* raylib - examples (s.d.) At: [https://www.raylib.com](https://www.raylib.com/) (Accessed 24/10/2025). 

* Digital Painting. Creative Freedom. (s.d.) At: [Krita | Digital Painting. Creative Freedom.](https://krita.org/en/) (Accessed 23/10/2025). 

* Sketchbook - For everyone who loves to draw (s.d.) At: [https://www.sketchbook.com](https://www.sketchbook.com/) (Accessed 23/10/2025). 

* Microsoft Copilot: Your AI companion (s.d.) At: [https://copilot.microsoft.com](https://copilot.microsoft.com/) (Accessed 23/10/2025). 

* Typedown - Free download and install on Windows | Microsoft Store (s.d.) At: [Typedown - Free download and install on Windows | Microsoft Store](https://apps.microsoft.com/detail/9p8tcw4h2hb4?hl=en-gb&gl=US) (Accessed 24/10/2025). 

* 5.7 million+ Stunning Free Images to Use Anywhere - Pixabay (s.d.) At: [https://pixabay.com/](https://pixabay.com/) (Accessed 24/10/2025). 

* jsfxr - 8 bit sound maker and online sfx generator (s.d.) At: [https://sfxr.me/](https://sfxr.me/) (Accessed 23/10/2025). 

* Beginning C++ Programming - From Beginner to Beyond | Udemy (s.d.) At: [Beginning C++ Programming - From Beginner to Beyond | Udemy](https://www.udemy.com/course/beginning-c-plus-plus-programming/?couponCode=25BBPMXINACTIVE) (Accessed 24/10/2025). 

* Get Started in raylib in 20 minutes! (2024) Directed by Programming With Nick. At: [Get Started in raylib in 20 minutes! - YouTube](https://www.youtube.com/watch?v=RGzj-PF7D74) (Accessed 24/10/2025). 

* 🔥Pong Game with C++ and Raylib - Beginner Tutorial - YouTube (s.d.) At: [🔥Pong Game with C++ and Raylib - Beginner Tutorial - YouTube](https://www.youtube.com/watch?v=VLJlTaFvHo4) (Accessed 24/10/2025). 

* C++ Snake game using raylib - Beginner Tutorial 🐍 (OOP) (2023) Directed by Programming With Nick. At: [C++ Snake game using raylib - Beginner Tutorial 🐍 (OOP) - YouTube](https://www.youtube.com/watch?v=LGqsnM_WEK4) (Accessed 24/10/2025).
