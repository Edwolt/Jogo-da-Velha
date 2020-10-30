import pygame
from pygame import Color
from pygame import draw
from pygame.time import Clock
from pygame.locals import *


FUNDO = Color(0, 0, 0)
TRACO = Color(255, 255, 255)
VENCE = Color(255, 0, 0)


simetrias = (
    (0, 1, 2, 3, 4, 5, 6, 7, 8),
    (2, 1, 0, 5, 4, 3, 8, 7, 6),
    (2, 5, 8, 1, 4, 7, 0, 3, 6),
    (8, 5, 2, 7, 4, 1, 6, 3, 0),
    (8, 7, 6, 5, 4, 3, 2, 1, 0),
    (6, 7, 8, 3, 4, 5, 0, 1, 2),
    (6, 3, 0, 7, 4, 1, 8, 5, 2),
    (0, 3, 6, 1, 4, 7, 2, 5, 8)
)

simetrias_reversa = (
    (0, 1, 2, 3, 4, 5, 6, 7, 8),
    (2, 1, 0, 5, 4, 3, 8, 7, 6),
    (6, 3, 0, 7, 4, 1, 8, 5, 2),
    (8, 5, 2, 7, 4, 1, 6, 3, 0),
    (8, 7, 6, 5, 4, 3, 2, 1, 0),
    (6, 7, 8, 3, 4, 5, 0, 1, 2),
    (2, 5, 8, 1, 4, 7, 0, 3, 6),
    (0, 3, 6, 1, 4, 7, 2, 5, 8)
)


class Solucao:
    def __init__(self, path):
        with open('../mapa.txt', 'r') as file:
            a, b = file.readline().split(' ')
            a, b = int(a), int(b)
            self.mapa = [int(i) for i in file.readlines()]

        with open(path, 'r') as file:
            self.sol = [int(i) for i in file.readlines()]

    def calc_simetria(self, jogo):
        minimo = 0x3f3f3f3f
        sim = -1
        for i in range(8):
            numero = 0
            for j in range(9):
                numero += jogo[simetrias_reversa[i][j]] * (3 ** j)

            if numero < minimo:
                minimo = numero
                sim = i
        return sim

    def calc_min(self, jogo, sim):
        numero = 0
        for j in range(9):
            numero += jogo[simetrias_reversa[sim][j]] * (3 ** j)
        return numero

    def get(self, jogo):
        sim = self.calc_simetria(jogo)
        minimo = self.calc_min(jogo, sim)
        gene = self.mapa[minimo]
        jogada = self.sol[gene]
        return simetrias_reversa[sim][jogada]


vitorias = (
    (0, 1, 2),
    (3, 4, 5),
    (6, 7, 8),
    (0, 3, 6),
    (1, 4, 7),
    (2, 5, 8),
    (0, 4, 8),
    (2, 4, 6)
)


class Jogo:
    def __init__(self):
        # Vazio: 0
        # X: 1
        # O: 2
        self.jogo = [0] * 9

        self.jogador = 1
        self.vencedor = 0
        self.linha = None
        self.velha = False

    def desenha_fundo(self, tela):
        tela.fill(FUNDO)
        draw.line(tela, TRACO, (200, 0), (200, 600), 25)
        draw.line(tela, TRACO, (400, 0), (400, 600), 25)
        draw.line(tela, TRACO, (0, 200), (600, 200), 25)
        draw.line(tela, TRACO, (0, 400), (600, 400), 25)

    def desenha_x(self, tela, pos):
        x, y = pos
        x, y = x * 200, y * 200
        draw.line(tela, TRACO, (x + 40, y + 40), (x + 160, y + 160), 25)
        draw.line(tela, TRACO, (x + 160, y + 40), (x + 40, y + 160), 25)

    def desenha_o(self, tela, pos):
        x, y = pos
        x, y = x * 200, y * 200
        draw.circle(tela, TRACO, (x + 100, y + 100), 70, 25)

    def desenha_v(self, tela):
        draw.line(tela, VENCE, (50, 50), (300, 550), 70)
        draw.line(tela, VENCE, (300, 550), (550, 50), 70)

    def desenha_linha(self, tela):
        a, _, b = self.linha
        xa, ya = a % 3, a // 3
        pa = xa * 200 + 50, ya * 200 + 50
        xb, yb = b % 3, b // 3
        pb = xb * 200 + 150, yb * 200 + 150
        draw.line(tela, VENCE, pa, pb, 70)

    def desenha(self, tela):
        self.desenha_fundo(tela)
        for k, i in enumerate(self.jogo):
            if i == 1:
                self.desenha_x(tela, (k % 3, k // 3))
            if i == 2:
                self.desenha_o(tela, (k % 3, k // 3))

        if self.velha:
            self.desenha_v(tela)
        elif self.vencedor != 0:
            self.desenha_linha(tela)

        pygame.display.flip()

    def procura_vitoria(self):
        for a, b, c in vitorias:
            if self.jogo[a] == self.jogo[b] == self.jogo[c]:
                self.vencedor = self.jogo[a]
                self.linha = (a, b, c)
        for i in self.jogo:
            if i == 0:
                return
        self.velha = True

    def troca_vez(self):
        self.jogador = 2 if self.jogador == 1 else 1

    def fazer_jogada(self, i):
        if self.vencedor != 0 or self.velha:
            return False
        if self.jogo[i] != 0:
            print(f'BUG: Jogando em cima de {i}')
            return False

        self.jogo[i] = self.jogador
        self.troca_vez()
        jogo.procura_vitoria()
        return True

    def on_click(self, pos):
        x, y = pos
        x, y = x // 200, y // 200
        i = y*3 + x
        if self.jogo[i] == 0:
            return self.fazer_jogada(i)


if __name__ == "__main__":
    pygame.init()
    tela = pygame.display.set_mode((600, 600))
    jogo = Jogo()
    jogo.desenha(tela)
    clock = Clock()
    solucao = Solucao('../minmax.txt')

    while True:
        clock.tick(30)
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                quit(0)
            elif event.type == MOUSEBUTTONDOWN:
                if jogo.on_click(event.pos):
                    jogo.fazer_jogada(solucao.get(jogo.jogo))
            elif event.type == KEYDOWN and event.key == K_ESCAPE:
                jogo.__init__()

        jogo.desenha(tela)
