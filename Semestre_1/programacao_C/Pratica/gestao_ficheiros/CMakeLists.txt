cmake_minimum_required(VERSION 3.10)

# Projeto em linguagem C (nao C++)
project(gestao_ficheiros C)

set(CMAKE_C_STANDARD 99)

# Todos os ficheiros .c que fazem parte do programa
add_executable(gestao_ficheiros
        main.c
        utilitarios.c
        ficheiros_texto.c
        ficheiros_binarios.c)
