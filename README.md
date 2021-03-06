# Jogo da Velha

Implementação de um jogo da velha jogado entre uma pessoa e o computador

[![Vídeo explicativo](http://img.youtube.com/vi/hnjac7k0mpk/0.jpg)](http://www.youtube.com/watch?v=hnjac7k0mpk "Vídeo explicativo")

Primeiramente, é necessário criar um arquivo chamado mapa.txt .
para isso use o comando:
`make run_mapa`

Depois é necessário calcular as jogadas que o computador fará
para isso use o comando para calcular verificando todas as possibilidade:
`make run_minmax`
ou use `make run_evolutivo` para calcular usando um algoritmo evolutivo

Para poder rodar o jogo use o comando (precisa do python3 e dos modulo listados em requirements.txt):
`make run_jogo`

Ou simplesmente use o comando `make run_all`

# Modulos

A pasta modulos contém os arquivos que são compartilhado entre os outros programas

# Mapa

Contém a programação para criar um mapa,
que tranforma um determinado jogo em uma posição no cromossomo,
tornando menor o tamanho do cromossomo por pegar apenas jogos validos
e eliminar jogos que não são simétricos

## Get
jogo -> valor minimo -> mapa -> genes -> simetria^-1 -> jogada

## Set
jogo -> valor minimo -> mapa -> gene \
jogada -> simetria

# Minmax

Calcula jogadas verificando todas as possibilidade,
esse algoritmo demora alguns segundos para executar.
Depois de calcular as melhores jogadas salva em um arquivo

# Evolutivo

Calcula quais jogadas fazer usando um algoritmo evolutivo para isso

# Jogo

Um pequeno programa criado em python usando a biblioteca pygame para exibir na tela um jogo da velha
e permitir que o usuário jogue contra a máquina

![Jogo da Velha](images/jogo_da_velha.png)
