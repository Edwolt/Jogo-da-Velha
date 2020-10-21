import pygame
from pygame.locals import *

size = 600, 600
FUNDO = 0, 0, 0
TRACO = 255, 255, 255
VENCE = 255, 0, 0

def desenhar_jogo(tela, jogo, vencedor):
    tela.fill(FUNDO)
    pygame.draw.line(tela, TRACO, (200, 0), (200, 600), 25)
    pygame.draw.line(tela, TRACO, (400, 0), (400, 600), 25)
    pygame.draw.line(tela, TRACO, (0, 200), (600, 200), 25)
    pygame.draw.line(tela, TRACO, (0, 400), (600, 400), 25)



    for k, i in enumerate(jogo):
        if jogo == 0:
            continue

        x, y = k % 3, k // 3
        x *= 200
        y *= 200
        # sobrou 140 pixels para desenhar
        if i == 1:
            pygame.draw.line(tela, TRACO, (x+40, y+40), (x+160, y+160), 25)
            pygame.draw.line(tela, TRACO, (x+160, y+40), (x+40, y+160), 25)
        if i == 2:
            pygame.draw.circle(tela, TRACO, (x+100, y+100), 70, 25)

    if vencedor == 4:
        pygame.draw.line(tela, VENCE, (50, 50), (300, 550), 70)
        pygame.draw.line(tela, VENCE, (300, 550), (550, 50), 70)
    elif vencedor != 0:
        a, _, b = procura_vitoria(jogo)
        xa, ya = a % 3, a // 3
        pa = xa * 200 + 50, ya * 200 + 50
        xb, yb = b % 3, b // 3
        pb = xb * 200 + 150, yb * 200 + 150
        pygame.draw.line(tela, VENCE, pa, pb, 70)
        

    pygame.display.flip()



pygame.init()
tela = pygame.display.set_mode(size)
jogo = [0, 0, 0, 0, 0, 0, 0, 0,  0]
vencedor = 0

def procura_vitoria(jogo):
    # False: nada
    # True: Velha
    # Tuple: vitória
    possibilidades = (
        (0, 1, 2),
        (3, 4, 5),
        (6, 7, 8),
        (0, 3, 6),
        (1, 4, 7),
        (2, 5, 8),
        (0, 4, 8),
        (2, 4, 6)
    )

    for a, b, c in possibilidades:
        if jogo[a] == jogo[b] == jogo[c]:
            return (a, b, c)
    for i in jogo:
        if i == 0:
            return False
    return True

def quem_venceu(jogo):
    a = procura_vitoria(jogo)
    if a == False:
        return 0
    elif isinstance(a, tuple):
        return jogo[a[0]]
    return 4

jogador = 1
desenhar_jogo(tela, jogo, vencedor)
while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            quit(0)
        elif event.type == MOUSEBUTTONDOWN:
            if vencedor != 0:
                continue
            x, y = event.pos
            x, y = x // 200, y // 200
            if jogo[y*3 + x] == 0:
                jogo[y*3 + x] = jogador
                vencedor = quem_venceu(jogo)
                desenhar_jogo(tela, jogo, vencedor)
                jogador = 2 if jogador == 1 else 1
