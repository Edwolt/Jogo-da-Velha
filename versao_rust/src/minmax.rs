use crate::jogo::{Jogo, Vez};

pub fn minmax(jogo: &Jogo) -> u8 {
    fn rec_minmax(jogo: &Jogo, maximizador: Vez) -> i8 {
        match jogo.resultado() {
            Vez::V => return 0,
            Vez::X | Vez::O => {
                if Vez::X == maximizador {
                    return 1;
                } else {
                    return -1;
                }
            }
            Vez::Z => (),
        }

        if jogo.vez == maximizador {
            // Se for a minha vez

            let mut zero = false;
            for i in 0..jogo.data.len() {
                if jogo.data[i] != Vez::Z {
                    continue;
                }

                let mut jogo_aux = jogo.clone();
                jogo_aux.jogar(i);

                let aux = rec_minmax(&jogo_aux, maximizador);
                if aux == 1 {
                    return 1;
                } else if aux == 0 {
                    zero = true;
                }
            }

            return if zero { 0 } else { 1 };
        } else {
            // Se for a vez do oponente

            let mut zero = false;
            for i in 0..jogo.data.len() {
                if jogo.data[i] != Vez::Z {
                    continue;
                }

                let mut jogo_aux = jogo.clone();
                jogo_aux.jogar(i);

                let aux = rec_minmax(&jogo_aux, maximizador);
                if aux == -1 {
                    return -1;
                } else {
                    zero = true;
                }
            }

            return if zero { 0 } else { 1 };
        }
    }

    let mut jogada: u8 = 0;
    let mut melhor: i8 = -2;
    for i in 0..jogo.data.len() {
        if jogo.data[i] == Vez::Z {
            continue;
        }

        let mut jogo_aux = jogo.clone();
        jogo_aux.jogar(i);

        let aux = rec_minmax(&jogo_aux, jogo.vez);
        if aux > melhor {
            melhor = aux;
            jogada = i as u8;
        }
    }

    jogada
}
