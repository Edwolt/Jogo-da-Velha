use crate::jogo::Jogo;
use std::fs;
use std::fs::File;
use std::io::Result;
use std::io::{BufWriter, Write};

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
        let minimos: Vec<usize> = jogos.iter().map(|i| i.minimo()).collect();

        // [N]
        let mut freq = vec![false; 3usize.pow(9)];
        for &i in &minimos {
            freq[i] = true;
        }

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
            let num = i.numero();
            mapa[num] = mapa_min[i.minimo()];
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
            let dado = match dado.trim().parse::<isize>().expect("Mapa invalido") {
                -1 => None,
                n @ _ => Some(n as usize),
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
