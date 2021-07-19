use std::borrow::BorrowMut;

use crate::jogo::{Jogo, Vez};
use crate::mapa::Mapa;
use crate::solucao::Solucao;

/// Armazena dados do indivíduo
/// incluindo a solução e outros dados pertinentes
///
/// Possui metodos para ser indivíduo do algoritmo evolutivo
#[derive(Clone)]
pub struct Individuo {
    pub solucao: Solucao,
    pub fitness: i32,
}

impl Individuo {
    #[allow(dead_code)]
    pub fn criar(mapa: &Mapa) -> Individuo {
        Individuo {
            solucao: Solucao::criar(mapa),
            fitness: 0,
        }
    }

    /// Cria indivíduo com valores aleatórios
    pub fn random(mapa: &Mapa) -> Individuo {
        Individuo {
            solucao: Solucao::random(mapa),
            fitness: 0,
        }
    }

    /// Faz dois inviduos jogar atualizando o fitness deles
    pub fn compete(&mut self, mapa: &Mapa, oponente: &mut Individuo) -> i32 {
        /// Faz duas soluções competir retornando o resultado
        fn solucao_compete(mapa: &Mapa, jogador_x: &mut Solucao, jogador_o: &mut Solucao) -> i32 {
            let mut jogo: Jogo = Jogo::criar();

            while !jogo.resultado().terminou() {
                let jogador = match jogo.vez {
                    Vez::X => jogador_x.borrow_mut(),
                    Vez::O => jogador_o.borrow_mut(),
                    _ => panic!("Erro Interno: Solução retornou um valor invalido"),
                };

                let jogada = jogador.get_jogada(mapa, &jogo);
                let jogada = jogador.corrige_jogada(mapa, &jogo, jogada);

                jogo.jogar(jogada.unwrap() as usize);
            }

            match jogo.resultado() {
                Vez::X => 1,
                Vez::O => -1,
                _ => 0,
            }
        }

        solucao_compete(mapa, &mut self.solucao, &mut oponente.solucao)
            - solucao_compete(mapa, &mut oponente.solucao, &mut self.solucao)
    }

    /// Cria um novo indivíduo
    /// com um mistrura das características do pai e da mae
    pub fn crossover(pai: &Individuo, mae: &Individuo) -> Individuo {
        Individuo {
            solucao: Solucao::crossover(&pai.solucao, &mae.solucao),
            fitness: 0,
        }
    }

    /// Aplica mutação no indivíduo
    ///
    /// Cada gene tem o valor do parâmetro mutacao de chance de sofrer mutação
    ///
    /// É garantido que pelo menos um gene sofrerá mutação
    pub fn mutacao(&mut self, mutacao: f64) {
        self.solucao.mutacao(mutacao);
    }
}
