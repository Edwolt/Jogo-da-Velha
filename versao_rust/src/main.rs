mod individuo;
mod jogo;
mod mapa;
mod minmax;
mod populacao;
mod solucao;

use std::io;
use std::path::Path;
use std::process::Command;

use device_query::{DeviceQuery, DeviceState, Keycode};

use crate::jogo::Jogo;
use crate::mapa::Mapa;
use crate::minmax::minmax;
use crate::populacao::Populacao;
use crate::solucao::Solucao;

fn main() {
    loop {
        println!("[1] Jogar");
        println!("[2] Mapa");
        println!("[3] Minmax");
        println!("[4] Evolutivo");
        println!("[0] Sair");

        let opcao = {
            let mut buffer = String::new();
            io::stdin()
                .read_line(&mut buffer)
                .expect("Esperava um número");
            buffer.trim().parse::<u8>().expect("Esperava um número")
        };

        match opcao {
            0 => break,
            1 => opcao_jogar(),
            2 => opcao_mapa(),
            3 => opcao_minmax(),
            4 => opcao_evolutivo(),
            _ => break,
        }
    }
}

fn opcao_jogar() {
    Command::new("sh")
        .arg(Path::new("jogar.sh"))
        .current_dir(Path::new("./python"))
        .status()
        .expect("Não foi possível rodar o jogo em python");
}

fn opcao_mapa() {
    let mapa = Mapa::criar();
    mapa.salvar("mapa.txt").expect("Falha ao salvar mapa");

    println!("Mapa salvo em mapa.txt");
    println!("Tamanho do mapa: {}", mapa.data.len());
    println!("Tamanho do cromossomo: {}", mapa.tam_cromossomo)
}

fn opcao_minmax() {
    let mapa = Mapa::carregar("mapa.txt").expect("Falha ao carregar mapa");
    let jogos = Jogo::possibilidades();
    let mut solucao = Solucao::criar(&mapa);

    for jg in jogos {
        solucao.set_jogada(&mapa, &jg, Some(minmax(&jg)));
    }

    solucao
        .salvar("minmax.txt")
        .expect("Falha ao salvar solução");
    println!("Solução salva em minmax.txt");
}

fn opcao_evolutivo() {
    // TODO Não está funcionando como deveria

    const N: usize = 200;
    const PREDADOS: usize = 2;
    const PERIODO_PREDACAO: u128 = 25;
    const PERIODO_INFORMACAO: u128 = 10;
    const MUTACAO: f64 = 0.002;

    let device = DeviceState::new();
    // Não funciona exatamente igual ao kbhit
    let sair = || {
        let keys = device.get_keys();
        if keys.is_empty() {
            false
        } else {
            keys.iter().any(|k| k == &Keycode::Q)
        }
    };
    let mapa = Mapa::carregar("mapa.txt").expect("Falha ao carregar mapa");

    let mut populacao = Populacao::criar(&mapa, N);
    populacao.fitness(&mapa);

    for geracao in 0u128..1000u128 {
        if sair() {
            println!("Sair!");
            break;
        }

        populacao._torneio();
        populacao.fitness(&mapa);
        if PERIODO_INFORMACAO != 0 && geracao % PERIODO_INFORMACAO == 0 {
            println!(
                "Geração {:3} {} {}",
                geracao,
                populacao.pop.first().unwrap().fitness,
                populacao.pop.last().unwrap().fitness
            );
        }

        populacao._predacao_sintese(&mapa, PREDADOS);
        if PERIODO_PREDACAO != 0 && geracao % PERIODO_PREDACAO == 0 {
            populacao._predacao_randomica(&mapa, PREDADOS);
        }

        populacao._mutacao(MUTACAO);
    }

    populacao.fitness(&mapa);
    let melhor = &mut populacao.pop.first_mut().unwrap().solucao;
    melhor.correcao(&mapa);

    melhor
        .salvar("evolutivo.txt")
        .expect("Falha ao salvar solução");
    println!("Solução salva em evolutivo.txt")
}
