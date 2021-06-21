use std::fs::{self, File};
use std::io::Result;
use std::io::{BufWriter, Write};
use std::ops::{Index, IndexMut};

use crate::jogo::Jogo;

#[derive(Debug)]
pub struct Mapa {
    mapa: Vec<Option<usize>>,
    tam_cromossomo: usize,
}

impl Mapa {
    pub fn criar() -> Mapa {
        // Considere M o número de jogos possíveis
        // Considere N o número arranjos de Z, X e O
        // Considere T o tamanho do cromossomo (max(minimos))

        // [M]
        let jogos = Jogo::possibilidades();

        // [M] 0..M -> 0..N
        let minimos: Vec<u32> = jogos.iter().map(|i| i.minimo()).collect();

        // [N]
        let mut freq = vec![false; 3usize.pow(9)];
        minimos.iter().for_each(|&i| freq[i as usize] = true);

        let tam_cromossomo: usize = freq
            .iter()
            .fold(0, |count, &i| if i { count + 1 } else { count });

        // [N] 0..N -> 0..T
        let mut mapa_min = vec![None; 3usize.pow(9)];
        let mut acc = 0;
        for (i, &b) in freq.iter().enumerate() {
            if b {
                mapa_min[i] = Some(acc);
                acc += 1;
            }
        }

        // [N] 0..N -> 0..T
        let mut tam_mapa = usize::MIN;
        let mut mapa = vec![None; 3usize.pow(9)];
        for i in &jogos {
            let num = i.numero() as usize;
            mapa[num] = mapa_min[i.minimo() as usize];
            tam_mapa = tam_mapa.max(num);
        }
        tam_mapa += 1; // Tem que incluir o maior também

        mapa.resize(tam_mapa, None);

        Mapa {
            mapa,
            tam_cromossomo,
        }
    }

    pub fn salvar(&self, path: &str) -> Result<()> {
        let mut arquivo = BufWriter::new(File::create(path)?);
        writeln!(&mut arquivo, "{} {}", self.mapa.len(), self.tam_cromossomo)?;

        for &i in &self.mapa {
            let m = match i {
                None => -1,
                Some(i) => i as isize,
            };
            writeln!(&mut arquivo, "{}", m)?;
        }

        Ok(())
    }

    pub fn carregar(path: &str) -> Result<Mapa> {
        let conteudo = fs::read_to_string(path)?;
        let mut conteudo = conteudo.split("\n");

        let mut tupla = conteudo.next().expect("Mapa vazio").trim().split(" ");
        let tam_mapa = tupla
            .next()
            .expect("Mapa invalido")
            .trim()
            .parse::<usize>()
            .expect("Mapa invalido");
        let tam_cromossomo = tupla
            .next()
            .expect("Mapa invalido")
            .trim()
            .parse::<usize>()
            .expect("Mapa invalido");

        let mut mapa: Vec<Option<usize>> = Vec::with_capacity(tam_mapa);
        for dado in conteudo {
            let dado = dado.trim().parse::<isize>().expect("Mapa invalido");
            let dado = match dado {
                -1 => None,
                n => Some(n as usize),
            };
            mapa.push(dado);
        }

        Ok(Mapa {
            mapa,
            tam_cromossomo,
        })
    }

    pub fn len(&self) -> usize {
        self.mapa.len()
    }

    pub fn tam_cromossomo(&self) -> usize {
        self.tam_cromossomo
    }
}

impl Index<usize> for Mapa {
    type Output = Option<usize>;
    fn index(&self, i: usize) -> &Option<usize> {
        &self.mapa[i]
    }
}

impl IndexMut<usize> for Mapa {
    fn index_mut(&mut self, i: usize) -> &mut Option<usize> {
        &mut self.mapa[i]
    }
}
