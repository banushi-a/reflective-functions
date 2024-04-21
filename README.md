## Title:

3D Function Visualizer

## Using The Program

./project
-lod [integer for level of detail, default is 200]
-l [integer, how many units from the center should we render, default is 10]
"[y = f(x, z) in [Reverse Polish Notation](https://www.wikiwand.com/en/Reverse_Polish_notation) as strings, up to 9 unique functions]"

Some *fun*ctions:

- 3D Gaussian

  - ./project "e -1 x x * 2 / * z z * 2 / - pow"

- Ripple

  - ./project "x x * z z * + 2 * sin 2 /" -lod 300

- "Sphere"

  - ./project "16 x x * - z z * - sqrt" "16 x x * - z z * - sqrt -1 *" -lod 1000
  - **Note**: Handling complex numbers is a TODO item, so as of now complex results get mapped to zero, resulting in some extra triangles being rendered

- Variety of Sample Functions
  - ./project "e -1 x x * 2 / * z z * 2 / - pow" "x sin z sin +" "x z -" "-2" "x z 2 * +"

## YouTube/Dropbox/Drive Link:

https://youtu.be/o0IBH0cNsQU

## Screenshots

<img width="1279" alt="4300ex1" src="https://github.com/Spring24Graphics/finalproject-banushi-a/assets/89368127/a834520c-d81b-4c11-af7e-9bd15af7a092"><img width="1278" alt="4300ex2" src="https://github.com/Spring24Graphics/finalproject-banushi-a/assets/89368127/19cc3005-42b1-4920-ad51-5b3b9515bfba"><img width="1279" alt="4300ex3" src="https://github.com/Spring24Graphics/finalproject-banushi-a/assets/89368127/9dec8261-763e-419f-b8c2-de0381047ff8">
