use std::io;

mod jogo;

fn main() {
    loop {
        println!("[1] Jogar");
        println!("[2] Mapa");
        println!("[3] Minmax");
        println!("[4] Evolutivo");
        println!("[0] Sair");

        let option = {
            let mut buffer: String = String::new();
            io::stdin()
                .read_line(&mut buffer)
                .expect("Esperava um número");
            buffer.parse::<u8>().expect("Esperava um número")
        };

        match option {
            0 => break,
            1 | 2 | 3 | 4 => println!("Não implementado ainda"),
            _ => break,
        }
    }
}
