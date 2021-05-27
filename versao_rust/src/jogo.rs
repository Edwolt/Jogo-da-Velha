use std::ops::{Index, IndexMut};
use Vez::{O, V, X, Z};

pub const SIMETRIAS: [[usize; 9]; 8] = [
    [0, 1, 2, 3, 4, 5, 6, 7, 8],
    [2, 1, 0, 5, 4, 3, 8, 7, 6],
    [2, 5, 8, 1, 4, 7, 0, 3, 6],
    [8, 5, 2, 7, 4, 1, 6, 3, 0],
    [8, 7, 6, 5, 4, 3, 2, 1, 0],
    [6, 7, 8, 3, 4, 5, 0, 1, 2],
    [6, 3, 0, 7, 4, 1, 8, 5, 2],
    [0, 3, 6, 1, 4, 7, 2, 5, 8],
];

pub const SIMETRIAS_REVERSA: [[usize; 9]; 8] = [
    [0, 1, 2, 3, 4, 5, 6, 7, 8],
    [2, 1, 0, 5, 4, 3, 8, 7, 6],
    [6, 3, 0, 7, 4, 1, 8, 5, 2],
    [8, 5, 2, 7, 4, 1, 6, 3, 0],
    [8, 7, 6, 5, 4, 3, 2, 1, 0],
    [6, 7, 8, 3, 4, 5, 0, 1, 2],
    [2, 5, 8, 1, 4, 7, 0, 3, 6],
    [0, 3, 6, 1, 4, 7, 2, 5, 8],
];

pub const VITORIAS: [(usize, usize, usize); 8] = [
    (0, 1, 2),
    (3, 4, 5),
    (6, 7, 8),
    (0, 3, 6),
    (1, 4, 7),
    (2, 5, 8),
    (0, 4, 8),
    (2, 4, 6),
];

pub const POW3: [usize; 10] = [
    3usize.pow(0),
    3usize.pow(1),
    3usize.pow(2),
    3usize.pow(3),
    3usize.pow(4),
    3usize.pow(5),
    3usize.pow(6),
    3usize.pow(7),
    3usize.pow(8),
    3usize.pow(9),
];

/// Um valor no tabuleiro, a vez do Jogador ou o resultado do jogo
#[derive(Eq, PartialEq, Clone, Copy)]
enum Vez {
    /// Tabuleiro: Espaço vazio
    /// Vez:       -
    /// Resultado: Jogo não terminou
    Z,

    /// Tabuleiro: Jogada do X
    /// Vez:       Vez do X
    /// Resultado: Jogador X venceu
    X,

    /// Tabuleiro: Jogada do O
    /// Vez:       Vez do O
    /// Resultado: Jogador O venceu
    O,

    /// Tabuleiro: -
    /// Vez:       -
    /// Resultado: Jogo deu velha
    V,
}

impl Vez {
    const fn from_num(val: usize) -> Vez {
        match val {
            0 => Z,
            1 => X,
            2 => O,
            3 => V,
            _ => Z,
        }
    }

    /// Muda a jogada entre X e O
    const fn trocar(self) -> Vez {
        match self {
            Z => Z,
            X => O,
            O => X,
            V => V,
        }
    }

    const fn venceu(self) -> bool {
        match self {
            X | O => true,
            _ => false,
        }
    }

    const fn velha(self) -> bool {
        matches!(self, V)
    }

    const fn venceu_ou_velha(self) -> bool {
        match self {
            X | O | V => true,
            _ => false,
        }
    }

    const fn num(self) -> usize {
        match self {
            Z => 0,
            X => 1,
            O => 2,
            V => 3,
        }
    }
}

#[derive(Clone)]
struct Jogo {
    tab: [Vez; 9],
}

impl Jogo {
    fn new() -> Jogo {
        Jogo { tab: [Z; 9] }
    }

    fn numero(&self) -> usize {
        let mut numero = 0;
        for i in 0..self.len() {
            numero += self[i].num() * POW3[i];
        }
        numero
    }

    fn minimo(&self) -> usize {
        let mut minimo = usize::MAX;
        for i in SIMETRIAS_REVERSA.iter() {
            let mut numero = 0;
            for j in 0..i.len() {
                numero += self[j].num() * POW3[j];
            }
            minimo = minimo.min(numero);
        }
        minimo
    }

    fn simetria(&self) -> usize {
        let mut minimo = usize::MAX;
        let mut simetria = 0;

        for i in 0..SIMETRIAS_REVERSA.len() {
            let mut numero = 0;
            for j in 0..SIMETRIAS_REVERSA[i].len() {
                numero += self[SIMETRIAS_REVERSA[i][j]].num() * POW3[i];
            }

            if numero < minimo {
                minimo = numero;
                simetria = i;
            }
        }

        simetria
    }

    fn valor(&self, simetria: usize) -> usize {
        let mut numero = 0;
        for i in 0..self.len() {
            numero += SIMETRIAS_REVERSA[simetria][i] * POW3[i];
        }
        numero
    }

    fn resultado(&self) -> Vez {
        for &(a, b, c) in VITORIAS.iter() {
            if self[a] == Z && self[a] == self[b] && self[b] == self[c] {
                return self[a];
            }
        }

        for &i in self.tab.iter() {
            if i == Z {
                return Z;
            }
        }

        V
    }

    fn possibilidades() -> Vec<Jogo> {
        fn rec_possibilidades(jogos: &mut Vec<Jogo>, vez: Vez) {
            for i in 0..9 {
                let atual = jogos.last().unwrap();
                if let Z = atual[i] {
                    continue;
                }

                let mut novo = atual.clone();
                novo[i] = vez;
                if !novo.resultado().venceu_ou_velha() {
                    jogos.push(novo);
                    rec_possibilidades(jogos, vez.trocar());
                }
            }
        }

        let mut jogos: Vec<Jogo> = vec![Jogo::new()];
        rec_possibilidades(&mut jogos, X);
        jogos
    }

    const fn len(&self) -> usize {
        9
    }
}

impl Index<usize> for Jogo {
    type Output = Vez;
    fn index(&self, i: usize) -> &Vez {
        &self.tab[i]
    }
}

impl IndexMut<usize> for Jogo {
    fn index_mut(&mut self, i: usize) -> &mut Vez {
        &mut self.tab[i]
    }
}
