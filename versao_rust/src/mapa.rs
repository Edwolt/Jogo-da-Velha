use std::fs::{self, File};
use std::io::Result;
use std::io::{BufWriter, Write};

use crate::jogo::Jogo;

#[derive(Debug)]
pub struct Mapa {
    pub data: Vec<Option<usize>>,

    /// Tamanho do cromossomo de mapa
    pub tam_cromossomo: usize,
}

impl Mapa {
    /// Gera um mapa e salva em mapa.txt
    ///
    /// Esse mapa serve para mapear todos os possivei jogos
    /// eliminando jogos simetricos e fazendo com que o vetor
    /// com todas as jogadas a ser feita seja menor
    pub fn criar() -> Mapa {
        // Considere M o número de jogos possíveis
        // Considere N o número arranjos de Z, X e O
        // Considere T o tamanho do cromossomo (max(minimos))

        // [M]
        let jogos = Jogo::possibilidades();

        // [M] 0..M -> 0..N
        let minimos: Vec<u32> = jogos.iter().map(|jg| jg.minimo()).collect();

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
        for jg in &jogos {
            let num = jg.numero() as usize;
            mapa[num] = mapa_min[jg.minimo() as usize];
            tam_mapa = tam_mapa.max(num);
        }
        tam_mapa += 1; // Tem que incluir o maior também

        mapa.resize(tam_mapa, None);

        Mapa {
            data: mapa,
            tam_cromossomo,
        }
    }

    /// Salva mapa em uma arquivo em path
    pub fn salvar(&self, path: &str) -> Result<()> {
        let mut arquivo = BufWriter::new(File::create(path)?);
        writeln!(&mut arquivo, "{} {}", self.data.len(), self.tam_cromossomo)?;

        for &i in &self.data {
            let m = match i {
                None => -1,
                Some(i) => i as isize,
            };
            writeln!(&mut arquivo, "{}", m)?;
        }

        Ok(())
    }

    /// Carrega o mapa e retorna ele
    pub fn carregar(path: &str) -> Result<Mapa> {
        let conteudo = fs::read_to_string(path)?;
        let mut conteudo = conteudo.trim().split("\n");

        let mut tupla = conteudo.next().expect("Mapa vazio").trim().split(" ");

        // let _tam_mapa = tupla
        //     .next()
        //     .expect("Mapa invalido")
        //     .trim()
        //     .parse::<usize>()
        //     .expect("Mapa invalido");
        tupla.next().expect("Mapa invalido"); // ignora tam_mapa

        let tam_cromossomo = tupla
            .next()
            .expect("Mapa invalido")
            .trim()
            .parse::<usize>()
            .expect("Mapa invalido");

        let mapa: Vec<Option<usize>> = conteudo
            .map(|dado| {
                let dado = dado.trim().parse::<isize>().expect("Mapa invalido");
                match dado {
                    -1 => None,
                    n => Some(n as usize),
                }
            })
            .collect();

        Ok(Mapa {
            data: mapa,
            tam_cromossomo,
        })
    }
}
