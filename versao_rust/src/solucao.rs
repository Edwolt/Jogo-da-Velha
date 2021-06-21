use rand::{self, Rng};
use std::fs::{self, File};
use std::io::Result;
use std::io::{BufWriter, Write};
use std::ops::{Index, IndexMut};

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

        let mut cromossomo: Vec<Option<u8>> = Vec::with_capacity(tam);
        for _ in 0..tam {
            cromossomo.push(Some(random.gen_range(0..9)));
        }
        Solucao { cromossomo }
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

    pub fn carregar(&self, path: &str, tam_cromossomo: usize) -> Result<Solucao> {
        let conteudo = fs::read_to_string(path)?;
        let conteudo = conteudo.split("\n");

        let mut cromossomo: Vec<Option<u8>> = Vec::with_capacity(tam_cromossomo);
        for dado in conteudo {
            let dado = dado.trim().parse::<isize>().expect("Solução invalida");
            let dado = match dado {
                -1 => None,
                n => Some(n as u8),
            };
            cromossomo.push(dado);
        }

        Ok(Solucao { cromossomo })
    }

    pub fn correcao(&mut self, mapa: &Mapa) {
        let mut random = rand::thread_rng();

        let jogos = Jogo::possibilidades();
        for j in jogos {
            let mut jogada = self.get_jogada(mapa, &j);
            if jogada.is_none() || j.data[jogada.unwrap() as usize] != Vez::Z {
                while j.data[jogada.unwrap() as usize] != Vez::Z {
                    jogada = Some(random.gen_range(0..9));
                }
                self.set_jogada(mapa, &j, jogada);
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
