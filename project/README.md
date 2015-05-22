# Plataforma base para MC723 #

## O que é uma plataforma? ##

Uma *plataforma* é um software capaz de emular/simular um hardware com características reais. No nosso caso, temos um processador MIPS, como já utilizado no projeto anterior, um módulo de memória e um barramento, além do software que executará sobre esta plataforma.

## Como utilizar a plataforma base? ##

No caso geral, dois comandos são necessários para compilar e executar a plataforma:

{
make
make run
}

ou, de forma simplificada

{
make all run
}

O primeiro comando (_make all_) compila toda a plataforma, incluindo as pastas com cada um dos componentes. O segundo comando (_make run_) executa a plataforma com os parâmetros corretos para executar o software simulado. No nosso exemplo, este é um Hello World.

## O que configurar? ##

Se copiar esta pasta inteira para dentro de um computador do IC-3, ela deve continuar funcionando. Caso queira ter uma versão no seu notebook, você deve configurar as primeiras linhas do _Makefile_ principal do pacote. Não deve ser necessário copiar nada além deste arquivo. No entanto, é necessário ter SystemC, ArchC e os cross-compilers já instalados nesta outra máquina. Veja as instruções para instalação na página da disciplina.
