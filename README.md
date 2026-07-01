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

Por padrão, o simulador utiliza o teste localizados na pasta [/testcases/example](./testcases/example). Para o funcionamento correto, é fundamental que existam estes 3 arquivos dentro da pasta escolhida:
- [file.txt](./testcases/example/files.txt) -> Arquivo de informações do disco e operações
- [processes.txt](./testcases/example/processes.txt) -> Arquivo de processos
- [string.txt](./testcases/example/string.txt) -> Arquivo com as referências a memória

**JAMAIS DELETE ESSES ARQUIVOS DA PASTA EXAMPLE**

Você pode alterar as informações presentes nos mesmos para testar funcionalidades, ou criar uma nova pasta e adicionar novos cenários de teste, mas jamais deletá-los.

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

O comando de execução foi projetado para ler três arquivos obrigatórios de uma pasta no diretório `/testcases` (processes.txt, files.txt e string.txt).

#### Cenário Padrão (`example`)
Por padrão, se você executar apenas o comando abaixo, o sistema rodará os testes contidos na pasta [/testcases/example](./testcases/example):
```bash
make run 
```
#### Cenário Customizado
Caso queira rodar testes de outra pasta basta passar a variável TESTCASE com o nome da respectiva pasta criada dentro do diretório `/testcases`:
```bash
# Exemplo para rodar os testes da pasta /testcases/test1
make run TESTCASE=test1
```

### **Apagar os arquivos gerados**
```bash
make clean
```

## Desenvolvedores
* [Iasmim de Queiroz Freitas](https://github.com/iasmimqf)
* [Maxwell Oliveira dos Reis](https://github.com/MaxwellOliveira01)
* [Rafael Oliveira Bonach](https://github.com/RafaBonach)