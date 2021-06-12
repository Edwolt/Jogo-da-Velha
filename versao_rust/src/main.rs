mod jogo;
mod mapa;

use std::io;

use mapa::Mapa;

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
            1 | 3 | 4 => println!("Não implementado ainda"),
            2 => {
                let mapa = Mapa::criar();
                mapa.salvar("mapa.txt").expect("Falha ao salvar mapa");

                println!("Mapa salvo em mapa.txt");
                println!("Tamanho do mapa: {}", mapa.len());
                println!("Tamanho do cromossomo: {}", mapa.tam_cromossomo());
            }
            _ => break,
        }
    }
}
