use rand::{self, Rng};
use std::borrow::BorrowMut;

use crate::jogo::{Jogo, Vez};
use crate::mapa::Mapa;
use crate::solucao::Solucao;

#[derive(Clone)]
pub struct Individuo {
    pub solucao: Solucao,
    pub fitness: i32,
}

impl Individuo {
    pub fn criar(tam: usize) -> Individuo {
        Individuo {
            solucao: Solucao::criar(tam),
            fitness: 0,
        }
    }

    pub fn random(tam: usize) -> Individuo {
        Individuo {
            solucao: Solucao::random(tam),
            fitness: 0,
        }
    }

    pub fn compete(&mut self, mapa: &Mapa, oponente: &mut Individuo) -> i32 {
        fn solucao_compete(mapa: &Mapa, jogador_x: &mut Solucao, jogador_o: &mut Solucao) -> i32 {
            let mut random = rand::thread_rng();
            let mut jogo: Jogo = Jogo::criar();

            while !jogo.resultado().terminou() {
                let jogador = match jogo.vez {
                    Vez::X => jogador_x.borrow_mut(),
                    Vez::O => jogador_o.borrow_mut(),
                    _ => panic!("Erro Interno: Solução retornou um valor invalido"),
                };

                let jogada = jogador.get_jogada(mapa, &jogo);
                let jogada = corrige_jogada(&jogo, mapa, jogador, jogada);

                jogo.jogar(jogada.unwrap() as usize);
            }

            match jogo.resultado() {
                Vez::X => 1i32,
                Vez::O => -1i32,
                _ => 0i32,
            }
        }

        solucao_compete(mapa, &mut self.solucao, &mut oponente.solucao)
            - solucao_compete(mapa, &mut oponente.solucao, &mut self.solucao)
    }

    pub fn crossover(tam: usize, pai: &Individuo, mae: &Individuo) -> Individuo {
        Individuo {
            solucao: Solucao::crossover(tam, &pai.solucao, &mae.solucao),
            fitness: 0,
        }
    }

    pub fn mutacao(&mut self, mutacao: f64) {
        self.solucao.mutacao(mutacao);
    }
}

/// Corrige jogada dentro da solução se for necessário, e retorna a nova jogada
fn corrige_jogada(
    jogo: &Jogo,
    mapa: &Mapa,
    jogador: &mut Solucao,
    jogada: Option<u8>,
) -> Option<u8> {
    if jogada.is_none() || jogo.data[jogada.unwrap() as usize] != Vez::Z {
        let mut random = rand::thread_rng();
        let mut jogada: u8 = 0;
        while jogo.data[jogada as usize] != Vez::Z {
            jogada = random.gen_range(0..9);
        }
        (*jogador).set_jogada(mapa, &jogo, Some(jogada));
        Some(jogada)
    } else {
        jogada
    }
}
