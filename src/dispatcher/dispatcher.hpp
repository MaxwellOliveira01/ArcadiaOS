/*
esse aqui seria o chefao
ele que mandaria em tudo
usaria tudo que implementamos nos outros arquivos
entao talvez um loadProcesses(), tambem teremos que ter um loop pra verificar as filas e o quantum
no final tem que printar umas coisas, seria esse cara tmb
*/

/*
A função do dispacher é criar os processos. Ele receber uma lista contendo em ordem as
seguintes informações:
<tempo de inicialização>, <prioridade>, <tempo de processador>, <tamanho máximo do conjunto de
trabalho>, <requisição da impressora>, <requisição do scanner>, <requisição do modem>,
<requisição do disco SATA>
Tendo base nessas informações, ele criará e retornará um processo.*/

#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "../process/process.hpp"
#include <iostream>
#include <vector>
#include <string>

class Dispatcher {
    private:
        int nextPid;

        std::string toString(const ProcessData& process = ProcessData()) const;
        
    public:
        Dispatcher() : nextPid(1) {};

        ProcessData createProcessFromInput(const std::vector<int>& inputData);

};

#endif