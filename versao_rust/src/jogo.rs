use std::fmt::{Debug, Formatter};

/// Pega uma jogada e retorna onde ela seria naquela simetria
///
/// jogada -> jogada'
pub const SIMETRIAS: [[u8; 9]; 8] = [
    [0, 1, 2, 3, 4, 5, 6, 7, 8],
    [2, 1, 0, 5, 4, 3, 8, 7, 6],
    [2, 5, 8, 1, 4, 7, 0, 3, 6],
    [8, 5, 2, 7, 4, 1, 6, 3, 0],
    [8, 7, 6, 5, 4, 3, 2, 1, 0],
    [6, 7, 8, 3, 4, 5, 0, 1, 2],
    [6, 3, 0, 7, 4, 1, 8, 5, 2],
    [0, 3, 6, 1, 4, 7, 2, 5, 8],
];

/// Pega uma jogada em um jogo que passou por simetria e
/// a tranforma em uma jogada antes de passar pela simtria
///
/// jogada <- jogada'
pub const SIMETRIAS_REVERSA: [[u8; 9]; 8] = [
    [0, 1, 2, 3, 4, 5, 6, 7, 8],
    [2, 1, 0, 5, 4, 3, 8, 7, 6],
    [6, 3, 0, 7, 4, 1, 8, 5, 2],
    [8, 5, 2, 7, 4, 1, 6, 3, 0],
    [8, 7, 6, 5, 4, 3, 2, 1, 0],
    [6, 7, 8, 3, 4, 5, 0, 1, 2],
    [2, 5, 8, 1, 4, 7, 0, 3, 6],
    [0, 3, 6, 1, 4, 7, 2, 5, 8],
];

/// Vetor com todas as opções de vitória
pub const VITORIAS: [(u8, u8, u8); 8] = [
    (0, 1, 2),
    (3, 4, 5),
    (6, 7, 8),
    (0, 3, 6),
    (1, 4, 7),
    (2, 5, 8),
    (0, 4, 8),
    (2, 4, 6),
];

/// Retorna 3 elevado n
///
/// ```
/// POW3[i] = 3u32.pow(i)
/// ```
pub const POW3: [u32; 10] = [
    3u32.pow(0),
    3u32.pow(1),
    3u32.pow(2),
    3u32.pow(3),
    3u32.pow(4),
    3u32.pow(5),
    3u32.pow(6),
    3u32.pow(7),
    3u32.pow(8),
    3u32.pow(9),
];

/// Um valor no tabuleiro, a vez do Jogador ou o resultado do jogo
#[derive(Eq, PartialEq, Clone, Copy, Debug)]
pub enum Vez {
    /// * Tabuleiro: Espaço vazio
    /// * Vez:       -
    /// * Resultado: Jogo não terminou
    Z,

    /// * Tabuleiro: Jogada do X
    /// * Vez:       Vez do X
    /// * Resultado: Jogador X venceu
    X,

    /// * Tabuleiro: Jogada do O
    /// * Vez:       Vez do O
    /// * Resultado: Jogador O venceu
    O,

    /// * Tabuleiro: -
    /// * Vez:       -
    /// * Resultado: Jogo deu velha
    V,
}

impl Vez {
    /// Retorna a `Vez` correpondente ao número
    pub const fn from_num(val: u8) -> Vez {
        match val {
            0 => Vez::Z,
            1 => Vez::X,
            2 => Vez::O,
            3 => Vez::V,
            _ => Vez::Z,
        }
    }

    /// Retorna o número correspondente a `Vez`
    pub const fn num(self) -> u8 {
        match self {
            Vez::Z => 0,
            Vez::X => 1,
            Vez::O => 2,
            Vez::V => 3,
        }
    }

    //===== Vez =====//

    /// Muda a jogada entre X e O
    pub const fn trocar(self) -> Vez {
        match self {
            Vez::Z => Vez::Z,
            Vez::X => Vez::O,
            Vez::O => Vez::X,
            Vez::V => Vez::V,
        }
    }

    //===== Resultado =====//

    /// Retorna se alguém venceu o jogo
    pub const fn venceu(self) -> bool {
        match self {
            Vez::X | Vez::O => true,
            _ => false,
        }
    }

    /// Retorna se o jogo deu velha
    pub const fn velha(self) -> bool {
        matches!(self, Vez::V)
    }

    /// Retorna se alguém venceu o jogo ou se deu velha
    pub const fn terminou(self) -> bool {
        match self {
            Vez::X | Vez::O | Vez::V => true,
            _ => false,
        }
    }
}

#[derive(Clone)]
pub struct Jogo {
    pub data: [Vez; 9],
    pub vez: Vez,
}

impl Jogo {
    pub const fn criar() -> Jogo {
        Jogo {
            data: [Vez::Z; 9],
            vez: Vez::X,
        }
    }

    /// Cria um novo jogo usando o array passado
    pub const fn criar_com(jogo: [Vez; 9]) -> Jogo {
        /// Calcula qual o próximo jogador a jogar
        const fn proximo_jogador(jogo: [Vez; 9]) -> Vez {
            let mut x: u8 = 0;
            let mut o: u8 = 0;

            // for vez in &jogo {
            //     match vez {
            //         Vez::X => x += 1,
            //         Vez::O => o += 1,
            //         _ => (),
            //     }
            // }
            let mut i: usize = 0;
            while i < jogo.len() {
                match jogo[i] {
                    Vez::X => x += 1,
                    Vez::O => o += 1,
                    _ => (),
                }

                i += 1;
            }

            if x == o {
                Vez::X
            } else {
                Vez::O
            }
        }

        Jogo {
            data: jogo,
            vez: proximo_jogador(jogo),
        }
    }

    /// Retorna o valor do jogo
    pub fn numero(&self) -> u32 {
        self.data.iter().enumerate().fold(0, |acc, (i, jogada)| {
            let num = jogada.num() as u32;
            let res = num * POW3[i];
            acc + res
        })
    }

    /// Retorna o menor valor entre os jogos e seus simétricos
    pub fn minimo(&self) -> u32 {
        SIMETRIAS_REVERSA.iter().fold(u32::MAX, |minimo, sim| {
            let numero = sim.iter().enumerate().fold(0, |acc, (i, &idx)| {
                let idx = idx as usize;
                let num = (self.data[idx as usize].num() as u32) * POW3[i];
                let res = num * POW3[i];
                acc + res
            });

            minimo.min(numero)
        })
    }

    /// Retorna qual a simétria usada
    /// para que o jogo tenha o menor valor possível
    pub fn simetria(&self) -> usize {
        let mut minimo = u32::MAX;
        let mut simetria = 0;

        for (i, sim) in SIMETRIAS_REVERSA.iter().enumerate() {
            let numero = sim.iter().enumerate().fold(0, |acc, (j, &idx)| {
                let idx = idx as usize;
                let num = self.data[idx].num() as u32;
                let res = num * POW3[j];
                acc + res
            });

            if numero < minimo {
                minimo = numero;
                simetria = i;
            }
        }

        simetria
    }

    /// Retorna o valor so jogo na simétria
    pub fn valor(&self, simetria: usize) -> u32 {
        SIMETRIAS_REVERSA[simetria]
            .iter()
            .enumerate()
            .fold(0, |acc, (i, &idx)| {
                let idx = idx as usize;
                let num = self.data[idx].num() as u32;
                let res = num * POW3[i];
                acc + res
            })
    }

    /// Retorna quem venceu o jogo ou se ele ainda está acontecendo
    pub fn resultado(&self) -> Vez {
        for &(a, b, c) in &VITORIAS {
            let a = a as usize;
            let b = b as usize;
            let c = c as usize;

            if self.data[a] != Vez::Z
                && self.data[a] == self.data[b]
                && self.data[b] == self.data[c]
            {
                return self.data[a];
            }
        }

        if self.data.iter().any(|&i| i == Vez::Z) {
            return Vez::Z;
        } else {
            return Vez::V;
        }
    }

    /// Retorna um `Vec` de todos os jogos possiveis de acontecer
    pub fn possibilidades() -> Vec<Jogo> {
        fn rec_possibilidades(jogos: &mut Vec<Jogo>, atual: Jogo) {
            for i in 0..atual.data.len() {
                if atual.data[i] == Vez::Z {
                    let mut novo = atual.clone();
                    novo.jogar(i);
                    if !novo.resultado().terminou() {
                        jogos.push(novo.clone());
                        rec_possibilidades(jogos, novo);
                    }
                }
            }
        }

        let atual = Jogo::criar();
        let mut jogos: Vec<Jogo> = vec![atual.clone()];
        rec_possibilidades(&mut jogos, atual);
        jogos
    }

    /// Faz uma jogada (A struct sabe de quem é vez)
    pub fn jogar(&mut self, jogada: usize) {
        self.data[jogada] = self.vez;
        self.vez = self.vez.trocar();
    }
}

impl Debug for Jogo {
    /// Exibe como está o jogo, `jogo.numero()` e `jogo.minimo()`
    fn fmt(&self, f: &mut Formatter) -> std::fmt::Result {
        for vez in &self.data {
            match vez {
                Vez::Z => write!(f, "Z")?,
                Vez::X => write!(f, "X")?,
                Vez::O => write!(f, "O")?,
                Vez::V => write!(f, "V")?,
            }
        }
        write!(f, " => {} ({})", self.numero(), self.minimo())?;
        Ok(())
    }
}
