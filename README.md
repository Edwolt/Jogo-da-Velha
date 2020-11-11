# Jogo da Velha
Implementação de um jogo da velha jogado entre uma pessoa e o computador

Primeiramente, é necessário criar um arquivo chamado mapa.txt .
para isso use o comando: 
`make comp_mapa run_mapa`

Depois é necessário calcular as jogadas que o computador fará
para isso use o comando:
`make comp_minmax run_minmax`

Para poder rodar o jogo use o comando:
`make run_jogo`

# Modulos
A pasta modulos contém os arquivos que são compartilhado entre os outros programas

# Mapa
Contém a programação para criar um mapa,
que tranforma um determinado jogo em uma posição no cromossomo,
tornando menor o tamanho do cromossomo por pegar apenas jogos validos
e eliminar jogos que não são simétricos

# Minmax
Calcula jogadas verificando todas as possibilidade,
esse algoritmo demora alguns segundos para executar.
Depois de calcular as melhores jogadas salva em um arquivo

# Jogo
Um pequeno programa criado em python para exibir na tela um jogo da velha
e permitir que o usuário jogue contra a máquina