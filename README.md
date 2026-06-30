# ArcadiaOS

## Sobre
**ArcadiaOS** é um pseudo-SO desenvolvido para a disciplina **Fundamentos de Sistemas Operacionais** da **Universidade de Brasília**. Seu propósito é aplicar os conhecimentos teóricos aprendidos durante a disciplina em um softwares simples capaz de gerenciar recursos e proporcionar abstração ao usuário.

## Prerequisitos

Para compilar e executar, certifique-se de que sua máquina tenha:
- Um compilador compatível com o padrão C++ (c++17 ou superior).
    - **Linux:** GCC (g++) ou Clang
    - **Windows:** MinGW-w64 (g++)
    - **MacOS:** Clang

Para verificar se o compilador está instalado, execute:
```bash
g++ --version
```

ou 

```bash
clang++ --version
```

Se esses comandos devem retornar a versão do compilador.

## Como rodar?

Para executar o projeto, é necessário primeiro compila-lo, para psteriormente executa-lo. Para isso, é necessário executar os seguintes comandos make a baixo.

**Compila o codigo**
```bash
make
```
ou 
```bash
make all
```

**Executar o código**
```bash
make run 
```

**Apagar os arquivos gerados**
```bash
make clean
```