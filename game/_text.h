#pragma once
#include <string>

namespace Text {
std::string introEmail = R"(<$FFFF00$>>> New e-Hail: <<: 
<$00FF00$>
Welcome:
<$FFFFFF$>
You have been accepted into the <$FF00FF$>EsoBinaria Research Program<$FFFFFF$>.
Our goal here is to <$FF0000$>[REDACTED]<$FFFFFF$> the EsoBinaria device.
To do this we must understand how it works.
We have a tome documenting its inputs and outputs but no instructions for how to build the device.
Your aim is to recreate small parts of the mechanism via reverse-<$FF0000$>[REDACTED]<$FFFFFF$>.
<$00FF00$>
Mission Statement:
<$FFFFFF$>
We are the binary star system, two parts that orbit each other.
We are the eternal void.
We are the light.
We are the EsoBinaria, the device that we seek to <$FF0000$>[REDACTED]<$FFFFFF$>.

------------------------------------------------
<$00FF00$>PRESS THE <ESC> KEY TO ENTER ESO-OS
)";
std::string hukE0a = "Hey there, I'm huk. Heard this was your first day! I'm here to train you on how to use this system. Let's get started! \n\nPress any key to continue.";
std::string hukE0b = "First things first. Listen to what I say very carefully because the ESObinaria is a very complex machine.";
std::string hukE0c = "The last thing I need is some kid like you muddying it up!";
std::string hukE0d = "Second thing, second. You can't break it. So don't worry about that. If you get stuck just click 'reset' from the 'edit' bar in on the top of the screen.";
std::string hukE0e = "What you're looking at now is the main interface. I'll get out of your way in just a second so you can see it better.";
std::string hukE0f = "Like I mentioned earlier, at the top of the screen we have the menu bar. You can use this to edit the machine, save your progress, reset the machine etc.";
std::string hukE0g = "Below that we have the grid (which I'm currrently covering). This is where you'll be placing the tiles.";
std::string hukE0h = "On the bottom of the screen we have the tiles bar. You can select a tile by clicking on it. You can place a tile on the grid by clicking on an empty cell.";
std::string hukE0i = "Below the tiles bar we have the 'ESOP' area. This is a mystical ESO-Processing language we invented to document and troubleshoot the EsoBinaria.";
std::string hukE0j = "Don't worry, you dont need to write that! Your tiles will magically generate the script.";
std::string hukE0k = "Try it now. Click the 'IN_B' tile and place it on the grid. Thats the one with 2 dots!";

std::string hukE1a = "I forgot to mention one of the most critical parts of the interface! The 'tests' area. This is where you'll see if your solution is working correctly.";
std::string hukE1b = "You can see it on the right side of the screen.";
std::string hukE1c = "Test cases have inputs and outputs. The inputs are on the left side of the test case and the output is on the right.";
std::string hukE1d = "The inputs on the left of the test case correspond with the tiles 'IN_A', 'IN_B', 'IN_C'. The state of the inputs is shown by the color potion ball.";
std::string hukE1e = "If the ball is blue, the input is true. If the ball is yellow, the input is false.";
std::string hukE1f = "The output on the right of the test case corresponds with the 'OUT' tile. The state of the output is shown by the color of the final potion ball.";
std::string hukE1g = "To complete a job you need to use the tiles to build a solution that satisfies every test case at once.";
std::string hukE1h = "If you see a green mark by the test, you're good to go! \n\nIf you see a red mark, you need to go back and fix your solution.";
std::string hukE1i = "Hover over a test case to see the state of the inputs for that test.";
std::string hukE1j = "Remember, If you ever get stuck, click the 'edit' button in the top menu bar and click 'reset'.";
std::string hukE1k = "Now try placing the 'NOT' tile on the grid. This one that looks like an green X.\n\nPlace it before the 'IN_A' tile!";

std::string hukE2a = "Good work kiddo. Now try using the clear tile to remove all the tiles from the grid. It's the red X on the left.";
std::string hukE2b = "As you remove tiles you will see error messages pop up. Don't worry, that's just the system telling you that you need to fix something.";
std::string hukE2c = "Normally you will want to heed the error messages but those can be ignored for now. Just remove all the tiles.";
std::string hukE2d = "You can 'comment' tiles by hitting the 'c' key or using the middle mouse button when over a tile. This will help you keep track of what each tile is doing.";
std::string hukE2e = "You can toggle the 'parentheses' on tiles by hitting the 'tab' key or using the right mouse button when over a tile. This will help you group tiles together.";
std::string hukE2f = "After removing everything, place the 'IN_A' tile to finish this job.";

std::string hukE3a = "This enigma has 2 test cases. The first test case has all inputs set to false. It wants an output of true.";
std::string hukE3b = "The second test case has its first input (a) set to true and the test to false. It wants an output of true.";
std::string hukE3c = "How can we build a solution that satisfies both test cases?";
std::string hukE3d = "These enigmas can get pretty tricky. I'm going to get some lunch and let you sort this one out yourself.";
std::string hukE3e = "After you sort this one out, you can get started on some real work.";
std::string hukE3f = "Good luck!.";

std::string hukM0a = "Hey kid, I'm back from lunch now... You're really getting the hang of this! I'm impressed.";
std::string hukM0b = "This enigma has 4 test cases. Don't worry though someday you'll be working on enigmas with 8 test cases!";
std::string hukM0c = "More test cases means more conflicts. You'll need to build a solution that satisfies all the test cases at once.";
std::string hukM0d = "Notice that this enigma has all 4 of its outputs set to false. This could be good hint that you'll need to use the 'NOT' tile.";
std::string hukM0e = "However I think if you look a little closer you will *see* the solution is simple.";
std::string hukM0f = "See if you can solve this one with only one tile!";

std::string hukM1a = "Ok kiddo, my work here is almost done. You're almost ready to take on the world!";
std::string hukM1b = "Well, the world of EsoBinaria at least. But we already knew that would happen...";
std::string hukM1c = "From here on out you are going to be on your own. You'll have to solve the enigmas without my help.";
std::string hukM1d = "I have some new recruits to train. They're not as smart as you but they're eager to learn.";
std::string hukM1e = "If you run into any trouble just consult the tome. It has all the information you need. You got one of those right?";
std::string hukM1f = "Anyways, good luck kid! I know you'll do great! See you at the Hydra party when we finally get this thing together!";
std::string hukM1g = "{END OF PRE-RECORDED TRAINING MESSAGES}";

std::string hukH69a = "Haha Nice";
};