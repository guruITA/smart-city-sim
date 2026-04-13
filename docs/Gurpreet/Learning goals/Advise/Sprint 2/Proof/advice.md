# Smart cities learning group goal: Advise

- Name: Gurpreet Singh
- Date: 13-04-2026

I recommend using a separate header file, such as pin.h, to store all GPIO pin definitions for the ESP32-S3 project. This makes the code easier to read because the pin numbers are written once in a clear place instead of being spread across the .ino file as magic numbers. It also helps the whole team see which pins are already in use and what each pin is used for. Mats Otten gave me feedback that I was using magic numbers in the code, for example in ```StreetLight lamp(4, 5, 650, 1000);```, and this was a clear reason to improve the structure.

With a header file, the hardware settings are separated from the program logic. That makes the project easier to maintain, easier to reuse, and safer to change later. If a pin assignment needs to be updated, the team only has to change it in one file instead of searching through the whole codebase. This also reduces mistakes when several people work on the same embedded project and helps keep the shared code organized.

Because of this, I think a custom header file for pin definitions is the best choice for this project. It supports clearer teamwork, avoids confusion, and makes the final code structure more suitable for reuse and merging in the shared ESP32-S3 software.