use rand::prelude::SliceRandom;
use rand::{self, Rng};
use std::fs::{self, File};
use std::io::Result;
use std::io::{BufWriter, Write};
use std::ops::{Index, IndexMut};

use crate::individuo::Individuo;
use crate::jogo::{Jogo, Vez, SIMETRIAS, SIMETRIAS_REVERSA};
use crate::mapa::Mapa;

pub struct Solucao {
    pub cromossomo: Vec<Option<u8>>,
}

impl Solucao {
    pub fn criar(tam: usize) -> Solucao {
        Solucao {
            cromossomo: vec![Some(0); tam],
        }
    }

    pub fn random(tam: usize) -> Solucao {
        let mut random = rand::thread_rng();
        Solucao {
            cromossomo: (0..tam).map(|_| Some(random.gen_range(0..9))).collect(),
        }
    }

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

    pub fn carregar(&self, path: &str, _tam_cromossomo: usize) -> Result<Solucao> {
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

    pub fn correcao(&mut self, mapa: &Mapa) {
        let mut random = rand::thread_rng();

        let jogos = Jogo::possibilidades();
        for j in jogos {
            let jogada = self.get_jogada(mapa, &j);
            if jogada.is_none() || j.data[jogada.unwrap() as usize] != Vez::Z {
                let mut jogada: u8 = 0;
                while j.data[jogada as usize] != Vez::Z {
                    jogada = random.gen_range(0..9);
                }
                self.set_jogada(mapa, &j, Some(jogada));
            }
        }
    }

    pub fn get_jogada(&self, mapa: &Mapa, jogo: &Jogo) -> Option<u8> {
        let sim = jogo.simetria();
        let num = jogo.numero() as usize;
        let gene = mapa.data[num].unwrap();
        let jogada = self[gene].unwrap() as usize;

        Some(SIMETRIAS_REVERSA[sim][jogada])
    }

    pub fn set_jogada(&mut self, mapa: &Mapa, jogo: &Jogo, jogada: Option<u8>) {
        let sim = jogo.simetria() as usize;
        let num = jogo.numero() as usize;
        let gene = mapa.data[num].unwrap();

        let jogada = jogada.unwrap() as usize;
        let jogada = SIMETRIAS[sim][jogada];
        self[gene] = Some(jogada);
    }

    pub fn crossover(tam: usize, pai: &Solucao, mae: &Solucao) -> Solucao {
        let mut random = rand::thread_rng();
        let cromossomo: Vec<Option<u8>> = pai
            .cromossomo
            .iter()
            .zip(mae.cromossomo.iter())
            .map(|(&p, &m)| if random.gen_bool(0.5) { p } else { m })
            .collect();
        Solucao { cromossomo }
    }

    pub fn mutacao(&mut self, mutacao: f64) {
        let mut random = rand::thread_rng();
        let len = self.cromossomo.len();
        *self.cromossomo.choose_mut(&mut random).unwrap() = Some(random.gen_range(0..9));
        // self.cromossomo[random.gen_range(0..len)] = Some(random.gen_range(0..9));

        for i in &mut self.cromossomo {
            if random.gen_bool(mutacao) {
                *i = Some(random.gen_range(0..9))
            }
        }
    }
}

impl Index<usize> for Solucao {
    type Output = Option<u8>;
    fn index(&self, i: usize) -> &Option<u8> {
        &self.cromossomo[i]
    }
}

impl IndexMut<usize> for Solucao {
    fn index_mut(&mut self, i: usize) -> &mut Option<u8> {
        &mut self.cromossomo[i]
    }
}
