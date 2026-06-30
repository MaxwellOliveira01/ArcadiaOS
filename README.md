# ArcadiaOS

## Sobre
**ArcadiaOS** é um pseudo-SO desenvolvido para a disciplina **Fundamentos de Sistemas Operacionais** da **Universidade de Brasília**. Seu propósito é aplicar os conhecimentos teóricos aprendidos durante a disciplina em um softwares simples capaz de gerenciar recursos e proporcionar abstração ao usuário.

## Prerequisitos

Para compilar e executar, certifique-se de que sua máquina tenha:
- Um compilador compatível com o padrão C++ (c++17 ou superior).
    - **Linux:** GCC (g++) ou Clang
    - **Windows:** MinGW-w64 (g++)
    - **MacOS:** Clang
- **GNU Make**, utilizado para automatizar a compilação.

Para verificar se o compilador está instalado, execute:
```bash
g++ --version
```

ou 

```bash
clang++ --version
```

Em seguida, verifique se o **GNU Make** também está instalado:
```bash
make --version
```

Se os comandos retornarem suas respectivas versões, o ambiente está pronto para compilar o projeto.

## Como rodar?

Primeiro, é fundamental que tenham 3 arquivos na pasta [/testcases](./testcases/) :
- [file.txt](./testcases/files.txt) -> Arquivo de informações do disco e operações
- [processes.txt](./testcases/processes.txt) -> Arquivo de processos
- [string.txt](./testcases/string.txt) -> Arquivo com as referências a memória

**JAMAIS DELETE ESSES ARQUIVOS**

Você pode alterar as informações presentes nos mesmos para testar funcionalidades, mas jamais deletar.

---
Em seguida, basta compilar o projeto e executar o código utilizando os comandos **make** abaixo.

### **Compila o codigo**
```bash
make
```
ou 
```bash
make all
```

### **Executar o código**
```bash
make run 
```

### **Apagar os arquivos gerados**
```bash
make clean
```

## Desenvolvedores
* [Iasmim de Queiroz Freitas](https://github.com/iasmimqf)
* [Maxwell Oliveira dos Reis](https://github.com/MaxwellOliveira01)
* [Rafael Oliveira Bonach](https://github.com/RafaBonach)