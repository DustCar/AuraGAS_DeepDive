# About
This is my first Unreal Engine project using the Gameplay Ability System (GAS). This game was made in conjunction with the Udemy course made by Stephen Ulibarri, called "Unreal Engine 5 - Gameplay Ability System - Top Down RPG".

Throughout this project, I learned most of the core concepts that GAS revolves around, like Ability System Component, Attribute Set, Gameplay Abilities, and more that I will briefly mention in this README. 

I tried staying close to what the course provided to focus on learning the concepts, but there are quite a few decisions that I made that deviated from the course, as well as new additions that weren't present. And despite implementing some systems that was taught during the course, I have my own preferences to said systems and alternatives for them (i.e. using Model-View-ViewModel in the latter half for the sake of learning).

To play the game, just head over to the Releases page and download the zip under "Initial Completed Game Build".

# Core Concepts Learned
These are what I would say are the concepts that I learned the most about and retained the most from. They are also what I would be most confident about from everything that I learned.

### AbilitySystemComponent/AttributeSet
- How to set up custom ASC/AS on player state and on characters
- Setting up attributes on AS and handling changes due to GameplayEffects
  - Creating ExecCalcs and MMCs
- Giving and handling abilities on the ASC
### GameplayAbilities
- How to activate, commit cooldown and cost, and end abilities
### GameplayEffects
- How to create them through BP, and through C++ dynamically
- How to make GameplayEffectSpecs and apply them to self/target
- How to set up a custom GameplayEffectContext and set/get custom variables on it
### GameplayTags
- How to set up native GameplayTags for C++ and BP, and how to set them up in the editor
- I heavily used these for data-driven abilities, and attributes
### GameplayCues
- How to create Bursts, Notify Actor, and Notify Static variants
- How to add to actors, and how to execute them
### WidgetControllers
This section does not pertain to GAS but was a concept that I learned a great deal about and found very very useful in keeping systems separated from knowing each other. I really love the Model-View-Controller model and how effective it can be in separating concerns.
	- How to set up widget controllers (i.e. setting up classes where we get the data (ASC, PlayerState, etc.))
	- Binding delegates from data and broadcasting that data to UserWidgets
	
# Additional Concepts
Here are some additional concepts that I learned from this project but had less impact on me, either in terms of confidence or usefulness. Not to say that any of these are bad.

### MVVM using Unreal's experimental plugin
I used MVVM for my load menu widgets during the later parts of the course, but didn't really like it as much as using WidgetControllers. This could be partly due to the Unreal plugin still being relatively new and still in beta.
The setup is not any harder than that of the widget controllers but some of its niches made me not like it as much as creating my own widget controllers, although I understand why the engine wouldn't want to use the ViewModels if nothing is bound on the widget from the model.

Apart from some niches in setting up, the MVVM plugin is relatively friendly when the setup is correct. Binding variables to widgets is really straightforward and simple.

### AbilityTasks/AsyncTasks
For these tasks, I am not as confident in them as the main concepts but found these to be very useful. I can understand how to create them and how they work on a base level. However, I am definitely not comfortable enough with them yet. I didn't nearly have enough time with them as others but I can figure them out.

That should be most of the concepts that I wanted to point out that were mostly new to me. There are other concepts that I implemented that pertains to the above but I think they are straightforward. If you have any questions about decisions I made or how I implemented things, I would be happy to explain.

# Game Details
This game is a basic top-down RPG with levels, attributes, abilities, enemies, and other basics of many RPGs. All the assets I used were provided in the course. I have done playtesting throughout production and for the final build but I am sure there are still many bugs that I might have missed, so I apologize in advance. Now I do try to cover most bugs while developing so hopefully, nothing major is still there.

### Controls
The gameplay is very simple and only a few controls are needed.
- Movement: WASD/Click-to-move/Left Mouse Button Hold-to-move
- Use Ability: Target Enemy+Input/Shift+LMB/Input Button

### Abilities
I implemented a few offensive and passive abilities. These can be unlocked and equipped into any slot that is available on the hotbar. Offensive abilities are split into damage types, and each damage type has an associated debuff effect. Fire and Lightning have debuffs that have effects, while Physical and Arcane do not, which makes it harder to see if the effect is applied. Since I only implemented a few offensive abilities, the other ability slots will remain locked.

### Leveling Up
You can level up by killing monsters and upgrade/unlock abilities using spell points earned from leveling up. You can also upgrade your main attributes using attribute points also earned from leveling up.

### Enemies
The enemies use behavior trees and tasks to find you and attack you. Enemies can respawn again after re-entering a level and reaching their triggers. The enemies can also debuff the player. They can drop consumables that you can pick up to heal/replenish mana. Currently these are the only pickups, I have not implemented loot, like equipment, in this game.

### Saving
You can create up to 3 saves, and you can delete them too. The only way to save progress in game though, is to find checkpoints. Currently, these checkpoints are one-time use only, meaning that you might want to use them after making progress. I was planning to make checkpoints interactable, after reaching them, to give an option to save again but ultimately decided to leave that to later projects. I only include beacons that do nothing much other than show where you have been already.

### Dungeons/Entrances
There are entrances in the dungeons that allows you to travel from one dungeon to another. These entrances also act as save points so you can use these as save games in case all checkpoints have been reached. Currently, I have only provided two dungeons so there is technically only one entrance to each other.

That should cover most of what the game provides. I am sure there are things that I may have left out that you may see in game but because of my decision to keep it basic, anything new should not be too difficult to figure out.

# Project Statements
I ultimately decided to keep this project simple since my whole goal with it was to learn about GAS and get comfortable with it. Additionally, the extensive time I spent on it started giving me fatigue along the way. I also felt that if I ever wanted to implement something then it should be from the ground up with my own implementations. I felt that I should use this project as more of a template for future projects and implement any new systems then, although I did take some liberties that diverted from the course since I thought they felt like better alternatives.

Overall, after 1 year and 2 months in the making, I managed to finish the course and reach a "good" spot to complete this project. I may revisit it later but for now, I can say that I completed my first small RPG game.
