mod individuo;
mod jogo;
mod mapa;
mod minmax;
mod populacao;
mod solucao;

use std::io;

use crate::jogo::Jogo;
use crate::mapa::Mapa;
use crate::minmax::minmax;
use crate::solucao::Solucao;

fn main() {
    loop {
        println!("[1] Jogar");
        println!("[2] Mapa");
        println!("[3] Minmax");
        println!("[4] Evolutivo");
        println!("[0] Sair");

        let option = {
            let mut buffer = String::new();
            io::stdin()
                .read_line(&mut buffer)
                .expect("Esperava um número");
            buffer.trim().parse::<u8>().expect("Esperava um número")
        };

        match option {
            0 => break,
            1 | 4 => println!("Não implementado ainda"),
            2 => {
                let mapa = Mapa::criar();
                mapa.salvar("mapa.txt").expect("Falha ao salvar mapa");

                println!("Mapa salvo em mapa.txt");
                println!("Tamanho do mapa: {}", mapa.data.len());
                println!("Tamanho do cromossomo: {}", mapa.tam_cromossomo);
            }
            3 => {
                let mapa = Mapa::carregar("mapa.txt").expect("Falha ao carregar mapa");
                let jogos = Jogo::possibilidades();
                let mut solucao = Solucao::criar(mapa.tam_cromossomo);

                for j in jogos {
                    solucao.set_jogada(&mapa, &j, Some(minmax(&j)));
                }

                solucao.salvar("minmax.txt").expect("Falha ao salvar mapa");
                println!("Solução salva em minmax.txt");
            }
            _ => break,
        }
    }
}
