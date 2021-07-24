use rand::prelude::SliceRandom;
use rand::{self, Rng};
use std::fs::{self, File};
use std::io::Result;
use std::io::{BufWriter, Write};

use crate::jogo::{Jogo, Vez, SIMETRIAS, SIMETRIAS_REVERSA};
use crate::mapa::Mapa;

/// Armazena uma possível solução para o jogo da velha
#[derive(Clone)]
pub struct Solucao {
    pub cromossomo: Vec<Option<u8>>,
}

impl Solucao {
    pub fn criar(mapa: &Mapa) -> Solucao {
        let tam = mapa.tam_cromossomo;

        Solucao {
            cromossomo: vec![Some(0); tam],
        }
    }

    /// Cria um objeto solução com valores aleatórios
    pub fn random(mapa: &Mapa) -> Solucao {
        let tam = mapa.tam_cromossomo;
        let mut random = rand::thread_rng();

        Solucao {
            cromossomo: (0..tam).map(|_| Some(random.gen_range(0..9))).collect(),
        }
    }

    /// Salva a solução no arquivo em `path`
    pub fn salvar(&self, path: &str) -> Result<()> {
        let mut arquivo = BufWriter::new(File::create(path)?);

        for &i in &self.cromossomo {
            let m = match i {
                Some(n) => n as i8,
                None => -1,
            };
            writeln!(arquivo, "{}", m)?;
        }

        Ok(())
    }

    /// Carrega a solução no arquivo `path`
    #[allow(dead_code)]
    pub fn carregar(&self, _mapa: &Mapa, path: &str) -> Result<Solucao> {
        let conteudo = fs::read_to_string(path)?;
        let cromossomo: Vec<Option<u8>> = conteudo
            .split("\n")
            .map(|dado| {
                let dado = dado.trim().parse::<isize>().expect("Solução invalida");
                match dado {
                    -1 => None,
                    n => Some(n as u8),
                }
            })
            .collect();
        Ok(Solucao { cromossomo })
    }

    /// Corrige jogada dentro da solução se for necessário
    /// e retorna a nova jogada
    #[inline]
    pub fn corrige_jogada(&mut self, mapa: &Mapa, jogo: &Jogo, jogada: Option<u8>) -> Option<u8> {
        if jogada.is_none() || jogo.data[jogada.unwrap() as usize] != Vez::Z {
            let mut random = rand::thread_rng();

            let mut jogada: u8 = 0;
            while jogo.data[jogada as usize] != Vez::Z {
                jogada = random.gen_range(0..9);
            }
            self.set_jogada(mapa, &jogo, Some(jogada));
            Some(jogada)
        } else {
            jogada
        }
    }

    /// Corrige a solução para que ela não faça jogadas inválidas
    pub fn correcao(&mut self, mapa: &Mapa) {
        let jogos = Jogo::possibilidades();
        for jg in jogos {
            let jogada = self.get_jogada(mapa, &jg);
            let _ = self.corrige_jogada(mapa, &jg, jogada);
        }
    }

    /// Retorna a jogada que a solução diz que deve ser feita para
    /// uma determinada situação do jogo
    pub fn get_jogada(&self, mapa: &Mapa, jogo: &Jogo) -> Option<u8> {
        let sim = jogo.simetria();
        let num = jogo.numero() as usize;
        let gene = mapa.data[num].unwrap();
        let jogada = self.cromossomo[gene].unwrap() as usize;

        Some(SIMETRIAS_REVERSA[sim][jogada])
    }

    /// Guarda na solução que nesse jogo e em seus simétricos
    /// deve ser feito a essa jogada
    pub fn set_jogada(&mut self, mapa: &Mapa, jogo: &Jogo, jogada: Option<u8>) {
        let sim = jogo.simetria() as usize;
        let num = jogo.numero() as usize;
        let gene = mapa.data[num].unwrap();

        let jogada = jogada.unwrap() as usize;
        let jogada = SIMETRIAS[sim][jogada];
        self.cromossomo[gene] = Some(jogada);
    }

    /// Cria um nova solução
    /// com um mistrura das características do pai e da mãe
    pub fn crossover(pai: &Solucao, mae: &Solucao) -> Solucao {
        let mut random = rand::thread_rng();

        let cromossomo: Vec<Option<u8>> = pai
            .cromossomo
            .iter()
            .zip(mae.cromossomo.iter())
            .map(|(&p, &m)| if random.gen_bool(0.5) { p } else { m })
            .collect();
        Solucao { cromossomo }
    }

    /// Aplica mutação na solução
    ///
    /// Cada gene tem o valor do parâmetro mutação de chance de sofrer mutação
    ///
    /// É garantido que pelo menos um gene sofrerá mutação
    pub fn mutacao(&mut self, mutacao: f64) {
        let mut random = rand::thread_rng();
        // let len = self.cromossomo.len();
        // self.cromossomo[random.gen_range(0..len)] = Some(random.gen_range(0..9));
        *self.cromossomo.choose_mut(&mut random).unwrap() = Some(random.gen_range(0..9));

        for i in &mut self.cromossomo {
            if random.gen_bool(mutacao) {
                *i = Some(random.gen_range(0..9))
            }
        }
    }
}
