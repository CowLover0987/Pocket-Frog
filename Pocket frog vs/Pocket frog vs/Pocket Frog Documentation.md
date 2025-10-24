🎮 Project Documentation: Pocket Frog 

1. Introduction 
* Project Overview:  

Pocket Frog is a 2D side-scrolling arcade game developed in C++ with Raylib. Players control Duke Leggington II, a frog knight armed with a button shield and needle sword, battling wool-creature enemies in a crocheter’s pocket dimension. The player must defeat the enemies, discarded scraps of wool now with glowing eyes and claws, along the way and reach then beat their high score of enemies defeated or wool collected. 

* Theme: Pocket 

Following the theme my game is within a pocket dimension, my pocket dimension, the pocket of a crocheter. Therefore, the design and world creation choice all came into play from what I regularly have within my own pockets. 

* Technologies Used:  

The language used in this project is C++ with the library Raylib, as for the IDE I used Visual Studio. 

2. How the Project Works 
* Core Gameplay Mechanics:  

A side scroller game consists of the player moving only left, right and jumping/attacking left and right, these movements are controlled by the arrow keys on the player’s keyboard with the addition of the space bar for attacking in which every direction the player currently is facing. For exiting the game before dying it is listed within the controls to use the escape button which will also trigger the save for the high score, otherwise when the player health is depleted to game over that also triggers the save for the high score for the next play. 

Figure 1:

 ![loading-ag-526](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Figures/Screenshot%202025-10-17%20124145.png) 

This is a mini arcade like game so there is no narrative goal in mind, it’s purely about gameplay; reaching a high score and then besting it.  

In further development adding specific goals/numbers to reach in a playthrough may create the feeling of having levels to beat. 

* Game Flow:  

There is no menu start, from opening the application you are introduced to the game itself but after the first 10 seconds the welcome message with disappear and the player is left with the controls, health and score. All displayed in the corners but readable as to minimise possible disruption to the gameplay.  

The end sequence, once the player health reaches 0, the screen is cleared with a black background and a red ‘GAME OVER’ appears with control keys for wither player again or exiting the application. 

* System Architecture: 

* Overview of major systems (e.g., player controller, enemy AI, UI, level loading). 

Player Controller – this handles the player movement including walking, jumping and attacking, all with catches for the necessary direction the player is facing for the animation frame of each.  



Each of the singular frames is connected the direction and current action of the player as to display the correct frame.  

Collision boxes allow for the player’s collisions with other objects, including coming into contact with obstacles and for giving/taking damage to/from the enemy. 

Figure 2: 

![loading-ag-528](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Figures/Screenshot%202025-10-22%20154935.png) 

The player is also the anchor for how the camera moves through out gameplay; the camera is locked onto the player’s position and moves accordingly to keep the player in the centre of the screen. 

Enemy AI – this manages how the enemies behave, including their idle/wander/chase/attack states. Each of these acting on the proximity of the player to the enemy and switching between each of them when needed. 

Reusing the same collision logic built for the player, the collision boxes for the enemy allow for the enemy to be prevented from moving through obstacles and give/receive damages to/from the player when their collision boxes overlap. 

The animation frame is accommodated to switch to between left and right depending on which direction the wander/chase state is moving in. 

Enemy Code Snippet: 
```c
// Figure out how far the player is from the enemy
Vector2 toPlayer = { playerPos.x - position.x, playerPos.y - position.y }; float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y); 

 // Face the direction the enemy is moving
if (velocity.x > 0) facingRight = true; else if (velocity.x < 0) facingRight = false;
// If the player is close enough, chase them
if (distance < detectionRadius) {
// Only move left or right (not up/down)
float length = std::sqrt(toPlayer.x * toPlayer.x);    if (length != 0) {        velocity.x = (toPlayer.x / length) * chaseSpeed;    }    velocity.y = 0; } else {
// If the player is far away, wander back and forth slowly
velocity.x = std::sin(GetTime()) * speed;    velocity.y = 0; }  
```
UI system – displays the health of the player, the current and high score, the player controls and the welcome message within the beginning. All scores and health updating to display the correct number of hits given to the player and given to the enemies to allow for the player always know what their score/health is. 

* How these systems interact. 

Player controller and: 

Enemy AI – the player’s position is needed for when the player and enemy are in a certain proximity the chase state is activated in the enemy. The enemy and players collisions when overlapping trigger taking or receiving damage. The players position also dictates the erasing of an enemy if the player moves a certain amount past as to clear memory. 

Camera Systems – The camera requires the player’s positions in order stay locked on and make it appear the player is in the centre of the window, also to be able to create smooth jumping without causing sudden camera movement with player jumping. 

UI system – the UI system needs the current score and health in order to update and display it correctly. 

* If relevant, include a simple diagram or flowchart. 

* Data & Assets:  

* Sprites 

Sprites and graphics are kept within the Resource folder, including player, enemy and environment graphics and my imported font, and are names according to their purpose and direction if necessary. E.g. frog_right.png, attack_left.png etc. 

* Sound 

Sounds are kept within the Resource folder, in a folder named Music and Sound Affects. 

* Scripts 

All scripts are kept within a main game folder and are named according to their object and then further defined by the file type. 

3. How Others Can Build, Run, or Test It 
* Requirements: 

* Software – Windows. 

* Hardware – a keyboard for controls. 

* Installation Steps: 

* Download the zip folder from itch.io. 

* Unzip the folder and first open the README.txt. 

* Running the application ‘Pocket Frog vs’, open it without the need of an IDE. 

* Testing Instructions: 

* Within the game folde there is a READ ME file wit a lnk to the feedback form. 

* If there are any bugs encountered the form has the link to a bug report. 
4. Key Design and Development Decisions 
* Artistic Choices: 

Duke Legginton II’s design came from the very thing I had in my pocket at the time of this project assignment; a small plushie frog called Leggs. I drew the duke with a close likeness to Leggs, but the added button shield and needle sword came from my hobby as a crocheter and sewer, in which I always carried my crafting tools and materials around with me. 

Figure 3: 

![loading-ag-530](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Figures/idle_right.png) 

The enemies design came from the supply of wool balls I have but throughout my bags and clothing there are scraps of wool that became the idea to have them as enemies, since they do float around both in my clothing and bag pockets. The addition of the eyes and claws were to allow for them to ‘see’ the player to chase and then to be able to attack. 

Figure 4: 

![loading-ag-532](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Figures/enemy_left.png) 

The bushed/objects were originally going to be flowers made from thread that had been tied and cut from sewing projects but drawing thread became unrecognisable when scaled down for the game so working from using bobby pins in that design, I drew a selection of random bobby pins within a pin cushion. Once used within in the game they became more of obstacles that plants, but their design was directly taken from the colour of my wrist pin cushion that I leave with my large supply of embroidery threads. 

Figure 5: 

![loading-ag-534](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Figures/bush_2.png) 

The background and enemies were created via Copilot (and then edited) with the prompts: 

Wool Enemy - ‘Create an evil bundle of wool with evil yellow eyes and claws’. 

Loopable Background - ‘Create a loopable background of a forest where the trees are topped with bundles of wool and the lily pads are fabric that been sewed’. 

* Gameplay Design:  

The mechanic design was firstly chosen to facilitate my beginner experience to creating games then built to resemble that of arcade games as the first side scroller games. 

As arcade games were the first that were built, things like pong, Pac-man and space invades, creating my own became symbolic for my first published game but in my own style and creative choices. 

The core gameplay is jumping obstacles and attacking enemies, it’s a simple and responsive arcade-type game experience with the only aim of building up a score or for the theme and design each enemy defeated = 1 wool bundle collected. 

The difficulty is low as it’s not a challenging game, it’s a simple game loop for short play sessions but still allow for people to feel they still need to beat their high score. 

* Challenges & Solutions: Briefly discuss major problems and how you solved them. 

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

5. Instructions for Users, Testers, or Collaborators 
* For Users (Players): 

* How to start and play the game. 

The game is published on the online platform Itch.io and is a zip file that needs to be downloaded from the site. After unzipping the folder, it is asked within the ‘download instructions’ on itch.io to read the text file called: READ ME FIRST. 

Within in this file it explains what the different files are that were downloaded and which of them is the game itself along with credits of the game's creation. 

Figure 6: 

![loading-ag-536](file:///C:/Users/saman/OneDrive/Documents/GitHub/Pocket-Frog/Pocket%20frog%20vs/x64/Debug/Resource%20Files/Figures/Screenshot%202025-10-22%20155348.png) 

The game has a simple aim for the player: Defeat as many enemies as they can, and from there to beat their high score. 



* Known issues or limitations. 

The game window has set limits to size so it cannot be made bigger or smaller, these limitations were implemented for the consistency and size of the graphics. 

* For Testers: 

* What to focus on testing. 

Testers are given the game in early access while it’s still in development as to gather feedback for both possible bug spotting and correction but also for improvements. 

Pocket frog is a simple looped point gathering game styled to fit the pocket theme, testers, some knowing previously what the game is and other given it blind, some technically experience and some non-technical, are all given the game form and questions to answer. 

Questions are a mix of quantitively and qualitive as to allow for rating to be made then analysed but also descriptions and feedback to be given for building on. 

The questions ask about gameplay, graphics, audio, any bugs, the platform they played on, accessibility, time spent playing and general feedback and if they’d recommend the game to others. 

* How to submit feedback. 

In the game folders READ ME file there is a link to take the testers to a google form and within that, if needed, to be taken to another form for reporting bugs found. 

The form is linked to a spread sheet to track the inputs from the form to choose and create graphs from a specific question’s answers.   



6. Future Improvements 
* Planned features or expansions. 

* Enemy expansion – the idea for the enemies came was first defined to have several forms, each with a slightly different shape but more importantly to have different colours so despite the fact each kill still equals a point, there would be a separate counter for the colour of wool collected. This would expand the main goal into needing to collect certain colours for either a prize, upgrade or that gathered wool would create/crochet something, the something becoming like a medal/badge that would be saved and unique to the player’s progress. 

* Platforms – at the moment Duke Leggington walks across a flat landscape which is somewhat boring and minimises the use of the jump mechanic, adding more platforms also allowed for more places for the spawning of enemies which could expand the types of enemies more, only some like to be up high for example, it would also add more exploration and dept to the players time. 

* Alternate forms of attack – the player plays as Duke Leggington and during development the idea of using the frog tongue as a form of attack was dismissed as the needle is within the theme of (crocheter’s) pocket but adding it as another form to take out enemies allows for long distance combat. 

* Levels/other locations – Adding levels will allow for the added goals of different types of colour wool to collect to be changed each level and allow for a range of goals to be set but not all at once creating a pace for the player. 

* Possible optimizations or refactoring ideas. 

The player animation became a style choice but creating a walking/jumping/attacking animation cycle rather than a singular frame may be a player satisfaction change needed. 

7. Conclusion 
* Using the raylib library 

* Creating a graphic game without the use of a game engine 

* Using Visual Studios 

* First Published game on public platform 
8. Feedback and Evaluation 

8.1 Data Collection 

* Methods Used: 

* How you gathered feedback (e.g., online survey, interviews, observation). 

* Number and type of participants (e.g., 10 players aged 15–25). 

* When and where testing took place. 

8.2 Summary of Feedback 

* Positive Responses: 

* What players enjoyed (e.g., visuals, controls, atmosphere). 

* Negative Responses or Issues Found: 

* Common complaints or confusing areas. 

* Bugs or usability issues noted by players. 

* Quantitative Data (if applicable): 

* Include simple stats or charts (e.g., “80% found controls easy to use”). 

8.3 Analysis and Insights 

* Interpret what the feedback means: 

* What patterns or trends appeared? 

* Did players respond as expected? 

* How did the feedback confirm or challenge your design goals? 

8.4 Changes Made Based on Feedback 

* List specific adjustments you made or plan to make: 

* Example: “Reduced player speed slightly for better control.” 

* Example: “Added tutorial pop-ups based on confusion reported.” 

8.5 Reflection 

* What you learned from player testing. 

* How the feedback improved your understanding of design, user experience, or accessibility.
