@echo off

REM Compila um projeto Raylib + Raygui estruturado

set SRC=src\main.c src\menu.c src\game.c src\text_utils.c src\player.c src\utils.c
set OUT=app.exe
set INCLUDES=-Iinclude -IC:\raylib\include
set LIBS=-LC:\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm

gcc %SRC% %INCLUDES% %LIBS% -o %OUT%

if %errorlevel% neq 0 (
    echo ERRO na compilação.
) else (
    echo Compilado com sucesso: %OUT%
)
