🐦 Fly Bird – Pro
SDL2-Based 2D Game in C

Fly Bird – Pro is a polished Flappy Bird–style 2D game developed in C using SDL2.
The project demonstrates game loops, event handling, animations, audio integration, UI design, and file handling — making it a strong resume and GitHub showcase project.


🚀 Key Highlights (Recruiter-Friendly)

Built completely in C using SDL2 ecosystem
Smooth gameplay with 60 FPS
Modular state-based architecture
Interactive UI buttons with animations
Sound effects & background audio
Persistent high-score system
Clean code with proper resource management


🎮 Game Features
🟢 Multiple Game States
    Main Menu
    Play Mode
    Pause
    Game Over

🐤 Physics-Based Gameplay
    Gravity simulation
    Jump mechanics
    Collision detection

🎨 Visual Effects
    Parallax scrolling background
    Animated hover & click buttons
    Smooth rendering using SDL Renderer

🔊 Audio Integration
    Jump sound
    Hover & click sounds
    Game over sound
    (implemented using SDL_mixer)


💾 File Handling
    High score stored persistently using file I/O

🛠️ Technologies & Tools Used
Category            	Technologies
Language	                C
Graphics	                SDL2
Images	                    SDL_image
Text	                    SDL_ttf
Audio	                    SDL_mixer
Version Control	            Git & GitHub
Platform	                Windows / Linux


📂 Project Structure
FlyBird/
├── src/
│   └── main.c
│
├── assets/
│   ├── bg_far.png
│   ├── bg_near.png
│   ├── bird.png
│   ├── pipe.png
│   ├── font.ttf
│   ├── jump.wav
│   ├── hover.wav
│   ├── click.wav
│   └── gameover.wav
│
├── .gitignore
├── README.md


    ▶️ How to Build & Run
🔹 Windows (MinGW)

gcc src/main.c -o flybird ^
-IC:/SDL2/include ^
-LC:/SDL2/lib ^
-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

Run:
flybird.exe


🔹 Linux
gcc src/main.c -o flybird \
-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
./flybird


📸 Screenshots
(Add gameplay screenshots here)
Example:
    screenshots/
    ├── menu.png
    ├── gameplay.png
    ├── gameover.png


🧠 What I Learned From This Project
    Implementing real-time game loops
    Handling keyboard & mouse events
    Using state machines for clean game logic
    Integrating audio & graphics in C
    Managing memory & resources safely
    Using Git & GitHub for version control


👤 Author
    Sourabh Nagarkar
    📌 Computer Science Student
    📌 Interested in Game Development & Systems Programming


🔗 GitHub: https://github.com/sourabhnagarkar


📄 License
This project is developed for educational and learning purposes.


⭐ Recruiter Note
This project demonstrates strong fundamentals in C programming, system-level libraries, and game development concepts, making it suitable for entry-level software, game dev, or systems programming roles.