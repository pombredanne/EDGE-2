cl main.cpp -I ..\3rd\glew\include -I ..\3rd\freeglut\include -I ..\16 -I ..\3rd\ -I ..\..\3rd ..\3rd\freeglut\src\*.c -DFREEGLUT_LIB_PRAGMAS=0 -DFREEGLUT_STATIC ..\3rd\glew\src\glew.c -DGLEW_STATIC opengl32.lib ..\16\eve\window.cpp ..\16\eve\camera.cpp -I ..\13 -I ..\3rd\glfw\include ..\16\eve\glfw.c -I .. 
